#include <infostd.h>

INFOSTD_FSM_STATE infostd_fsm_state_init(void) { 

  INFOSTD_FSM_STATE ifss = NULL;

  ifss = (INFOSTD_FSM_STATE)malloc(SIZE_INFOSTD_FSM_STATE);
  memset(ifss,0,SIZE_INFOSTD_FSM_STATE);
  
  ifss->transitions = infostd_dyn_ltab_init(100);
  ifss->states = infostd_dyn_ltab_init(100);
  ifss->num_trans = 0;

  ifss->lookup = infostd_hash_long_init(100);

  return ifss;
}

void infostd_fsm_state_free(INFOSTD_FSM_STATE ifss) { 

  if(ifss->transitions)
    infostd_dyn_ltab_free(ifss->transitions);
  if(ifss->states)
    infostd_dyn_ltab_free(ifss->states);

  if(ifss->lookup)
    infostd_hash_long_free(ifss->lookup);
}


INFOSTD_FSM infostd_fsm_init(unsigned long size) { 
  INFOSTD_FSM ifsm = NULL;

  ifsm = (INFOSTD_FSM)malloc(SIZE_INFOSTD_FSM);
  memset(ifsm,0,SIZE_INFOSTD_FSM);
  
  ifsm->state = INFOSTD_FSM_START_STATE;
  ifsm->states = infostd_dyn_ptab_init(size);

  ifsm->final_states = infostd_hash_long_init(size);

  return ifsm;
}

void infostd_fsm_free(INFOSTD_FSM ifsm) { 

  unsigned long i;
  INFOSTD_FSM_STATE ifss;

  if(!ifsm) 
    return;

  if(ifsm->final_states)
    infostd_hash_long_free(ifsm->final_states);

  for(i=0;i<ifsm->max_state;i++) {
    ifss = infostd_dyn_ptab_get(ifsm->states,i);
    if(ifss)
      infostd_fsm_state_free(ifss);
  }

  if(ifsm->states)
    infostd_dyn_ptab_free(ifsm->states);

  free(ifsm);
}

void infostd_fsm_add_transition(INFOSTD_FSM ifsm,
				unsigned long input,
				unsigned long curr_state,
				unsigned long new_state) { 
    
  INFOSTD_FSM_STATE ifss;

  ifss = infostd_dyn_ptab_get(ifsm->states,curr_state);
  if(!ifss) {
    ifss = infostd_fsm_state_init();
    infostd_dyn_ptab_set(ifsm->states,curr_state,ifss);
  }
  
  infostd_hash_long_add(ifss->lookup,input,new_state);

  infostd_dyn_ltab_set(ifss->transitions,ifss->num_trans,input);
  infostd_dyn_ltab_set(ifss->states,ifss->num_trans,new_state);
  ifss->num_trans++;

  if(new_state > ifsm->max_state)
    ifsm->max_state = new_state;

  ifsm->num_states++;
}

void infostd_fsm_add_final(INFOSTD_FSM ifsm,
			   unsigned long state) { 

  infostd_hash_long_add(ifsm->final_states,state,1);
}

unsigned long infostd_fsm_state(INFOSTD_FSM ifsm) { 

  return ifsm->state;
}

int infostd_fsm_state_final(INFOSTD_FSM ifsm) { 
  
  if(infostd_hash_long_info(ifsm->final_states,ifsm->state)!=ULONG_MAX)
    return 1;

  return 0;
}

unsigned long infostd_fsm_state_transitions(INFOSTD_FSM ifsm, 
					    unsigned long state,
					    INFOSTD_DYN_LTAB trans,
					    INFOSTD_DYN_LTAB states) { 

  INFOSTD_FSM_STATE ifss;
  unsigned long i;

  ifss = infostd_dyn_ptab_get(ifsm->states,state);

  if(!ifss)
    return 0;

  for(i=0;i<ifss->num_trans;i++) {
    infostd_dyn_ltab_set(trans,i,infostd_dyn_ltab_get(ifss->transitions,i));
    infostd_dyn_ltab_set(states,i,infostd_dyn_ltab_get(ifss->states,i));
  }
  
  return ifss->num_trans;
}

int infostd_fsm_transition_exists(INFOSTD_FSM ifsm, 
				  unsigned long state,
				  unsigned long data) { 
  
  INFOSTD_FSM_STATE ifss;
  unsigned long s = 0;
  
  ifss = infostd_dyn_ptab_get(ifsm->states,state);
  if(!ifss)
    return 0;

  if((s = infostd_hash_long_info(ifss->lookup,data))!=ULONG_MAX)
    return 1;

  return 0;
}

int infostd_fsm_transition_state(INFOSTD_FSM ifsm, 
				 unsigned long state,
				 unsigned long data) { 
  
  INFOSTD_FSM_STATE ifss;
  unsigned long s = 0;
  
  ifss = infostd_dyn_ptab_get(ifsm->states,state);
  if(!ifss)
    return 0;

  if((s = infostd_hash_long_info(ifss->lookup,data))!=ULONG_MAX)
    return s;

  return 0;
}

int infostd_fsm_transition(INFOSTD_FSM ifsm, unsigned long data) { 
  
  INFOSTD_FSM_STATE ifss;
  unsigned long state = 0;
  
  /*printf("FSM: current state %lu, input: %c",ifsm->state,data);*/

  ifss = infostd_dyn_ptab_get(ifsm->states,ifsm->state);
  if(!ifss)
    return 0;

  if((state = infostd_hash_long_info(ifss->lookup,data))!=ULONG_MAX) {
    ifsm->state=state;
    /*printf(", new state: %lu\n",state);*/
    return 1;
  }
  /*printf("\n");*/
  return 0;
}

INFOSTD_FSM infostd_fsm_build_matcher(char *str, 
				      unsigned long length) {
  
  INFOSTD_FSM ifsm;
  unsigned long i, size = length*2;
  
  ifsm = infostd_fsm_init(size);

  for(i=INFOSTD_FSM_START_STATE;i<length;i++) {
    infostd_fsm_add_transition(ifsm,str[i],i,i+1);
  }
  infostd_fsm_add_final(ifsm,i);

  return ifsm;
}

void infostd_fsm_add_matcher(INFOSTD_FSM ifsm,
			     char *str, 
			     unsigned long length) {

  unsigned long i;
  unsigned long s = INFOSTD_FSM_START_STATE;
  unsigned long end = 0;

  for(i=0;i<length && !end;i++) {
    if(infostd_fsm_transition_exists(ifsm,s,str[i]))
      s = infostd_fsm_transition_state(ifsm,s,str[i]);
    else {
      infostd_fsm_add_transition(ifsm,str[i],s,ifsm->max_state+1);
      s = ifsm->max_state;
      end = 1;
    }    
  }

  for(;i<length;i++) {
    infostd_fsm_add_transition(ifsm,str[i],s,ifsm->max_state+1);
    s = ifsm->max_state;
  }

  infostd_fsm_add_final(ifsm,s);
}

int infostd_fsm_matches(INFOSTD_FSM ifsm,
			char* str,
			unsigned long length) {

  unsigned long i;

  ifsm->state = INFOSTD_FSM_START_STATE;
  for(i=0;i<length;i++) {
    if(!infostd_fsm_transition(ifsm,str[i]))
      return 0;
  }

  return infostd_fsm_state_final(ifsm);
}

#include <infostd.h>

/* Longs */
INFOSTD_LQUEUE infostd_lqueue_init(unsigned long size) {
  
  INFOSTD_LQUEUE lq;

  lq = (INFOSTD_LQUEUE)malloc(SIZE_INFOSTD_LQUEUE);
  memset(lq,0,SIZE_INFOSTD_LQUEUE);

  lq->size = size;
  lq->tab = infostd_dyn_ltab_init(size);

  return lq;
}

void infostd_lqueue_free(INFOSTD_LQUEUE lq) {

  if(!lq)
    return;

  if(lq->tab)
    infostd_dyn_ltab_free(lq->tab);

  free(lq);
}

unsigned long infostd_lqueue_push(INFOSTD_LQUEUE lq,
				  unsigned long data) {

  if(!lq)
    return 0;
  
  if(lq->num == lq->size)
    return 0;

  lq->num++;

  infostd_dyn_ltab_set(lq->tab,lq->pos,data);
  lq->pos++;

  lq->end = lq->pos;
  
  if(lq->end > lq->size)
    lq->end = lq->pos = 0;
  
  return 1;
}

unsigned long infostd_lqueue_pop(INFOSTD_LQUEUE lq) {

  unsigned long data;
  
  if(!lq->num)
    return 0;
  
  data = infostd_dyn_ltab_get(lq->tab,lq->start);
  
  lq->start++;
  lq->num--;

  if(lq->start > lq->size)
    lq->start = 0;

  return data;
}

/* Floats */
INFOSTD_FQUEUE infostd_fqueue_init(unsigned long size) {
  
  INFOSTD_FQUEUE fq;

  fq = (INFOSTD_FQUEUE)malloc(SIZE_INFOSTD_FQUEUE);
  memset(fq,0,SIZE_INFOSTD_FQUEUE);

  fq->size = size;
  fq->tab = infostd_dyn_ftab_init(size);

  return fq;
}

void infostd_fqueue_free(INFOSTD_FQUEUE fq) {

  if(!fq)
    return;

  if(fq->tab)
    infostd_dyn_ftab_free(fq->tab);

  free(fq);
}

unsigned long infostd_fqueue_push(INFOSTD_FQUEUE fq,
				  float data) {

  if(!fq)
    return 0;
  
  if(fq->num == fq->size)
    return 0;

  fq->num++;

  infostd_dyn_ftab_set(fq->tab,fq->pos,data);
  fq->pos++;

  fq->end = fq->pos;
  
  if(fq->end > fq->size)
    fq->end = fq->pos = 0;
  
  return 1;
}

float infostd_fqueue_pop(INFOSTD_FQUEUE fq) {

  float data;
  
  if(!fq->num)
    return 0;
  
  data = infostd_dyn_ftab_get(fq->tab,fq->start);
  
  fq->start++;
  fq->num--;

  if(fq->start > fq->size)
    fq->start = 0;

  return data;
}

/* Double */
INFOSTD_DQUEUE infostd_dqueue_init(unsigned long size) {
  
  INFOSTD_DQUEUE dq;

  dq = (INFOSTD_DQUEUE)malloc(SIZE_INFOSTD_DQUEUE);
  memset(dq,0,SIZE_INFOSTD_DQUEUE);

  dq->size = size;
  dq->tab = infostd_dyn_dtab_init(size);

  return dq;
}

void infostd_dqueue_free(INFOSTD_DQUEUE dq) {

  if(!dq)
    return;

  if(dq->tab)
    infostd_dyn_dtab_free(dq->tab);

  free(dq);
}

unsigned long infostd_dqueue_push(INFOSTD_DQUEUE dq,
				  double data) {

  if(!dq)
    return 0;
  
  if(dq->num == dq->size)
    return 0;

  dq->num++;

  infostd_dyn_dtab_set(dq->tab,dq->pos,data);
  dq->pos++;

  dq->end = dq->pos;
  
  if(dq->end > dq->size)
    dq->end = dq->pos = 0;
  
  return 1;
}

double infostd_dqueue_pop(INFOSTD_DQUEUE dq) {

  double data;
  
  if(!dq->num)
    return 0;
  
  data = infostd_dyn_dtab_get(dq->tab,dq->start);
  
  dq->start++;
  dq->num--;

  if(dq->start > dq->size)
    dq->start = 0;

  return data;
}
  
/* Pointers */
INFOSTD_PQUEUE infostd_pqueue_init(unsigned long size) {
  
  INFOSTD_PQUEUE pq;

  pq = (INFOSTD_PQUEUE)malloc(SIZE_INFOSTD_PQUEUE);
  memset(pq,0,SIZE_INFOSTD_PQUEUE);

  pq->size = size;
  pq->tab = infostd_dyn_ptab_init(size);

  return pq;
}

void infostd_pqueue_free(INFOSTD_PQUEUE pq) {

  if(!pq)
    return;

  if(pq->tab)
    infostd_dyn_ptab_free(pq->tab);

  free(pq);
}

unsigned long infostd_pqueue_push(INFOSTD_PQUEUE pq,
				  void *data) {

  if(!pq)
    return 0;
  
  if(pq->num == pq->size)
    return 0;

  pq->num++;

  infostd_dyn_ptab_set(pq->tab,pq->pos,data);
  pq->pos++;

  pq->end = pq->pos;
  
  if(pq->end > pq->size)
    pq->end = pq->pos = 0;
  
  return 1;
}

void *infostd_pqueue_pop(INFOSTD_PQUEUE pq) {

  void *data;
  
  if(!pq->num)
    return 0;
  
  data = infostd_dyn_ptab_get(pq->tab,pq->start);
  
  pq->start++;
  pq->num--;

  if(pq->start > pq->size)
    pq->start = 0;

  return data;
}

/* Character Queue */
INFOSTD_CQUEUE infostd_cqueue_init(unsigned long size) {
  
  INFOSTD_CQUEUE cq;

  cq = (INFOSTD_CQUEUE)malloc(SIZE_INFOSTD_CQUEUE);
  memset(cq,0,SIZE_INFOSTD_CQUEUE);

  cq->size = size;
  cq->tab = infostd_dyn_ctab_init(size);

  return cq;
}

void infostd_cqueue_free(INFOSTD_CQUEUE cq) {

  if(!cq)
    return;

  if(cq->tab)
    infostd_dyn_ctab_free(cq->tab);

  free(cq);
}

unsigned long infostd_cqueue_push(INFOSTD_CQUEUE cq,
				  char data) {

  if(!cq)
    return 0;
  
  if(cq->num == cq->size)
    return 0;

  cq->num++;

  infostd_dyn_ctab_set(cq->tab,cq->pos,data);
  cq->pos++;

  cq->end = cq->pos;
  
  if(cq->end > cq->size)
    cq->end = cq->pos = 0;
  
  return 1;
}

char infostd_cqueue_pop(INFOSTD_CQUEUE cq) {

  char data;
  
  if(!cq->num)
    return 0;
  
  data = infostd_dyn_ctab_get(cq->tab,cq->start);
  
  cq->start++;
  cq->num--;

  if(cq->start > cq->size)
    cq->start = 0;

  return data;
}

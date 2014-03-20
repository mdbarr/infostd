#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <infostd.h>

#define HASH_SIZE 10

int infostd_pheap_info_strcmp(void* a, void* b) {

  return strcmp((char*)a,(char*)b);
}

int main(int argc, char** argv) {
  
  INFOSTD_REGEXP_ENGINE engine;
  INFOSTD_REGEXP_RESULT result;
  INFOSTD_DYN_STR ids, ids2;
  INFOSTD_DYN_PTAB ipt;
  INFOSTD_DYN_LTAB ilt;
  INFOSTD_DYN_FTAB ift;
  INFOSTD_DYN_DTAB idt;
  INFOSTD_HASH_STR ihs;
  INFOSTD_HASH_LONG ihl;
  INFOSTD_PHEAP iph;
  INFOSTD_DHEAP idh;
  INFOSTD_LHEAP_INFO ilhi;
  INFOSTD_FHEAP_INFO ifhi;
  INFOSTD_DHEAP_INFO idhi;
  INFOSTD_DYN_STRS iss;
  INFOSTD_TOKENIZER tkz;
  INFOSTD_TOKEN itk, itk2;
  INFOSTD_FSM ifsm;
  INFOSTD_LQUEUE lq;
  INFOSTD_CQUEUE cq;
  unsigned long id, info;  
  double dinfo;
  float finfo;
  FILE* fp;
  char* ln;
  int n;

  printf("Dynamic String Testing...\n");
  ids = infostd_dyn_str_init(100);
  infostd_dyn_str_printf(ids,"this %x is %lu %c %s",55,3389433,'a',"test.");
  printf("IDS: %s\n",ids->buff);


  printf("\nRegular Expression Testing...\n");
  engine = infostd_regexp_engine_init();
  result = infostd_regexp_result_init();

  /* Simple match */
  n = infostd_regexp(engine,result,"aabbcc",6,
			  "/a+b+c+/");

  infostd_regexp_result_print(result);

  /* Complex match */
  n = infostd_regexp(engine,result,"aaBbcc",6,
			  "/(a+)(b+)(c+)/i");

  infostd_regexp_result_print(result);

  /* Simple Substitution */
  n = infostd_regexp(engine,result,"aabbcc",6,
			  "s/ab+c/tester/");

  infostd_regexp_result_print(result);

  /* Comples Substitution 1 */
  n = infostd_regexp(engine,result,"AaBbcC",6,
			  "s/ab+c/tester/i");

  infostd_regexp_result_print(result);

  /* Complex Substitution 2 */
  n = infostd_regexp(engine,result,"abcabc",6,
			  "s/abc/efgh/g");

  infostd_regexp_result_print(result);

  /* Complex Substitution 3 */
  n = infostd_regexp(engine,result,"abcabc",6,
			  "s/(a)(b)(c)/e$1f$2g$3h/g");

  infostd_regexp_result_print(result);

  /* Complex Substitution 4 */
  n = infostd_regexp(engine,result,"abcabc",6,
			  "s/(a)(b)(c)/e$1f$2g$3h/gi");

  infostd_regexp_result_print(result);

  /* Complex Substitution 5 */
  n = infostd_regexp(engine,result,"abcadc",6,
			  "s/a(.)c/$1/g");

  infostd_regexp_result_print(result);

  /* Complex Substitution 6 */
  n = infostd_regexp(engine,result,"aaaa",6,
			  "s/a/test /g");

  infostd_regexp_result_print(result);

  infostd_regexp_result_free(result);
  infostd_regexp_engine_free(engine);

  /* String Functions */
  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);

  infostd_dyn_str_strcpy(ids,"this is a test");
  printf("\nString: %s\n",ids->buff);

  /* CRC32 */
  printf("CRC32: %lu\n",infostd_crc32(ids->buff,ids->length));
  
  /* Base64 Encode/Decode */
  infostd_base64_encode(ids->buff,ids->length,ids2);
  infostd_base64_decode(ids2->buff,ids2->length,ids);

  fp = fopen("test.dstr","w");
  infostd_dyn_str_write(ids2,fp);
  fclose(fp);
  infostd_dyn_str_free(ids2);
  
  fp = fopen("test.dstr","r");
  ids2 = infostd_dyn_str_read(fp);
  fclose(fp);

  printf("BASE64 Encode: %s\n",ids2->buff);
  printf("BASE64 Decode: %s\n",ids->buff);	
  
  infostd_dyn_str_free(ids);
  infostd_dyn_str_free(ids2);

  /* Dynamic Pointers */
  ipt = infostd_dyn_ptab_init(10);
  infostd_dyn_ptab_set(ipt,5,(void*)"PTAB Tester");
  printf("PTAB: %s\n",(char*)infostd_dyn_ptab_get(ipt,5));
  infostd_dyn_ptab_free(ipt);

  /* Dynamic Long */
  printf("DYN_LTAB Tester with read/write\n");
  ilt = infostd_dyn_ltab_init(10);
  infostd_dyn_ltab_set(ilt,2,7);
  
  fp = fopen("test.ltab","w");
  infostd_dyn_ltab_write(ilt,fp);
  fclose(fp);
  infostd_dyn_ltab_free(ilt);
  
  fp = fopen("test.ltab","r");
  ilt = infostd_dyn_ltab_read(fp);
  fclose(fp);

  printf("LTAB: %lu\n",infostd_dyn_ltab_get(ilt,2));
  infostd_dyn_ltab_free(ilt);

  /* Dynamic Float */
  printf("DYN_FTAB Tester\n");
  ift = infostd_dyn_ftab_init(10);
  infostd_dyn_ftab_set(ift,2,.567);
  printf("FTAB: %f\n",infostd_dyn_ftab_get(ift,2));
  infostd_dyn_ftab_free(ift);

  /* Dynamic Doubles */
  printf("DYN_DTAB Tester\n");
  idt = infostd_dyn_dtab_init(10);
  infostd_dyn_dtab_set(idt,2,0.010203);
  printf("DTAB: %f\n",infostd_dyn_dtab_get(idt,2));
  infostd_dyn_dtab_free(idt);

  /* Hashes */
  printf("HASH Tester\n");
  printf("LONG END: %lu\n",ULONG_MAX);

  ids = infostd_dyn_str_init(100);

  infostd_dyn_str_strcpy(ids,"this");
  printf("HASH: %s %lu\n",ids->buff,infostd_hash_str_hash(ids->buff,
							  HASH_SIZE));

  infostd_dyn_str_strcpy(ids,"that");
  printf("HASH: %s %lu\n",ids->buff,infostd_hash_str_hash(ids->buff,
							  HASH_SIZE));

  infostd_dyn_str_strcpy(ids,"sass");
  printf("HASH: %s %lu\n",ids->buff,infostd_hash_str_hash(ids->buff,
							  HASH_SIZE));

  infostd_dyn_str_strcpy(ids,"score");
  printf("HASH: %s %lu\n",ids->buff,infostd_hash_str_hash(ids->buff,
							  HASH_SIZE));

  infostd_dyn_str_strcpy(ids,"through");
  printf("HASH: %s %lu\n",ids->buff,infostd_hash_str_hash(ids->buff,
							  HASH_SIZE));

  infostd_dyn_str_free(ids);

  ihs = infostd_hash_str_init(HASH_SIZE);
  printf("\nHASH: Adding keys...\n");
  infostd_hash_str_add(ihs,"this",1);
  infostd_hash_str_add(ihs,"that",2);
  infostd_hash_str_add(ihs,"sass",3);
  infostd_hash_str_add(ihs,"score",4);
  infostd_hash_str_add(ihs,"through",5);

  infostd_hash_str_rm(ihs,"sass");

  printf("HASH INFO: this - %lu\n",infostd_hash_str_info(ihs,"this"));
  printf("HASH INFO: that - %lu\n",infostd_hash_str_info(ihs,"that"));
  printf("HASH INFO: sass - %lu\n",infostd_hash_str_info(ihs,"sass"));
  printf("HASH INFO: score - %lu\n",infostd_hash_str_info(ihs,"score"));
  printf("HASH INFO: through - %lu\n",infostd_hash_str_info(ihs,"through"));
  printf("HASH INFO: crap - %lu\n",infostd_hash_str_info(ihs,"crap"));
  
  infostd_hash_str_free(ihs);

  /* Hash with string info */
  ihs = infostd_hash_str_init(HASH_SIZE);
  printf("\nHASH STRING Testing with string info and read/write\n");
  infostd_hash_str_add_string(ihs,"this","the string is this");
  infostd_hash_str_add_string(ihs,"that","set to that");
  infostd_hash_str_add_string(ihs,"sass","a sass one");
  infostd_hash_str_add_string(ihs,"sass","a sass one");
  infostd_hash_str_add_string(ihs,"sass","and this..");
  infostd_hash_str_add_string(ihs,"sass","and more sass");
  infostd_hash_str_add_string(ihs,"score","we scored");
  infostd_hash_str_add_string(ihs,"through","we made it through them all");

  fp = fopen("test.hs","w");
  infostd_hash_str_write(ihs,fp);
  fclose(fp);
  infostd_hash_str_free(ihs);
  
  fp = fopen("test.hs","r");
  ihs = infostd_hash_str_read(fp);
  fclose(fp);  

  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);
  
  infostd_dyn_str_strcpy(ids,"this");
  id = infostd_hash_str_string(ihs,ids->buff,ids2);
  printf("HASH STRING: %lu %s - %s.\n",id,ids->buff,ids2->buff);

  infostd_dyn_str_strcpy(ids,"that");
  id = infostd_hash_str_string(ihs,ids->buff,ids2);
  printf("HASH STRING: %lu %s - %s.\n",id,ids->buff,ids2->buff);

  infostd_dyn_str_strcpy(ids,"sass");
  id = infostd_hash_str_string(ihs,ids->buff,ids2);
  printf("HASH STRING: %lu %s - %s.\n",id,ids->buff,ids2->buff);

  infostd_dyn_str_strcpy(ids,"score");
  id = infostd_hash_str_string(ihs,ids->buff,ids2);
  printf("HASH STRING: %lu %s - %s.\n",id,ids->buff,ids2->buff);

  infostd_dyn_str_strcpy(ids,"through");
  id = infostd_hash_str_string(ihs,ids->buff,ids2);
  printf("HASH STRING: %lu %s - %s.\n",id,ids->buff,ids2->buff);

  infostd_dyn_str_free(ids);
  infostd_dyn_str_free(ids2);
  infostd_hash_str_free(ihs);

  /* Hash Long */
  ihl = infostd_hash_long_init(100);
  printf("\nHASH LONG Testing with read/write\n");

  infostd_hash_long_add(ihl,546345,1);
  infostd_hash_long_add(ihl,865,2);
  infostd_hash_long_add(ihl,49087,3);
  infostd_hash_long_add(ihl,96798,4);
  infostd_hash_long_add(ihl,85856850,5);

  infostd_hash_long_rm(ihl,865);

  fp = fopen("test.hl","w");
  infostd_hash_long_write(ihl,fp);
  fclose(fp);
  infostd_hash_long_free(ihl);
  
  fp = fopen("test.hl","r");
  ihl = infostd_hash_long_read(fp);
  fclose(fp);  
  
  id = 546345;
  printf("HASHLONG: %lu %lu\n",id,infostd_hash_long_info(ihl,id));
  id = 865;
  printf("HASHLONG: %lu %lu\n",id,infostd_hash_long_info(ihl,id));
  id = 49087;
  printf("HASHLONG: %lu %lu\n",id,infostd_hash_long_info(ihl,id));
  id = 96798;
  printf("HASHLONG: %lu %lu\n",id,infostd_hash_long_info(ihl,id));
  id = 85856850;
  printf("HASHLONG: %lu %lu\n",id,infostd_hash_long_info(ihl,id));

  infostd_hash_long_free(ihl);

  printf("\nTesting Pointer Heaps\n");
  iph = infostd_pheap_init(INFOSTD_HEAP_MIN,infostd_pheap_info_strcmp,10);
  infostd_pheap_push(iph,(void*)"b");
  infostd_pheap_push(iph,(void*)"e");
  infostd_pheap_push(iph,(void*)"a");
  infostd_pheap_push(iph,(void*)"c");
  infostd_pheap_push(iph,(void*)"d");

  while((ln=infostd_pheap_pop(iph))!=(void*)ULONG_MAX) {
    printf("PHEAP_MIN: %s\n",ln);
  }
  infostd_pheap_free(iph);

  printf("\nTesting Long Info Heaps\n");
  ilhi = infostd_lheap_info_init(INFOSTD_HEAP_MAX,10);
  infostd_lheap_info_push(ilhi,5,1);
  infostd_lheap_info_push(ilhi,2,2);
  infostd_lheap_info_push(ilhi,1,3);
  infostd_lheap_info_push(ilhi,3,4);
  infostd_lheap_info_push(ilhi,4,5);

  while((id=infostd_lheap_info_pop(ilhi,&info))!=ULONG_MAX) {
    printf("LHEAP_INFO_MAX: %lu %lu\n",id,info);
  }
  infostd_lheap_info_free(ilhi);

  ilhi = infostd_lheap_info_init(INFOSTD_HEAP_MIN,10);
  infostd_lheap_info_push(ilhi,5,1);
  infostd_lheap_info_push(ilhi,2,2);
  infostd_lheap_info_push(ilhi,1,3);
  infostd_lheap_info_push(ilhi,3,4);
  infostd_lheap_info_push(ilhi,4,5);

  while((id=infostd_lheap_info_pop(ilhi,&info))!=ULONG_MAX) {
    printf("LHEAP_INFO_MIN: %lu %lu\n",id,info);
  }
  infostd_lheap_info_free(ilhi);

  printf("\nTesting Float Info Heaps\n");
  ifhi = infostd_fheap_info_init(INFOSTD_HEAP_MAX,10);
  infostd_fheap_info_push(ifhi,0.5,1);
  infostd_fheap_info_push(ifhi,0.2,2);
  infostd_fheap_info_push(ifhi,0.1,3);
  infostd_fheap_info_push(ifhi,0.3,4);
  infostd_fheap_info_push(ifhi,0.4,5);

  while((finfo=infostd_fheap_info_pop(ifhi,&id))!=ULONG_MAX) {
    printf("FHEAP_INFO_MAX: %lu %f\n",id,finfo);
  }
  infostd_fheap_info_free(ifhi);

  printf("\nTesting Double Heaps\n");
  idh = infostd_dheap_init(INFOSTD_HEAP_MIN,10);
  infostd_dheap_push(idh,0.50505);
  infostd_dheap_push(idh,0.20202);
  infostd_dheap_push(idh,0.10101);
  infostd_dheap_push(idh,0.30303);
  infostd_dheap_push(idh,0.40404);

  while((dinfo=infostd_dheap_pop(idh))!=ULONG_MAX) {
    printf("DHEAP_MIN: %lf\n",dinfo);
  }
  infostd_dheap_free(idh);

  printf("\nTesting Double Info Heaps\n");
  idhi = infostd_dheap_info_init(INFOSTD_HEAP_MAX,10);
  infostd_dheap_info_push(idhi,0.50505,1);
  infostd_dheap_info_push(idhi,0.20202,2);
  infostd_dheap_info_push(idhi,0.10101,3);
  infostd_dheap_info_push(idhi,0.30303,4);
  infostd_dheap_info_push(idhi,0.40404,5);

  while((dinfo=infostd_dheap_info_pop(idhi,&id))!=ULONG_MAX) {
    printf("DHEAP_INFO_MAX: %lu %lf\n",id,dinfo);
  }
  infostd_dheap_info_free(idhi);

  /* Dynamic Strings */
  printf("\nTesting Dynamic Strings\n");

  iss = infostd_dyn_strs_init(100);
  infostd_dyn_strs_add(iss,"this");
  infostd_dyn_strs_add(iss,"that");
  infostd_dyn_strs_add(iss,"sass");
  infostd_dyn_strs_add(iss,"score");
  infostd_dyn_strs_add(iss,"through");
	
  printf("# Strings: %lu\n",infostd_dyn_strs_num(iss));
  printf("DYN_STRS(%d): %s\n",0,infostd_dyn_strs_get(iss,0));
  printf("DYN_STRS(%d): %s\n",1,infostd_dyn_strs_get(iss,1));
  printf("DYN_STRS(%d): %s\n",2,infostd_dyn_strs_get(iss,2));
  printf("DYN_STRS(%d): %s\n",3,infostd_dyn_strs_get(iss,3));
  printf("DYN_STRS(%d): %s\n",4,infostd_dyn_strs_get(iss,4));

  infostd_dyn_strs_set(iss,2,"and more sass than ever");
  infostd_dyn_strs_set(iss,0,"yo");
  infostd_dyn_strs_set(iss,2,"and more sass than ever and so on");

  fp = fopen("test.dss","w");
  infostd_dyn_strs_write(iss,fp);
  fclose(fp);
  infostd_dyn_strs_free(iss);
  
  fp = fopen("test.dss","r");
  iss = infostd_dyn_strs_read(fp);
  fclose(fp);  
  
  infostd_dyn_strs_set(iss,0,"crap");
  infostd_dyn_strs_set(iss,0,"crappy");

  printf("DYN_STRS(%d): %s\n",0,infostd_dyn_strs_get(iss,0));
  printf("DYN_STRS(%d): %s\n",1,infostd_dyn_strs_get(iss,1));
  printf("DYN_STRS(%d): %s\n",2,infostd_dyn_strs_get(iss,2));
  printf("DYN_STRS(%d): %s\n",3,infostd_dyn_strs_get(iss,3));
  printf("DYN_STRS(%d): %s\n",4,infostd_dyn_strs_get(iss,4));
  
  infostd_dyn_strs_free(iss);

  /* Tokenizer Set */
  ids = infostd_dyn_str_init(100);
  infostd_dyn_str_strcpy(ids,"This is a test. Keep-on testing... Thank's to M. Sass.");

  printf("\nTokenizer SET testing\n");

  printf("SET: %s\n",INFOSTD_TOKENIZER_DEFAULT_SET);
  tkz = infostd_tokenizer_init_set(INFOSTD_TOKENIZER_DEFAULT_SET);
  itk = infostd_token_init(tkz);
 
  for(itk2=infostd_tokenizer_token_first(tkz,itk,ids->buff);
      itk2;
      itk2=infostd_tokenizer_token_next(tkz,itk,ids->buff)) {
    
    if(itk2->type == INFOSTD_TOKEN_WORD)
      printf("WORD: ");
    else if(itk2->type == INFOSTD_TOKEN_SEP)
      printf("SEPERATOR: ");
    else
      printf("NONE: ");
    printf("%s\n",itk->ids->buff);
  }

  infostd_token_free(itk);
  infostd_tokenizer_free(tkz);

  printf("\nTokenizer REGEXP testing\n");

  printf("REGEXP: %s\n",INFOSTD_TOKENIZER_DEFAULT_REGEXP);

  tkz = infostd_tokenizer_init_regexp(INFOSTD_TOKENIZER_DEFAULT_REGEXP);

  fp = fopen("test.tkz","w");
  infostd_tokenizer_write(tkz,fp);
  fclose(fp);

  infostd_tokenizer_free(tkz);

  fp = fopen("test.tkz","r");
  tkz = infostd_tokenizer_read(fp);
  fclose(fp);

  itk = infostd_token_init(tkz);
 
  for(itk2=infostd_tokenizer_token_first(tkz,itk,ids->buff);
      itk2;
      itk2=infostd_tokenizer_token_next(tkz,itk,ids->buff)) {
    
    if(itk2->type == INFOSTD_TOKEN_WORD)
      printf("WORD: ");
    else if(itk2->type == INFOSTD_TOKEN_SEP)
      printf("SEPERATOR: ");
    else
      printf("NONE: ");
    printf("%s\n",itk->ids->buff);
  }

  infostd_dyn_str_free(ids);
  infostd_token_free(itk);
  infostd_tokenizer_free(tkz);

  /* Metaphone / Edit distance test */
  printf("Testing Secondary Speller Functions\n\n");

  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);
  ilt = infostd_dyn_ltab_init(100);

  infostd_dyn_str_strcpy(ids,"test");
  
  infostd_speller_metaphone(ids->buff,ids->length,ids2);
  printf("METAPHONE: %s -> %s\n",ids->buff,ids2->buff);

  printf("STRING EDIT DISTANCE: %s -> %s = %ld\n",ids->buff,ids2->buff,
	 infostd_speller_edit_distance(ilt,ids->buff,ids->length,
				       ids2->buff,ids2->length));
  
  infostd_dyn_str_strcpy(ids,"superstition");
  infostd_dyn_str_strcpy(ids2,"supervision");

  printf("STRING EDIT DISTANCE: %s -> %s = %ld\n",ids->buff,ids2->buff,
	 infostd_speller_edit_distance(ilt,ids->buff,ids->length,
				       ids2->buff,ids2->length));

  infostd_dyn_str_strcpy(ids,"appropriate meaning");
  infostd_dyn_str_strcpy(ids2,"approximate matching");

  printf("STRING EDIT DISTANCE: %s -> %s = %ld\n",ids->buff,ids2->buff,
	 infostd_speller_edit_distance(ilt,ids->buff,ids->length,
				       ids2->buff,ids2->length));

  if(ids)
    infostd_dyn_str_free(ids);

  if(ids2)
    infostd_dyn_str_free(ids2);

  if(ilt)
    infostd_dyn_ltab_free(ilt);

  printf("\nFINITE STATE MACHINE TESTING\n");

  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);

  infostd_dyn_str_strcpy(ids,"finite state machine");
  infostd_dyn_str_strcpy(ids2,"finite sass thing");

  printf("Building FSM matcher for \'%s\'\n",ids->buff);
  ifsm = infostd_fsm_build_matcher(ids->buff,ids->length);
  
  printf("Testing against \'%s\': %d\n",ids->buff,
	 infostd_fsm_matches(ifsm,ids->buff,ids->length));

  printf("Testing against \'%s\': %d\n",ids2->buff,
	 infostd_fsm_matches(ifsm,ids2->buff,ids2->length));

  printf("Adding additional matcher for: \'%s\'\n",ids2->buff);

  infostd_fsm_add_matcher(ifsm,ids2->buff,ids2->length);

  printf("Testing against \'%s\': %d\n",ids->buff,
	 infostd_fsm_matches(ifsm,ids->buff,ids->length));

  printf("Testing against \'%s\': %d\n",ids2->buff,
	 infostd_fsm_matches(ifsm,ids2->buff,ids2->length));

  infostd_dyn_str_strcpy(ids,"finite status machine");
  infostd_dyn_str_strcpy(ids2,"uber sass");

  printf("Adding additional matcher for: \'%s\'\n",ids2->buff);

  infostd_fsm_add_matcher(ifsm,ids2->buff,ids2->length);

  printf("Testing against \'%s\': %d\n",ids->buff,
	 infostd_fsm_matches(ifsm,ids->buff,ids->length));

  printf("Testing against \'%s\': %d\n",ids2->buff,
	 infostd_fsm_matches(ifsm,ids2->buff,ids2->length));

  infostd_dyn_str_strcpy(ids,"finite ");
  infostd_dyn_str_strcpy(ids2,"finite");

  printf("Adding additional matcher for: \'%s\'\n",ids2->buff);

  infostd_fsm_add_matcher(ifsm,ids2->buff,ids2->length);

  printf("Testing against \'%s\': %d\n",ids->buff,
	 infostd_fsm_matches(ifsm,ids->buff,ids->length));

  printf("Testing against \'%s\': %d\n",ids2->buff,
	 infostd_fsm_matches(ifsm,ids2->buff,ids2->length));

  if(ids)
    infostd_dyn_str_free(ids);

  if(ids2)
    infostd_dyn_str_free(ids2);

  if(ifsm)
    infostd_fsm_free(ifsm);
  
  printf("\nLONG QUEUE TESTING\n");
  
  lq = infostd_lqueue_init(5);
  infostd_lqueue_push(lq,1);
  infostd_lqueue_push(lq,2);
  infostd_lqueue_push(lq,3);
  infostd_lqueue_push(lq,4);
  infostd_lqueue_push(lq,5);

  printf("POP1: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);
  printf("POP2: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);

  infostd_lqueue_push(lq,6);
  infostd_lqueue_push(lq,7);

  printf("POP3: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);

  infostd_lqueue_push(lq,8);

  printf("POP4: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);
  printf("POP5: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);
  printf("POP6: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);
  printf("POP7: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);
  printf("POP8: %ld %ld\n",infostd_lqueue_pop(lq),lq->start);

  if(lq)
    infostd_lqueue_free(lq);

  printf("\nLONG QUEUE TESTING\n");
  
  cq = infostd_cqueue_init(5);
  infostd_cqueue_push(cq,'a');
  infostd_cqueue_push(cq,'b');
  infostd_cqueue_push(cq,'c');
  infostd_cqueue_push(cq,'d');
  infostd_cqueue_push(cq,'e');

  printf("POP1: %c %c\n",infostd_cqueue_pop(cq),cq->start);
  printf("POP2: %c %c\n",infostd_cqueue_pop(cq),cq->start);

  infostd_cqueue_push(cq,'f');
  infostd_cqueue_push(cq,'g');

  printf("POP3: %c %c\n",infostd_cqueue_pop(cq),cq->start);

  infostd_cqueue_push(cq,'h');

  printf("POP4: %c %c\n",infostd_cqueue_pop(cq),cq->start);
  printf("POP5: %c %c\n",infostd_cqueue_pop(cq),cq->start);
  printf("POP6: %c %c\n",infostd_cqueue_pop(cq),cq->start);
  printf("POP7: %c %c\n",infostd_cqueue_pop(cq),cq->start);
  printf("POP8: %c %c\n",infostd_cqueue_pop(cq),cq->start);

  if(cq)
    infostd_cqueue_free(cq);

  return 0;
}

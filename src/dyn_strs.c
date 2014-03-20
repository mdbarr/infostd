#include <infostd.h>

INFOSTD_DYN_STRS infostd_dyn_strs_init(unsigned long size) {

  INFOSTD_DYN_STRS iss;

  iss = (INFOSTD_DYN_STRS)malloc(SIZE_INFOSTD_DYN_STRS);
  memset(iss,0,SIZE_INFOSTD_DYN_STRS);

  iss->str = infostd_dyn_str_init(size);
  iss->pos = infostd_dyn_ltab_init(100);

  return iss;    
}

void infostd_dyn_strs_free(INFOSTD_DYN_STRS iss) {
  
  if(iss->str)
    infostd_dyn_str_free(iss->str);

  if(iss->pos)
    infostd_dyn_ltab_free(iss->pos);

  free(iss);
}

unsigned char* infostd_dyn_strs_get(INFOSTD_DYN_STRS iss,
				    unsigned long pos) {

  unsigned long offset;
  if(pos > iss->num)
    return NULL;

  offset = infostd_dyn_ltab_get(iss->pos,pos);

  return iss->str->buff + offset;
}

void infostd_dyn_strs_set(INFOSTD_DYN_STRS iss,
			  unsigned long pos,
			  unsigned char* str) {

  unsigned long start, end, len, i, p;
  long offset;

  if(pos > iss->num)
    return;

  start = infostd_dyn_ltab_get(iss->pos,pos);
  end = strlen(iss->str->buff+start)+start;
  len = strlen(str);
  
  offset=infostd_dyn_str_replace(iss->str,start,end,str);

  for(i=pos+1;i<iss->num;i++) {
    p = infostd_dyn_ltab_get(iss->pos,i);
    infostd_dyn_ltab_set(iss->pos,i,p+offset);
  }
}

unsigned long infostd_dyn_strs_add(INFOSTD_DYN_STRS iss,
				   unsigned char* str) {
  
  unsigned long n, pos;

  n = iss->num;
  iss->num++;

  pos = iss->str->length;

  infostd_dyn_str_strcat(iss->str,str);
  infostd_dyn_str_addchar(iss->str,'\0');

  infostd_dyn_ltab_set(iss->pos,n,pos);

  return n;
}

void infostd_dyn_strs_clear(INFOSTD_DYN_STRS iss) {
  
  iss->num = 0;
  infostd_dyn_str_clear(iss->str);
}

unsigned long infostd_dyn_strs_num(INFOSTD_DYN_STRS iss) {
  
  return iss->num;
}

void infostd_dyn_strs_write(INFOSTD_DYN_STRS iss,
			    FILE* out) {

  unsigned long type;
  
  type = TYPE_INFOSTD_DYN_STRS;


  fwrite(&type,sizeof(unsigned long),1,out);
  fwrite(&iss->num,sizeof(unsigned long),1,out);
  infostd_dyn_ltab_write(iss->pos,out);
  infostd_dyn_str_write(iss->str,out);  
}

INFOSTD_DYN_STRS infostd_dyn_strs_read(FILE* in) {

  
  INFOSTD_DYN_STRS iss = NULL;
  unsigned long type, num;

  fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_STRS)
    return NULL;

  fread(&num,sizeof(unsigned long),1,in);
  
  iss = (INFOSTD_DYN_STRS)malloc(SIZE_INFOSTD_DYN_STRS);
  memset(iss,0,SIZE_INFOSTD_DYN_STRS);

  iss->pos = infostd_dyn_ltab_read(in);
  iss->str = infostd_dyn_str_read(in);
  iss->num = num;

  return iss;
}

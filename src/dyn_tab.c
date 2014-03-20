#include <infostd.h>

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Table of Pointers                                                 |
|                                                                             |
*----------------------------------------------------------------------------*/

INFOSTD_DYN_PTAB infostd_dyn_ptab_init(unsigned long size) {

  INFOSTD_DYN_PTAB ipt;

  ipt = (INFOSTD_DYN_PTAB)malloc(SIZE_INFOSTD_DYN_PTAB);
  memset(ipt,0,SIZE_INFOSTD_DYN_PTAB);

  ipt->buff = (void**)malloc(sizeof(void*)*size);
  memset(ipt->buff,0,sizeof(void*)*size);
  ipt->size = size;

  return ipt;
}

void infostd_dyn_ptab_free(INFOSTD_DYN_PTAB ipt) {

  if(!ipt)
    return;
  
  if(ipt->buff)
    free(ipt->buff);
  free(ipt);
}

void infostd_dyn_ptab_grow(INFOSTD_DYN_PTAB ipt,
			   unsigned long nsize) {
  
  void* tmp;
  unsigned long ns;
  
  if(!ipt)
    return;
  
  if(nsize < ipt->size)
    return;

  for(ns=ipt->size*2;ns<nsize;ns*=2);

  tmp = (void**)malloc(sizeof(void*)*ns);
  memset(tmp,0,sizeof(void*)*ns);

  memcpy(tmp,ipt->buff,sizeof(void*)*ipt->size);
  
  free(ipt->buff);

  ipt->buff = tmp;
  ipt->size = ns;
} 

void* infostd_dyn_ptab_get(INFOSTD_DYN_PTAB ipt,
			   unsigned long pos) {
  
  if(pos >= ipt->size)
    infostd_dyn_ptab_grow(ipt,pos);

  return ipt->buff[pos];
}

void* infostd_dyn_ptab_set(INFOSTD_DYN_PTAB ipt,
			   unsigned long pos,
			   void *p) {
  
  if(pos >= ipt->size)
    infostd_dyn_ptab_grow(ipt,pos);

  return ipt->buff[pos] = p;
}

void** infostd_dyn_ptab_buff(INFOSTD_DYN_PTAB ipt) {
  
  return ipt->buff;
}

INFOSTD_DYN_PTAB infostd_dyn_ptab_read(FILE* in) {

  INFOSTD_DYN_PTAB ipt;
  unsigned long type, size, st;

  st = fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_PTAB)
    return NULL;

  st = fread(&size,sizeof(unsigned long),1,in);
  
  ipt = infostd_dyn_ptab_init(size);
  st = fread(ipt->buff,sizeof(void *),ipt->size,in);
 
  return ipt;
}

void infostd_dyn_ptab_write(INFOSTD_DYN_PTAB ipt, 
			    FILE* out) {

  unsigned long type = TYPE_INFOSTD_DYN_PTAB;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&ipt->size,sizeof(unsigned long),1,out);
  fwrite(ipt->buff,sizeof(void*),ipt->size,out);
}

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Table of Longs                                                    |
|                                                                             |
*----------------------------------------------------------------------------*/

INFOSTD_DYN_LTAB infostd_dyn_ltab_init(unsigned long size) {

  INFOSTD_DYN_LTAB ilt;

  ilt = (INFOSTD_DYN_LTAB)malloc(SIZE_INFOSTD_DYN_LTAB);
  memset(ilt,0,SIZE_INFOSTD_DYN_LTAB);

  ilt->buff = (unsigned long*)malloc(sizeof(unsigned long)*size);
  memset(ilt->buff,0,sizeof(unsigned long)*size);
  ilt->size = size;

  return ilt;
}

void infostd_dyn_ltab_free(INFOSTD_DYN_LTAB ilt) {

  if(!ilt)
    return;
  
  if(ilt->buff)
    free(ilt->buff);
  free(ilt);
}

void infostd_dyn_ltab_grow(INFOSTD_DYN_LTAB ilt,
			   unsigned long nsize) {
  
  unsigned long* tmp;
  unsigned long ns;
  
  if(!ilt)
    return;
  
  if(nsize < ilt->size)
    return;

  for(ns=ilt->size*2;ns<nsize;ns*=2);

  tmp = (unsigned long*)malloc(sizeof(unsigned long)*ns);
  memset(tmp,0,sizeof(unsigned long)*ns);

  memcpy(tmp,ilt->buff,sizeof(unsigned long)*ilt->size);
  
  free(ilt->buff);
  
  ilt->buff = tmp;
  ilt->size = ns;
} 

unsigned long infostd_dyn_ltab_get(INFOSTD_DYN_LTAB ilt,
				   unsigned long pos) {
  
  if(pos >= ilt->size)
    infostd_dyn_ltab_grow(ilt,pos);
  
  return ilt->buff[pos];
}

unsigned long infostd_dyn_ltab_set(INFOSTD_DYN_LTAB ilt,
				   unsigned long pos,
				   unsigned long l) {
  
  if(pos >= ilt->size)
    infostd_dyn_ltab_grow(ilt,pos);
  
  return ilt->buff[pos] = l;
}

unsigned long* infostd_dyn_ltab_buff(INFOSTD_DYN_LTAB ilt) {
  
  return ilt->buff;
}

INFOSTD_DYN_LTAB infostd_dyn_ltab_read(FILE* in) {

  INFOSTD_DYN_LTAB ilt;
  unsigned long type, size, st;

  st = fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_LTAB)
    return NULL;

  st = fread(&size,sizeof(unsigned long),1,in);
  
  ilt = infostd_dyn_ltab_init(size);
  st = fread(ilt->buff,sizeof(unsigned long),ilt->size,in);
 
  return ilt;
}

void infostd_dyn_ltab_write(INFOSTD_DYN_LTAB ilt, 
			    FILE* out) {

  unsigned long type = TYPE_INFOSTD_DYN_LTAB;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&ilt->size,sizeof(unsigned long),1,out);
  fwrite(ilt->buff,sizeof(unsigned long),ilt->size,out);
}

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Table of Floats                                                   |
|                                                                             |
*----------------------------------------------------------------------------*/

INFOSTD_DYN_FTAB infostd_dyn_ftab_init(unsigned long size) {

  INFOSTD_DYN_FTAB ift;

  ift = (INFOSTD_DYN_FTAB)malloc(SIZE_INFOSTD_DYN_FTAB);
  memset(ift,0,SIZE_INFOSTD_DYN_FTAB);

  ift->buff = (float*)malloc(sizeof(float)*size);
  memset(ift->buff,0,sizeof(float)*size);
  ift->size = size;

  return ift;
}

void infostd_dyn_ftab_free(INFOSTD_DYN_FTAB ift) {

  if(!ift)
    return;
  
  if(ift->buff)
    free(ift->buff);
  free(ift);
}

void infostd_dyn_ftab_grow(INFOSTD_DYN_FTAB ift,
			   unsigned long nsize) {
  
  float* tmp;
  unsigned long ns;
  
  if(!ift)
    return;
  
  if(nsize < ift->size)
    return;

  for(ns=ift->size*2;ns<nsize;ns*=2);

  tmp = (float*)malloc(sizeof(float)*ns);
  memset(tmp,0,sizeof(float)*ns);

  memcpy(tmp,ift->buff,sizeof(float)*ift->size);
  
  free(ift->buff);
  
  ift->buff = tmp;
  ift->size = ns;
} 

float infostd_dyn_ftab_get(INFOSTD_DYN_FTAB ift,
			   unsigned long pos) {
  
  if(pos >= ift->size)
    infostd_dyn_ftab_grow(ift,pos);
  
  return ift->buff[pos];
}

float infostd_dyn_ftab_set(INFOSTD_DYN_FTAB ift,
			   unsigned long pos,
			   float l) {
  
  if(pos >= ift->size)
    infostd_dyn_ftab_grow(ift,pos);
  
  return ift->buff[pos] = l;
}

float* infostd_dyn_ftab_buff(INFOSTD_DYN_FTAB ift) {
  
  return ift->buff;
}

INFOSTD_DYN_FTAB infostd_dyn_ftab_read(FILE* in) {

  INFOSTD_DYN_FTAB ift;
  unsigned long type, size, st;

  st = fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_FTAB)
    return NULL;

  st = fread(&size,sizeof(unsigned long),1,in);
  
  ift = infostd_dyn_ftab_init(size);
  st = fread(ift->buff,sizeof(float),ift->size,in);
 
  return ift;
}

void infostd_dyn_ftab_write(INFOSTD_DYN_FTAB ift, 
			    FILE* out) {

  unsigned long type = TYPE_INFOSTD_DYN_FTAB;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&ift->size,sizeof(unsigned long),1,out);
  fwrite(ift->buff,sizeof(float),ift->size,out);
}

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Table of Doubles                                                  |
|                                                                             |
*----------------------------------------------------------------------------*/

INFOSTD_DYN_DTAB infostd_dyn_dtab_init(unsigned long size) {

  INFOSTD_DYN_DTAB idt;

  idt = (INFOSTD_DYN_DTAB)malloc(SIZE_INFOSTD_DYN_DTAB);
  memset(idt,0,SIZE_INFOSTD_DYN_DTAB);

  idt->buff = (double*)malloc(sizeof(double)*size);
  memset(idt->buff,0,sizeof(double)*size);
  idt->size = size;

  return idt;
}

void infostd_dyn_dtab_free(INFOSTD_DYN_DTAB idt) {

  if(!idt)
    return;
  
  if(idt->buff)
    free(idt->buff);
  free(idt);
}

void infostd_dyn_dtab_grow(INFOSTD_DYN_DTAB idt,
			   unsigned long nsize) {
  
  double* tmp;
  unsigned long ns;
  
  if(!idt)
    return;
  
  if(nsize < idt->size)
    return;

  for(ns=idt->size*2;ns<nsize;ns*=2);

  tmp = (double*)malloc(sizeof(double)*ns);
  memset(tmp,0,sizeof(double)*ns);

  memcpy(tmp,idt->buff,sizeof(double)*idt->size);
  
  free(idt->buff);
  
  idt->buff = tmp;
  idt->size = ns;
} 

double infostd_dyn_dtab_get(INFOSTD_DYN_DTAB idt,
			   unsigned long pos) {
  
  if(pos >= idt->size)
    infostd_dyn_dtab_grow(idt,pos);
  
  return idt->buff[pos];
}

double infostd_dyn_dtab_set(INFOSTD_DYN_DTAB idt,
			   unsigned long pos,
			   double l) {
  
  if(pos >= idt->size)
    infostd_dyn_dtab_grow(idt,pos);
  
  return idt->buff[pos] = l;
}

double* infostd_dyn_dtab_buff(INFOSTD_DYN_DTAB idt) {
  
  return idt->buff;
}

INFOSTD_DYN_DTAB infostd_dyn_dtab_read(FILE* in) {

  INFOSTD_DYN_DTAB idt;
  unsigned long type, size, st;

  st = fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_DTAB)
    return NULL;

  st = fread(&size,sizeof(unsigned long),1,in);
  
  idt = infostd_dyn_dtab_init(size);
  st = fread(idt->buff,sizeof(double),idt->size,in);
 
  return idt;
}

void infostd_dyn_dtab_write(INFOSTD_DYN_DTAB idt, 
			    FILE* out) {

  unsigned long type = TYPE_INFOSTD_DYN_DTAB;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&idt->size,sizeof(unsigned long),1,out);
  fwrite(idt->buff,sizeof(double),idt->size,out);
}

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Table of Characters                                               |
|                                                                             |
*----------------------------------------------------------------------------*/

INFOSTD_DYN_CTAB infostd_dyn_ctab_init(unsigned long size) {

  INFOSTD_DYN_CTAB ict;

  ict = (INFOSTD_DYN_CTAB)malloc(SIZE_INFOSTD_DYN_CTAB);
  memset(ict,0,SIZE_INFOSTD_DYN_CTAB);

  ict->buff = (char*)malloc(sizeof(char)*size);
  memset(ict->buff,0,sizeof(char)*size);
  ict->size = size;

  return ict;
}

void infostd_dyn_ctab_free(INFOSTD_DYN_CTAB ict) {

  if(!ict)
    return;
  
  if(ict->buff)
    free(ict->buff);
  free(ict);
}

void infostd_dyn_ctab_grow(INFOSTD_DYN_CTAB ict,
			   unsigned long nsize) {
  
  char *tmp;
  unsigned long ns;
  
  if(!ict)
    return;
  
  if(nsize < ict->size)
    return;

  for(ns=ict->size*2;ns<nsize;ns*=2);

  tmp = (char*)malloc(sizeof(char)*ns);
  memset(tmp,0,sizeof(char)*ns);

  memcpy(tmp,ict->buff,sizeof(char)*ict->size);
  
  free(ict->buff);

  ict->buff = tmp;
  ict->size = ns;
} 

char infostd_dyn_ctab_get(INFOSTD_DYN_CTAB ict,
			   unsigned long pos) {
  
  if(pos >= ict->size)
    infostd_dyn_ctab_grow(ict,pos);

  return ict->buff[pos];
}

char infostd_dyn_ctab_set(INFOSTD_DYN_CTAB ict,
			  unsigned long pos,
			  char c) {
  
  if(pos >= ict->size)
    infostd_dyn_ctab_grow(ict,pos);

  return ict->buff[pos] = c;
}

char* infostd_dyn_ctab_buff(INFOSTD_DYN_CTAB ict) {
  
  return ict->buff;
}

INFOSTD_DYN_CTAB infostd_dyn_ctab_read(FILE* in) {

  INFOSTD_DYN_CTAB ict;
  unsigned long type, size, st;

  st = fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_CTAB)
    return NULL;

  st = fread(&size,sizeof(unsigned long),1,in);
  
  ict = infostd_dyn_ctab_init(size);
  st = fread(ict->buff,sizeof(void *),ict->size,in);
 
  return ict;
}

void infostd_dyn_ctab_write(INFOSTD_DYN_CTAB ict, 
			    FILE* out) {

  unsigned long type = TYPE_INFOSTD_DYN_CTAB;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&ict->size,sizeof(unsigned long),1,out);
  fwrite(ict->buff,sizeof(char),ict->size,out);
}

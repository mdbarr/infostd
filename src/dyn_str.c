#include <infostd.h>

/* Dynamic String */
INFOSTD_DYN_STR infostd_dyn_str_init(unsigned long size) {

  INFOSTD_DYN_STR ids;

  ids = (INFOSTD_DYN_STR)malloc(SIZE_INFOSTD_DYN_STR);
  memset(ids,0,SIZE_INFOSTD_DYN_STR);

  ids->buff = (unsigned char*)malloc(sizeof(unsigned char)*size);
  memset(ids->buff,0,sizeof(unsigned char)*size);
  ids->size = size;

  return ids;
}

INFOSTD_DYN_STR infostd_dyn_str_init_with_str(unsigned char* str) {

  INFOSTD_DYN_STR ids;
  unsigned len, size;

  len = strlen(str);
  
  size = len+1;

  ids = (INFOSTD_DYN_STR)malloc(SIZE_INFOSTD_DYN_STR);
  memset(ids,0,SIZE_INFOSTD_DYN_STR);

  ids->buff = (unsigned char*)malloc(sizeof(unsigned char)*size);
  memset(ids->buff,0,sizeof(unsigned char)*size);
  ids->size = size;
  ids->length = len;

  strcpy(ids->buff,str);

  return ids;
}

INFOSTD_DYN_STR infostd_dyn_str_clone(INFOSTD_DYN_STR ids) {

  INFOSTD_DYN_STR tmp;
  
  if(!ids)
    return NULL;
 
  tmp = (INFOSTD_DYN_STR)malloc(SIZE_INFOSTD_DYN_STR);
  memcpy(tmp,ids,SIZE_INFOSTD_DYN_STR);
 
  tmp->buff = strdup(ids->buff);
 
  return tmp;
}

void infostd_dyn_str_free(INFOSTD_DYN_STR ids) {

  if(!ids)
    return;
  
  if(ids->buff)
    free(ids->buff);
  free(ids);
}

void infostd_dyn_str_grow(INFOSTD_DYN_STR ids,
			  unsigned long nsize) {
  
  unsigned char* tmp;
  unsigned long ns;
  
  if(!ids)
    return;
  
  if(nsize < ids->size)
    return;

  /* Increase the string size by a factor of 2 until it is big enough */
  for(ns=ids->size*2;ns<nsize;ns*=2);

  tmp = (unsigned char*)malloc(sizeof(unsigned char)*ns);
  memset(tmp,0,sizeof(unsigned char)*ns);

  memcpy(tmp,ids->buff,sizeof(unsigned char)*ids->size);
  
  free(ids->buff);

  ids->buff = tmp;
  ids->size = ns;
} 

unsigned long infostd_dyn_str_strcpy(INFOSTD_DYN_STR ids,
				     unsigned char* str) {

  unsigned long len;
  len = strlen(str);
  
  if(len>=ids->size)
    infostd_dyn_str_grow(ids,len);
  
  strcpy(ids->buff,str);
  ids->length = len;

  return ids->length;
}

unsigned long infostd_dyn_str_strncpy(INFOSTD_DYN_STR ids,
				      unsigned char* str,
				      unsigned long len) {
  
  if(len>=ids->size)
    infostd_dyn_str_grow(ids,len);
  
  strncpy(ids->buff,str,len);
  ids->length = len;

  return ids->length;
}

unsigned long infostd_dyn_str_strcat(INFOSTD_DYN_STR ids,
				     unsigned char* str) {

  unsigned long len;
  len = strlen(str);
  
  if(ids->length+len>=ids->size)
    infostd_dyn_str_grow(ids,ids->length+len);
  
  strcpy(ids->buff+ids->length,str);
  ids->length += len;

  return ids->length;
}

unsigned long infostd_dyn_str_strncat(INFOSTD_DYN_STR ids,
				      unsigned char* str,
				      unsigned long len) {
  
  if(ids->length+len>=ids->size)
    infostd_dyn_str_grow(ids,ids->length+len);
  
  strncpy(ids->buff+ids->length,str,len);
  ids->length += len;

  return ids->length;
}

unsigned long infostd_dyn_str_addchar(INFOSTD_DYN_STR ids,
				      unsigned char c) {
  
  if(ids->length+1>=ids->size)
    infostd_dyn_str_grow(ids,ids->length+1);
  
  ids->buff[ids->length] = c;
  ids->length++;
  return ids->length;
}

long infostd_dyn_str_replace(INFOSTD_DYN_STR ids,
			     unsigned long start,
			     unsigned long end,
			     unsigned char* str) {

  unsigned long i, len, rlen;
  long offset;

  len = strlen(str);
  rlen = end-start;
  offset = len-rlen;

  if(offset > 0 && ids->length+offset>=ids->size)
    infostd_dyn_str_grow(ids,ids->length+offset);
  
  if(offset > 0) {
    for(i=ids->length;i>start;i--) {
      if(i >= 0 && i < ids->size && i+offset >= 0 && i+offset < ids->size)
	ids->buff[i+offset] = ids->buff[i];
    }
  } else if(offset < 0) {
    for(i=start;i<=ids->length;i++) {
      if(i >= 0 && i < ids->size && i+offset >= 0 && i+offset < ids->size &&
	 i+offset >= start)
	ids->buff[i+offset] = ids->buff[i];
    }
  }
  strncpy(ids->buff+start,str,len);

  ids->length += offset;

  return offset;
}

void infostd_dyn_str_insert(INFOSTD_DYN_STR ids,
			    char* str,
			    unsigned long pos) {
  unsigned long i, len;
  len = strlen(str);

  if(len < 1) {
    return;
  }

  if(len + ids->length >= ids->size) {
    infostd_dyn_str_grow(ids, ids->length + len + 1);
  }

    for(i = ids->length; i >= pos; i--) {
      ids->buff[i + len] = ids->buff[i];
    }
    
    for(i = 0; i < len; i++) {
      ids->buff[pos + i] = str[i];
    }

    ids->length += len;
}
  
void infostd_dyn_str_clear(INFOSTD_DYN_STR ids) {

  if(ids->buff)
    memset(ids->buff,0,ids->size);

  ids->length = 0;
}

unsigned char* infostd_dyn_str_buff(INFOSTD_DYN_STR ids) {

  if(!ids)
    return NULL;

  return ids->buff;
}

unsigned long infostd_dyn_str_length(INFOSTD_DYN_STR ids) {
  
  return ids->length;
}

int infostd_dyn_str_fgets(INFOSTD_DYN_STR ids,
			  FILE* in) {

  int c;

  infostd_dyn_str_clear(ids);

  if(feof(in))
    return 0;

  while((c=getc(in)) != EOF && c != '\0' &&  c != '\n' && c != '\r')
    infostd_dyn_str_addchar(ids,(unsigned char)c);

  if(c == EOF && ids->length == 0)
    return 0;
  
  return 1;
}

void infostd_dyn_str_lower(INFOSTD_DYN_STR ids) {
  
  unsigned long i;

  for(i=0;i<ids->length;i++) 
    ids->buff[i] = tolower(ids->buff[i]);
}

void infostd_dyn_str_upper(INFOSTD_DYN_STR ids) {

  unsigned long i;
  
  for(i=0;i<ids->length;i++) 
    ids->buff[i] = tolower(ids->buff[i]);
}

void infostd_dyn_str_printf(INFOSTD_DYN_STR ids,
			    char* format,
			    ...) {

  unsigned long len, i;
  char tmp[64];
  va_list ap;
  int *p;


  len = strlen(format);
  
  infostd_dyn_str_clear(ids);

  va_start(ap,format);
  for(i=0;i<len;i++) {
    if(format[i] == '%' && i<len-1) {
      i++;
      switch (format[i]) {
      
      case 'a':
	snprintf(tmp,63,"%a",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'A':
	snprintf(tmp,63,"%A",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'c':
	snprintf(tmp,63,"%c",(unsigned char)va_arg(ap,int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'i':
      case 'd':
	snprintf(tmp,63,"%d",va_arg(ap,int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'e':
	snprintf(tmp,63,"%e",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'E':
	snprintf(tmp,63,"%E",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'f':
	snprintf(tmp,63,"%f",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'F':
	snprintf(tmp,63,"%F",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'g':
	snprintf(tmp,63,"%g",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'G':
	snprintf(tmp,63,"%G",va_arg(ap,double));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'o':
	snprintf(tmp,63,"%o",va_arg(ap,unsigned int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'p':
	snprintf(tmp,63,"%p",va_arg(ap,void *));
	infostd_dyn_str_strcat(ids,tmp);
	break;
	
      case 's':
	infostd_dyn_str_strcat(ids,va_arg(ap,char*));
	break;

      case 'u':
	snprintf(tmp,63,"%u",va_arg(ap,unsigned int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'x':
	snprintf(tmp,63,"%x",va_arg(ap,unsigned int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'X':
	snprintf(tmp,63,"%X",va_arg(ap,unsigned int));
	infostd_dyn_str_strcat(ids,tmp);
	break;

      case 'l':
	if(i<len-1) {
	  i++;
	  switch (format[i]) {	
	    
	  case 'i':
	  case 'd':
	    snprintf(tmp,63,"%ld",va_arg(ap,long));
	    infostd_dyn_str_strcat(ids,tmp);
	    break;

	  case 'f':
	    snprintf(tmp,63,"%lf",va_arg(ap,double));
	    infostd_dyn_str_strcat(ids,tmp);
	    break;

	  case 'u':
	    snprintf(tmp,63,"%lu",va_arg(ap,unsigned long));
	    infostd_dyn_str_strcat(ids,tmp);
	    break;	
	  }
	}
	break;

      case 'n':
	p = va_arg(ap,int *);
	*p = ids->length;
	break;

      default:
	infostd_dyn_str_addchar(ids,format[i]);
	break;

      }
    } else
      infostd_dyn_str_addchar(ids,format[i]);
  }
  va_end(ap);
}

unsigned long infostd_dyn_str_delchar(INFOSTD_DYN_STR ids) {

  if(ids->length > 0) {
    ids->length--;
    ids->buff[ids->length] = 0;
  }
  return ids->length;
}

unsigned long infostd_dyn_str_addchar_at(INFOSTD_DYN_STR ids, char c, int p) {

  unsigned long i;

  if(ids->length > 0) {
    if(ids->length + 1 >= ids->size)
      infostd_dyn_str_grow(ids,ids->length + 1);

    for(i=ids->length; i > 0 && i >= p; i--) {
      ids->buff[i] = ids->buff[i-1];
    }

  }
  ids->buff[p] = c;
  ids->length++;
  
  return ids->length;
}

unsigned long infostd_dyn_str_delchar_at(INFOSTD_DYN_STR ids, int p) {

    unsigned long i;

    if(ids->length > 0 && p >= 0) {
      for(i=p; i < ids->length; i++) {
	ids->buff[i] = ids->buff[i+1];
      }
      ids->length--;
      ids->buff[ids->length] = 0;
    }
    return ids->length;
}

unsigned long infostd_dyn_str_kill_from(INFOSTD_DYN_STR ids, int p) {
  
  unsigned long i;

  if(ids->length && p < ids->length) {
    for(i=p; i < ids->length; i++) {
      ids->buff[i] = 0;
    }
  }
  ids->length = p;

  return ids->length;
}

void infostd_dyn_str_write(INFOSTD_DYN_STR ids,
			   FILE* out) {

  unsigned long type;
  
  type = TYPE_INFOSTD_DYN_STR;

  fwrite(&type,sizeof(unsigned long),1,out);
  fwrite(&ids->length,sizeof(unsigned long),1,out);
  fwrite(ids->buff,sizeof(char),ids->length,out);
}

INFOSTD_DYN_STR infostd_dyn_str_read(FILE* in) {

  INFOSTD_DYN_STR ids = NULL;
  unsigned long type, size;

  fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_DYN_STR)
    return NULL;

  fread(&size,sizeof(unsigned long),1,in);
  
  ids = infostd_dyn_str_init(size+1);
  fread(ids->buff,sizeof(char),size,in);

  ids->length = size;

  return ids;
}


#include <infostd.h>

unsigned char* INFOSTD_TOKENIZER_DEFAULT_SET = "[A-Za-z0-9\\-]";
unsigned char* INFOSTD_TOKENIZER_DEFAULT_REGEXP = "/(\\w+(\'s|\'t|\'nt)?|\\u.)/";

unsigned long infostd_token_tab_init(unsigned char* tab,
				     unsigned char* set) {


  unsigned char f, l, *ln;
  unsigned long i, num;
  int n;

  memset(tab,0,sizeof(unsigned char)*256);
  
  i = num = f = l = n = 0;
  ln=set;

  if(*ln!='[') {

    tab[(int)*ln]=1;
    num++;
  } else {
    ln++;

    if(*ln=='^') {
      n=1;
      ln++;
    }
    
    while(*ln!=']' && *ln!='\0'){

      if(*ln=='\\') {
	ln++;
	switch (*ln) {

	case '0':
	  ln++;
	  f='\0';
	  break;

	case 'n':
	  ln++;
	  f='\n';
	  break;

	case 't':
	  ln++;
	  f='\t';
	  break;

	case 'a':
	  ln++;
	  f='\a';
	  break;

	case 'r':
	  ln++;
	  f='\r';
	  break;

	case 'f':
	  ln++;
	  f='\f';
	  break;

	case 'e':
	  ln++;
	  f='\033';
	  break;

	case ']':
	case '[':
	case '-':
	case '\\':
	  f=*ln++;
	  break;

	case 'd':
	  f=0;
	  for(i='0';i<='9';i++) {
	    if(!tab[i]) {
	      tab[i]=1;
	      num++;
	    }
	  }
	  ln++;
	  break;

	case 'D':
	  for(f=0,i=1;i<253;i++) {
	    if(i<'0' || i>'9') {
	      if(!tab[i]){
		tab[i]=1;
		num++;
	      }
	    }
	  }
	  ln++;
	  break;
	case 'w':
	  for(f=0,i='a';i<='z';i++) {
	    if(!tab[i]) {
	      tab[i]=1;
	      num++;
	    }
	  }

	  for(i='A';i<='Z';i++) {
	    if(!tab[i]) {
	      tab[i]=1;
	      num++;
	    }
	  }

	  for(i='0';i<='9';i++) {
	    if(!tab[i]) {
	      tab[i]=1;
	      num++;
	    }
	  }

	  if(!tab['_']) {
	    tab['_']=1;
	    num++;
	  }

	  ln++;
	  break;

	case 'W':
	  for(f=0,i=1;i<253;i++) {
	    if(i<'0' ||
	       (i>'9' && i<'A') ||
	       (i>'Z' && i<'_') ||
	       (i>'_' && i<'a') ||
	       i>'z'){
	      if(!tab[i]) {
		tab[i]=1;
		num++;
	      }
	    }
	  }

	  ln++;
	  break;

	case 's':
	  f=0;

	  if(!tab[' ']) {
	    tab[' ']=1;
	    num++;
	  }

	  if(!tab['\t']) {
	    tab['\t']=1;
	    num++;
	  }

	  if(!tab['\n']) {
	    tab['\n']=1;
	    num++;
	  }

	  if(!tab['\r']) {
	    tab['\r']=1;
	    num++;
	  }

	  if(!tab['\f']) {
	    tab['\f']=1;
	    num++;
	  }

	  ln++;
	  break;

	case 'S':
	  f=0;
	  for(i=1;i<253;i++) {
	    if(i!=' ' &&
	       i!='\t' &&
	       i!='\n' &&
	       i!='\r' &&
	       i!='\f'){
	      if(!tab[i]){
		tab[i]=1;
		num++;
	      }
	    }
	  }
	  ln++;
	  break;

	default:
	  f=*ln++;
	  break;
	}

      } else
	f=*ln++;

      if(*ln=='-') {
	ln++;

	if(*ln=='\\') {
	  ln++;

	  switch (*ln) {

	  case '0':
	    ln++;
	    l='\0';
	    break;

	  case 'n':
	    ln++;
	    l='\n';
	    break;

	  case 't':
	    ln++;
	    l='\t';
	    break;

	  case 'a':
	    ln++;
	    f='\a';
	    break;

	  case 'r':
	    ln++;
	    f='\r';
	    break;

	  case 'f':
	    ln++;
	    f='\f';
	    break;

	  case 'e':
	    ln++;
	    f='\033';
	    break;

	  case ']':
	  case '[':
	  case '^':
	  case '-':
	  case '\\':
	    l=*ln++;
	    break;

	  default:
	    l=*ln++;
	    break;
	  }

	} else
	  l=*ln++;

	if(f) {
	  for(i=f;i<=l;i++){
	    if(!tab[i]){
	      tab[i]=1;
	      num++;
	    }
	  }
	}
      } else {
	if(f) {
	  if(!tab[(int)f]) {
	    tab[(int)f]=1;
	    num++;
	  }
	}
      }
    }
  }

  if(n) {
    for(num=0,i=0;i<256;i++){
      if(tab[i]){
	tab[i]=0;
      } else{
	if(i>0 && i<253){
	  tab[i]=1;
	  num++;
	}
      }
    }
  }

  return num;
}

INFOSTD_TOKEN infostd_token_init(INFOSTD_TOKENIZER tkz) {

  INFOSTD_TOKEN itk;
  

  itk = (INFOSTD_TOKEN)malloc(SIZE_INFOSTD_TOKEN);
  memset(itk,0,SIZE_INFOSTD_TOKEN);
  
  itk->ids = infostd_dyn_str_init(100);

  itk->type = INFOSTD_TOKEN_NONE;
  itk->parent_type = tkz->type;

  if(tkz->type == INFOSTD_TOKENIZER_SET) {
    itk->data0 = (void*)malloc(sizeof(unsigned char)*256);
    infostd_token_tab_init((unsigned char*)itk->data0,tkz->data->buff);    
  } else if(tkz->type == INFOSTD_TOKENIZER_REGEXP) {  
    itk->data0 = infostd_regexp_engine_init();
    itk->data1 = infostd_regexp_result_init();
  }

  return itk;
}

void infostd_token_free(INFOSTD_TOKEN itk) {

  if(itk->ids)
    infostd_dyn_str_free(itk->ids);

  if(itk->data0) {
    if(itk->parent_type == INFOSTD_TOKENIZER_SET) {
      if(itk->data0)
	free(itk->data0);
    } else if(itk->parent_type == INFOSTD_TOKENIZER_REGEXP)
      if(itk->data0)
	infostd_regexp_engine_free((INFOSTD_REGEXP_ENGINE)itk->data0);
      if(itk->data1)
	infostd_regexp_result_free((INFOSTD_REGEXP_RESULT)itk->data1);
      if(itk->data2)
	free(itk->data2);
  }
  
  free(itk);
}

INFOSTD_TOKENIZER infostd_tokenizer_init_set(unsigned char* set) {

  INFOSTD_TOKENIZER tkz;

  tkz = (INFOSTD_TOKENIZER)malloc(SIZE_INFOSTD_TOKENIZER);
  memset(tkz,0,SIZE_INFOSTD_TOKENIZER);

  tkz->type = INFOSTD_TOKENIZER_SET;

  tkz->data = infostd_dyn_str_init_with_str(set);

  tkz->token_first = infostd_token_first_set;
  tkz->token_next = infostd_token_next_set;

  return tkz;
}

INFOSTD_TOKENIZER infostd_tokenizer_init_regexp(unsigned char* regexp) {

  INFOSTD_TOKENIZER tkz;
  
  tkz = (INFOSTD_TOKENIZER)malloc(SIZE_INFOSTD_TOKENIZER);
  memset(tkz,0,SIZE_INFOSTD_TOKENIZER);
  
  tkz->type = INFOSTD_TOKENIZER_REGEXP;

  tkz->data = infostd_dyn_str_init_with_str(regexp);

  tkz->token_first = infostd_token_first_regexp;
  tkz->token_next = infostd_token_next_regexp;

  return tkz;
}

void infostd_tokenizer_free(INFOSTD_TOKENIZER tkz) {
  
  if(tkz->data)
    infostd_dyn_str_free(tkz->data);

  free(tkz);
}


INFOSTD_TOKEN infostd_token_first_set(INFOSTD_TOKENIZER tkz,
				      INFOSTD_TOKEN itk,
				      unsigned char* str) {

  itk->pos=0;
  return infostd_token_next_set(tkz,itk,str);
}

INFOSTD_TOKEN infostd_token_next_set(INFOSTD_TOKENIZER tkz,
				     INFOSTD_TOKEN itk,
				     unsigned char* str) {

  unsigned long pos, start, end, i, len;
  char* tab;

  tab = (unsigned char*)itk->data0;
  len = strlen(str);
  pos = itk->pos;

  if(pos >= len)
    return NULL;

  if(tab[str[pos]]) {
    itk->type = INFOSTD_TOKEN_WORD;
    for(i=pos,start=pos;i<len&&tab[str[i]];i++);
    end=i;    
  } else {
    itk->type = INFOSTD_TOKEN_SEP;
    for(i=pos,start=pos;i<len&&!(tab[str[i]]);i++);
    end=i;
  }

  infostd_dyn_str_clear(itk->ids);
  infostd_dyn_str_strncpy(itk->ids,str+start,end-start);

  itk->start = start;
  itk->end = end;
  itk->pos = end;

  return itk;
}

INFOSTD_TOKEN infostd_token_first_regexp(INFOSTD_TOKENIZER tkz,
					 INFOSTD_TOKEN itk,
					 unsigned char* str) {
  

  INFOSTD_REGEXP_ENGINE eng;
  INFOSTD_REGEXP_RESULT res;
  unsigned long len, pos,i, start, end;
  unsigned char *tab;

  if(itk->data2)
    free(itk->data2);

  len = strlen(str);
  tab = (unsigned char*)malloc(sizeof(unsigned char)*len);
  memset(tab,0,sizeof(unsigned char)*len);

  eng = (INFOSTD_REGEXP_ENGINE)itk->data0;
  res = (INFOSTD_REGEXP_RESULT)itk->data1;
  itk->data2 = tab;
  
  pos = i = start = end = 0;

  while(infostd_regexp(eng,res,str+pos,len-pos,tkz->data->buff)) {
    if(!res->type == INFOSTD_REGEXP_MATCH)
      break;

    start = infostd_dyn_ltab_get(res->positions,0) + pos;
    end = infostd_dyn_ltab_get(res->positions,1) + pos;
    
    for(i=start;i<end;i++)
      tab[i]=1;

    pos = end;
    
    if(pos >= len)
      break;
  }

  itk->pos=0;
  return infostd_token_next_set(tkz,itk,str);
}

INFOSTD_TOKEN infostd_token_next_regexp(INFOSTD_TOKENIZER tkz,
					INFOSTD_TOKEN itk,
					unsigned char* str) {
  

  unsigned long pos, start, end, i, len;
  char* tab;

  tab = (unsigned char*)itk->data2;
  len = strlen(str);
  pos = itk->pos;

  if(pos >= len)
    return NULL;

  if(tab[pos]) {
    itk->type = INFOSTD_TOKEN_WORD;
    for(i=pos,start=pos;i<len&&tab[i];i++);
    end=i;    
  } else {
    itk->type = INFOSTD_TOKEN_SEP;
    for(i=pos,start=pos;i<len&&!(tab[i]);i++);
    end=i;
  }

  infostd_dyn_str_clear(itk->ids);
  infostd_dyn_str_strncpy(itk->ids,str+start,end-start);

  itk->start = start;
  itk->end = end;
  itk->pos = end;

  return itk;
}

void infostd_tokenizer_write(INFOSTD_TOKENIZER tkz,
			     FILE* out) {

  unsigned long type = TYPE_INFOSTD_TOKENIZER;
  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&tkz->type,sizeof(int),1,out);
  fwrite(&tkz->data->length,sizeof(unsigned long),1,out);
  fwrite(tkz->data->buff,sizeof(char),tkz->data->length,out);
}

INFOSTD_TOKENIZER infostd_tokenizer_read(FILE* in) {

  INFOSTD_TOKENIZER tkz;
  
  unsigned long type, len;

  fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_TOKENIZER)
    return NULL;

  tkz = (INFOSTD_TOKENIZER)malloc(SIZE_INFOSTD_TOKENIZER);
  memset(tkz,0,SIZE_INFOSTD_TOKENIZER);
  
  fread(&tkz->type,sizeof(int),1,in);

  fread(&len,sizeof(unsigned long),1,in);

  tkz->data = infostd_dyn_str_init(len+1);
  fread(tkz->data->buff,sizeof(char),len,in);

  if(tkz->type == INFOSTD_TOKENIZER_SET) {
    tkz->token_first = infostd_token_first_set;
    tkz->token_next = infostd_token_next_set;
  } else if(tkz->type == INFOSTD_TOKENIZER_REGEXP) {
    tkz->token_first = infostd_token_first_regexp;
    tkz->token_next = infostd_token_next_regexp;
  }

  return tkz;
}

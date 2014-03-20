#include <infostd.h>

INFOSTD_REGEXP_ENGINE infostd_regexp_engine_init(void) {
  
  INFOSTD_REGEXP_ENGINE eng;
  
  eng = malloc(SIZE_INFOSTD_REGEXP_ENGINE);
  memset(eng,0,SIZE_INFOSTD_REGEXP_ENGINE);

  eng->pat = infostd_dyn_str_init(100);
  eng->subpat = infostd_dyn_str_init(100);
  eng->nspat = infostd_dyn_str_init(100);
  eng->str = infostd_dyn_str_init(100);

  memset(eng->opt,0,26);

  return eng;
}

void infostd_regexp_engine_free(INFOSTD_REGEXP_ENGINE eng) {

  regfree(&eng->rx_comp);

  if(eng->pat)
    infostd_dyn_str_free(eng->pat);
  if(eng->subpat)
    infostd_dyn_str_free(eng->subpat);
  if(eng->nspat)
    infostd_dyn_str_free(eng->nspat);
  if(eng->str)
    infostd_dyn_str_free(eng->str);
  
  free(eng);
}

void infostd_regexp_engine_clear(INFOSTD_REGEXP_ENGINE eng) {

  regfree(&eng->rx_comp);

  if(eng->pat)
    infostd_dyn_str_clear(eng->pat);
  if(eng->subpat)
    infostd_dyn_str_clear(eng->subpat);
  if(eng->nspat)
    infostd_dyn_str_clear(eng->nspat);
  if(eng->str)
    infostd_dyn_str_clear(eng->str);

  memset(eng->opt,0,26);
}

INFOSTD_REGEXP_RESULT infostd_regexp_result_init(void) {

  INFOSTD_REGEXP_RESULT res;
  
  res = malloc(SIZE_INFOSTD_REGEXP_RESULT);
  memset(res,0,SIZE_INFOSTD_REGEXP_RESULT);

  res->result = infostd_dyn_str_init(100);
  res->positions = infostd_dyn_ltab_init(100);
  
  return res;
}

int infostd_regexp_result_error(INFOSTD_REGEXP_RESULT res,
				char *s) {  

  res->type = INFOSTD_REGEXP_ERROR;
  infostd_dyn_str_strcpy(res->result,s);
  res->num_matches=0;

  return 0;
}

int infostd_regexp_result_nomatch(INFOSTD_REGEXP_RESULT res) {
  
  res->type = INFOSTD_REGEXP_NOMATCH;
  res->num_matches=0;

  return 0;
}

void infostd_regexp_result_print(INFOSTD_REGEXP_RESULT res) {  

  unsigned long i, len;

  switch (res->type) {
  case INFOSTD_REGEXP_NONE:
    printf("Result Type: None\n");
    return;
    break;
  case INFOSTD_REGEXP_NOMATCH:
    printf("Result Type: No Match\n");
    return;
    break;
  case INFOSTD_REGEXP_ERROR:
    printf("Result Type: Error\n");
    printf("%s\n",res->result->buff);
    return;
    break;
  case INFOSTD_REGEXP_SUBSTITUTION:
    printf("Result Type: Substitution\n");
    printf("%s\n",res->result->buff);
    return;
    break;
  case INFOSTD_REGEXP_MATCH:
    printf("Result Type: Match\n");
    for(i=0,len=0;i<res->num_matches;i++) {
      printf("  %s (%lu/%lu)\n",res->result->buff+len,
	     infostd_dyn_ltab_get(res->positions,i*2),
	     infostd_dyn_ltab_get(res->positions,(i*2)+1));
      len+=strlen(res->result->buff+len)+1;
    }
    return;
    break;
  }
}

void infostd_regexp_result_clear(INFOSTD_REGEXP_RESULT res) {  

  res->type = INFOSTD_REGEXP_NONE;

  if(res->result)
    infostd_dyn_str_clear(res->result);
  
  res->num_matches=0;
}

void infostd_regexp_result_free(INFOSTD_REGEXP_RESULT res) {  

  if(res->result)
    infostd_dyn_str_free(res->result);
  if(res->positions)
    infostd_dyn_ltab_free(res->positions);
  free(res);
}

void infostd_regexp_make_pattern(INFOSTD_REGEXP_ENGINE eng,
				 char* s,
				 int len) {
  
  int i, inbr;

  infostd_dyn_str_clear(eng->pat);

  for(i=0,inbr=0;i<len;i++) {
    
    if(s[i] == '[')
      inbr=1;
    if(s[i] == ']')
      inbr=0;
    
    if(i<len-1 && s[i] == '\\') {
      i++;
      
      if(!inbr)
	infostd_dyn_str_strcat(eng->pat,"[");

      switch (s[i]) {
      case 'S':
	if(!inbr) infostd_dyn_str_addchar(eng->pat,'^');
      case 's':
	infostd_dyn_str_strcat(eng->pat,"[:space:]");
	break;
      case 'W':
	if(!inbr) infostd_dyn_str_addchar(eng->pat,'^');
      case 'w':
	infostd_dyn_str_strcat(eng->pat,"[:alpha:]");
	infostd_dyn_str_strcat(eng->pat,"[:digit:]");
	infostd_dyn_str_addchar(eng->pat,'-');
	break;
      case 'D':
	if(!inbr) infostd_dyn_str_addchar(eng->pat,'^');
      case 'd':
	infostd_dyn_str_strcat(eng->pat,"[:digit:]");
	break;
      case 'l':
	infostd_dyn_str_strcat(eng->pat,"a-z");
	break;
      case 'u':
	infostd_dyn_str_strcat(eng->pat,"A-Z");
	break;
      default:
	infostd_dyn_str_addchar(eng->pat,'\\');
	infostd_dyn_str_addchar(eng->pat,s[i]);
      }
      if(!inbr)
	infostd_dyn_str_strcat(eng->pat,"]"); 
    } else 
      infostd_dyn_str_addchar(eng->pat,s[i]);
  }
}

void infostd_regexp_make_substitution(INFOSTD_REGEXP_ENGINE eng,
				      char* sub,      
				      unsigned long len,
				      char* orig) {

  unsigned long i, d, s, e;

  infostd_dyn_str_clear(eng->nspat);
  
  for(i=0;i<len;i++) {
    if(i<len-1 && sub[i] == '$' && isdigit(sub[i+1])) {
      for(s=i,e=i+1;e<len&&isdigit(sub[e]);e++);
      d=atol(sub+s+1);  
      if(d < REGEXP_MAX_MATCH && eng->pmatch[d].rm_so!=-1 && 
	 eng->pmatch[d].rm_eo != -1) {
	infostd_dyn_str_strncat(eng->nspat,orig+eng->pmatch[d].rm_so,
				eng->pmatch[d].rm_eo-eng->pmatch[d].rm_so);
      }
      i=e-1;
    } else
      infostd_dyn_str_addchar(eng->nspat,sub[i]);
  }
}
  
int infostd_regexp(INFOSTD_REGEXP_ENGINE eng,
		   INFOSTD_REGEXP_RESULT res,
		   char *s, 
		   unsigned long slen,
		   char *r) { 
  
  unsigned char sub=0, global=0;
  char *ln, *ln2, *ln3;
  int i, n, len, flags, matches;
  long offset;

  infostd_regexp_result_clear(res);
  infostd_regexp_engine_clear(eng);

  if(r[0]=='s')
    sub=1;
  
  /* Find the start and end */
  if((ln=strchr(r,'/')) && 
     (ln2=strrchr(r,'/')) &&
     ln != ln2) {

    /* Copy out the flags */
    strncpy(eng->opt,ln2+1,25);
    
    flags = REG_EXTENDED;

    /* Parse the flags */
    for(i=0,len=strlen(eng->opt);i<len;i++) {
      switch (eng->opt[i]) {
      case 'i':
        /* REG_ICASE */
	flags |= REG_ICASE;
	break;
      case 'g':
	/* REG_STARTEND if !REG_NEWLINE*/
	global=1;
	break;
      case 's':
	/* !REG_NEWLINE */
	flags ^= REG_NEWLINE;
	break;
      case 'm':
	/* REG_NEWLINE */
	flags |= REG_NEWLINE;
	break;
      default:
	return infostd_regexp_result_error(res,"unknown flag in options");
      }
    }
    
    /* Check the start */
    if((sub && ln-r!=1) ||
       (!sub && ln!=r))
      return infostd_regexp_result_error(res,"garbage before pattern start");
    
    /* Goto start of pattern */
    ln++;
    
    if(sub) {
      /* A substitution type pattern */
      for(ln3=ln;ln3<ln2;ln3++)
	if(*ln3=='/' && *(ln3-1)!='\\')
	  break;

      if(!ln3 || ln3 == ln2)
	return infostd_regexp_result_error(res,"invalid substitution pattern");
      
      /* Needs a pattern rewriter for perl type patterns 
	 e for extended, subs in vars.
	 x for ignore whitespace? 
	 \s = [:space:]
	 alpha, digit, alnum
      */
      infostd_regexp_make_pattern(eng,ln,ln3-ln);
      ln3++;
      infostd_dyn_str_strncpy(eng->subpat,ln3,ln2-ln3);
      infostd_dyn_str_strcpy(eng->str,s);
      
      if(regcomp(&eng->rx_comp,eng->pat->buff,flags) < 0) 
	return infostd_regexp_result_error(res,"regexp compilation error");

      if(global) {
	offset=matches=len=0;

	while((n=regexec(&eng->rx_comp, s+len, REGEXP_MAX_MATCH, 
			 eng->pmatch, 0)) != REG_NOMATCH) {
	  if(n < 0) 
	    return infostd_regexp_result_error(res,"regexp execution error");
	 
	  infostd_regexp_make_substitution(eng,eng->subpat->buff,
					   eng->subpat->length,s+len);

	  offset += infostd_dyn_str_replace(eng->str,
					    eng->pmatch[0].rm_so+offset+len,
					    eng->pmatch[0].rm_eo+offset+len,
					    eng->nspat->buff);

	  infostd_dyn_ltab_set(res->positions,matches*2,eng->pmatch[0].rm_so);

	  infostd_dyn_ltab_set(res->positions,
			       (matches*2)+1,
			       eng->pmatch[0].rm_eo);

	  matches++;
	  len+=eng->pmatch[0].rm_eo;
	  
	  if(len >= slen)
	    break;
	}
	
	if(matches) {
	  res->type = INFOSTD_REGEXP_SUBSTITUTION;
	  infostd_dyn_str_strncpy(res->result,eng->str->buff,
				  eng->str->length);
	  res->num_matches=matches;
	  return res->num_matches;

	} 
	return infostd_regexp_result_nomatch(res);
	
      } else {
	n=regexec(&eng->rx_comp, s, REGEXP_MAX_MATCH, eng->pmatch, 0);

	if(n == 0) {
	  
	  infostd_regexp_make_substitution(eng,eng->subpat->buff,
					   eng->subpat->length,s);

	  infostd_dyn_str_replace(eng->str,eng->pmatch[0].rm_so,
				  eng->pmatch[0].rm_eo, eng->nspat->buff);

	  infostd_dyn_ltab_set(res->positions,0,eng->pmatch[0].rm_so);
	  infostd_dyn_ltab_set(res->positions,1,eng->pmatch[0].rm_eo);
	 
	  res->type = INFOSTD_REGEXP_SUBSTITUTION;
	  infostd_dyn_str_strncpy(res->result,eng->str->buff,
				  eng->str->length);
	  return res->num_matches=1;
	} else if(n == REG_NOMATCH) {
	  return infostd_regexp_result_nomatch(res);
	} else 
	  return infostd_regexp_result_error(res,"regexp execution error");
      }
    } else {
      /* A matching pattern */
      infostd_regexp_make_pattern(eng,ln,ln2-ln);

      if(regcomp(&eng->rx_comp,eng->pat->buff,flags) < 0) 
	return infostd_regexp_result_error(res,"regexp compilation error");

      n=regexec(&eng->rx_comp, s, REGEXP_MAX_MATCH, eng->pmatch, 0);

      if(n == 0) {
	res->type = INFOSTD_REGEXP_MATCH;
	
	for(i=0;i<REGEXP_MAX_MATCH;i++) {
	  if(eng->pmatch[i].rm_so != -1) {
	    if(res->result->length > 0)
	      infostd_dyn_str_addchar(res->result,'\0');
	    infostd_dyn_str_strncat(res->result,s+eng->pmatch[i].rm_so,
				    eng->pmatch[i].rm_eo - 
				    eng->pmatch[i].rm_so);

	    infostd_dyn_ltab_set(res->positions,
				 res->num_matches*2,
				 eng->pmatch[i].rm_so);
	    infostd_dyn_ltab_set(res->positions,
				 (res->num_matches*2)+1,
				 eng->pmatch[i].rm_eo);
	    
	    res->num_matches++;
	  }
	}
	return res->num_matches;
      } else if(n == REG_NOMATCH)
	return infostd_regexp_result_nomatch(res);
      else
	return infostd_regexp_result_error(res,"regexp execution error");
    }
  } else
    return infostd_regexp_result_error(res,"invalid pattern");

  return infostd_regexp_result_nomatch(res);
}

#include <infostd.h>

#define INFOSTD_METAPHONE_VOWEL(x) ((x=='e'||x=='i'||x=='a'||x=='o'||x=='u')?1:0)

unsigned long INFOSTD_METAPHONE_MAX = 20;
unsigned long INFOSTD_SPELLER_SUGGESTION_MAX = 5;

/*----------------------------------------------------------------------------*
|                                                                             |
|   Metaphone Algorithm as defined by Lawrence Philips and modified by        |
|   Michael Kuhn                                                              |
|                                                                             |
*----------------------------------------------------------------------------*/
unsigned long infostd_speller_metaphone(unsigned char* str,
					unsigned long len,
					INFOSTD_DYN_STR mstr) {
  
  unsigned long i;
  unsigned char d, d1, c, c1, c2, p, skip;

  infostd_dyn_str_clear(mstr);

  for(i=0,p='\0';i<len && i < INFOSTD_METAPHONE_MAX;i++) {
    c = str[i];

    if(i<len-1)
      c1 = str[i+1];
    else
      c1 = '\0';

    if(c == c1)
      continue;
    
    if(i<len-2)
      c2 = str[i+2];
    else
      c2 = '\0';
    
    d = d1 = '\0';
    skip = 0;

    /* Exceptions */
    if(i==0) {
      if((c == 'a' && c1 == 'e') || (c == 'g' && c1 == 'n') ||
	 (c == 'k' && c1 == 'n') || (c == 'p' && c1 == 'n') ||
	 (c == 'w' && c1 == 'r')) {
	skip=1;
	c = d = c1;
      } else if(c == 'x')
	c = d = 's';
      else if(c == 'w' && c1 == 'h') {
	skip=1;
	d = c;
      }   
    }

    /* Transformations */
    switch (c) {

    case 'b':
      if(p == 'm' && !c1)
	d = '\0';
      else
	d = 'b';
      break;

    case 'c':
      if((c1 == 'i' && c2 == 'a') || c1 == 'h')
	d = 'x';
      else if(c1 == 'i' || c1 == 'e' || c1 == 'y')
	if(p == 's')
	  d = '\0';
	else
	  d = 's';
      else
	d = 'k';
      break;

    case 'd':
      if(c1 == 'g' && (c2 == 'e' || c2 == 'i' || c2 == 'y'))
	d = 'j';
      else
	d = 't';
      break;

    case 'f':
    case 'j':
    case 'l':
    case 'm':
    case 'n':      
    case 'r':
      d = c;
      break;

    case 'g':
      if((c1 == 'h' && c2 && !INFOSTD_METAPHONE_VOWEL(p)) ||
	 (c1 == 'n' && !c2) || (i==len-3 && c1 == 'n' && c2 == 'e' &&
				str[i+3] == 'd') ||
	 (p == 'd' && c1 == 'e') || (p == 'd' && c1 == 'i') ||
	 (p == 'd' && c1 == 'y'))
	d = '\0';
      else if((c1 == 'e' || c1 == 'i' || c1 == 'y') && p != 'g')
	d = 'j';
      else
	d = 'k';
      break;
      
    case 'h':
      if((INFOSTD_METAPHONE_VOWEL(p) && !INFOSTD_METAPHONE_VOWEL(c1)) ||
	 p == 'c' || p == 's' || p == 'p' || p == 't' || p == 'g') 
	d = '\0';
      else
	d = 'h';
      break;

    case 'k':
      if(p == 'c')
	d = '\0';
      d = c;
      break;

    case 'p':
      if(p == 'h')
	d = 'f';
      else
	d = c;
      break;

    case 'q':
      d = 'k';
      break;

    case 's':
      if(c1 == 'h' || (c1 == 'i' && (c2 == 'a' || c2 == 'o')))
	d = 'x';
      else
	d = 's';
      break;

    case 't':
      if(c1 == 'i' && (c2 == 'a' || c2 == 'o'))
	d = 'x';
      else if(c1 == 'h')
	d = '0';
      else
	d = c;
      break;
      
    case 'v':
      d = 'f';
      break;

    case 'w':
    case 'y':
      if(!INFOSTD_METAPHONE_VOWEL(c1))
	d = '\0';
      else 
	d = c;
      break;

    case 'x':
      d = 'k';
      d1 = 's';
      break;

    case 'z':
      d = 's';
      break;

    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      if(i == 0)
	d = c;
      break;	
            
    }    

    if(d)
      infostd_dyn_str_addchar(mstr,d);
    if(d1)
      infostd_dyn_str_addchar(mstr,d1);
    
    p = c;

    if(skip)
      i++;
  }

  return mstr->length;  
}

/*----------------------------------------------------------------------------*
|                                                                             |
|   Levenshtein distance, a.ka. string edit distance:                         |
|    Returns the number of inserts, substitutions or deletions between        |
|    two strings.                                                             |
|                                                                             |
*----------------------------------------------------------------------------*/
unsigned long infostd_speller_edit_distance_min(INFOSTD_DYN_LTAB dists,
						unsigned long size,
						unsigned long i,
						unsigned long j,
						unsigned long d) {

  unsigned long a, b, c, e;

  a = infostd_dyn_ltab_get(dists,((i-1)*size)+j) + 1;
  b = infostd_dyn_ltab_get(dists,(i*size)+(j-1)) + 1;
  c = infostd_dyn_ltab_get(dists,((i-1)*size)+(j-1)) + d;
  
  e = a;
  if(b < e)
    e = b;
  if(c < e)
    e = c;
  
  return e;
}

unsigned long infostd_speller_edit_distance(INFOSTD_DYN_LTAB dists,
					    unsigned char* str1,
					    unsigned long slen1,
					    unsigned char* str2,
					    unsigned long slen2) {

  unsigned long i, j, c;
  
  if(slen1 == 0)
    return slen2;
  if(slen2 == 0)
    return slen1;
  
  /* Initial distances */
  for(i=0;i<=slen1;i++) {    
    for(j=0;j<=slen2;j++) {
      infostd_dyn_ltab_set(dists,(i*slen2)+j,0);
      infostd_dyn_ltab_set(dists,j,j);
    }
    infostd_dyn_ltab_set(dists,i*slen2,i);
  }

  /* The edit distance */
  for(i=1;i<=slen1;i++) {    
    for(j=1;j<=slen2;j++) {
      c = (str1[i-1] == str2[j-1]) ? 0 : 1;
      infostd_dyn_ltab_set(dists, (i*slen2)+j,
			   infostd_speller_edit_distance_min(dists,slen2,
							     i,j,c));
    }
  }
  
  return infostd_dyn_ltab_get(dists,(slen1*slen2)+slen2);
}
					    			
INFOSTD_SPELLER infostd_speller_init(unsigned long size) {

  INFOSTD_SPELLER isp;

  isp = (INFOSTD_SPELLER)malloc(SIZE_INFOSTD_SPELLER);
  memset(isp,0,SIZE_INFOSTD_SPELLER);

  isp->dictionary = infostd_hash_str_init(size);
  isp->metaphones = infostd_hash_str_init(size);
  isp->ignores = infostd_hash_str_init(size);
  isp->mappings = infostd_hash_str_init(size);

  isp->ids = infostd_dyn_str_init(100);
  isp->ids2 = infostd_dyn_str_init(100);
  isp->idl = infostd_dyn_ltab_init(100);
  isp->idss = infostd_dyn_strs_init(100);

  return isp;
}

void infostd_speller_free(INFOSTD_SPELLER isp) {

  if(isp->dictionary)
    infostd_hash_str_free(isp->dictionary);

  if(isp->metaphones)
    infostd_hash_str_free(isp->metaphones);

  if(isp->ignores)
    infostd_hash_str_free(isp->ignores);

  if(isp->mappings)
    infostd_hash_str_free(isp->mappings);
  
  if(isp->ids)
    infostd_dyn_str_free(isp->ids);

  if(isp->ids2)
    infostd_dyn_str_free(isp->ids2);

  if(isp->idl)
    infostd_dyn_ltab_free(isp->idl);

  if(isp->idss)
    infostd_dyn_strs_free(isp->idss);

  free(isp);
}
     
unsigned long infostd_speller_add(INFOSTD_SPELLER isp,
				  unsigned char* word) {

  unsigned long n, pos;

  infostd_dyn_str_clear(isp->ids);
  infostd_dyn_str_clear(isp->ids2);

  /* Set the words */
  infostd_dyn_str_strcpy(isp->ids,word);
  infostd_dyn_str_strcpy(isp->ids2,word);
  
  infostd_dyn_str_lower(isp->ids2);

  /* Add the word with proper case */
  n = infostd_hash_str_add_string(isp->dictionary,isp->ids2->buff,
				  isp->ids->buff);

  /* Generate the metaphone */
  infostd_speller_metaphone(isp->ids2->buff,isp->ids2->length,isp->ids);

  /* Add the metaphone with proper case */
  if((pos = infostd_hash_str_info(isp->metaphones,isp->ids->buff)) 
     != ULONG_MAX) {
    infostd_hash_str_string(isp->metaphones,isp->ids->buff,isp->ids2);

    infostd_dyn_str_addchar(isp->ids2,' ');
    infostd_dyn_str_strcat(isp->ids2,word);
    infostd_hash_str_add_string(isp->metaphones,isp->ids->buff,
				isp->ids2->buff);
    
  } else    
    infostd_hash_str_add_string(isp->metaphones,isp->ids->buff,word);    
 
  return n;
}

unsigned long infostd_speller_add_ignore(INFOSTD_SPELLER isp,
					 unsigned char* word) {

  infostd_dyn_str_clear(isp->ids);
  infostd_dyn_str_strcpy(isp->ids,word);
  infostd_dyn_str_lower(isp->ids);
  return infostd_hash_str_add(isp->ignores,isp->ids->buff,1);
}

unsigned long infostd_speller_add_mapping(INFOSTD_SPELLER isp,
					  unsigned char* word,
					  unsigned char* map) {

  infostd_dyn_str_clear(isp->ids);
  infostd_dyn_str_strcpy(isp->ids,word);
  infostd_dyn_str_lower(isp->ids);
  return infostd_hash_str_add_string(isp->mappings,isp->ids->buff,map);
}

void infostd_speller_write(INFOSTD_SPELLER isp,
			   FILE* out) {
  unsigned long type;

  type = TYPE_INFOSTD_SPELLER;

  fwrite(&type,sizeof(unsigned long),1,out);

  infostd_hash_str_write(isp->dictionary,out);
  infostd_hash_str_write(isp->metaphones,out);
  infostd_hash_str_write(isp->ignores,out);
  infostd_hash_str_write(isp->mappings,out);
}

INFOSTD_SPELLER infostd_speller_read(FILE* in) {

  INFOSTD_SPELLER isp;
  unsigned long type;

  fread(&type,sizeof(unsigned long),1,in);

  if(type != TYPE_INFOSTD_SPELLER)
    return NULL;

  isp = (INFOSTD_SPELLER)malloc(SIZE_INFOSTD_SPELLER);
  memset(isp,0,SIZE_INFOSTD_SPELLER);
  
  isp->dictionary = infostd_hash_str_read(in);
  isp->metaphones = infostd_hash_str_read(in);
  isp->ignores = infostd_hash_str_read(in);
  isp->mappings = infostd_hash_str_read(in);

  isp->ids = infostd_dyn_str_init(100);
  isp->ids2 = infostd_dyn_str_init(100);
  isp->idl = infostd_dyn_ltab_init(100);
  isp->idss = infostd_dyn_strs_init(100);

  return isp;
}

int infostd_speller_check(INFOSTD_SPELLER isp,
			  unsigned char* word) {

  infostd_dyn_str_clear(isp->ids);
  infostd_dyn_str_strcpy(isp->ids,word);
  infostd_dyn_str_lower(isp->ids);

  if(infostd_hash_str_info(isp->ignores,isp->ids->buff)!=ULONG_MAX)
    return 1;

  if(infostd_hash_str_info(isp->dictionary,isp->ids->buff)!=ULONG_MAX)
    return 1;

  return 0;
}

double infostd_speller_suggestion_score(INFOSTD_SPELLER isp,
					unsigned char* word,
					unsigned char* sugg) {

  unsigned long wlen, slen;

  wlen = strlen(word);
  slen = strlen(sugg);

  return (double) infostd_speller_edit_distance(isp->idl,word,wlen,
						sugg,slen);
}

unsigned long infostd_speller_suggestions(INFOSTD_SPELLER isp,
					  unsigned char* word,
					  INFOSTD_DYN_STRS idss) {

  INFOSTD_DYN_PTAB idp;
  INFOSTD_DHEAP_INFO dhi;
  unsigned long i, n, pos, len;
  unsigned char *ln;

  infostd_dyn_str_clear(isp->ids);
  infostd_dyn_str_strcpy(isp->ids,word);
  infostd_dyn_str_lower(isp->ids);
  
  if((pos=infostd_hash_str_info(isp->mappings,isp->ids->buff))!=ULONG_MAX) {
    infostd_hash_str_string(isp->mappings,isp->ids->buff,isp->ids2);
    for(i=1,ln=strchr(isp->ids2->buff,' ');ln;ln++,ln=strchr(ln,' '),i++)
      infostd_dyn_strs_add(idss,ln);
    return i;
  }

  infostd_speller_metaphone(isp->ids->buff,isp->ids->length,isp->ids2);

  if((pos=infostd_hash_str_info(isp->metaphones,isp->ids2->buff))!=ULONG_MAX){
    idp = infostd_dyn_ptab_init(100);
    dhi = infostd_dheap_info_init(INFOSTD_HEAP_MIN,100);

    infostd_hash_str_string(isp->metaphones,isp->ids2->buff,isp->ids);    
    len = isp->ids->length;
    for(i=0,n=0,ln=isp->ids->buff;i<=len;i++) {
      if(isp->ids->buff[i] == ' ')
	isp->ids->buff[i] = '\0';

      if(isp->ids->buff[i] == '\0') {
	infostd_dyn_ptab_set(idp,n,(void*)ln);
	infostd_dheap_info_push(dhi,infostd_speller_suggestion_score(isp,
								     word,
								     ln),n);
	ln=isp->ids->buff+i+1;
	n++;
      }
    }
    infostd_dyn_strs_clear(idss);
    for(i=0;i<n&&i<INFOSTD_SPELLER_SUGGESTION_MAX;i++) {
      if(infostd_dheap_info_pop(dhi,&pos)==(double)ULONG_MAX)
	break;
      ln = (char*)infostd_dyn_ptab_get(idp,pos);
      infostd_dyn_strs_add(idss,ln);
    }
    
    if(idp)
      infostd_dyn_ptab_free(idp);
    if(dhi)
      infostd_dheap_info_free(dhi);

    return i;
  }
  return 0;
}

INFOSTD_SPELLER infostd_speller_load_config(char* filename) {

  INFOSTD_SPELLER isp=NULL;
  INFOSTD_DYN_STR ids, ids2;
  char* ln;
  
  FILE *fc, *fp;

  fc = fopen(filename,"r");
  if(!fc)
    return NULL;

  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);

  while(infostd_dyn_str_fgets(ids,fc)) {

    if(ids->buff[0] == '#' || ids->buff[0] == '\0')
      continue;

    if(!strncasecmp(ids->buff,"SPELLER=",8)) {
      if((fp=fopen(ids->buff+8,"r"))) {
	isp=infostd_speller_read(fp);
	fclose(fp);
      }
    } else if(!strncasecmp(ids->buff,"USERDICT=",9) && isp) {
      if((fp=fopen(ids->buff+9,"r"))) {
	while(infostd_dyn_str_fgets(ids2,fp)) {
	  infostd_speller_add(isp,ids2->buff);
	}
	fclose(fp);
      }
    } else if(!strncasecmp(ids->buff,"IGNORE=",7) && isp) {
      if((fp=fopen(ids->buff+7,"r"))) {
	while(infostd_dyn_str_fgets(ids2,fp)) {
	  infostd_speller_add_ignore(isp,ids2->buff);
	}
	fclose(fp);
      }
    } else if(!strncasecmp(ids->buff,"MAPPINGS=",9) && isp) {
      if((fp=fopen(ids->buff+9,"r"))) {
	while(infostd_dyn_str_fgets(ids2,fp)) {
	  if((ln=strchr(ids2->buff,','))) {
	    *ln='\0'; ln++;	    
	    infostd_speller_add_mapping(isp,ids2->buff,ln);
	  }
	}
	fclose(fp);
      }
    }    
  }

  fclose(fc);
  
  infostd_dyn_str_free(ids);
  infostd_dyn_str_free(ids2);
  
  return isp;
}

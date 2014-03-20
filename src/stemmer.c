#include <infostd.h>

INFOSTD_STEMMER infostd_stemmer_init(unsigned long size) {

  INFOSTD_STEMMER ist;

  ist = (INFOSTD_STEMMER)malloc(SIZE_INFOSTD_STEMMER);
  memset(ist,0,SIZE_INFOSTD_STEMMER);

  ist->roots = infostd_hash_str_init(size);
  ist->ids_roots = infostd_dyn_str_init(100);
  ist->forms = infostd_hash_str_init(size);

  ist->ids = infostd_dyn_str_init(100);

  return ist;
}

void infostd_stemmer_free(INFOSTD_STEMMER ist) {

  if(ist->roots)
    infostd_hash_str_free(ist->roots);

  if(ist->ids_roots)
    infostd_dyn_str_free(ist->ids_roots);

  if(ist->forms)
    infostd_hash_str_free(ist->forms);
  
  if(ist->ids)
    infostd_dyn_str_free(ist->ids);

  free(ist);
}

INFOSTD_STEMMER infostd_stemmer_build(FILE* in,
				      unsigned long size) {

  INFOSTD_STEMMER ist;
  INFOSTD_HASH_STR root_hash;
  INFOSTD_DYN_STR ids;
  unsigned long id;
  char *root, *form;

  ist = infostd_stemmer_init(size);
  root_hash = infostd_hash_str_init(size);
  ids = infostd_dyn_str_init(100);

  while(infostd_dyn_str_fgets(ids,in)) {

    if(ids->buff[0] == '#' || ids->buff[0] == '\0')
      continue;

    if((root=strchr(ids->buff,','))) {
      *root='\0';
      root++;

      form = ids->buff;

      /* Add it to the roots string */
      if((id=infostd_hash_str_info(root_hash,root))==ULONG_MAX) {
	id = ist->ids_roots->length;
	infostd_dyn_str_strcat(ist->ids_roots,root);
	infostd_dyn_str_addchar(ist->ids_roots,'\0');
      }      

      /* Add it to the roots hash */
      if(infostd_hash_str_info(ist->roots,form)==ULONG_MAX)
	infostd_hash_str_add(ist->roots,form,id);
	
      /* Add it to the forms list */
      if((id=infostd_hash_str_info(ist->forms,root))==ULONG_MAX)
	infostd_hash_str_add_string(ist->forms,root,root);

      infostd_hash_str_string(ist->forms,root,ist->ids);

      if(!strstr(ist->ids->buff,form)) {
	    infostd_dyn_str_addchar(ist->ids,' ');
	    infostd_dyn_str_strcat(ist->ids,form);
	    infostd_hash_str_add_string(ist->forms,root,ist->ids->buff);
      }
    }
  }
  
  infostd_hash_str_free(root_hash);
  infostd_dyn_str_free(ids);

  return ist;
}

unsigned long infostd_stemmer_root(INFOSTD_STEMMER ist,
				   char* word,
				   INFOSTD_DYN_STR ids) {
  
  unsigned long id;

  /* lowercase? */
  if((id=infostd_hash_str_info(ist->roots,word))!=ULONG_MAX) {
    infostd_dyn_str_clear(ids);
    infostd_dyn_str_strcpy(ids,ist->ids_roots->buff+id);
    return 1;
  }
  return 0;
}

unsigned long infostd_stemmer_forms(INFOSTD_STEMMER ist,
				    char* word,
				    INFOSTD_DYN_STR ids) {

  unsigned long i;
  char* ln;

  if(infostd_stemmer_root(ist,word,ist->ids)) {
    if(infostd_hash_str_string(ist->forms,ist->ids->buff,ids)!=ULONG_MAX) {
      for(i=1,ln=strchr(ids->buff,' ');ln;ln++,ln=strchr(ln,' '),i++); 
      return i;
    }
    return 0;
  }
  return 0;
}

void infostd_stemmer_write(INFOSTD_STEMMER ist,
			   FILE* out) {

  unsigned long type;

  type = TYPE_INFOSTD_STEMMER;

  fwrite(&type,sizeof(unsigned long),1,out);

  infostd_hash_str_write(ist->roots,out);
  infostd_dyn_str_write(ist->ids_roots,out);
  infostd_hash_str_write(ist->forms,out);
}

INFOSTD_STEMMER infostd_stemmer_read(FILE* in) {

  INFOSTD_STEMMER ist;
  unsigned long type;

  fread(&type,sizeof(unsigned long),1,in);

  if(type != TYPE_INFOSTD_STEMMER)
    return NULL;

  ist = (INFOSTD_STEMMER)malloc(SIZE_INFOSTD_STEMMER);
  memset(ist,0,SIZE_INFOSTD_STEMMER);
  
  ist->roots = infostd_hash_str_read(in);
  ist->ids_roots = infostd_dyn_str_read(in);
  ist->forms = infostd_hash_str_read(in);

  ist->ids = infostd_dyn_str_init(100);

  return ist;
}

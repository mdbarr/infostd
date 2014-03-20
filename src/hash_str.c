#include <infostd.h>

unsigned long infostd_hash_str_hash(unsigned char *str,
				    unsigned long size) {

  unsigned long hash, i, len;
  
  len = strlen(str);

  for(hash=0,i=0;i<len;i++) {
    hash += str[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash % size;
}

INFOSTD_HASH_STR_TAB infostd_hash_str_tab_init(unsigned char* str,
					       unsigned long str_len,
					       unsigned long info) {

  INFOSTD_HASH_STR_TAB hst;
  
  hst = (INFOSTD_HASH_STR_TAB)malloc(SIZE_INFOSTD_HASH_STR_TAB);
  memset(hst,0,SIZE_INFOSTD_HASH_STR_TAB);

  hst->str = (char*)malloc(sizeof(char)*str_len);
  memset(hst->str,0,str_len);
  strcpy(hst->str,str);
  
  hst->str_len = str_len;
  hst->info = info;
  
  return hst;
}

void infostd_hash_str_tab_free(INFOSTD_HASH_STR_TAB hst) {

  if(hst->str)
    free(hst->str);
  free(hst);
}

INFOSTD_HASH_STR infostd_hash_str_init(unsigned long size) {

  INFOSTD_HASH_STR ihs;

  if(size == 0)
    return NULL;

  ihs = (INFOSTD_HASH_STR)malloc(SIZE_INFOSTD_HASH_STR);
  memset(ihs,0,SIZE_INFOSTD_HASH_STR);
  
  ihs->size = size;

  ihs->tab = (INFOSTD_HASH_STR_TAB*)malloc(ihs->size * 
					   SIZE_INFOSTD_HASH_STR_TAB);

  memset(ihs->tab,0,ihs->size*SIZE_INFOSTD_HASH_STR_TAB);

  return ihs;
}

void infostd_hash_str_free(INFOSTD_HASH_STR ihs) {
  
  INFOSTD_HASH_STR_TAB tmp,next;
  unsigned long i;

  if(ihs->tab) {
    for(i=0;i<ihs->size;i++) {
      for(tmp=ihs->tab[i];tmp;) {
	next = tmp->next;
	infostd_hash_str_tab_free(tmp);
	tmp = next;
      }
    }
    free(ihs->tab);
  }

  if(ihs->info)
    infostd_dyn_str_free(ihs->info);

  free(ihs); 
}

unsigned long infostd_hash_str_rm(INFOSTD_HASH_STR ihs,
				  unsigned char* key) {
  
  INFOSTD_HASH_STR_TAB l,p;
  unsigned long pos, len;
  
  if(!ihs->num_elem)
    return ULONG_MAX;

  pos = infostd_hash_str_hash(key,ihs->size);

  len = strlen(key)+1;
  
  for(l=ihs->tab[pos],p=NULL;l;p=l,l=l->next) {
    if(l->str_len==len && (!memcmp(l->str,key,sizeof(char)*len)))
      break;
  }

  if(!l)
    return ULONG_MAX;
  
  if(p)
    p->next=l->next;

  if(l == ihs->tab[pos])
    ihs->tab[pos] = l->next;

  infostd_hash_str_tab_free(l);  

  ihs->num_elem--;
  
  return ihs->num_elem;
}  

unsigned long infostd_hash_str_info_chg(INFOSTD_HASH_STR ihs,
					unsigned char* key,
					unsigned long info) {
  
  INFOSTD_HASH_STR_TAB l;
  unsigned long pos, len;
  
  if(!ihs->num_elem)
    return ULONG_MAX;

  pos = infostd_hash_str_hash(key,ihs->size);

  len = strlen(key)+1;
  
  for(l=ihs->tab[pos];l;l=l->next) {
    if(l->str_len==len && (!memcmp(l->str,key,sizeof(char)*len)))
      break;      
  }

  if(l) {
    if(info != ULONG_MAX)
      l->info = info;

    return l->info; 
  }
  return ULONG_MAX;
}

unsigned long infostd_hash_str_info(INFOSTD_HASH_STR ihs,
				    unsigned char* key) {

  return infostd_hash_str_info_chg(ihs,key,ULONG_MAX);
}

unsigned long infostd_hash_str_string(INFOSTD_HASH_STR ihs,
				      unsigned char* key,
				      INFOSTD_DYN_STR out) { 
  unsigned long id;

  id = infostd_hash_str_info(ihs,key);
  if(id != ULONG_MAX && ihs->info && id < ihs->info->size){
    infostd_dyn_str_strcpy(out,&(ihs->info->buff[id]));
  }
  return id;
}

unsigned long infostd_hash_str_add(INFOSTD_HASH_STR ihs,
				   unsigned char* key,
				   unsigned long info) {

  INFOSTD_HASH_STR_TAB tmp, l;
  unsigned long pos, len, id;

  pos = infostd_hash_str_hash(key,ihs->size);

  len = strlen(key)+1;

  id = infostd_hash_str_info_chg(ihs,key,info);
  if(id != ULONG_MAX)
    return ihs->num_elem;

  l = infostd_hash_str_tab_init(key,len,info);
  if(ihs->tab[pos]) {
    for(tmp=ihs->tab[pos];tmp&&tmp->next;tmp=tmp->next);
    tmp->next = l;
  } else {
    ihs->tab[pos] = l;
  }
  ihs->num_elem++;
  
  return ihs->num_elem;
}

unsigned long infostd_hash_str_add_string(INFOSTD_HASH_STR ihs,
					  unsigned char* key,
					  unsigned char* info) {

  unsigned long pos;
 
  if(!ihs->info)
    ihs->info = infostd_dyn_str_init(100);
  
  pos = ihs->info->length;

  infostd_dyn_str_strcat(ihs->info,info);
  infostd_dyn_str_addchar(ihs->info,'\0');

  return infostd_hash_str_add(ihs,key,pos);
}

INFOSTD_HASH_STR infostd_hash_str_read(FILE* in) {

  INFOSTD_HASH_STR ihs;
  INFOSTD_HASH_STR_TAB l, t, tmp;
  unsigned long type, size, i, c, n, num;

  fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_HASH_STR)
    return NULL;

  fread(&size,sizeof(unsigned long),1,in);
  
  ihs = infostd_hash_str_init(size);

  fread(&ihs->num_elem,sizeof(unsigned long),1,in);
  
  for(i=fread(&c,sizeof(unsigned long),1,in);c!=ULONG_MAX;
      i=fread(&c,sizeof(unsigned long),1,in)) {

    t=l=NULL;
    for(i=fread(&n,sizeof(unsigned long),1,in);n!=ULONG_MAX;
	i=fread(&n,sizeof(unsigned long),1,in)) {

      tmp = (INFOSTD_HASH_STR_TAB)malloc(SIZE_INFOSTD_HASH_STR_TAB);
      memset(tmp,0,SIZE_INFOSTD_HASH_STR_TAB);

      tmp->str_len = n;

      tmp->str = (char*)malloc(sizeof(char)*tmp->str_len);      
      memset(tmp->str,0,tmp->str_len);
      fread(tmp->str,sizeof(char),tmp->str_len,in);

      fread(&tmp->info,sizeof(unsigned long),1,in);
      
      if(t==NULL) {
	l = t = tmp;
      } else {
	t->next = tmp;
	t = tmp;
      }
      
      num++;
    }
    
    ihs->tab[c] = l;
  }
  
  fread(&c,sizeof(unsigned long),1,in);

  if(c != 0) {
    ihs->info = infostd_dyn_str_init(c);
    fread(ihs->info->buff,sizeof(char),c,in);
  }
  
  return ihs;
}

void infostd_hash_str_write(INFOSTD_HASH_STR ihs, 
			    FILE* out) {

  INFOSTD_HASH_STR_TAB l;
  INFOSTD_DYN_STR infostr = NULL;
  unsigned long type, end, i, ninfo;

  if(ihs->info)
    infostr = infostd_dyn_str_init(ihs->info->length+1);

  type = TYPE_INFOSTD_HASH_STR;
  end = ULONG_MAX;

  fwrite(&type,sizeof(unsigned long),1,out);

  fwrite(&ihs->size,sizeof(unsigned long),1,out);

  fwrite(&ihs->num_elem,sizeof(unsigned long),1,out);

  for(i=0;i<ihs->size;i++) {
    if(ihs->tab[i]) {
      fwrite(&i,sizeof(unsigned long),1,out);
      for(l=ihs->tab[i];l;l=l->next) {
	fwrite(&l->str_len,sizeof(unsigned long),1,out);
	fwrite(l->str,sizeof(char),l->str_len,out);
	if(infostr) {
	  ninfo = infostr->length;
	  infostd_dyn_str_strcat(infostr,ihs->info->buff+l->info);
	  infostd_dyn_str_addchar(infostr,'\0');
	  l->info=ninfo;
	}
	fwrite(&l->info,sizeof(unsigned long),1,out);       
      }
      fwrite(&end,sizeof(unsigned long),1,out);
    }
  }
  fwrite(&end,sizeof(unsigned long),1,out);

  end = 0;

  if(infostr) {
    infostd_dyn_str_free(ihs->info);
    ihs->info = infostr;

    fwrite(&ihs->info->length,sizeof(unsigned long),1,out);
    fwrite(ihs->info->buff,sizeof(char),ihs->info->length,out);
  } else 
    fwrite(&end,sizeof(unsigned long),1,out);
}

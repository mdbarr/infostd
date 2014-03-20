#include <infostd.h>

unsigned long infostd_hash_long_hash(unsigned long key,
				     unsigned long size) {

  return (key*62342347) % size;
}

INFOSTD_HASH_LONG_TAB infostd_hash_long_tab_init(unsigned long key,
						 unsigned long info) {

  INFOSTD_HASH_LONG_TAB hlt;
  
  hlt = (INFOSTD_HASH_LONG_TAB)malloc(SIZE_INFOSTD_HASH_LONG_TAB);
  memset(hlt,0,SIZE_INFOSTD_HASH_LONG_TAB);
  
  hlt->key = key;
  hlt->info = info;
  
  return hlt;
}

void infostd_hash_long_tab_free(INFOSTD_HASH_LONG_TAB hlt) {

  free(hlt);
}

INFOSTD_HASH_LONG infostd_hash_long_init(unsigned long size) {

  INFOSTD_HASH_LONG ihl;

  if(size == 0)
    return NULL;

  ihl = (INFOSTD_HASH_LONG)malloc(SIZE_INFOSTD_HASH_LONG);
  memset(ihl,0,SIZE_INFOSTD_HASH_LONG);
  
  ihl->size = size;

  ihl->tab = (INFOSTD_HASH_LONG_TAB*)malloc(ihl->size * 
					    SIZE_INFOSTD_HASH_LONG_TAB);

  memset(ihl->tab,0,ihl->size*SIZE_INFOSTD_HASH_LONG_TAB);

  return ihl;
}

void infostd_hash_long_free(INFOSTD_HASH_LONG ihl) {
  
  INFOSTD_HASH_LONG_TAB tmp,next;
  unsigned long i;

  if(ihl->tab) {
    for(i=0;i<ihl->size;i++) {
      for(tmp=ihl->tab[i];tmp;) {
	next = tmp->next;
	infostd_hash_long_tab_free(tmp);
	tmp = next;
      }
    }
    free(ihl->tab);
  }

  free(ihl); 
}

unsigned long infostd_hash_long_rm(INFOSTD_HASH_LONG ihl,
				   unsigned long key) {
  
  INFOSTD_HASH_LONG_TAB l,p;
  unsigned long pos;
  
  if(!ihl->num_elem)
    return ULONG_MAX;

  pos = infostd_hash_long_hash(key,ihl->size);
  
  for(l=ihl->tab[pos],p=NULL;l;p=l,l=l->next) {
    if(l->key == key)
      break;
  }

  if(!l)
    return ULONG_MAX;
  
  if(p)
    p->next=l->next;

  if(l == ihl->tab[pos])
    ihl->tab[pos] = l->next;

  infostd_hash_long_tab_free(l);  

  ihl->num_elem--;
  
  return ihl->num_elem;
}  

unsigned long infostd_hash_long_info_chg(INFOSTD_HASH_LONG ihl,
					 unsigned long key,
					 unsigned long info) {
  
  INFOSTD_HASH_LONG_TAB l;
  unsigned long pos;
  
  if(!ihl->num_elem)
    return ULONG_MAX;

  pos = infostd_hash_long_hash(key,ihl->size);
  
  for(l=ihl->tab[pos];l;l=l->next) {
    if(l->key == key)
      break;      
  }

  if(l) {
    if(info != ULONG_MAX)
      l->info = info;

    return l->info; 
  }
  return ULONG_MAX;
}

unsigned long infostd_hash_long_info(INFOSTD_HASH_LONG ihl,
				     unsigned long key) {

  return infostd_hash_long_info_chg(ihl,key,ULONG_MAX);
}

unsigned long infostd_hash_long_add(INFOSTD_HASH_LONG ihl,
				    unsigned long key,
				    unsigned long info) {
  
  INFOSTD_HASH_LONG_TAB tmp, l;
  unsigned long pos, id;

  pos = infostd_hash_long_hash(key,ihl->size);

  id = infostd_hash_long_info_chg(ihl,key,info);
  if(id != ULONG_MAX)
    return ihl->num_elem;

  l = infostd_hash_long_tab_init(key,info);
  if(ihl->tab[pos]) {
    for(tmp=ihl->tab[pos];tmp&&tmp->next;tmp=tmp->next);
    tmp->next = l;
  } else {
    ihl->tab[pos] = l;
  }
  ihl->num_elem++;
  
  return ihl->num_elem;
}

INFOSTD_HASH_LONG infostd_hash_long_read(FILE* in) {

  INFOSTD_HASH_LONG ihl;
  INFOSTD_HASH_LONG_TAB l, t, tmp;
  unsigned long type, size, i, c, n, num;

  fread(&type,sizeof(unsigned long),1,in);
  if(type != TYPE_INFOSTD_HASH_LONG)
    return NULL;

  fread(&size,sizeof(unsigned long),1,in);
  
  ihl = infostd_hash_long_init(size);

  fread(&ihl->num_elem,sizeof(unsigned long),1,in);
  
  for(i=fread(&c,sizeof(unsigned long),1,in);c!=ULONG_MAX;
      i=fread(&c,sizeof(unsigned long),1,in)) {

    t=l=NULL;
    for(i=fread(&n,sizeof(unsigned long),1,in);n!=ULONG_MAX;
	i=fread(&n,sizeof(unsigned long),1,in)) {

      tmp = (INFOSTD_HASH_LONG_TAB)malloc(SIZE_INFOSTD_HASH_LONG_TAB);
      memset(tmp,0,SIZE_INFOSTD_HASH_LONG_TAB);

      tmp->key = n;

      fread(&tmp->info,sizeof(unsigned long),1,in);
      
      if(t==NULL) {
	l = t = tmp;
      } else {
	t->next = tmp;
	t = tmp;
      }
      
      num++;
    }
    
    ihl->tab[c] = l;
  }
    
  return ihl;
}

void infostd_hash_long_write(INFOSTD_HASH_LONG ihl, 
			     FILE* out) {

  INFOSTD_HASH_LONG_TAB l;
  unsigned long type, end, i;
  
  type = TYPE_INFOSTD_HASH_LONG;
  end = ULONG_MAX;
  
  fwrite(&type,sizeof(unsigned long),1,out);
  
  fwrite(&ihl->size,sizeof(unsigned long),1,out);
  
  fwrite(&ihl->num_elem,sizeof(unsigned long),1,out);
  
  for(i=0;i<ihl->size;i++) {
    if(ihl->tab[i]) {
      fwrite(&i,sizeof(unsigned long),1,out);
      for(l=ihl->tab[i];l;l=l->next) {
	fwrite(&l->key,sizeof(unsigned long),1,out);
	fwrite(&l->info,sizeof(unsigned long),1,out);       
      }
      fwrite(&end,sizeof(unsigned long),1,out);
    }
  }
  fwrite(&end,sizeof(unsigned long),1,out);
}

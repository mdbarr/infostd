#include <infostd.h>

#define INFOSTD_HEAP_PARENT(x) ((x-1)>>1)
#define INFOSTD_HEAP_LEFT(x)   ((x << 1)+1)
#define INFOSTD_HEAP_RIGHT(x)  ((x << 1)+2)

/* Generic pointer compare */
int infostd_pheap_gencmp(void* a, void* b) {
  
  if(a == b)
    return 0;
  else if(a > b)
    return 1;
  else
    return -1;
}

/* Pointer Heap */
INFOSTD_PHEAP infostd_pheap_init(INFOSTD_HEAP_TYPE type,
				 int (*compare)(void*, void*),
				 unsigned long size) {

  INFOSTD_PHEAP iph;
  
  iph = (INFOSTD_PHEAP)malloc(SIZE_INFOSTD_PHEAP);
  memset(iph,0,SIZE_INFOSTD_PHEAP);

  iph->type = type;

  if(compare)
    iph->compare = compare;
  else
    iph->compare = infostd_pheap_gencmp;

  iph->keys = infostd_dyn_ptab_init(size);

  return iph;
}

void infostd_pheap_free(INFOSTD_PHEAP iph) {
 
  if(iph->keys)
    infostd_dyn_ptab_free(iph->keys);

  free(iph);
}

unsigned long infostd_pheap_push(INFOSTD_PHEAP iph,
				 void* key) {

  unsigned long i, p;
  void **keys, *ktmp;
  
  infostd_dyn_ptab_set(iph->keys,iph->num_elem,key);

  iph->num_elem++;
  
  keys = iph->keys->buff;
  i=iph->num_elem-1;

  while(i>0 && ((iph->type == INFOSTD_HEAP_MAX && 
		 (iph->compare(keys[INFOSTD_HEAP_PARENT(i)],key)<0)) ||
		(iph->type == INFOSTD_HEAP_MIN && 
		 (iph->compare(keys[INFOSTD_HEAP_PARENT(i)],key)>0)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    i=p;
  }

  return iph->num_elem;
}

void* infostd_pheap_pop(INFOSTD_PHEAP iph) {

  void* key;

  if(!iph->num_elem)
    return (void*)ULONG_MAX;

  key = iph->keys->buff[0];
  iph->num_elem--;

  if(iph->num_elem) {
    iph->keys->buff[0] = iph->keys->buff[iph->num_elem];
    infostd_pheap_sort(iph);
  }

  return key;
}

void infostd_pheap_sort(INFOSTD_PHEAP iph) {
 

  void **keys, *ktmp;
  unsigned long i,l,r,m;
  
  keys = iph->keys->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<iph->num_elem) && 
       ((iph->type == INFOSTD_HEAP_MAX && iph->compare(keys[l],keys[i])>0) ||
	(iph->type == INFOSTD_HEAP_MIN && iph->compare(keys[l],keys[i])<0)))
      m=l;
    else
      m=i;
    
    if((r<iph->num_elem) && 
       ((iph->type == INFOSTD_HEAP_MAX && iph->compare(keys[r],keys[m])>0) ||
	(iph->type == INFOSTD_HEAP_MIN && iph->compare(keys[r],keys[m])<0)))
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      i=m;
    } else
      break;
  }
}

/* Pointer Info Heap */
INFOSTD_PHEAP_INFO infostd_pheap_info_init(INFOSTD_HEAP_TYPE type,
					   int (*compare)(void*, void*),
					   unsigned long size) {

  INFOSTD_PHEAP_INFO iphi;
  
  iphi = (INFOSTD_PHEAP_INFO)malloc(SIZE_INFOSTD_PHEAP_INFO);
  memset(iphi,0,SIZE_INFOSTD_PHEAP_INFO);

  iphi->type = type;

  if(compare)
    iphi->compare = compare;
  else
    iphi->compare = infostd_pheap_gencmp;

  iphi->keys = infostd_dyn_ptab_init(size);
  iphi->infos = infostd_dyn_ltab_init(size);

  return iphi;
}

void infostd_pheap_info_free(INFOSTD_PHEAP_INFO iphi) {
 
  if(iphi->keys)
    infostd_dyn_ptab_free(iphi->keys);

  if(iphi->infos)
    infostd_dyn_ltab_free(iphi->infos);

  free(iphi);
}

unsigned long infostd_pheap_info_push(INFOSTD_PHEAP_INFO iphi,
				      void* key,
				      unsigned long info) {

  unsigned long i, p;
  void **keys, *ktmp;
  unsigned long itmp, *infos;
  
  infostd_dyn_ptab_set(iphi->keys,iphi->num_elem,key);
  infostd_dyn_ltab_set(iphi->infos,iphi->num_elem,info);

  iphi->num_elem++;
  
  keys = iphi->keys->buff;
  infos = iphi->infos->buff;
  i=iphi->num_elem-1;

  while(i>0 && ((iphi->type == INFOSTD_HEAP_MAX && 
		 (iphi->compare(keys[INFOSTD_HEAP_PARENT(i)],key)<0)) ||
		(iphi->type == INFOSTD_HEAP_MIN && 
		 (iphi->compare(keys[INFOSTD_HEAP_PARENT(i)],key)>0)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    itmp=infos[p];
    infos[p]=infos[i];
    infos[i]=itmp;

    i=p;
  }

  return iphi->num_elem;
}

void* infostd_pheap_info_pop(INFOSTD_PHEAP_INFO iphi,
			     unsigned long* pinfo) {

  void* key;

  if(!iphi->num_elem)
    return (void*)ULONG_MAX;

  key = iphi->keys->buff[0];
  *pinfo = iphi->infos->buff[0];
  iphi->num_elem--;
  if(iphi->num_elem) {
    iphi->keys->buff[0] = iphi->keys->buff[iphi->num_elem];
    iphi->infos->buff[0] = iphi->infos->buff[iphi->num_elem];

    infostd_pheap_info_sort(iphi);
  }

  return key;
}

void infostd_pheap_info_sort(INFOSTD_PHEAP_INFO iphi) {
 

  void **keys, *ktmp;
  unsigned long *infos, itmp;
  unsigned long i,l,r,m;
  
  keys = iphi->keys->buff;
  infos = iphi->infos->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<iphi->num_elem) && 
       ((iphi->type == INFOSTD_HEAP_MAX && iphi->compare(keys[l],keys[i])>0) ||
	(iphi->type == INFOSTD_HEAP_MIN && iphi->compare(keys[l],keys[i])<0)))
      m=l;
    else
      m=i;
    
    if((r<iphi->num_elem) && 
       ((iphi->type == INFOSTD_HEAP_MAX && iphi->compare(keys[r],keys[m])>0) ||
	(iphi->type == INFOSTD_HEAP_MIN && iphi->compare(keys[r],keys[m])<0)))
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      itmp=infos[i];
      infos[i]=infos[m];
      infos[m]=itmp;

      i=m;
    } else
      break;
  }
}

/* Long Heap */
INFOSTD_LHEAP infostd_lheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size) {
  
  INFOSTD_LHEAP ilh;
  
  ilh = (INFOSTD_LHEAP)malloc(SIZE_INFOSTD_LHEAP);
  memset(ilh,0,SIZE_INFOSTD_LHEAP);

  ilh->type = type;

  ilh->keys = infostd_dyn_ltab_init(size);

  return ilh;
}

void infostd_lheap_free(INFOSTD_LHEAP ilh) {
 
  if(ilh->keys)
    infostd_dyn_ltab_free(ilh->keys);

  free(ilh);
}

unsigned long infostd_lheap_push(INFOSTD_LHEAP ilh,
				 unsigned long key) {

  unsigned long i, p;
  unsigned long *keys, ktmp;
  
  infostd_dyn_ltab_set(ilh->keys,ilh->num_elem,key);
  
  ilh->num_elem++;
  
  keys = ilh->keys->buff;
  i=ilh->num_elem-1;

  while(i>0 && ((ilh->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(ilh->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    i=p;
  }

  return ilh->num_elem;
}

unsigned long infostd_lheap_pop(INFOSTD_LHEAP ilh) {

  unsigned long key;

  if(!ilh->num_elem)
    return ULONG_MAX;

  key = ilh->keys->buff[0];
  ilh->num_elem--;

  if(ilh->num_elem) {
    ilh->keys->buff[0] = ilh->keys->buff[ilh->num_elem];
    infostd_lheap_sort(ilh);
  }

  return key;
}

void infostd_lheap_sort(INFOSTD_LHEAP ilh) {
 
  unsigned long *keys, ktmp;
  unsigned long i,l,r,m;

  keys = ilh->keys->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<ilh->num_elem) && 
       ((ilh->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(ilh->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<ilh->num_elem) && 
       ((ilh->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(ilh->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      i=m;
    } else
      break;
  }
}

/* Long Info Heap */
INFOSTD_LHEAP_INFO infostd_lheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size) {

  INFOSTD_LHEAP_INFO ilhi;
  
  ilhi = (INFOSTD_LHEAP_INFO)malloc(SIZE_INFOSTD_LHEAP_INFO);
  memset(ilhi,0,SIZE_INFOSTD_LHEAP_INFO);

  ilhi->type = type;

  ilhi->keys = infostd_dyn_ltab_init(size);
  ilhi->infos = infostd_dyn_ltab_init(size);

  return ilhi;
}

void infostd_lheap_info_free(INFOSTD_LHEAP_INFO ilhi) {
 
  if(ilhi->keys)
    infostd_dyn_ltab_free(ilhi->keys);

  if(ilhi->infos)
    infostd_dyn_ltab_free(ilhi->infos);

  free(ilhi);
}

unsigned long infostd_lheap_info_push(INFOSTD_LHEAP_INFO ilhi,
				      unsigned long key,
				      unsigned long info) {

  unsigned long i, p, itmp, ktmp;
  unsigned long *keys, *infos;
  
  infostd_dyn_ltab_set(ilhi->keys,ilhi->num_elem,key);
  infostd_dyn_ltab_set(ilhi->infos,ilhi->num_elem,info);
  
  ilhi->num_elem++;
  
  keys = ilhi->keys->buff;
  infos = ilhi->infos->buff;
  i=ilhi->num_elem-1;

  while(i>0 && ((ilhi->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(ilhi->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    itmp=infos[p];
    infos[p]=infos[i];
    infos[i]=itmp;

    i=p;
  }

  return ilhi->num_elem;
}

unsigned long infostd_lheap_info_pop(INFOSTD_LHEAP_INFO ilhi,
				     unsigned long* pinfo) {

  unsigned long key;

  if(!ilhi->num_elem)
    return ULONG_MAX;

  key = ilhi->keys->buff[0];
  *pinfo = ilhi->infos->buff[0];
  ilhi->num_elem--;
  if(ilhi->num_elem) {
    ilhi->keys->buff[0] = ilhi->keys->buff[ilhi->num_elem];
    ilhi->infos->buff[0] = ilhi->infos->buff[ilhi->num_elem];

    infostd_lheap_info_sort(ilhi);
  }

  return key;
}

void infostd_lheap_info_sort(INFOSTD_LHEAP_INFO ilhi) {
 
  unsigned long *keys, *infos;
  unsigned long itmp, ktmp;
  unsigned long i,l,r,m;

  keys = ilhi->keys->buff;
  infos = ilhi->infos->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<ilhi->num_elem) && 
       ((ilhi->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(ilhi->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<ilhi->num_elem) && 
       ((ilhi->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(ilhi->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      itmp=infos[i];
      infos[i]=infos[m];
      infos[m]=itmp;

      i=m;
    } else
      break;
  }
}

/* Float Heap */
INFOSTD_FHEAP infostd_fheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size) {

  INFOSTD_FHEAP ifh;
  
  ifh = (INFOSTD_FHEAP)malloc(SIZE_INFOSTD_FHEAP);
  memset(ifh,0,SIZE_INFOSTD_FHEAP);

  ifh->type = type;

  ifh->keys = infostd_dyn_ftab_init(size);

  return ifh;
}

void infostd_fheap_free(INFOSTD_FHEAP ifh) {
 
  if(ifh->keys)
    infostd_dyn_ftab_free(ifh->keys);

  free(ifh);
}

unsigned long infostd_fheap_push(INFOSTD_FHEAP ifh,
				 float key) {

  unsigned long i, p;
  float *keys, ktmp;
  
  infostd_dyn_ftab_set(ifh->keys,ifh->num_elem,key);
  
  ifh->num_elem++;
  
  keys = ifh->keys->buff;
  i=ifh->num_elem-1;

  while(i>0 && ((ifh->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(ifh->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    i=p;
  }

  return ifh->num_elem;
}

float infostd_fheap_pop(INFOSTD_FHEAP ifh) {

  float key;

  if(!ifh->num_elem)
    return ULONG_MAX;

  key = ifh->keys->buff[0];
  ifh->num_elem--;

  if(ifh->num_elem) {
    ifh->keys->buff[0] = ifh->keys->buff[ifh->num_elem];
    infostd_fheap_sort(ifh);
  }

  return key;
}

void infostd_fheap_sort(INFOSTD_FHEAP ifh) {
 

  float *keys, ktmp;
  unsigned long i,l,r,m;

  keys = ifh->keys->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<ifh->num_elem) && 
       ((ifh->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(ifh->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<ifh->num_elem) && 
       ((ifh->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(ifh->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      i=m;
    } else
      break;
  }
}

/* Float Info Heap */
INFOSTD_FHEAP_INFO infostd_fheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size) {

  INFOSTD_FHEAP_INFO ifhi;
  
  ifhi = (INFOSTD_FHEAP_INFO)malloc(SIZE_INFOSTD_FHEAP_INFO);
  memset(ifhi,0,SIZE_INFOSTD_FHEAP_INFO);

  ifhi->type = type;

  ifhi->keys = infostd_dyn_ftab_init(size);
  ifhi->infos = infostd_dyn_ltab_init(size);

  return ifhi;
}

void infostd_fheap_info_free(INFOSTD_FHEAP_INFO ifhi) {
 
  if(ifhi->keys)
    infostd_dyn_ftab_free(ifhi->keys);

  if(ifhi->infos)
    infostd_dyn_ltab_free(ifhi->infos);

  free(ifhi);
}

unsigned long infostd_fheap_info_push(INFOSTD_FHEAP_INFO ifhi,
				      float key,
				      unsigned long info) {

  unsigned long i, p;
  float *keys, ktmp;
  unsigned long itmp, *infos;
  
  infostd_dyn_ftab_set(ifhi->keys,ifhi->num_elem,key);
  infostd_dyn_ltab_set(ifhi->infos,ifhi->num_elem,info);
  
  ifhi->num_elem++;
  
  keys = ifhi->keys->buff;
  infos = ifhi->infos->buff;
  i=ifhi->num_elem-1;

  while(i>0 && ((ifhi->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(ifhi->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    itmp=infos[p];
    infos[p]=infos[i];
    infos[i]=itmp;

    i=p;
  }

  return ifhi->num_elem;
}

float infostd_fheap_info_pop(INFOSTD_FHEAP_INFO ifhi,
			     unsigned long* pinfo) {

  float key;

  if(!ifhi->num_elem)
    return ULONG_MAX;

  key = ifhi->keys->buff[0];
  *pinfo = ifhi->infos->buff[0];
  ifhi->num_elem--;
  if(ifhi->num_elem) {
    ifhi->keys->buff[0] = ifhi->keys->buff[ifhi->num_elem];
    ifhi->infos->buff[0] = ifhi->infos->buff[ifhi->num_elem];

    infostd_fheap_info_sort(ifhi);
  }

  return key;
}

void infostd_fheap_info_sort(INFOSTD_FHEAP_INFO ifhi) {
 

  float *keys, ktmp;
  unsigned long *infos, itmp;
  unsigned long i,l,r,m;

  keys = ifhi->keys->buff;
  infos = ifhi->infos->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<ifhi->num_elem) && 
       ((ifhi->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(ifhi->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<ifhi->num_elem) && 
       ((ifhi->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(ifhi->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      itmp=infos[i];
      infos[i]=infos[m];
      infos[m]=itmp;

      i=m;
    } else
      break;
  }
}
/* Double Heap */
INFOSTD_DHEAP infostd_dheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size) {

  INFOSTD_DHEAP idh;
  
  idh = (INFOSTD_DHEAP)malloc(SIZE_INFOSTD_DHEAP);
  memset(idh,0,SIZE_INFOSTD_DHEAP);

  idh->type = type;

  idh->keys = infostd_dyn_dtab_init(size);

  return idh;
}

void infostd_dheap_free(INFOSTD_DHEAP idh) {
 
  if(idh->keys)
    infostd_dyn_dtab_free(idh->keys);

  free(idh);
}

unsigned long infostd_dheap_push(INFOSTD_DHEAP idh,
				 double key) {

  unsigned long i, p;
  double *keys, ktmp;
  
  infostd_dyn_dtab_set(idh->keys,idh->num_elem,key);
  
  idh->num_elem++;
  
  keys = idh->keys->buff;
  i=idh->num_elem-1;

  while(i>0 && ((idh->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(idh->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    i=p;
  }

  return idh->num_elem;
}

double infostd_dheap_pop(INFOSTD_DHEAP idh) {

  double key;

  if(!idh->num_elem)
    return ULONG_MAX;

  key = idh->keys->buff[0];
  idh->num_elem--;

  if(idh->num_elem) {
    idh->keys->buff[0] = idh->keys->buff[idh->num_elem];
    infostd_dheap_sort(idh);
  }

  return key;
}

void infostd_dheap_sort(INFOSTD_DHEAP idh) {
 

  double *keys, ktmp;
  unsigned long i,l,r,m;

  keys = idh->keys->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<idh->num_elem) && 
       ((idh->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(idh->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<idh->num_elem) && 
       ((idh->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(idh->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      i=m;
    } else
      break;
  }
}

/* Double Info Heap */
INFOSTD_DHEAP_INFO infostd_dheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size) {

  INFOSTD_DHEAP_INFO idhi;
  
  idhi = (INFOSTD_DHEAP_INFO)malloc(SIZE_INFOSTD_DHEAP_INFO);
  memset(idhi,0,SIZE_INFOSTD_DHEAP_INFO);

  idhi->type = type;

  idhi->keys = infostd_dyn_dtab_init(size);
  idhi->infos = infostd_dyn_ltab_init(size);

  return idhi;
}

void infostd_dheap_info_free(INFOSTD_DHEAP_INFO idhi) {
 
  if(idhi->keys)
    infostd_dyn_dtab_free(idhi->keys);

  if(idhi->infos)
    infostd_dyn_ltab_free(idhi->infos);

  free(idhi);
}

unsigned long infostd_dheap_info_push(INFOSTD_DHEAP_INFO idhi,
				      double key,
				      unsigned long info) {

  unsigned long i, p;
  double *keys, ktmp;
  unsigned long itmp, *infos;
  
  infostd_dyn_dtab_set(idhi->keys,idhi->num_elem,key);
  infostd_dyn_ltab_set(idhi->infos,idhi->num_elem,info);
  
  idhi->num_elem++;
  
  keys = idhi->keys->buff;
  infos = idhi->infos->buff;
  i=idhi->num_elem-1;

  while(i>0 && ((idhi->type == INFOSTD_HEAP_MAX && 
		 (keys[INFOSTD_HEAP_PARENT(i)]<key)) ||
		(idhi->type == INFOSTD_HEAP_MIN && 
		 (keys[INFOSTD_HEAP_PARENT(i)]>key)))) {
    
    p=INFOSTD_HEAP_PARENT(i);
    ktmp=keys[p];
    keys[p]=keys[i];
    keys[i]=ktmp;

    itmp=infos[p];
    infos[p]=infos[i];
    infos[i]=itmp;

    i=p;
  }

  return idhi->num_elem;
}

double infostd_dheap_info_pop(INFOSTD_DHEAP_INFO idhi,
			      unsigned long* pinfo) {

  double key;

  if(!idhi->num_elem)
    return ULONG_MAX;

  key = idhi->keys->buff[0];
  *pinfo = idhi->infos->buff[0];
  idhi->num_elem--;
  if(idhi->num_elem) {
    idhi->keys->buff[0] = idhi->keys->buff[idhi->num_elem];
    idhi->infos->buff[0] = idhi->infos->buff[idhi->num_elem];

    infostd_dheap_info_sort(idhi);
  }

  return key;
}

void infostd_dheap_info_sort(INFOSTD_DHEAP_INFO idhi) {
 

  double *keys, ktmp;
  unsigned long *infos, itmp;
  unsigned long i,l,r,m;

  keys = idhi->keys->buff;
  infos = idhi->infos->buff;

  i = l = r = m = 0;

  while(1) {
    l = INFOSTD_HEAP_LEFT(i);
    r = INFOSTD_HEAP_RIGHT(i);
    
    if((l<idhi->num_elem) && 
       ((idhi->type == INFOSTD_HEAP_MAX && keys[l]>keys[i]) ||
	(idhi->type == INFOSTD_HEAP_MIN && keys[l]<keys[i])))
      m=l;
    else
      m=i;
    
    if((r<idhi->num_elem) && 
       ((idhi->type == INFOSTD_HEAP_MAX && keys[r]>keys[m]) ||
	(idhi->type == INFOSTD_HEAP_MIN && keys[r]<keys[m])))      
      m=r;
    
    if(m!=i) {
      ktmp=keys[i];
      keys[i]=keys[m];
      keys[m]=ktmp;

      itmp=infos[i];
      infos[i]=infos[m];
      infos[m]=itmp;

      i=m;
    } else
      break;
  }
}

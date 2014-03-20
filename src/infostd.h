/*----------------------------------------------------------------------------*
|                                                                             |
|   Infostd - A generic text and information handling library                 |
|                                                                             |
*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <regex.h>

#define INFOSTD_VERSION "0.5.2"

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic String Structure and Functions                                    |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef struct str_infostd_dyn_str {
  /* The character buffer */
  unsigned char* buff;
  /* The current length of the string */
  unsigned long length;
  /* The max size of the string */
  unsigned long size;
} *INFOSTD_DYN_STR;
#define SIZE_INFOSTD_DYN_STR (sizeof(struct str_infostd_dyn_str))
#define TYPE_INFOSTD_DYN_STR 826326

INFOSTD_DYN_STR infostd_dyn_str_init(unsigned long size);

INFOSTD_DYN_STR infostd_dyn_str_init_with_str(unsigned char* str);

INFOSTD_DYN_STR infostd_dyn_str_clone(INFOSTD_DYN_STR ids);

void infostd_dyn_str_free(INFOSTD_DYN_STR ids);

void infostd_dyn_str_grow(INFOSTD_DYN_STR ids,
			  unsigned long nsize);

unsigned long infostd_dyn_str_strcpy(INFOSTD_DYN_STR ids,
				     unsigned char* str);
unsigned long infostd_dyn_str_strncpy(INFOSTD_DYN_STR ids,
				      unsigned char* str,
				      unsigned long len);

unsigned long infostd_dyn_str_strcat(INFOSTD_DYN_STR ids,
				     unsigned char* str);

unsigned long infostd_dyn_str_strncat(INFOSTD_DYN_STR ids,
				      unsigned char* str,
				      unsigned long len);

unsigned long infostd_dyn_str_addchar(INFOSTD_DYN_STR ids,
				      unsigned char c);

long infostd_dyn_str_replace(INFOSTD_DYN_STR ids,
			     unsigned long start,
			     unsigned long end,
			     unsigned char* str);

void infostd_dyn_str_insert(INFOSTD_DYN_STR ids,
			    char* str,
			    unsigned long pos);

void infostd_dyn_str_clear(INFOSTD_DYN_STR ids);

unsigned char* infostd_dyn_str_buff(INFOSTD_DYN_STR ids);

unsigned long infostd_dyn_str_length(INFOSTD_DYN_STR ids);

int infostd_dyn_str_fgets(INFOSTD_DYN_STR ids,
			  FILE* in);

void infostd_dyn_str_lower(INFOSTD_DYN_STR ids);

void infostd_dyn_str_upper(INFOSTD_DYN_STR ids);

void infostd_dyn_str_printf(INFOSTD_DYN_STR ids,
			    char* format,
			    ...);

unsigned long infostd_dyn_str_delchar(INFOSTD_DYN_STR ids);

unsigned long infostd_dyn_str_addchar_at(INFOSTD_DYN_STR ids,
					 char c,
					 int p);

unsigned long infostd_dyn_str_delchar_at(INFOSTD_DYN_STR ids,
					 int p);

unsigned long infostd_dyn_str_kill_from(INFOSTD_DYN_STR ids,
					int p);

void infostd_dyn_str_write(INFOSTD_DYN_STR ids,
			   FILE* out);

INFOSTD_DYN_STR infostd_dyn_str_read(FILE* in);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Array ofs Longs, Floats, and Pointers                             |
|                                                                             |
*----------------------------------------------------------------------------*/

/* Pointers */
typedef struct str_infostd_dyn_ptab {
  /* The data */
  void** buff;
  /* The size */
  unsigned long size;

} *INFOSTD_DYN_PTAB;
#define SIZE_INFOSTD_DYN_PTAB (sizeof(struct str_infostd_dyn_ptab))
#define TYPE_INFOSTD_DYN_PTAB 349576

INFOSTD_DYN_PTAB infostd_dyn_ptab_init(unsigned long size);

void infostd_dyn_ptab_free(INFOSTD_DYN_PTAB ipt);

void infostd_dyn_ptab_grow(INFOSTD_DYN_PTAB ipt,
			   unsigned long nsize);

void* infostd_dyn_ptab_get(INFOSTD_DYN_PTAB ipt,
			   unsigned long pos);
  
void* infostd_dyn_ptab_set(INFOSTD_DYN_PTAB ipt,
			   unsigned long pos,
			   void *p);

void** infostd_dyn_ptab_buff(INFOSTD_DYN_PTAB ipt);

INFOSTD_DYN_PTAB infostd_dyn_ptab_read(FILE* in);

void infostd_dyn_ptab_write(INFOSTD_DYN_PTAB ipt, 
			    FILE* out);
/* Longs */
typedef struct str_infostd_dyn_ltab {
  /* The data */
  unsigned long* buff;
  /* The size */
  unsigned long size;

} *INFOSTD_DYN_LTAB;
#define SIZE_INFOSTD_DYN_LTAB (sizeof(struct str_infostd_dyn_ltab))
#define TYPE_INFOSTD_DYN_LTAB 923562

INFOSTD_DYN_LTAB infostd_dyn_ltab_init(unsigned long size);

void infostd_dyn_ltab_free(INFOSTD_DYN_LTAB ilt);

void infostd_dyn_ltab_grow(INFOSTD_DYN_LTAB ilt,
			   unsigned long nsize);

unsigned long infostd_dyn_ltab_get(INFOSTD_DYN_LTAB ilt,
				   unsigned long pos);

unsigned long infostd_dyn_ltab_set(INFOSTD_DYN_LTAB ilt,
				   unsigned long pos,
				   unsigned long l);

unsigned long* infostd_dyn_ltab_buff(INFOSTD_DYN_LTAB ilt);

INFOSTD_DYN_LTAB infostd_dyn_ltab_read(FILE* in);

void infostd_dyn_ltab_write(INFOSTD_DYN_LTAB ilt, 
			    FILE* out);

/* Floats */
typedef struct str_infostd_dyn_ftab {
  /* The data */
  float* buff;
  /* The size */
  unsigned long size;

} *INFOSTD_DYN_FTAB;
#define SIZE_INFOSTD_DYN_FTAB (sizeof(struct str_infostd_dyn_ftab))
#define TYPE_INFOSTD_DYN_FTAB 1358254

INFOSTD_DYN_FTAB infostd_dyn_ftab_init(unsigned long size);

void infostd_dyn_ftab_free(INFOSTD_DYN_FTAB ift);

void infostd_dyn_ftab_grow(INFOSTD_DYN_FTAB ift,
			   unsigned long nsize);

float infostd_dyn_ftab_get(INFOSTD_DYN_FTAB ift,
			   unsigned long pos);

float infostd_dyn_ftab_set(INFOSTD_DYN_FTAB ift,
			   unsigned long pos,
			   float l);

float* infostd_dyn_ftab_buff(INFOSTD_DYN_FTAB ift);

INFOSTD_DYN_FTAB infostd_dyn_ftab_read(FILE* in);

void infostd_dyn_ftab_write(INFOSTD_DYN_FTAB ift, 
			    FILE* out);

/* Doubles */
typedef struct str_infostd_dyn_dtab {
  /* The data */
  double* buff;
  /* The size */
  unsigned long size;

} *INFOSTD_DYN_DTAB;
#define SIZE_INFOSTD_DYN_DTAB (sizeof(struct str_infostd_dyn_dtab))
#define TYPE_INFOSTD_DYN_DTAB 1358257

INFOSTD_DYN_DTAB infostd_dyn_dtab_init(unsigned long size);

void infostd_dyn_dtab_free(INFOSTD_DYN_DTAB idt);

void infostd_dyn_dtab_grow(INFOSTD_DYN_DTAB idt,
			   unsigned long nsize);

double infostd_dyn_dtab_get(INFOSTD_DYN_DTAB idt,
			    unsigned long pos);

double infostd_dyn_dtab_set(INFOSTD_DYN_DTAB idt,
			    unsigned long pos,
			    double l);

double* infostd_dyn_dtab_buff(INFOSTD_DYN_DTAB idt);

INFOSTD_DYN_DTAB infostd_dyn_dtab_read(FILE* in);

void infostd_dyn_dtab_write(INFOSTD_DYN_DTAB idt, 
			    FILE* out);

/* Characters */
typedef struct str_infostd_dyn_ctab {
  /* The data */
  char* buff;
  /* The size */
  unsigned long size;

} *INFOSTD_DYN_CTAB;
#define SIZE_INFOSTD_DYN_CTAB (sizeof(struct str_infostd_dyn_ctab))
#define TYPE_INFOSTD_DYN_CTAB 727811

INFOSTD_DYN_CTAB infostd_dyn_ctab_init(unsigned long size);

void infostd_dyn_ctab_free(INFOSTD_DYN_CTAB ipt);

void infostd_dyn_ctab_grow(INFOSTD_DYN_CTAB ipt,
			   unsigned long nsize);

char infostd_dyn_ctab_get(INFOSTD_DYN_CTAB ipt,
			  unsigned long pos);

char infostd_dyn_ctab_set(INFOSTD_DYN_CTAB ipt,
			  unsigned long pos,
			  char c);

char* infostd_dyn_ctab_buff(INFOSTD_DYN_CTAB ipt);

INFOSTD_DYN_CTAB infostd_dyn_ctab_read(FILE* in);

void infostd_dyn_ctab_write(INFOSTD_DYN_CTAB ipt, 
			    FILE* out);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Perl style Regular Expressions                                            |
|                                                                             |
*----------------------------------------------------------------------------*/

#define REGEXP_MAX_MATCH 127

typedef struct str_infostd_regexp_eng {

  /* The pattern */
  INFOSTD_DYN_STR pat;
  /* The substituion pattern */
  INFOSTD_DYN_STR subpat;
  /* The marked up subsitution pattern */
  INFOSTD_DYN_STR nspat;
  /* The string to build */
  INFOSTD_DYN_STR str;

  /* Flags */
  char opt[26];

  /* The compiled pattern */
  regex_t rx_comp;

  /* The regular expression matches */
  regmatch_t pmatch[REGEXP_MAX_MATCH];   
} *INFOSTD_REGEXP_ENGINE;
#define SIZE_INFOSTD_REGEXP_ENGINE (sizeof(struct str_infostd_regexp_eng))

typedef enum {
  INFOSTD_REGEXP_NONE,
  INFOSTD_REGEXP_ERROR, INFOSTD_REGEXP_NOMATCH,
  INFOSTD_REGEXP_MATCH, INFOSTD_REGEXP_SUBSTITUTION
} INFOSTD_REGEXP_RESULT_TYPE;
  
typedef struct str_infostd_regexp_res {
  /* The type */
  INFOSTD_REGEXP_RESULT_TYPE type;
  /* The string contain whatever the result is */
  INFOSTD_DYN_STR result;
  /* The positions in the original string of each result */
  INFOSTD_DYN_LTAB positions;
  /* Number of matches for a match type result */
  unsigned long num_matches;

} *INFOSTD_REGEXP_RESULT;
#define SIZE_INFOSTD_REGEXP_RESULT (sizeof(struct str_infostd_regexp_res))

INFOSTD_REGEXP_ENGINE infostd_regexp_engine_init(void);

void infostd_regexp_engine_clear(INFOSTD_REGEXP_ENGINE eng);

void infostd_regexp_engine_free(INFOSTD_REGEXP_ENGINE eng);

INFOSTD_REGEXP_RESULT infostd_regexp_result_init(void);

int infostd_regexp_result_error(INFOSTD_REGEXP_RESULT res,
				char *s);

int infostd_regexp_result_nomatch(INFOSTD_REGEXP_RESULT res);

void infostd_regexp_result_print(INFOSTD_REGEXP_RESULT res);

void infostd_regexp_result_clear(INFOSTD_REGEXP_RESULT res);

void infostd_regexp_result_free(INFOSTD_REGEXP_RESULT res);

void infostd_regexp_make_pattern(INFOSTD_REGEXP_ENGINE eng,
				 char* s,
				 int len);

void infostd_regexp_make_substitution(INFOSTD_REGEXP_ENGINE eng,
				      char* sub,      
				      unsigned long len,
				      char* orig);

int infostd_regexp(INFOSTD_REGEXP_ENGINE eng,
		   INFOSTD_REGEXP_RESULT res,
		   char *s, 
		   unsigned long slen,
		   char *r);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Dynamic Strings                                                           |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef struct str_infostd_dyn_strs {
  /* The string */
  INFOSTD_DYN_STR str;
  /* The positions */
  INFOSTD_DYN_LTAB pos;
  /* Number of strings */
  unsigned long num;
} *INFOSTD_DYN_STRS;
#define SIZE_INFOSTD_DYN_STRS (sizeof(struct str_infostd_dyn_strs))
#define TYPE_INFOSTD_DYN_STRS 826327

INFOSTD_DYN_STRS infostd_dyn_strs_init(unsigned long size);

void infostd_dyn_strs_free(INFOSTD_DYN_STRS iss);

unsigned char* infostd_dyn_strs_get(INFOSTD_DYN_STRS iss,
				    unsigned long pos);

void infostd_dyn_strs_set(INFOSTD_DYN_STRS iss,
			  unsigned long pos,
			  unsigned char* str);

unsigned long infostd_dyn_strs_add(INFOSTD_DYN_STRS iss,
				   unsigned char* str);

void infostd_dyn_strs_clear(INFOSTD_DYN_STRS iss);

unsigned long infostd_dyn_strs_num(INFOSTD_DYN_STRS iss);

void infostd_dyn_strs_write(INFOSTD_DYN_STRS iss,
			    FILE* out);

INFOSTD_DYN_STRS infostd_dyn_strs_read(FILE* in);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Text CRC and Encoding Functions                                           |
|                                                                             |
*----------------------------------------------------------------------------*/

unsigned long infostd_crc32(char *str, 
			    unsigned long len);

unsigned long infostd_base64_encode(char* in, 
				    unsigned long len, 
				    INFOSTD_DYN_STR out);

unsigned long infostd_base64_decode(char* in, 
				    unsigned long len,
				    INFOSTD_DYN_STR out);

/*----------------------------------------------------------------------------*
|                                                                             |
|   String Hash with lookup                                                   |
|                                                                             |
*----------------------------------------------------------------------------*/

typedef struct str_infostd_hash_str_tab {
  unsigned char* str;
  unsigned long str_len;
  unsigned long info;
  
  struct str_infostd_hash_str_tab* next;
} *INFOSTD_HASH_STR_TAB;
#define SIZE_INFOSTD_HASH_STR_TAB (sizeof(struct str_infostd_hash_str_tab))

typedef struct str_infostd_hash_str {
  INFOSTD_HASH_STR_TAB *tab;
  unsigned long size;

  unsigned long num_elem;

  INFOSTD_DYN_STR info;

} *INFOSTD_HASH_STR;
#define SIZE_INFOSTD_HASH_STR (sizeof(struct str_infostd_hash_str))
#define TYPE_INFOSTD_HASH_STR 728954

unsigned long infostd_hash_str_hash(unsigned char *str,
				    unsigned long size);

INFOSTD_HASH_STR_TAB infostd_hash_str_tab_init(unsigned char* str,
					       unsigned long str_len,
					       unsigned long info);

void infostd_hash_str_tab_free(INFOSTD_HASH_STR_TAB hst);

INFOSTD_HASH_STR infostd_hash_str_init(unsigned long size);

void infostd_hash_str_free(INFOSTD_HASH_STR ihs);

unsigned long infostd_hash_str_rm(INFOSTD_HASH_STR ihs,
				  unsigned char* key);

unsigned long infostd_hash_str_info_update(INFOSTD_HASH_STR ihs,
					   unsigned char* key,
					   unsigned long info);

unsigned long infostd_hash_str_info_chg(INFOSTD_HASH_STR ihs,
					unsigned char* key,
					unsigned long info);

unsigned long infostd_hash_str_info(INFOSTD_HASH_STR ihs,
				    unsigned char* key);

unsigned long infostd_hash_str_string(INFOSTD_HASH_STR ihs,
				      unsigned char* key,
				      INFOSTD_DYN_STR out);

unsigned long infostd_hash_str_add(INFOSTD_HASH_STR ihs,
				   unsigned char* key,
				   unsigned long info);

unsigned long infostd_hash_str_add_string(INFOSTD_HASH_STR ihs,
					  unsigned char* key,
					  unsigned char* info);

INFOSTD_HASH_STR infostd_hash_str_read(FILE* in);

void infostd_hash_str_write(INFOSTD_HASH_STR ihs, 
			    FILE* out);
/*----------------------------------------------------------------------------*
|                                                                             |
|   Hash Long                                                                 |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef struct str_infostd_hash_long_tab {
  unsigned long key;
  unsigned long info;
  
  struct str_infostd_hash_long_tab* next;
} *INFOSTD_HASH_LONG_TAB;
#define SIZE_INFOSTD_HASH_LONG_TAB (sizeof(struct str_infostd_hash_long_tab))

typedef struct str_infostd_hash_long {
  INFOSTD_HASH_LONG_TAB *tab;
  unsigned long size;
  unsigned long num_elem;  
} *INFOSTD_HASH_LONG;
#define SIZE_INFOSTD_HASH_LONG (sizeof(struct str_infostd_hash_long))
#define TYPE_INFOSTD_HASH_LONG 728957

unsigned long infostd_hash_long_hash(unsigned long key,
				     unsigned long size);

INFOSTD_HASH_LONG_TAB infostd_hash_long_tab_init(unsigned long key,
						 unsigned long info);

void infostd_hash_long_tab_free(INFOSTD_HASH_LONG_TAB hlt);

INFOSTD_HASH_LONG infostd_hash_long_init(unsigned long size);

void infostd_hash_long_free(INFOSTD_HASH_LONG ihl);

unsigned long infostd_hash_long_rm(INFOSTD_HASH_LONG ihl,
				   unsigned long key);

unsigned long infostd_hash_long_info_update(INFOSTD_HASH_LONG ihl,
					    unsigned long key,
					    unsigned long info);

unsigned long infostd_hash_long_info_chg(INFOSTD_HASH_LONG ihl,
					 unsigned long key,
					 unsigned long info);

unsigned long infostd_hash_long_info(INFOSTD_HASH_LONG ihl,
				     unsigned long key);

unsigned long infostd_hash_long_add(INFOSTD_HASH_LONG ihl,
				    unsigned long key,
				    unsigned long info);

INFOSTD_HASH_LONG infostd_hash_long_read(FILE* in);

void infostd_hash_long_write(INFOSTD_HASH_LONG ihl, 
			     FILE* out);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Heaps of Longs and Doubles                                                |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef enum {
  INFOSTD_HEAP_MAX, INFOSTD_HEAP_MIN
} INFOSTD_HEAP_TYPE;

/* Pointers */
typedef struct str_infostd_pheap {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_PTAB keys;

  int (*compare)(void*, void*);

} *INFOSTD_PHEAP;
#define SIZE_INFOSTD_PHEAP (sizeof(struct str_infostd_pheap))  

int infostd_pheap_gencmp(void* a, void* b);

INFOSTD_PHEAP infostd_pheap_init(INFOSTD_HEAP_TYPE type,
				 int (*compare)(void*, void*),
				 unsigned long size);

void infostd_pheap_free(INFOSTD_PHEAP iph);

unsigned long infostd_pheap_push(INFOSTD_PHEAP iph,
				 void* key);

void* infostd_pheap_pop(INFOSTD_PHEAP iph);

void infostd_pheap_sort(INFOSTD_PHEAP iph);

/* Pointer Info */
typedef struct str_infostd_pheap_info {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_PTAB keys;
  INFOSTD_DYN_LTAB infos;
  
  int (*compare)(void*, void*);
  
} *INFOSTD_PHEAP_INFO;
#define SIZE_INFOSTD_PHEAP_INFO (sizeof(struct str_infostd_pheap_info))  

int infostd_pheap_info_gencmp(void* a, void* b);

INFOSTD_PHEAP_INFO infostd_pheap_info_init(INFOSTD_HEAP_TYPE type,
					   int (*compare)(void*, void*),
					   unsigned long size);

void infostd_pheap_info_free(INFOSTD_PHEAP_INFO iphi);

unsigned long infostd_pheap_info_push(INFOSTD_PHEAP_INFO iphi,
				      void* key,
				      unsigned long info);

void* infostd_pheap_info_pop(INFOSTD_PHEAP_INFO iphi,
			     unsigned long *pinfo);

void infostd_pheap_info_sort(INFOSTD_PHEAP_INFO iphi);

/* Long Heap */
typedef struct str_infostd_lheap {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_LTAB keys;
} *INFOSTD_LHEAP;
#define SIZE_INFOSTD_LHEAP (sizeof(struct str_infostd_lheap))  

INFOSTD_LHEAP infostd_lheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size);

void infostd_lheap_free(INFOSTD_LHEAP ilh);

unsigned long infostd_lheap_push(INFOSTD_LHEAP ilh,
				 unsigned long key);

unsigned long infostd_lheap_pop(INFOSTD_LHEAP ilh);

void infostd_lheap_sort(INFOSTD_LHEAP ilh);

/* Long Info Heap */
typedef struct str_infostd_lheap_info {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_LTAB keys;
  INFOSTD_DYN_LTAB infos;
} *INFOSTD_LHEAP_INFO;
#define SIZE_INFOSTD_LHEAP_INFO (sizeof(struct str_infostd_lheap_info))  

INFOSTD_LHEAP_INFO infostd_lheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size);

void infostd_lheap_info_free(INFOSTD_LHEAP_INFO ilhi);

unsigned long infostd_lheap_info_push(INFOSTD_LHEAP_INFO ilhi,
				      unsigned long key,
				      unsigned long info);

unsigned long infostd_lheap_info_pop(INFOSTD_LHEAP_INFO ilhi,
				     unsigned long* pinfo);

void infostd_lheap_info_sort(INFOSTD_LHEAP_INFO ilhi);

/* Float Heap */
typedef struct str_infostd_fheap {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_FTAB keys;
} *INFOSTD_FHEAP;
#define SIZE_INFOSTD_FHEAP (sizeof(struct str_infostd_fheap))  

INFOSTD_FHEAP infostd_fheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size);

void infostd_fheap_free(INFOSTD_FHEAP ifh);

unsigned long infostd_fheap_push(INFOSTD_FHEAP ifh,
				 float key);

float infostd_fheap_pop(INFOSTD_FHEAP ifh);

void infostd_fheap_sort(INFOSTD_FHEAP ifh);

/* Float Info Heap */
typedef struct str_infostd_fheap_info {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_FTAB keys;
  INFOSTD_DYN_LTAB infos;
} *INFOSTD_FHEAP_INFO;
#define SIZE_INFOSTD_FHEAP_INFO (sizeof(struct str_infostd_fheap_info))  

INFOSTD_FHEAP_INFO infostd_fheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size);

void infostd_fheap_info_free(INFOSTD_FHEAP_INFO ifhi);

unsigned long infostd_fheap_info_push(INFOSTD_FHEAP_INFO ifhi,
				      float key,
				      unsigned long info);

float infostd_fheap_info_pop(INFOSTD_FHEAP_INFO ifhi,
			     unsigned long* pinfo);

void infostd_fheap_info_sort(INFOSTD_FHEAP_INFO ifhi);

/* Double Heap */
typedef struct str_infostd_dheap {
  INFOSTD_HEAP_TYPE type;
  unsigned long num_elem;
  INFOSTD_DYN_DTAB keys;
} *INFOSTD_DHEAP;
#define SIZE_INFOSTD_DHEAP (sizeof(struct str_infostd_dheap))  

INFOSTD_DHEAP infostd_dheap_init(INFOSTD_HEAP_TYPE type,
				 unsigned long size);

void infostd_dheap_free(INFOSTD_DHEAP idh);

unsigned long infostd_dheap_push(INFOSTD_DHEAP idh,
				 double key);


double infostd_dheap_pop(INFOSTD_DHEAP idh);

void infostd_dheap_sort(INFOSTD_DHEAP idh);

/* Double Info Heap*/
typedef struct str_infostd_dheap_info {
  INFOSTD_HEAP_TYPE type;
  
  unsigned long num_elem;
  INFOSTD_DYN_DTAB keys;
  INFOSTD_DYN_LTAB infos;
} *INFOSTD_DHEAP_INFO;
#define SIZE_INFOSTD_DHEAP_INFO (sizeof(struct str_infostd_dheap_info))  

INFOSTD_DHEAP_INFO infostd_dheap_info_init(INFOSTD_HEAP_TYPE type,
					   unsigned long size);

void infostd_dheap_info_free(INFOSTD_DHEAP_INFO idhi);

unsigned long infostd_dheap_info_push(INFOSTD_DHEAP_INFO idhi,
				      double key,
				      unsigned long info);

double infostd_dheap_info_pop(INFOSTD_DHEAP_INFO idhi,
			      unsigned long* pinfo);

void infostd_dheap_info_sort(INFOSTD_DHEAP_INFO idhi);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Tokenizer and Tokens                                                      |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef enum {
  INFOSTD_TOKENIZER_SET, INFOSTD_TOKENIZER_REGEXP
} INFOSTD_TOKENIZER_TYPE;

typedef enum {
  INFOSTD_TOKEN_NONE, INFOSTD_TOKEN_WORD, INFOSTD_TOKEN_SEP
} INFOSTD_TOKEN_TYPE;

typedef struct str_infostd_token {
  /* The type of the tokenizer that made the token */
  INFOSTD_TOKEN_TYPE type;
  INFOSTD_TOKENIZER_TYPE parent_type;

  /* The positions */
  unsigned long start;
  unsigned long end;

  /* The string */
  INFOSTD_DYN_STR ids;

  /* The position in the string */
  unsigned long pos;

  /* Data pointers */
  void* data0;
  void* data1;
  void* data2;

  /* Flags? end of sentence? dashes? capitalization */  

} *INFOSTD_TOKEN;
#define SIZE_INFOSTD_TOKEN (sizeof(struct str_infostd_token))

typedef struct str_infostd_tokenizer {
  /* The type */
  INFOSTD_TOKENIZER_TYPE type;

  /* Character data: the char set  or the regexp, etc */
  INFOSTD_DYN_STR data;
  
  /* Function pointers */
  INFOSTD_TOKEN (*token_first)(struct str_infostd_tokenizer*,INFOSTD_TOKEN,
			       unsigned char*);
  INFOSTD_TOKEN (*token_next)(struct str_infostd_tokenizer*,INFOSTD_TOKEN,
			      unsigned char*);
  
} *INFOSTD_TOKENIZER;
#define SIZE_INFOSTD_TOKENIZER (sizeof(struct str_infostd_tokenizer))
#define TYPE_INFOSTD_TOKENIZER 3394527

extern unsigned char* INFOSTD_TOKENIZER_DEFAULT_SET;
extern unsigned char* INFOSTD_TOKENIZER_DEFAULT_REGEXP;

#define infostd_tokenizer_token_first(x,y,z) (*(x->token_first))(x,y,z)
#define infostd_tokenizer_token_next(x,y,z) (*(x->token_next))(x,y,z)

unsigned long infostd_token_tab_init(unsigned char* tab,
				     unsigned char* set);

INFOSTD_TOKEN infostd_token_init(INFOSTD_TOKENIZER tkz);

void infostd_token_free(INFOSTD_TOKEN itk);

INFOSTD_TOKENIZER infostd_tokenizer_init_set(unsigned char* set);

INFOSTD_TOKENIZER infostd_tokenizer_init_regexp(unsigned char* regexp);

void infostd_tokenizer_free(INFOSTD_TOKENIZER tkz);

INFOSTD_TOKEN infostd_token_first_set(INFOSTD_TOKENIZER tkz,
				      INFOSTD_TOKEN itk,
				      unsigned char* str);

INFOSTD_TOKEN infostd_token_next_set(INFOSTD_TOKENIZER tkz,
				     INFOSTD_TOKEN itk,
				     unsigned char* str);

INFOSTD_TOKEN infostd_token_first_regexp(INFOSTD_TOKENIZER tkz,
					 INFOSTD_TOKEN itk,
					 unsigned char* str);

INFOSTD_TOKEN infostd_token_next_regexp(INFOSTD_TOKENIZER tkz,
					INFOSTD_TOKEN itk,
					unsigned char* str);

void infostd_tokenizer_write(INFOSTD_TOKENIZER tkz,
			     FILE* out);

INFOSTD_TOKENIZER infostd_tokenizer_read(FILE* in);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Spell checking and correction                                             |
|                                                                             |
*----------------------------------------------------------------------------*/
extern unsigned long INFOSTD_METAPHONE_MAX;
extern unsigned long INFOSTD_SPELLER_SUGGESTION_MAX;

typedef struct str_infostd_speller {
  /* The Hashes for words */
  INFOSTD_HASH_STR dictionary;
  INFOSTD_HASH_STR metaphones;

  INFOSTD_HASH_STR ignores;
  INFOSTD_HASH_STR mappings;

  /* Scratch memory */
  INFOSTD_DYN_STR ids, ids2;
  INFOSTD_DYN_LTAB idl;
  INFOSTD_DYN_STRS idss;
} *INFOSTD_SPELLER;
#define SIZE_INFOSTD_SPELLER (sizeof(struct str_infostd_speller))
#define TYPE_INFOSTD_SPELLER 2357459

unsigned long infostd_speller_metaphone(unsigned char* str,
					unsigned long len,
					INFOSTD_DYN_STR mstr);

unsigned long infostd_speller_edit_distance(INFOSTD_DYN_LTAB dists,
					    unsigned char* str1,
					    unsigned long slen1,
					    unsigned char* str2,
					    unsigned long slen2);

INFOSTD_SPELLER infostd_speller_init(unsigned long size);

void infostd_speller_free(INFOSTD_SPELLER isp);

unsigned long infostd_speller_add(INFOSTD_SPELLER isp,
				  unsigned char* word);

unsigned long infostd_speller_add_ignore(INFOSTD_SPELLER isp,
					 unsigned char* word);

unsigned long infostd_speller_add_mapping(INFOSTD_SPELLER isp,
					  unsigned char* word,
					  unsigned char* map);

void infostd_speller_write(INFOSTD_SPELLER isp,
			   FILE* out);

INFOSTD_SPELLER infostd_speller_read(FILE* in);

int infostd_speller_check(INFOSTD_SPELLER isp,
			  unsigned char* word);

double infostd_speller_suggestion_score(INFOSTD_SPELLER isp,
					unsigned char* word,
					unsigned char* sugg);

unsigned long infostd_speller_suggestions(INFOSTD_SPELLER isp,
					  unsigned char* word,
					  INFOSTD_DYN_STRS idss);

INFOSTD_SPELLER infostd_speller_load_config(char* filename);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Stemming and morphological equivalents                                    |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef struct str_infostd_stemmer {
  /* Form -> root */
  INFOSTD_HASH_STR roots;
  INFOSTD_DYN_STR ids_roots;
  /* Form -> all forms */
  INFOSTD_HASH_STR forms;

  /* Scratch memory */
  INFOSTD_DYN_STR ids, ids2;  
} *INFOSTD_STEMMER;
#define SIZE_INFOSTD_STEMMER (sizeof(struct str_infostd_stemmer))
#define TYPE_INFOSTD_STEMMER 5374986

INFOSTD_STEMMER infostd_stemmer_init(unsigned long size);

void infostd_stemmer_free(INFOSTD_STEMMER ist);

INFOSTD_STEMMER infostd_stemmer_build(FILE* in,
				      unsigned long size);

unsigned long infostd_stemmer_root(INFOSTD_STEMMER ist,
				   char* word,
				   INFOSTD_DYN_STR ids);

unsigned long infostd_stemmer_forms(INFOSTD_STEMMER ist,
				    char* word,
				    INFOSTD_DYN_STR ids);

void infostd_stemmer_write(INFOSTD_STEMMER ist,
			   FILE* out);

INFOSTD_STEMMER infostd_stemmer_read(FILE* in);

/*----------------------------------------------------------------------------*
|                                                                             |
|   Finite State Machine and Transducer                                       |
|                                                                             |
*----------------------------------------------------------------------------*/
typedef struct str_infostd_fsm_state {
  INFOSTD_DYN_LTAB transitions;
  INFOSTD_DYN_LTAB states;
  unsigned long num_trans;
  
  INFOSTD_HASH_LONG lookup;

} *INFOSTD_FSM_STATE;
#define SIZE_INFOSTD_FSM_STATE (sizeof(struct str_infostd_fsm_state))
#define TYPE_INFOSTD_FSM_STATE 3068984

typedef struct str_infostd_fsm {
  unsigned long state;

  INFOSTD_HASH_LONG final_states;
  
  INFOSTD_DYN_PTAB states;
  unsigned long num_states;

  unsigned long max_state;
  
} *INFOSTD_FSM;
#define SIZE_INFOSTD_FSM (sizeof(struct str_infostd_fsm))
#define TYPE_INFOSTD_FSM 3068985

#define INFOSTD_FSM_START_STATE 0

INFOSTD_FSM_STATE infostd_fsm_state_init(void);

void infostd_fsm_state_free(INFOSTD_FSM_STATE ifss);

INFOSTD_FSM infostd_fsm_init(unsigned long size);

void infostd_fsm_free(INFOSTD_FSM ifsm);

void infostd_fsm_add_transition(INFOSTD_FSM ifsm,
				unsigned long input,
				unsigned long curr_state,
				unsigned long new_state);

void infostd_fsm_add_final(INFOSTD_FSM ifsm,
			   unsigned long state);

unsigned long infostd_fsm_state(INFOSTD_FSM ifsm);

int infostd_fsm_state_final(INFOSTD_FSM ifsm);


unsigned long infostd_fsm_state_transitions(INFOSTD_FSM ifsm, 
					    unsigned long state,
					    INFOSTD_DYN_LTAB trans,
					    INFOSTD_DYN_LTAB states);

int infostd_fsm_transition_exists(INFOSTD_FSM ifsm, 
				  unsigned long state,
				  unsigned long data);

int infostd_fsm_transition_state(INFOSTD_FSM ifsm, 
				 unsigned long state,
				 unsigned long data);

int infostd_fsm_transition(INFOSTD_FSM ifsm, unsigned long data);

INFOSTD_FSM infostd_fsm_build_matcher(char *str, 
				      unsigned long length);

void infostd_fsm_add_matcher(INFOSTD_FSM ifsm,
			     char *str, 
			     unsigned long length);

int infostd_fsm_matches(INFOSTD_FSM ifsm,
			char* str,
			unsigned long length);


/*----------------------------------------------------------------------------*
|                                                                             |
|   Queues for longs, floats, doubles, and pointers                           |
|                                                                             |
*----------------------------------------------------------------------------*/

/* Longs */
typedef struct str_infostd_lqueue {
  unsigned long size;
  INFOSTD_DYN_LTAB tab;
  
  unsigned long start;
  unsigned long end;
  unsigned long pos;
  unsigned long num;

} *INFOSTD_LQUEUE;
#define SIZE_INFOSTD_LQUEUE (sizeof(struct str_infostd_lqueue))
#define TYPE_INFOSTD_LQUEUE 3071489

INFOSTD_LQUEUE infostd_lqueue_init(unsigned long size);
void infostd_lqueue_free(INFOSTD_LQUEUE lq);

unsigned long infostd_lqueue_push(INFOSTD_LQUEUE lq,
				  unsigned long data);

unsigned long infostd_lqueue_pop(INFOSTD_LQUEUE lq);

/* Floats */
typedef struct str_infostd_fqueue {
  unsigned long size;
  INFOSTD_DYN_FTAB tab;
  
  unsigned long start;
  unsigned long end;
  unsigned long pos;
  unsigned long num;

} *INFOSTD_FQUEUE;
#define SIZE_INFOSTD_FQUEUE (sizeof(struct str_infostd_fqueue))
#define TYPE_INFOSTD_FQUEUE 3071490

INFOSTD_FQUEUE infostd_fqueue_init(unsigned long size);
void infostd_fqueue_free(INFOSTD_FQUEUE fq);

unsigned long infostd_fqueue_push(INFOSTD_FQUEUE fq,
				  float data);

float infostd_fqueue_pop(INFOSTD_FQUEUE fq);

/* Doubles */
typedef struct str_infostd_dqueue {
  unsigned long size;
  INFOSTD_DYN_DTAB tab;
  
  unsigned long start;
  unsigned long end;
  unsigned long pos;
  unsigned long num;

} *INFOSTD_DQUEUE;
#define SIZE_INFOSTD_DQUEUE (sizeof(struct str_infostd_dqueue))
#define TYPE_INFOSTD_DQUEUE 3071491

INFOSTD_DQUEUE infostd_dqueue_init(unsigned long size);
void infostd_dqueue_free(INFOSTD_DQUEUE dq);

unsigned long infostd_dqueue_push(INFOSTD_DQUEUE dq,
				  double data);

double infostd_dqueue_pop(INFOSTD_DQUEUE dq);

/* Pointers */
typedef struct str_infostd_pqueue {
  unsigned long size;
  INFOSTD_DYN_PTAB tab;
  
  unsigned long start;
  unsigned long end;
  unsigned long pos;
  unsigned long num;

} *INFOSTD_PQUEUE;
#define SIZE_INFOSTD_PQUEUE (sizeof(struct str_infostd_pqueue))
#define TYPE_INFOSTD_PQUEUE 3071492

INFOSTD_PQUEUE infostd_pqueue_init(unsigned long size);
void infostd_pqueue_free(INFOSTD_PQUEUE pq);

unsigned long infostd_pqueue_push(INFOSTD_PQUEUE pq,
				  void *data);

void *infostd_pqueue_pop(INFOSTD_PQUEUE pq);

/* Characters */
typedef struct str_infostd_cqueue {
  unsigned long size;
  INFOSTD_DYN_CTAB tab;
  
  unsigned long start;
  unsigned long end;
  unsigned long pos;
  unsigned long num;

} *INFOSTD_CQUEUE;
#define SIZE_INFOSTD_CQUEUE (sizeof(struct str_infostd_cqueue))
#define TYPE_INFOSTD_CQUEUE 3070722

INFOSTD_CQUEUE infostd_cqueue_init(unsigned long size);
void infostd_cqueue_free(INFOSTD_CQUEUE cq);

unsigned long infostd_cqueue_push(INFOSTD_CQUEUE cq,
				  char data);

char infostd_cqueue_pop(INFOSTD_CQUEUE cq);

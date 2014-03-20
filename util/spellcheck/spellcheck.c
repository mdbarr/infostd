#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <infostd.h>

#define SPELLER_SIZE 100000

int main(int argc, char** argv) {
  
  INFOSTD_SPELLER isp;
  INFOSTD_DYN_STR ids, ids2;
  INFOSTD_DYN_STRS idss;
  unsigned long i, n;
  char *in=NULL, *out=NULL;
  int mode = 0;
  FILE *fin, *fout;

  if(argc == 4 && !strcasecmp(argv[1],"-b")) {
    in = argv[2];
    out = argv[3];    
    mode = 1;
  } else if(argc == 3 && !strcasecmp(argv[1],"-c")) {
    in = argv[2];
    mode=2;
  } else if(argc == 2) {
    in = argv[1];
    mode = 0;
  } else {
    printf("Usage: %s [-c speller.conf | -b in.words spell.speller | spell.speller]\n",argv[0]);
    exit(0);
  }

  if(mode == 0) {
  
    fin = fopen(in,"r");
    if(!fin) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }

    isp = infostd_speller_read(fin);
    
    if(!isp) {
      printf("Couldn't read speller %s.\n",in);
      exit(0);
    }

    fclose(fin);  

    ids = infostd_dyn_str_init(100);
    ids2 = infostd_dyn_str_init(100);
    idss = infostd_dyn_strs_init(100);

    while(infostd_dyn_str_fgets(ids,stdin)) {
      if(infostd_speller_check(isp,ids->buff))
	printf("CORRECT\n");
      else {
	printf("INCORRECT");
	if((n=infostd_speller_suggestions(isp,ids->buff,idss))>0) {
	  for(i=0;i<n;i++)
	    printf(" %s",infostd_dyn_strs_get(idss,i));
	}
	printf("\n");
      }
    }
	
    infostd_dyn_str_free(ids);  
    infostd_dyn_str_free(ids2);  
    infostd_dyn_strs_free(idss);
    infostd_speller_free(isp);

  } else if(mode == 1) {
    isp = infostd_speller_init(SPELLER_SIZE);
    
    if(!isp) {
      printf("Can't create speller in memory!\n");
      exit(0);
    }

    fin = fopen(in,"r");
    if(!fin) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }
    
    n = 0;

    ids = infostd_dyn_str_init(100);

    while(infostd_dyn_str_fgets(ids,fin)) {      
      n = infostd_speller_add(isp,ids->buff);
      if(n%50==1)
	printf(".");
    }
    printf("\n");

    fclose(fin);

    printf("Generated %lu words\n",n);

    fout = fopen(out,"w");
    if(!fout) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }

    infostd_speller_write(isp,fout);

    infostd_dyn_str_free(ids);
    infostd_speller_free(isp);
  } else if(mode == 2) {
    isp = infostd_speller_load_config(in);
    
    if(!isp) {
      printf("Couldn't read speller %s.\n",in);
      exit(0);
    }

    ids = infostd_dyn_str_init(100);
    ids2 = infostd_dyn_str_init(100);
    idss = infostd_dyn_strs_init(100);

    while(infostd_dyn_str_fgets(ids,stdin)) {
      if(infostd_speller_check(isp,ids->buff))
	printf("CORRECT\n");
      else {
	printf("INCORRECT");
	if((n=infostd_speller_suggestions(isp,ids->buff,idss))>0) {
	  for(i=0;i<n;i++)
	    printf(" %s",infostd_dyn_strs_get(idss,i));
	}
	printf("\n");
      }
    }
	
    infostd_dyn_str_free(ids);  
    infostd_dyn_str_free(ids2);  
    infostd_dyn_strs_free(idss);
    infostd_speller_free(isp);
  }
	 
  return 0;
}

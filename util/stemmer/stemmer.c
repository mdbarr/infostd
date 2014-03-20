#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <infostd.h>

#define STEMMER_SIZE 100000

int main(int argc, char** argv) {
  
  INFOSTD_STEMMER ist;
  INFOSTD_DYN_STR ids, ids2;
  char *in=NULL, *out=NULL;
  FILE *fin, *fout;
  int mode = 0;


  if(argc == 4 && !strcasecmp(argv[1],"-b")) {
    in = argv[2];
    out = argv[3];    
    mode = 1;
  } else if(argc == 2) {
    in = argv[1];
    mode = 0;
  } else {
    printf("Usage: %s [-b in.words stemm.stemmer | stemm.stemmer]\n",argv[0]);
    exit(0);
  }

  if(mode == 0) {
  
    fin = fopen(in,"r");
    if(!fin) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }

    ist = infostd_stemmer_read(fin);
    
    if(!ist) {
      printf("Couldn't read stemmer %s.\n",in);
      exit(0);
    }

    fclose(fin);  

    ids = infostd_dyn_str_init(100);
    ids2 = infostd_dyn_str_init(100);

    while(infostd_dyn_str_fgets(ids,stdin)) {
      if(infostd_stemmer_root(ist,ids->buff,ids2))
	printf("ROOT: %s\n",ids2->buff);
      if(infostd_stemmer_forms(ist,ids->buff,ids2))
	printf("FORMS: %s\n",ids2->buff);
    }
    
    infostd_dyn_str_free(ids);  
    infostd_dyn_str_free(ids2);  
    infostd_stemmer_free(ist);

  } else if(mode == 1) {
    ist = infostd_stemmer_init(STEMMER_SIZE);
    
    if(!ist) {
      printf("Can't create stemmer in memory!\n");
      exit(0);
    }

    fin = fopen(in,"r");
    if(!fin) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }
    
    ist = infostd_stemmer_build(fin,STEMMER_SIZE);
    if(!ist) {
      printf("Couldn't build stemmer!\n");
      exit(0);
    }
          
    fclose(fin);

    printf("Generated %lu roots.\n",ist->roots->num_elem);
    
    fout = fopen(out,"w");
    if(!fout) {
      printf("Can't open input file %s\n",in);
      exit(0);
    }

    infostd_stemmer_write(ist,fout);
    fclose(fout);

    infostd_stemmer_free(ist);
  }
	 
  return 0;
}

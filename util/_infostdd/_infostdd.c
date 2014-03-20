/****************************************************************************
  Simple tcp/ip server interface to infostd functionality:
     stemming and spelling
*****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <infostd.h>

#define PORT 4201

void usage(char* name) {

  printf("Usage:\n%s speller.config words.stemmer\n",name);
  exit(0);
}

unsigned long handle_request(INFOSTD_SPELLER isp,
			     INFOSTD_STEMMER ist,
			     int s) {
  
  INFOSTD_DYN_STR ids;
  INFOSTD_DYN_STRS idss;

  char buf[1024];
  char *ln, *ln2;
  int n,i,l;

  ids = infostd_dyn_str_init(100);
  idss = infostd_dyn_strs_init(100);
  
  memset(buf,0,1024);
  while((n = read(s,buf,1024)) > 0) {

    l = strlen(buf);
    for(i=0;i<l;i++) {
      if(buf[i] == '\n' || buf[i] == '\r')
	buf[i] = 0;
    }

    if(!strncasecmp(buf,"spell: ",7)) {
      ln = buf + 7;

      if(infostd_speller_check(isp,ln))
	send(s,"CORRECT\n",8,0);
      else {
	send(s,"INCORRECT",9,0);
	if((n=infostd_speller_suggestions(isp,ln,idss))>0) {
	  for(i=0;i<n;i++) {
	    ln2 = infostd_dyn_strs_get(idss,i);
	    send(s," ",1,0);
	    send(s,ln2,strlen(ln2),0);
	  }
	}
	send(s,"\n",1,0);
      }
    } else if(!strncasecmp(buf,"stem: ",6)) {
      ln = buf + 6;

      if(infostd_stemmer_root(ist,ln,ids)) {
	send(s,"ROOT: ",6,0);
	send(s,ids->buff,ids->length,0);
	send(s,"\n",1,0);
      }
      if(infostd_stemmer_forms(ist,ids->buff,ids)) {
	send(s,"FORMS: ",7,0);
	send(s,ids->buff,ids->length,0);
	send(s,"\n",1,0);
      }
    }

    memset(buf,0,1024);
  }  
  infostd_dyn_str_free(ids);
  infostd_dyn_strs_free(idss);
  close(s);
  return 0;
}

int main(int argc, char** argv) {

  INFOSTD_SPELLER isp;
  INFOSTD_STEMMER ist;
  struct sockaddr_in bind_addr, acc_addr;
  unsigned long s, d, r;
  FILE *fin;
  int len;

  if(argc < 3)
    usage(argv[0]);

  /* Load the speller */
  printf("Loading Speller... ");
  isp = infostd_speller_load_config(argv[1]);
  if(!isp) {
    printf("Error: can't load speller!\n");
    exit(0);
  }
  printf("done.\n");


  /* Load the stemmer */
  printf("Loading Stemmer... ");
  fin = fopen(argv[2],"r");
  if(!fin) {
    printf("Error: can't open stemmer!\n");
  }
  ist = infostd_stemmer_read(fin);
  if(!ist) {
    printf("Error: can't load stemmer!\n");
    exit(0);
  }
  printf("done.\n");

  
  /* Setup the socket */
  bind_addr.sin_family = AF_INET;
  bind_addr.sin_addr.s_addr = INADDR_ANY; 
  memset(bind_addr.sin_zero,0,8);
  bind_addr.sin_port = htons(PORT);
  len = sizeof(acc_addr);

  if((s = socket(AF_INET, SOCK_STREAM,0)) < 0) {
    printf("Error: can't creeate socket!\n");
    exit(0);
  }

  if((r = bind(s, (struct sockaddr*)&bind_addr, sizeof(bind_addr))) < 0) {
    printf("Error: can't bind to socket!\n");
    exit(0);
  }

  if(listen(s, 10)) {
    printf("Error: can't listen!\n");
    exit(0);
  }

  while((d = accept(s, (struct sockaddr*)&acc_addr, &len)) != -1) {
    handle_request(isp,ist,d);
  }
  
  return 0;
}

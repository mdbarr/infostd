#include <infostd.h>

static char infostd_base64_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned long infostd_base64_encode(char* in, 
				    unsigned long len, 
				    INFOSTD_DYN_STR out) {

  unsigned char c1, c2, c3, e1, e2, e3, e4;
  unsigned long i;

  infostd_dyn_str_clear(out);

  for(i=0;i<len;i+=3) {
    c1 = in[i];
    c2 = (i+1<len) ? in[i+1] : '=';
    c3 = (i+2<len) ? in[i+2] : '=';

    e1 = infostd_base64_table[c1 >> 2];
    e2 = infostd_base64_table[((c1 & 3) << 4) | (c2 >> 4)];
    e3 = (i+1<len) ? infostd_base64_table[((c2 & 0xF) << 2) | (c3 >> 6)] : '=';
    e4 = (i+2<len) ? infostd_base64_table[c3 & 0x3F] : '=';

    infostd_dyn_str_addchar(out,e1);
    infostd_dyn_str_addchar(out,e2);
    infostd_dyn_str_addchar(out,e3);
    infostd_dyn_str_addchar(out,e4);    
  }
  return(1);
}

unsigned long infostd_base64_decode(char* in, 
				    unsigned long len,
				    INFOSTD_DYN_STR out) {
  unsigned char c1,c2,c3,c4;
  unsigned char d1,d2,d3,d4;
  unsigned long i;
  unsigned char dtable[256];
  
  /* Build appropriate decode table */
  for (i = 0; i < 255; i++) {
    dtable[i] = 0x80;
  }
  for (i = 'A'; i <= 'Z'; i++) {
    dtable[i] = 0 + (i - 'A');
  }
  for (i = 'a'; i <= 'z'; i++) {
    dtable[i] = 26 + (i - 'a');
  }
  for (i = '0'; i <= '9'; i++) {
    dtable[i] = 52 + (i - '0');
  }
  dtable['+'] = 62;
  dtable['/'] = 63;
  dtable['='] = 0;
  
  infostd_dyn_str_clear(out);

  for(i=0;i<len;i+=4) {
    c1 = in[i];
    d1 = dtable[c1];

    c2 = in[i+1];
    d2 = dtable[c2];

    c3 = in[i+2];
    d3 = dtable[c3];

    c4 = in[i+3];
    d4 = dtable[c4];


    infostd_dyn_str_addchar(out,(d1 << 2) | (d2 >> 4));
    infostd_dyn_str_addchar(out,(d2 << 4) | (d3 >> 2));
    infostd_dyn_str_addchar(out,(d3 << 6) | d4);
  }
  
  return(0);
}

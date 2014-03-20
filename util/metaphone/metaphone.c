#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <infostd.h>

int main(void) {
  INFOSTD_DYN_STR ids, ids2;

  ids = infostd_dyn_str_init(100);
  ids2 = infostd_dyn_str_init(100);

  while(infostd_dyn_str_fgets(ids,stdin)) {
    infostd_dyn_str_lower(ids);
    infostd_speller_metaphone(ids->buff,ids->length,ids2);
    //printf("%s -> %s\n",ids->buff,ids2->buff);
    printf("%s\n",ids2->buff);
  }

  infostd_dyn_str_free(ids);
  infostd_dyn_str_free(ids2);

  return 0;
}

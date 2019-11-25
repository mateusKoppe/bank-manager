#include <stdio.h>
#include "account.h"

#define STORAGE_FILE "storage/account.txt"

int account_save (account ac) {
  FILE* file = fopen(STORAGE_FILE, "a");
  int size = fprintf(file, "%d$%s$%f$\n", ac.id, ac.client_name, ac.balance);
  fclose(file);
  if (size > 0) {
    return 0;
  }
  return 1;
}
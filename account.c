#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"

#define STORAGE_FILE "storage/account.txt"
#define TEMP_FILE "storage/account.temp.txt"
#define LINE_FORMAT "%d | %s | %f\n"

int account_list_init (account_list *l) {
  l->value = (account**) malloc(0);
  l->length = 0;
}

int account_list_push (account_list *l, account *value) {
  l->value = (account**) realloc(l->value, ((l->length + 1) * sizeof(account*)));
  l->value[l->length] = value;
  l->length++;
}

int account_save (account ac) {
  FILE* file = fopen(STORAGE_FILE, "a");
  int size = fprintf(file, LINE_FORMAT, ac.id, ac.client_name, ac.balance);
  fclose(file);
  if (size > 0) {
    return 0;
  }
  return 1;
}

int account_update (account ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  account* i_ac = malloc(sizeof(account));
  while (fscanf(file, LINE_FORMAT, &i_ac->id, &i_ac->client_name, &i_ac->balance) != EOF) {
    int is_finded = i_ac->id == ac.id;
    if (is_finded) {
      fprintf(temp, LINE_FORMAT, ac.id, ac.client_name, ac.balance);
    } else {
      fprintf(temp, LINE_FORMAT, i_ac->id, i_ac->client_name, i_ac->balance);
    }
  }
  free(i_ac);
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

account* account_search_for_id (int id) {
  account* ac = malloc(sizeof(account));
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, LINE_FORMAT, &ac->id, &ac->client_name, &ac->balance) != EOF) {
    is_finded = ac->id == id;
    if (is_finded) {
      break;
    }
  }

  fclose(file);
  if (!is_finded) {
    free(ac);
    return NULL;
  }

  return ac;
}

account* account_search_for_name (char* name) {
  account* ac = malloc(sizeof(account));
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, LINE_FORMAT, &ac->id, &ac->client_name, &ac->balance) != EOF) {
    is_finded = strcmp(ac->client_name, name) == 0;
    if (is_finded) {
      break;
    }
  }

  fclose(file);
  if (!is_finded) {
    free(ac);
    return NULL;
  }

  return ac;
}

account_list* account_get_all () {
  account_list *cl = malloc(sizeof(account_list*));
  account_list_init(cl);
  FILE* file = fopen(STORAGE_FILE, "r");
  account i_ac;

  while (fscanf(file, LINE_FORMAT, &i_ac.id, &i_ac.client_name, &i_ac.balance) != EOF) {
    account *ac = (account*) malloc(sizeof(account));
    ac->id = i_ac.id;
    strcpy(ac->client_name, i_ac.client_name);
    ac->balance = i_ac.balance;
    account_list_push(cl, ac);
  }

  fclose(file);
  return cl;
}
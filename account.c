#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"

#define STORAGE_FILE "storage/account.txt"

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
  int size = fprintf(file, "%d$%s$%f$\n", ac.id, ac.client_name, ac.balance);
  fclose(file);
  if (size > 0) {
    return 0;
  }
  return 1;
}

account* account_search_for_id (int id) {
  account* ac = malloc(sizeof(account));
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, "%d$%s$%f$\n", &ac->id, &ac->client_name, &ac->balance) != EOF) {
    is_finded = ac->id == id;
    if (is_finded) {
      strcpy(ac->client_name, strtok(ac->client_name, "$"));
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
  while (fscanf(file, "%d$%s$%f$\n", &ac->id, &ac->client_name, &ac->balance) != EOF) {
    strcpy(ac->client_name, strtok(ac->client_name, "$"));
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
  char* value = malloc(0);
  int actual_word = 0;
  int i = -1;
  int word_size = 0;
  while (!feof(file)) {
    char character = (char) fgetc(file);
    if (character != '$') {
      if (character == '\n') {
        continue;
      }
      char to_concat[] = {character};
      word_size++;
      value = realloc(value, sizeof (char*) * (word_size));
      if (word_size > 1) {
        strcat(value, to_concat);
      } else {
        strcpy(value, to_concat);
      }
    } else {
      switch (actual_word%3) {
        case 0:
          i++;
          account *ac = (account*) malloc(sizeof(account));
          ac->id = atoi(value);
          account_list_push(cl, ac);
          break;
        case 1:
          strcpy(cl->value[i]->client_name, value);
          break;
        case 2:
          cl->value[i]->balance = strtof(value, 0);
          break;
      }
      actual_word++;
      word_size = 0;
      free(value);
      value = malloc(0);
    }
  }
  fclose(file);
  return cl;
}
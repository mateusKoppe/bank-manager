#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"
#include "client.h"

#define STORAGE_FILE "storage/account.txt"
#define TEMP_FILE "storage/account.temp.txt"
#define ACCOUNT_LINE_FORMAT "%d | %d | %f\n"

account* account_new () {
  account* ac = malloc(sizeof(account));
  ac->client = client_new();
  ac->balance = 0;
  return ac;
}

int account_list_init (account_list *l) {
  l->value = (account**) malloc(0);
  l->length = 0;
}

int account_list_push (account_list *l, account *value) {
  l->value = (account**) realloc(l->value, ((l->length + 1) * sizeof(account*)));
  l->value[l->length] = value;
  l->length++;
}

int account_delete (account ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  account i_ac;
  while (fscanf(file, ACCOUNT_LINE_FORMAT, &i_ac.id, &i_ac.client_name, &i_ac.balance) != EOF) {
    if (i_ac.id == ac.id) {
      continue;
    }
    fprintf(temp, ACCOUNT_LINE_FORMAT, i_ac.id, i_ac.client_name, i_ac.balance);
  }
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

int account_save (account* ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  account* i_ac = account_new();
  int is_saved = 0;
  /* TODO: Handle client errors */
  client_save(ac->client);
  while (fscanf(file, ACCOUNT_LINE_FORMAT, &i_ac->id, &i_ac->client->id, &i_ac->balance) != EOF) {
    int is_bigger = i_ac->id > ac->id;
    if (is_bigger && !is_saved) {
      fprintf(temp, ACCOUNT_LINE_FORMAT, ac->id, ac->client->id, ac->balance);
      is_saved = 1;
    }
    fprintf(temp, ACCOUNT_LINE_FORMAT, i_ac->id, i_ac->client->id, i_ac->balance);
  }
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

int account_update (account ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  account* i_ac = malloc(sizeof(account));
  while (fscanf(file, ACCOUNT_LINE_FORMAT, &i_ac->id, &i_ac->client_name, &i_ac->balance) != EOF) {
    int is_finded = i_ac->id == ac.id;
    if (is_finded) {
      fprintf(temp, ACCOUNT_LINE_FORMAT, ac.id, ac.client_name, ac.balance);
    } else {
      fprintf(temp, ACCOUNT_LINE_FORMAT, i_ac->id, i_ac->client_name, i_ac->balance);
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
  account* ac = account_new();
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, ACCOUNT_LINE_FORMAT, &ac->id, &ac->client->id, &ac->balance) != EOF) {
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

  ac->client = client_search_for_id(ac->client->id);
  return ac;
}

account* account_search_for_name (char* name) {
  account* ac = malloc(sizeof(account));
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, ACCOUNT_LINE_FORMAT, &ac->id, &ac->client_name, &ac->balance) != EOF) {
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
  account* cl_temp = account_new();

  while (fscanf(file, ACCOUNT_LINE_FORMAT, &cl_temp->id, &cl_temp->client->id, &cl_temp->balance) != EOF) {
    account *ac = account_new();
    ac->id = cl_temp->id;
    ac->client = client_search_for_id(cl_temp->client->id);
    ac->balance = cl_temp->balance;
    account_list_push(cl, ac);
  }

  free(cl_temp);

  fclose(file);
  return cl;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"
#include "client.h"

#define STORAGE_FILE "storage/account.txt"
#define TEMP_FILE "storage/account.temp.txt"
#define LINE_FORMAT "%d | %d | %f\n"

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

int account_delete (account* ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  client* cl = client_search_for_id(ac->client->id);
  client_delete(cl);
  account* ac_temp = account_new();
  while (fscanf(file, LINE_FORMAT, &ac_temp->id, &ac_temp->client->id, &ac_temp->balance) != EOF) {
    if (ac_temp->id == ac->id) {
      continue;
    }
    fprintf(temp, LINE_FORMAT, ac_temp->id, ac_temp->client->id, ac_temp->balance);
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
  account* ac_temp = account_new();
  int is_saved = 0;
  /* TODO: Handle client errors */
  client_save(ac->client);
  while (fscanf(file, LINE_FORMAT, &ac_temp->id, &ac_temp->client->id, &ac_temp->balance) != EOF) {
    int is_bigger = ac_temp->id > ac->id;
    if (is_bigger && !is_saved) {
      fprintf(temp, LINE_FORMAT, ac->id, ac->client->id, ac->balance);
      is_saved = 1;
    }
    fprintf(temp, LINE_FORMAT, ac_temp->id, ac_temp->client->id, ac_temp->balance);
  }
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

int account_update (account* ac) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  client_update(ac->client);
  account* ac_temp = account_new();
  while (fscanf(file, LINE_FORMAT, &ac_temp->id, &ac_temp->client_name, &ac_temp->balance) != EOF) {
    int is_finded = ac_temp->id == ac->id;
    if (is_finded) {
      fprintf(temp, LINE_FORMAT, ac->id, ac->client_name, ac->balance);
    } else {
      fprintf(temp, LINE_FORMAT, ac_temp->id, ac_temp->client_name, ac_temp->balance);
    }
  }
  free(ac_temp);
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
  while (fscanf(file, LINE_FORMAT, &ac->id, &ac->client->id, &ac->balance) != EOF) {
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

account* account_search_for_client_id (int id) {
  account* ac = account_new();
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, LINE_FORMAT, &ac->id, &ac->client->id, &ac->balance) != EOF) {
    is_finded = ac->client->id == id;
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
  client* cl = client_search_for_name(name);
  if (!cl) {
    return NULL;
  }
  account* ac = account_search_for_client_id(cl->id);
  return ac;
}

account_list* account_get_all () {
  account_list *cl = malloc(sizeof(account_list*));
  account_list_init(cl);
  FILE* file = fopen(STORAGE_FILE, "r");
  account* cl_temp = account_new();

  while (fscanf(file, LINE_FORMAT, &cl_temp->id, &cl_temp->client->id, &cl_temp->balance) != EOF) {
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
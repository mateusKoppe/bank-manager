#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define STORAGE_FILE "storage/client.txt"
#define TEMP_FILE "storage/client.temp.txt"
#define LINE_FORMAT "%d | %s\n"
#define INDEX_NAME_FILE "storage/client.index-name.txt"
#define INDEX_NAME_TEMP_FILE "storage/client.index-name.temp.txt"
#define INDEX_NAME_LINE_FORMAT "%s | %d\n"

int client_get_last_id();
int client_save_index(client* cl);

client* client_new () {
  client *cl = (client*) malloc(sizeof(client));
  strcpy(cl->name, "");
  return cl;
}

int client_delete (client* cl) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  client* cl_temp = client_new();
  while (fscanf(file, LINE_FORMAT, &cl_temp->id, &cl_temp->name) != EOF) {
    if (cl_temp->id == cl->id) {
      continue;
    }
    fprintf(temp, LINE_FORMAT, cl_temp->id, cl_temp->name);
  }
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

int client_save (client* cl) {
  FILE* file = fopen(STORAGE_FILE, "a");
  client cl_temp;
  int last_id = client_get_last_id();
  cl->id = last_id + 1;
  int size = fprintf(file, LINE_FORMAT, cl->id, cl->name);
  fclose(file);
  if (size == 0) {
    return 1;
  }
  client_save_index(cl);

  return 0;
}

int client_save_index (client* cl) {
  FILE* file = fopen(INDEX_NAME_FILE, "r+");
  FILE* temp = fopen(INDEX_NAME_TEMP_FILE, "w");
  client* cl_temp = client_new();
  int is_saved = 0;
  while (fscanf(file, INDEX_NAME_LINE_FORMAT, &cl_temp->name, &cl_temp->id) != EOF) {
    int is_bigger = strcmp(cl_temp->name, cl->name) > 0;
    if (is_bigger && !is_saved) {
      fprintf(temp, INDEX_NAME_LINE_FORMAT, cl->name, cl->id);
      is_saved = 1;
    }
    fprintf(temp, INDEX_NAME_LINE_FORMAT, cl_temp->name, cl_temp->id);
  }
  if (!is_saved) {
    fprintf(temp, INDEX_NAME_LINE_FORMAT, cl->name, cl->id);
  }
  fclose(file);
  fclose(temp);
  remove(INDEX_NAME_FILE);
  rename(INDEX_NAME_TEMP_FILE, INDEX_NAME_FILE);
  return 0;
}

int client_update (client* cl) {
  FILE* file = fopen(STORAGE_FILE, "r+");
  FILE* temp = fopen(TEMP_FILE, "w");
  client* cl_temp = client_new();
  while (fscanf(file, LINE_FORMAT, &cl_temp->id, &cl_temp->name) != EOF) {
    int is_finded = cl_temp->id == cl->id;
    if (is_finded) {
      fprintf(temp, LINE_FORMAT, cl->id, cl->name);
    } else {
      fprintf(temp, LINE_FORMAT, cl_temp->id, cl_temp->name);
    }
  }
  free(cl_temp);
  fclose(file);
  fclose(temp);
  remove(STORAGE_FILE);
  rename(TEMP_FILE, STORAGE_FILE);
  return 0;
}

int client_get_last_id () {
  FILE* file = fopen(STORAGE_FILE, "r+");
  int id;
  char name[80];
  while (fscanf(file, LINE_FORMAT, &id, &name) != EOF) {}
  if (!id) {
    return 0;
  }
  return id;
}

client* client_search_for_id(int id) {
  client* cl = client_new();
  FILE* file = fopen(STORAGE_FILE, "r");
  int is_finded = 0;
  while (fscanf(file, LINE_FORMAT, &cl->id, &cl->name) != EOF) {
    is_finded = cl->id == id;
    if (is_finded) {
      break;
    }
  }

  fclose(file);
  if (!is_finded) {
    free(cl);
    return NULL;
  }
  return cl;
}

client* client_search_for_name(char* name) {
  FILE* file = fopen(INDEX_NAME_FILE, "r");
  char fetch_name[80];
  int fetch_id;
  int is_finded = 0;
  while (fscanf(file, INDEX_NAME_LINE_FORMAT, &fetch_name, &fetch_id) != EOF) {
    is_finded = strcmp(fetch_name, name) == 0;
    if (is_finded) {
      break;
    }
  }

  fclose(file);
  if (!is_finded) {
    return NULL;
  }

  client* cl = client_search_for_id(fetch_id);
  return cl;
}

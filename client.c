#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define STORAGE_FILE "storage/client.txt"
#define TEMP_FILE "storage/client.temp.txt"
#define LINE_FORMAT "%d | %s\n"

int client_get_last_id();

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
  FILE* file = fopen(STORAGE_FILE, "r");
  client* cl = client_new();
  int is_finded = 0;
  while (fscanf(file, LINE_FORMAT, &cl->id, &cl->name) != EOF) {
    is_finded = strcmp(cl->name, name) == 0;
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

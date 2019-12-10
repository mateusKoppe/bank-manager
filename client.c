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
  return 0;
}

client* client_search_for_name(char* name) {
  return 0;
}

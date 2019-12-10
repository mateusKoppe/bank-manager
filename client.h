#ifndef __CLIENT_LIB
#define __CLIENT_LIB

typedef struct {
  int id;
  char name[80];
} client;

int client_delete (client* cl);
int client_save (client* cl);
int client_update (client* cl);
client* client_new ();
client* client_search_for_id(int id);
client* client_search_for_name(char* name);

#endif
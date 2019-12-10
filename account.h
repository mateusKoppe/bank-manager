#ifndef __ACCOUNT_LIB
#define __ACCOUNT_LIB

#include "./client.h"

typedef struct {
  int id;
  client* client;
  float balance;
} account;

typedef struct {
  int length;
  account** value;
} account_list;

int account_list_init (account_list* l);
int account_list_push (account_list* l, account* value);
int account_delete (account* ac);
int account_save (account* ac);
int account_update (account* ac);
account* account_new ();
account* account_search_for_id(int id);
account* account_search_for_client_id (int id);
account* account_search_for_name(char* name);
account_list* account_get_all();

#endif
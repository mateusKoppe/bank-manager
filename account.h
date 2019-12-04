typedef struct {
  int id;
  char client_name[80];
  float balance;
} account;

typedef struct {
  int length;
  account** value;
} account_list;

int account_list_init (account_list *l);
int account_list_push (account_list *l, account *value);
int account_delete (account ac);
int account_save (account ac);
int account_update (account ac);
account* account_search_for_id(int id);
account* account_search_for_name(char* name);
account_list* account_get_all();
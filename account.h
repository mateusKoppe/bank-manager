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
int account_save (account ac);
account_list* account_get_all();
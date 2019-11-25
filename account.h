
typedef struct {
  int id;
  char client_name[80];
  float balance;
} account;

int account_save (account ac);
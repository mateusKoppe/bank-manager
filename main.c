#include <stdio.h>
#include "account.h"

void menu();
void add_new_account();

int main () {
  menu();
  return 0;
}

void menu () {
  int option;
  printf("- Bank Manager\n");
  printf("[1] - Add new account\n");
  scanf("%d", &option);
  switch (option)
  {
  case 1:
    add_new_account();
    break;
  
  default:
    printf("Invalid input, try again\n");
    menu();
    break;
  }
}

void add_new_account() {
  account newAccount;
  newAccount.balance = 0;
  printf("- Bank manager\n");
  printf("-- New account\n");
  printf("name: ");
  scanf("%s", &newAccount.client_name);
  printf("account: ");
  scanf("%d", &newAccount.id);
  int created_code = account_save(newAccount);
  if (created_code == 0) {
    printf("Account created successfully\n");
    menu();
  } else {
    printf("Error to create account\n");
  }
}
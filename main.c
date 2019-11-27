#include <stdio.h>
#include <stdlib.h>

#include "account.h"

void menu();
void add_new_account();
void list_accounts();

int main () {
  menu();
  return 0;
}

void menu () {
  int option;
  printf("- Bank Manager\n");
  printf("[1] - Add new account\n");
  printf("[2] - List accounts\n");
  scanf("%d", &option);
  switch (option) {
  case 1:
    add_new_account();
    break;

  case 2:
    list_accounts();
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

void list_accounts() {
  printf("- Bank manager\n");
  printf("-- Accounts list\n");
  account_list* list = account_get_all();
  for (int i = 0; i < list->length; i++) {
    account* ac = list->value[i];
    printf("%d - %s\n", ac->id, ac->client_name);
  }
  menu();
} 
#include <stdio.h>
#include <stdlib.h>

#include "account.h"

void menu();
void add_new_account();
void list_accounts();
void search_account();
void menu_account(account *ac);
void change_account_balance(account *ac);

int main () {
  menu();
  return 0;
}

void menu () {
  int option;
  printf("- Bank Manager\n");
  printf("[1] - Add new account\n");
  printf("[2] - List accounts\n");
  printf("[3] - Access account\n");
  scanf("%d", &option);
  switch (option) {
  case 1:
    add_new_account();
    break;

  case 2:
    list_accounts();
    break;

  case 3:
    search_account();
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

void search_account() {
  printf("- Bank manager\n");
  printf("-- Search for:\n");
  printf("[1] - Account id\n");
  printf("[2] - Client Name\n");
  int option;
  scanf("%d", &option);
  char search_for[80];
  scanf("%s", &search_for);
  account* ac;
  switch (option) {
    case 1:
      ac = account_search_for_id(atoi(search_for));
      break;
    case 2:
      ac = account_search_for_name(search_for);
      break;
    default:
      search_account();
      return;
  }

  if (!ac) {
    printf("Account not found\n");
    menu();
    return;
  }

  menu_account(ac);
  menu();
}

void menu_account(account *ac) {
  if (!ac) {
    printf("Invalid account\n");
    return;
  }
  printf("id     : %d\n", ac->id);
  printf("Client : %s\n", ac->client_name);
  printf("Balance: %f\n", ac->balance);
  printf("\n");
  printf("[1] - Update balance\n");
  printf("[2] - Exit\n");
  int option;
  scanf("%d", &option);
  switch (option) {
    case 1:
      change_account_balance(ac);
      break;
    case 2:
      menu();
      return;
  }
}

void change_account_balance (account *ac) {
  printf("Enter the new balance: ");
  scanf("%f", &ac->balance);
  account_update(*ac);
  menu_account(ac);
}
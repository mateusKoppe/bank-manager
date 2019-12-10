#include <stdio.h>
#include <stdlib.h>

#include "account.h"

void menu();
void add_new_account();
void list_accounts();
void search_account();
void menu_account(account *ac);
void change_account_balance(account *ac);
void delete_account(account *ac);
void screen_clean();
void enter_to_continue();

int main () {
  menu();
  return 0;
}

void menu () {
  int option;
  screen_clean();
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
    enter_to_continue();
    menu();
    break;
  }
}

void add_new_account() {
  account* newAccount = account_new();
  newAccount->balance = 0;
  screen_clean();
  printf("- Bank manager\n");
  printf("-- New account\n");
  printf("name: ");
  scanf("%s", &newAccount->client->name);
  printf("account: ");
  scanf("%d", &newAccount->id);
  int created_code = account_save(newAccount);
  if (created_code == 0) {
    printf("Account created successfully\n");
    enter_to_continue();
    menu();
  } else {
    printf("Error to create account\n");
    enter_to_continue();
    menu();
  }
}

void list_accounts() {
  screen_clean();
  printf("- Bank manager\n");
  printf("-- Accounts list\n");
  account_list* list = account_get_all();
  for (int i = 0; i < list->length; i++) {
    account* ac = list->value[i];
    printf("%d - %s\n", ac->id, ac->client_name);
  }
  enter_to_continue();
  menu();
}

void search_account() {
  screen_clean();
  printf("- Bank manager\n");
  printf("-- Search for:\n");
  printf("[1] - Account id\n");
  printf("[2] - Client Name\n");
  int option;
  scanf("%d", &option);
  char search_for[80];
  account* ac;
  switch (option) {
    case 1:
      printf("Enter account id\n");
      scanf("%s", &search_for);
      ac = account_search_for_id(atoi(search_for));
      break;
    case 2:
      printf("Enter client name\n");
      scanf("%s", &search_for);
      ac = account_search_for_name(search_for);
      break;
    default:
      search_account();
      return;
  }

  if (!ac) {
    printf("Account not found\n");
    enter_to_continue();
    menu();
    return;
  }

  menu_account(ac);
  menu();
}

void menu_account(account *ac) {
  if (!ac) {
    printf("Invalid account\n");
    menu();
    return;
  }
  screen_clean();
  printf("- Bank manager\n");
  printf("-- Profile:\n");
  printf("id     : %d\n", ac->id);
  printf("Client : %s\n", ac->client_name);
  printf("Balance: %f\n", ac->balance);
  printf("\n");
  printf("[1] - Update balance\n");
  printf("[2] - Delete account\n");
  printf("[3] - Back to menu\n");
  int option;
  scanf("%d", &option);
  switch (option) {
    case 1:
      change_account_balance(ac);
      break;
    case 2:
      delete_account(ac);
      break;
    case 3:
      menu();
      return;
    default:
      printf("Invalid option\n");
      enter_to_continue();
      menu_account(ac);
      break;
  }
}

void change_account_balance (account *ac) {
  printf("Enter the new balance: ");
  scanf("%f", &ac->balance);
  account_update(*ac);
  printf("Account updated\n");
  enter_to_continue();
  menu_account(ac);
}

void delete_account (account *ac) {
  printf("Are you sure? [yN]\n");
  char awnser;
  getchar();
  scanf("%c", &awnser);
  if (awnser == 'y') {
    account_delete(*ac);
    printf("Account deleted\n");
    enter_to_continue();
  }
  menu();
}

void screen_clean () {
  #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
  #endif

  #if defined(_WIN32) || defined(_WIN64)
    system("cls");
  #endif
}

void enter_to_continue () {
  printf("Press enter to continue...");
  getchar();
  getchar();
}
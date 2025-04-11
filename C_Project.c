#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For sleep function

#define MAX_TRANSACTIONS 100
#define MAX_USERNAME 50
#define MAX_PASSWORD 50

// ANSI Color Codes
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BOLD "\033[1m"
#define BLINK "\033[5m"

typedef struct {
    char type[10];  // "Income" or "Expense"
    char category[20];
    float amount;
} Transaction;

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    float balance;
    float budget;
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
} User;

User currentUser;

void printHeader(const char *title) {
    printf(CYAN "\n===================================\n" RESET);
    printf(BOLD CYAN " %s " RESET, title);
    printf(CYAN "\n===================================\n\n" RESET);
}

void loadingEffect(const char *message) {
    printf(YELLOW "%s" RESET, message);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void saveData() {
    FILE *file = fopen("finance_data.dat", "w");
    if (!file) {
        printf(RED "Error saving data!\n" RESET);
        return;
    }
    fwrite(&currentUser, sizeof(User), 1, file);
    fclose(file);
    loadingEffect("Saving data");
}

void loadData() {
    FILE *file = fopen("finance_data.dat", "r");
    if (file) {
        fread(&currentUser, sizeof(User), 1, file);
        fclose(file);
    }
}

void registerUser() {
    printHeader("User Registration");
    printf(YELLOW "Enter username: " RESET);
    scanf("%s", currentUser.username);
    printf(YELLOW "Enter password: " RESET);
    scanf("%s", currentUser.password);
    currentUser.balance = 0;
    currentUser.budget = 0;
    currentUser.transactionCount = 0;
    saveData();
    printf(GREEN "\nRegistration successful!\n" RESET);
}

int loginUser() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    printHeader("User Login");
    printf(YELLOW "Enter username: " RESET);
    scanf("%s", username);
    printf(YELLOW "Enter password: " RESET);
    scanf("%s", password);
    loadData();
    if (strcmp(username, currentUser.username) == 0 && strcmp(password, currentUser.password) == 0) {
        loadingEffect("Logging in");
        printf(GREEN "\nLogin successful! Welcome, %s!\n" RESET, currentUser.username);
        return 1;
    }
    printf(RED "\nInvalid credentials!\n" RESET);
    return 0;
}

void addTransaction() {
    if (currentUser.transactionCount >= MAX_TRANSACTIONS) {
        printf(RED "Transaction limit reached!\n" RESET);
        return;
    }
    printHeader("Add Transaction");
    Transaction t;
    printf(YELLOW "Enter transaction type (Income/Expense): " RESET);
    scanf("%s", t.type);
    printf(YELLOW "Enter category: " RESET);
    scanf("%s", t.category);
    printf(YELLOW "Enter amount: " RESET);
    scanf("%f", &t.amount);
    
    if (strcmp(t.type, "Income") == 0) {
        currentUser.balance += t.amount;
    } else {
        currentUser.balance -= t.amount;
    }
    
    currentUser.transactions[currentUser.transactionCount++] = t;
    saveData();
    printf(GREEN "\nTransaction added successfully!\n" RESET);
}

void viewTransactions() {
    printHeader("Transaction History");
    for (int i = 0; i < currentUser.transactionCount; i++) {
        printf("%s | %s | $%.2f\n", currentUser.transactions[i].type, currentUser.transactions[i].category, currentUser.transactions[i].amount);
    }
    printf(CYAN "-----------------------------------\n" RESET);
}

void setBudget() {
    printHeader("Set Budget");
    printf(YELLOW "Enter your monthly budget: " RESET);
    scanf("%f", &currentUser.budget);
    saveData();
    printf(GREEN "\nBudget updated successfully!\n" RESET);
}

void checkBudget() {
    float totalExpenses = 0;
    for (int i = 0; i < currentUser.transactionCount; i++) {
        if (strcmp(currentUser.transactions[i].type, "Expense") == 0) {
            totalExpenses += currentUser.transactions[i].amount;
        }
    }
    printHeader("Budget Check");
    printf("Total expenses: $%.2f | Budget: $%.2f\n", totalExpenses, currentUser.budget);
    int progress = (totalExpenses / currentUser.budget) * 20;
    printf("Budget Usage: [" MAGENTA);
    for (int i = 0; i < progress; i++) printf("$");
    for (int i = progress; i < 20; i++) printf(" ");
    printf(RESET "]\n");
    if (totalExpenses > currentUser.budget) {
        printf(RED BLINK "Warning: You have exceeded your budget!\n" RESET);
    }
}

void mainMenu() {
    int choice;
    while (1) {
        printHeader("Main Menu");
        printf(CYAN "1. Add Transaction\n" RESET);
        printf(CYAN "2. View Transactions\n" RESET);
        printf(CYAN "3. Set Budget\n" RESET);
        printf(CYAN "4. Check Budget\n" RESET);
        printf(CYAN "5. Exit\n" RESET);
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewTransactions(); break;
            case 3: setBudget(); break;
            case 4: checkBudget(); break;
            case 5: printf(GREEN "Exiting...\n" RESET); return;
            default: printf(RED "Invalid choice!\n" RESET);
        }
    }
}

int main() {
    int choice;
    printHeader("Welcome to Personal Finance Tracker");
    printf("1. Register\n");
    printf("2. Login\n");
    printf(YELLOW "Enter choice: " RESET);
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: registerUser(); break;
        case 2: if (!loginUser()) return 0; break;
        default: printf(RED "Invalid choice!\n" RESET); return 0;
    }
    
    mainMenu();
    return 0;
}


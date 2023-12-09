#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CUSTOMERS 100

char username[20]; // Global variable

// Function prototypes
void customerMenu();
void adminMenu();
void login(char role);
void inputUsageDetails(char* username);
void viewBill(char* username);
void viewAllBills();
void manageCustomers();
void addCustomer();
void removeCustomer();
void sortCustomers(); 
void searchCustomer();

// File paths
const char* userCredentialsFile = "user_credentials.txt";
const char* customerDataFile = "customer_data.txt";

// Other global variables and structures as needed

// Sample structure to represent a customer
struct Customer {
    char id[20];
    char name[50];
    float electricityUsage;
    float gasUsage;
    float totalBill;
};

int main() {
    char choice;

    do {
        printf("\n====== Customer Billing System ======\n");
        printf("1. Customer\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                customerMenu();
                break;
            case '2':
                adminMenu();
                break;
            case '3':
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != '3');

    return 0;
}

void customerMenu() {
    char customerChoice;
    //char username[20];
    do {
        printf("\n====== Customer Menu ======\n"); 
        printf("1. Login\n");
        printf("2. Input Usage Details\n"); //! needs to be put in after the login function
        printf("3. View Bill\n"); //! needs to be put in after the login function
        printf("4. Exit to main menu\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf(" %c", &customerChoice);

        switch (customerChoice) {
            case '1':
                login('C');
                printf("\nWelcome, %s!\n", username); //? Testing to see what's stored in username
                break;
            case '2': 
                inputUsageDetails(username);
                break;
            case '3':
                printf("Enter your customer ID: "); //! can be removed, will show up after the login function
                scanf("%s", username); //! can be removed, will show up after the login function
                viewBill(username);
                break;
            case '4':
                printf("Exiting to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (customerChoice != '4');
}

void adminMenu() {
    char adminChoice;
    do {
        printf("\n====== Admin Menu ======\n");
        printf("1. Login\n");
        printf("2. Manage customer\n"); //! needs to be put in after the login
        printf("3. Exit to main menu\n");
        printf("=======================\n");
        printf("Enter your choice: ");
        scanf(" %c", &adminChoice);

        switch (adminChoice) {
            case '1':
                login('A');
                break;
            case '2':
                manageCustomers();
            case '3':
                printf("Exiting to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (adminChoice != '2');
}

void login(char role) {
    //char username[20];
    char password[20];
    char fileUsername[20];
    char filePassword[20];
    char roleFromFile;
    FILE *file;

    printf("Enter username: ");
    scanf("%s", username);
    printf("%s\n", username); //? Testing what username is
    printf("Enter password: ");
    scanf("%s", password);
    printf("%s\n", password); //? Testing what password is

    file = fopen(userCredentialsFile, "r");
    if (file == NULL) {
        printf("Cannot open user credentials file.\n");
        return;
    }

    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        roleFromFile = fileUsername[0];
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0 && roleFromFile == role) {
            printf("Login successful!\n");
            fclose(file);
            return;
        }
    }

    printf("Invalid username or password. Login failed.\n");
    fclose(file);
}

void inputUsageDetails(char* username) {
    float electricityUsage, gasUsage;
    char line[100];
    FILE *file, *tempFile;

    printf("Enter electricity usage: ");
    scanf("%f", &electricityUsage);
    printf("Enter gas usage: ");
    scanf("%f", &gasUsage);

    file = fopen(customerDataFile, "r");
    tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char id[20];
        sscanf(line, "%s", id);
        if (strcmp(id, username) == 0) {
            fprintf(tempFile, "%s %f %f\n", username, electricityUsage, gasUsage);
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(customerDataFile);
    rename("temp.txt", customerDataFile);
}

void viewBill(char* username) {
    FILE *file;
    char id[20];
    float electricityUsage, gasUsage;
    float electricityRate = 0.12;
    float gasRate = 0.15;
    float totalBill;

    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    while (fscanf(file, "%s %f %f", id, &electricityUsage, &gasUsage) != EOF) {
        if (strcmp(id, username) == 0) {
            totalBill = (electricityUsage * electricityRate) + (gasUsage * gasRate);
            printf("Your total bill is: $%.2f\n", totalBill);
            fclose(file);
            return;
        }
    }

    printf("No data found for the given customer ID.\n");
    fclose(file);
}

void viewAllBills() {
    FILE *file;
    char id[20];
    float electricityUsage, gasUsage;
    float electricityRate = 0.12;
    float gasRate = 0.15;
    float totalBill;

    file = fopen(customerDataFile, "r"); 
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    printf("\n====== All Customer Bills ======\n");
    while (fscanf(file, "%s %f %f", id, &electricityUsage, &gasUsage) != EOF) {
        totalBill = (electricityUsage * electricityRate) + (gasUsage * gasRate);
        if (strcmp(id, "0.000000") == 0) {
            continue; // Skip to the next iteration of the loop if the ID is 0.000000
        }
        printf("Customer ID: %s, Electricity Usage: %.2f, Gas Usage: %.2f, Total Bill: $%.2f\n", id, electricityUsage, gasUsage, totalBill);
    }
    fclose(file);
}

void manageCustomers() {
    char adminChoice;
    do {
        printf("\n====== Manage Customers ======\n");
        printf("1. Add Customer\n");
        printf("2. Remove Customer\n");
        printf("3. Sort Customers\n");
        printf("4. Search Customer\n");
        printf("5. View All Bills\n");
        printf("6. Exit to admin menu\n");
        printf("=============================\n");
        printf("Enter your choice: ");
        scanf(" %c", &adminChoice);

        switch (adminChoice) {
            case '1':
                addCustomer();
                break;
            case '2':
                removeCustomer();
                break;
            case '3':
                sortCustomers();
                break;
            case '4':
                searchCustomer();
                break;
            case '5':
                viewAllBills();
                break;
            case '6':
                //printf("Exiting to admin menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (adminChoice != '6');
}

void addCustomer() {
    struct Customer newCustomer;
    FILE *file;

    printf("Enter customer ID: ");
    scanf("%s", newCustomer.id);
    printf("Enter customer name: ");
    scanf("%s", newCustomer.name);
    newCustomer.electricityUsage = 0;
    newCustomer.gasUsage = 0;
    newCustomer.totalBill = 0;

    file = fopen(customerDataFile, "a");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    fprintf(file, "%s %s %f %f %f\n", newCustomer.id, newCustomer.name, newCustomer.electricityUsage, newCustomer.gasUsage, newCustomer.totalBill);
    fclose(file);

    printf("Customer added successfully.\n");
}

void removeCustomer() {
    char username[20];
    char line[100];
    FILE *file, *tempFile;

    printf("Enter customer ID to remove: ");
    scanf("%s", username);

    file = fopen(customerDataFile, "r");
    tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char id[20];
        sscanf(line, "%s", id);
        if (strcmp(id, username) != 0) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(customerDataFile);
    rename("temp.txt", customerDataFile);

    printf("Customer removed successfully.\n");
}

// Comparison function for qsort
int compareCustomers(const void *a, const void *b) {
    struct Customer *customerA = (struct Customer *)a;
    struct Customer *customerB = (struct Customer *)b;
    return strcmp(customerA->id, customerB->id);
}

void sortCustomers() {
    struct Customer customers[MAX_CUSTOMERS];
    int numCustomers = 0;
    FILE *file;

    // Step 1: Open the file for reading
    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    // Step 2: Read all customer data into an array
    while (fscanf(file, "%s %s %f %f %f", customers[numCustomers].id, customers[numCustomers].name, &customers[numCustomers].electricityUsage, &customers[numCustomers].gasUsage, &customers[numCustomers].totalBill) != EOF) {
        numCustomers++;
    }
    fclose(file);

    // Step 3: Sort the array
    qsort(customers, numCustomers, sizeof(struct Customer), compareCustomers);

    // Step 4: Open the file for writing
    file = fopen(customerDataFile, "w");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    // Step 5: Write the sorted data back into the file
    for (int i = 0; i < numCustomers; i++) {
        fprintf(file, "%s %s %f %f %f\n", customers[i].id, customers[i].name, customers[i].electricityUsage, customers[i].gasUsage, customers[i].totalBill);
    }

    // Step 6: Close the file
    fclose(file);
}

void searchCustomer() {
    char username[20];
    char line[100];
    FILE *file;

    printf("Enter customer ID to search: ");
    scanf("%s", username);

    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char id[20];
        sscanf(line, "%s", id);
        if (strcmp(id, username) == 0) {
            printf("Found customer: %s\n", line);
            fclose(file);
            return;
        }
    }

    printf("No customer found with the given ID.\n");
    fclose(file);
}
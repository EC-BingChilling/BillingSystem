#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void customerMenu();
void adminMenu();
void login(char role);
void inputUsageDetails(char* customerId);
void viewBill(char* customerId);
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
    do {
        printf("\n====== Customer Menu ======\n");
        printf("1. Login\n");
        printf("2. Exit to main menu\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf(" %c", &customerChoice);

        switch (customerChoice) {
            case '1':
                login('C');
                break;
            case '2':
                printf("Exiting to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (customerChoice != '2');
}

void adminMenu() {
    char adminChoice;
    do {
        printf("\n====== Admin Menu ======\n");
        printf("1. Login\n");
        printf("2. Exit to main menu\n");
        printf("=======================\n");
        printf("Enter your choice: ");
        scanf(" %c", &adminChoice);

        switch (adminChoice) {
            case '1':
                login('A');
                break;
            case '2':
                printf("Exiting to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (adminChoice != '2');
}

void login(char role) {
    // Implement login functionality here
    // Validate credentials from the respective file
    // For simplicity, you can use hardcoded credentials for now
    // Update the logic to read from the files

    char username[20];
    char password[20];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if ((strcmp(username, "admin") == 0 && strcmp(password, "adminpass") == 0 && role == 'A') ||
        (strcmp(username, "customer") == 0 && strcmp(password, "customerpass") == 0 && role == 'C')) {
        printf("Login successful!\n");

        if (role == 'A') {
            // Admin menu
            // Implement admin menu functionality
            // Call functions like viewAllBills, manageCustomers, etc.
        } else {
            // Customer menu
            // Implement customer menu functionality
            // Call functions like inputUsageDetails, viewBill, etc.
        }
    } else {
        printf("Invalid username or password. Login failed.\n");
    }
}

void inputUsageDetails(char* customerId) {
    // Implement functionality for customers to input usage details
    // Update the customer's data in the customer_data.txt file
}

void viewBill(char* customerId) {
    // Implement functionality for customers to view their bills
    // Read the customer's data from the customer_data.txt file and display it
}

void viewAllBills() {
    // Implement functionality for admin to view all bills
    // Read all customer data from the customer_data.txt file and display it
}

void manageCustomers() {
    char adminChoice;
    do {
        printf("\n====== Manage Customers ======\n");
        printf("1. Add Customer\n");
        printf("2. Remove Customer\n");
        printf("3. Sort Customers\n");
        printf("4. Search Customer\n");
        printf("5. Exit to admin menu\n");
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
                printf("Exiting to admin menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (adminChoice != '5');
}

void addCustomer() {
    // Implement functionality to add a new customer
    // Append the new customer's data to the customer_data.txt file
}

void removeCustomer() {
    // Implement functionality to remove a customer
    // Delete the customer's data from the customer_data.txt file
}

void sortCustomers() {
    // Implement functionality to sort customers (e.g., by ID or name)
    // Read all customer data, sort it, and update the customer_data.txt file
}

void searchCustomer() {
    // Implement functionality to search for a customer
    // Read all customer data, search for the customer, and display the result
}

//need to implement the actual file I/O operations, data validation, and error handling
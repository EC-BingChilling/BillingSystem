#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CUSTOMERS 100 // set max amount of characters

char username[20]; // Global variable to store username

// Function prototypes
void customerMenu();
int login(char role); // takes in parameter char named role(C or A) and returns an integer
void inputUsageDetails(char* username); // takes in char* pointer named username and returns nothing, function may use or modify contents of the username array
void viewBill(char* username); // takes in char* pointer and the function may use username to retrieve and display bill info for corresponding customer
void viewAllBills();
void manageCustomers();
int addCustomer();
void removeCustomer();
void sortCustomers(); 
int searchCustomer();
int userCredentials();


// File paths
const char* userCredentialsFile = "user_credentials.txt"; // pointer to constant char so the data it points to which is the string cant be modified thru the pointer
const char* customerDataFile = "customer_data.txt"; // pointer to constant char so the data it points to which is also the string cannot be modified thru the pointer
// sturct for customer 
struct Customer {
    char id[50];
    char name[50];
    char password[50];
    float electricityUsage;
    float gasUsage;
    float totalBill;
};

int main() {
    // define switch statement variables
    char choice;
    int options;

    do { // main menu
        printf("\n====== Customer Billing System ======\n");
        printf("1. Customer\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                // customer options 
                printf("1. Login\n");
                printf("2. Create User\n");
                printf("3. Exit\n");
                printf("Enter option: ");
                scanf("%d", &options);
                
                switch (options) {
                    case 1:
                        if (login('C')) {  // calling the login function to see if login is correct 
                            memmove(username, username + 1, strlen(username)); // Removes the 'C' from the username
                            printf("\nWelcome, %s!\n", username); // welcomes the user 
                            customerMenu();  // calling customer menu 
                            break;
                        }
                        else{
                            printf("Login failed"); // returns this statment when login is incorrect 
                            break;
                        }
                    case 2:
                        addCustomer();  // calling function to create new user 
                        break;
                    case 3:
                        printf("Exiting the program. Goodbye!\n"); // leaving the menu 
                        exit(0);
                    default:
                        printf("Invalid choice. Please enter again.\n");  // output when wrong option is entered 
                } break;
            case '2':
                if (login('A')) {  // calling login function with A character for admin login check 
                    memmove(username, username + 1, strlen(username)); // Removes the 'A' from the username
                    printf("\nWelcome, %s!\n", username);
                    manageCustomers(); // admin menu 
                    break;
                        }
                else {
                    printf("Login failed\n"); // not correct login
                    break;
                        }
            case '3':
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        

    } while (choice != '3'); // keeps the opions of the main menu going till 3 is entered 

    return 0;
}


void customerMenu() {
    char customerChoice;
    
    do {
        printf("\n====== Customer Menu ======\n"); 
        printf("1. Input Usage Details\n"); 
        printf("2. View Bill\n"); 
        printf("3. Exit to main menu\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf(" %c", &customerChoice);

        

        switch (customerChoice) {
            case '1': 
                inputUsageDetails(username); // calling input usage function with username as parametere so the right users info is entered
                break;
            case '2':
                viewBill(username); // users bill is viewed 
                break;
            case '3':
                printf("Exiting to main menu.\n");
                main();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        

    } while (customerChoice != '3');
}

// login function 
int login(char role) {
    // variables 
    char password[20];
    char fileUsername[20];
    char filePassword[20];
    char roleFromFile;
    FILE *file;

    // getting login information
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // opening file to read 
    file = fopen(userCredentialsFile, "r");
    // if file does not exist inform the user 
    if (file == NULL) {
        printf("Cannot open user credentials file.\n");
        return 0;
    }

    // loops through the file  and gets username and password from the file
    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        roleFromFile = fileUsername[0]; // the C or A character is saved in rolefromFile 
        // checks if username matches with any in the file and if the password matches     checks the A and C charcter 
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0 && roleFromFile == role) {
            printf("Login successful!\n");
            fclose(file);
            return 1; // if successful
        }
    }

    fclose(file);
    return 0; // if failed
}

void inputUsageDetails(char* username) {
    // variables 
    float electricityUsage, gasUsage,total;
    //predetermined rates 
    float electricityRate = 0.12; 
    float gasRate = 0.15;
    char line[100];
    FILE *tmpfile;
    FILE *file;

    // getting user input 
    printf("Enter electricity usage: ");
    scanf("%f", &electricityUsage);
    printf("Enter gas usage: ");
    scanf("%f", &gasUsage);
    // creating a temporary file for storing data
    file = fopen(customerDataFile, "r");
    tmpfile = fopen("temp.txt", "w");

    // checking if the file exists 
    if (file == NULL || tmpfile == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char id[20]; // stores the id from the file 
        sscanf(line, "%s", id); //reads the line and gets the id 
        if (strcmp(username, id) == 0) {  // compares the id extracted with username 
            total = (electricityUsage * electricityRate) + (gasUsage * gasRate);  // calculates the total bill
            fprintf(tmpfile, "%s %f %f %f\n", username, electricityUsage, gasUsage,total); // writes to the temp file 
        } else { 
            fputs(line, tmpfile); //if the id and username does match then it just puts the line in tmep as is
        }
    }
    // close both files 
    fclose(file);
    fclose(tmpfile);

    remove(customerDataFile); // delete the customer data file 
    rename("temp.txt", customerDataFile); // name the tempfile customer data file 
}

void viewBill(char* username) {
    //variables 
    FILE *file;
    char name[20];
    float electricityUsage, gasUsage;
    float total;

    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }
    // loops through the file and gets the id electric usage , gas usage and total
    while (fscanf(file, "%s %f %f %f", name, &electricityUsage, &gasUsage ,&total) != EOF) {
        if (strcmp(name, username) == 0) { // compare id and username 
            // if they match print the information 
            printf("Electricity Usage: %.2f\n", electricityUsage);
            printf("Gas Usage: %.2f\n", gasUsage);
            printf("Your total bill is: $%.2f\n", total);
            fclose(file);
            return;
        }
    }
    //if id and user name don't match return this mesage 
    printf("No data found for the given customer ID.\n");
    fclose(file);
}

void viewAllBills() {
    FILE *file;
    char name[20];
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
    // loops through the file and gets the id electric usage , gas usage and total
    while (fscanf(file, "%s %f %f %f", name, &electricityUsage, &gasUsage, &totalBill) != EOF) {
        // prints everyline 
        printf("Customer ID: %s, Electricity Usage: %.2f, Gas Usage: %.2f, Total Bill: $%.2f\n", name, electricityUsage, gasUsage, totalBill);
    }
    fclose(file);
}

void manageCustomers() { // admin menu
    char adminChoice;
    do {
        printf("\n====== Admin Menu ======\n");
        printf("1. Add Customer\n");
        printf("2. Remove Customer\n");
        printf("3. Sort Customers\n");
        printf("4. Search Customer\n");
        printf("5. View All Bills\n");
        printf("6. View All Credentials\n");
        printf("7. Exit to admin menu\n");
        printf("=============================\n");
        printf("Enter your choice: ");
        scanf(" %c", &adminChoice);
        
        //calling the related functions based on user input 
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
                userCredentials();
                break;
            case '7':
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (adminChoice != '7');
}

int addCustomer() {
    // create new struct variable 
    struct Customer newCustomer;

    FILE *file;
    FILE *file2;
    char line[100];

    // user input 
    printf("Enter customer name: ");
    scanf("%s",newCustomer.name);

    //check if the customer already exists 
    file2 = fopen(userCredentialsFile, "r");
    if (file2 == NULL) {
        printf("Cannot open user credentials file.\n");
        return 1;
    }
    // loops through and gets each line 
    while (fgets(line, sizeof(line), file2) != NULL) {
        char existingUsername[20]; // variable for username in file 
        sscanf(line, "%s", existingUsername); // gets username from file 
        if (strcmp(existingUsername, newCustomer.name) == 0) { // compare name in file to new name input 
            printf("Username already exists.\n"); // if already there tell usere 
            fclose(file2);
            return 1;
        }
    }
    fclose(file2);
    // not in file get password for user 
    printf("Enter customer password: ");
    scanf("%s", newCustomer.password);

    // set intial amounts to zero
    newCustomer.electricityUsage = 0;
    newCustomer.gasUsage = 0;
    newCustomer.totalBill = 0;

    // open files and check they exsit
    file = fopen(customerDataFile, "a");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
    }

    file2 = fopen(userCredentialsFile, "a");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
    }

    // write information to custormer credentials
    fprintf(file2, "%s %s\n", newCustomer.name, newCustomer.password);
    
    memmove(newCustomer.name, newCustomer.name + 1, strlen(newCustomer.name)); // Removes the 'C' from the username
    // write to customer data file 
    fprintf(file," %s %f %f %f\n" , newCustomer.name, newCustomer.electricityUsage, newCustomer.gasUsage, newCustomer.totalBill);
    
    fclose(file);
    fclose(file2);

    printf("Customer added successfully.\n");

    return 0;
}

void removeCustomer() {
    char username[20]; // Variable to store customer ID
    char line[100]; // Buffer to read lines from a file
    FILE *file, *tmpfile, *file2, *tmpfile2; // File pointers

    printf("Enter customer ID to remove: ");
    scanf("%s", username); // Reads the customer ID

    // opens the needed file to read and wrtie 
    file = fopen(customerDataFile, "r"); 
    file2 = fopen(userCredentialsFile, "r");
    tmpfile = fopen("temp.txt", "w"); 
    tmpfile2 = fopen("temp2.txt", "w"); 

    if (file == NULL || tmpfile == NULL || file2 == NULL) { // Checks if file opening failed
        printf("Cannot open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file2) != NULL) { // Reads lines from user credentials file
        char id[20]; // Variable to store an ID from the file
        sscanf(line, "%s", id); // Reads an ID from the line

        if (strcmp(id, username) != 0) { // Compares IDs, writes to temp file if not matching
            fputs(line, tmpfile2);
        }
    }

    memmove(username, username + 1, strlen(username)); // Removes the 'C' from the username

    while (fgets(line, sizeof(line), file) != NULL) { // Reads lines from customer data file
        char id[20]; // Variable to store an ID from the file
        sscanf(line, "%s", id); // Reads an ID from the line

        if (strcmp(id, username) != 0) { // Compares IDs, writes to temp file if not matching
            fputs(line, tmpfile);
        }
    }

    // closes all the files
    fclose(file); 
    fclose(file2);
    fclose(tmpfile); 
    fclose(tmpfile2); 

    remove(customerDataFile); // Removes original customer data file
    rename("temp.txt", customerDataFile); // Renames temporary file to the original

    remove(userCredentialsFile); // Removes original user credentials file
    rename("temp2.txt", userCredentialsFile); // Renames temporary file to the original

    printf("Customer removed successfully.\n"); // Prints success message
}

// Comparison function for qsort
int compareCustomers(const void *a, const void *b) {
    struct Customer *customerA = (struct Customer *)a;
    struct Customer *customerB = (struct Customer *)b;
    return strcmp(customerA->name, customerB->name);
}

void sortCustomers() {
    struct Customer customers[MAX_CUSTOMERS];
    struct Customer custlogin[MAX_CUSTOMERS];
    int numCustomers = 0;
    int numlogin =0;
    FILE *file;
    FILE *file2;

    // Step 1: Open the file for reading
    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }
    
    file2 = fopen(userCredentialsFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    // Step 2: Read all customer data into an array
    while (fscanf(file, " %s %f %f %f", customers[numCustomers].name, &customers[numCustomers].electricityUsage, &customers[numCustomers].gasUsage, &customers[numCustomers].totalBill) != EOF) {
        numCustomers++;
    }
    fclose(file);

     while (fscanf(file2, " %s %s", custlogin[numlogin].name,custlogin[numlogin].password) != EOF) {
        numlogin++;
    }
    fclose(file2);

    // Step 3: Sort the array
    qsort(customers, numCustomers, sizeof(struct Customer), compareCustomers);
    qsort(custlogin, numlogin, sizeof(struct Customer), compareCustomers);

    // Step 4: Open the file for writing
    file = fopen(customerDataFile, "w");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    file2 = fopen(userCredentialsFile, "w");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return;
    }

    // Step 5: Write the sorted data back into the file
    for (int i = 0; i < numCustomers; i++) {
        fprintf(file, "%s %f %f %f\n",customers[i].name, customers[i].electricityUsage, customers[i].gasUsage, customers[i].totalBill);
    }

    for (int i = 0; i < numlogin; i++) {
        fprintf(file2, " %s %s\n", custlogin[i].name, custlogin[i].password);
    }

    // Step 6: Close the file
    fclose(file);
    fclose(file2);

    printf("Sorted succesfully");
}

int searchCustomer() {
    //variables
    char username[20];
    char line[100];
    FILE *file;

    // user input 
    printf("Enter customer ID to search: ");
    scanf("%s", username);

    // open file and check 
    file = fopen(customerDataFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return 0;
    }

    memmove(username, username + 1, strlen(username)); // Removes the 'C' from the username

    // loops through all the lines from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char id[20];
        sscanf(line, "%s", id); // gets id from the lide 
        if (strcmp(id, username) == 0) { // compare id to username 
            printf("Found customer: %s\n", line);
            fclose(file);
            return 1;
        }
    }

    printf("No customer found with the given ID.\n");
    fclose(file);
    return 0;
}

int userCredentials() {
    FILE *file;

    file = fopen(userCredentialsFile, "r");
    if (file == NULL) {
        printf("Cannot open customer data file.\n");
        return 1;
    }
    char username[50];
    char password[50];

    printf("\n=============== All Customer Credentials ===============\n");
    printf("%-20s %-20s\n", "Name", "Password"); 
    printf("-------------------------------------------\n");
    while (fscanf(file, "%s %s", username, password) != EOF) { // get username and password from the file 
        printf("%-20s %-20s\n", username, password); //print the information 
    }
    fclose(file);
    return 0;
}

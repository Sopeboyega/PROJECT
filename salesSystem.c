#include<stdio.h>
#include<conio.h>    // For getch() function
#include<string.h>   // For string manipulation functions like strlen(), strcpy()
#include<ctype.h>    // For character classification functions like isdigit()

#define ENTER 13      // Define the ASCII value for Enter key
#define BACKSPACE 8   // Define the ASCII value for Backspace key
#define MAX_LENGTH 13 // Define the maximum length for username, password, and adminId

// Define a struct named 'user' to store user and admin information
struct user {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char adminId[MAX_LENGTH];
};

// Function to take input from the user
void takeInput(char ch[], int maxLength) {
    // Read input from the user
    fgets(ch, maxLength, stdin);

    // Remove newline character, if present
    if (ch[strlen(ch) - 1] == '\n') {
        ch[strlen(ch) - 1] = '\0';
    }
}

// Function to mask password input
void maskPassword(char password[], int maxLength) {
    char ch;
    int i = 0;

    while (1) {
        ch = getch(); // Read a character without echoing it to the console

        if (ch == ENTER) {
            password[i] = '\0'; // Null-terminate the password string
            break; // Exit the loop when Enter is pressed
        } else if (ch == BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the last character from the console
            }
        } else {
            // Mask the character with '*'
            password[i++] = ch;
            printf("*");
        }

        // Break the loop if maxLength is reached
        if (i == maxLength - 1)
            break;
    }
}

// Function to validate admin ID
int validateAdminID(const char id[]) {
    // Check if the ID length is valid
    if (strlen(id) != 6) {
        return 0;
    }

    // Check if the ID format is valid (e.g., all characters are digits)
    for (int i = 0; i < strlen(id); i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }

    return 1;
}

// Main function
int main() {
    struct user user; // Declare a struct variable 'user' to store user data
    int opt;          // Variable to store user's choice
    FILE *fp;         // File pointer to handle file operations

    // Display options for the user
    printf("Choose your option:\n ");
    printf("1. User\n ");
    printf("2. Admin\n ");

    // Prompt the user to enter their choice
    printf("Enter your choice:\n");
    scanf("%d", &opt);
    fflush(stdin); // Flush the input buffer to clear any remaining newline characters

    // If the user chooses option 1 (User)
    if (opt == 1) {
        printf("Enter your username: ");
        takeInput(user.username, MAX_LENGTH); // Call function to take username input
        printf("\nEnter your password: ");
        maskPassword(user.password, MAX_LENGTH); // Call function to mask password input
        printf("\n");

        // Open file in append binary mode
        fp = fopen("user.dat", "ab");
        if (fp == NULL) {
            printf("Error opening file!\n");
            return 1; // Exit the program with error code
        }

        // Write user data to file
        fwrite(&user, sizeof(struct user), 1, fp);

        // Close the file
        fclose(fp);
    }

    // If the user chooses option 2 (Admin)
    if (opt == 2) {
        struct user admin; // Declare a struct variable 'admin' to store admin data

        printf("Enter your username: ");
        takeInput(admin.username, MAX_LENGTH); // Call function to take username input
        printf("\nEnter your employee ID: ");
        takeInput(admin.adminId, MAX_LENGTH); // Call function to take admin ID input

        // Validate admin ID
        int isValidAdminID = validateAdminID(admin.adminId);
        if (isValidAdminID) {
            printf("Admin is registered.\n");

            // Open file in append binary mode
            fp = fopen("admin.dat", "ab");
            if (fp == NULL) {
                printf("Error opening file!\n");
                return 1; // Exit the program with error code
            }

            // Write admin data to file
            fwrite(&admin, sizeof(struct user), 1, fp);

            // Close the file
            fclose(fp);
        } else {
            printf("Invalid Admin ID. Please enter a valid Admin ID.\n");
        }
    }

    return 0; // Exit the program successfully
}
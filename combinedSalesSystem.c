#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COLUMNS 4
#define MAX_LENGTH 100
#define MAX_PRODUCTS 100
#define MAX_USERS 10

typedef struct {
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    float price;
    int quantity;
} Product;

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

typedef struct {
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    float price;
    int quantity;
} CartItem;

Product products[MAX_PRODUCTS];
CartItem cart[MAX_PRODUCTS];
User users[MAX_USERS];
int product_count = 0;
int cart_size = 0;
int user_count = 0;

const char *adminUsername = "admin";
const char *adminPassword = "adminpass";

// Function prototypes
int authenticate(const char *username, const char *password);
void addProduct();
void viewProducts();
void trackProducts();
void deleteProduct();
void populate_products(const char *file_name);
void display_items();
void add_to_cart(const char *name, int quantity);
void view_cart();
void checkout();
void exit_program();
void displayFormattedProducts();
void admin_menu();
void registerUser();

int main() {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("Welcome!\n");

    int choice;
    do {
        printf("\nMenu:\n1. Log in\n2. Register\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clean stdin buffer

        switch (choice) {
            case 1:
                printf("Username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0'; // Remove newline character

                printf("Password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0'; // Remove newline character

                if (authenticate(username, password) > 0) {
                    printf("Login successful.\n");
                    if (strcmp(username, adminUsername) == 0) {
                        admin_menu();
                    } else {
                        // User menu
                    }
                } else {
                    printf("Login failed. Please check your credentials.\n");
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                exit_program();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}

int authenticate(const char *username, const char *password) {
    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        return 1; // Admin
    }
    // Check if user exists in the users array
    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return 2; // User
        }
    }
    return 0; // Authentication failed
}

void registerUser() {
    if (user_count >= MAX_USERS) {
        printf("User registration limit reached.\n");
        return;
    }

    printf("Enter username: ");
    fgets(users[user_count].username, sizeof(users[user_count].username), stdin);
    users[user_count].username[strcspn(users[user_count].username, "\n")] = '\0'; // Remove newline character

    printf("Enter password: ");
    fgets(users[user_count].password, sizeof(users[user_count].password), stdin);
    users[user_count].password[strcspn(users[user_count].password, "\n")] = '\0'; // Remove newline character

    printf("User registration successful.\n");
    user_count++;
}

void admin_menu() {
    int choice;
    do {
        printf("\nAdmin Menu:\n1. Add Product\n2. View Products\n3. Track Products\n4. Delete Product\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                viewProducts();
                break;
            case 3:
                trackProducts();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                exit_program();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void addProduct() {
    if (product_count >= MAX_PRODUCTS) {
        printf("Product list is full!\n");
        return;
    }

    printf("Enter product ID: ");
    scanf("%s", products[product_count].product_id);
    printf("Enter product name: ");
    scanf("%s", products[product_count].name);
    printf("Enter product price: ");
    scanf("%f", &products[product_count].price);
    printf("Enter product quantity: ");
    scanf("%d", &products[product_count].quantity);

    product_count++;
    printf("Product added successfully!\n");
}

void viewProducts() {
    // Header
    printf("%-10s %-20s %-10s %s\n", "ID", "Name", "Price", "Quantity");

    // Product rows
    for (int i = 0; i < product_count; i++) {
        printf("%-10s %-20s $%-10.2f %-10d\n",
               products[i].product_id,
               products[i].name,
               products[i].price,
               products[i].quantity);
    }
}

void trackProducts() {
    float totalValue = 0;
    int totalQuantity = 0;

    for (int i = 0; i < product_count; i++) {
        totalValue += products[i].price * products[i].quantity;
        totalQuantity += products[i].quantity;
    }

    printf("\nTotal Products: %d\n", product_count);
    printf("Total Quantity: %d\n", totalQuantity);
    printf("Total Value: $%.2f\n", totalValue);
}

void deleteProduct() {
    char idToDelete[MAX_LENGTH];
    printf("Enter product ID to delete: ");
    scanf("%s", idToDelete);

    int found = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].product_id, idToDelete) == 0) {
            for (int j = i; j < product_count - 1; j++) {
                products[j] = products[j + 1];
            }
            product_count--;
            found = 1;
            printf("Product deleted successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Product not found.\n");
    }
}

void populate_products(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", file_name);
        exit(EXIT_FAILURE);
    }

    char line[MAX_COLUMNS * MAX_LENGTH];  // Allocate space for entire line
    product_count = 0; // Reset product count each time we populate products

    while (fgets(line, sizeof(line), file)) {
        if (product_count == 0) { // Skip header line
            product_count++;
            continue;
        }

        char *token = strtok(line, ",");
        int column_count = 0;
        Product product;

        while (token != NULL && column_count < MAX_COLUMNS) {
            switch (column_count) {
                case 0:
                    strcpy(product.product_id, token);
                    break;
                case 1:
                    strcpy(product.name, token);
                    break;
                case 2:
                    product.price = atof(token);
                    break;
                case 3:
                    product.quantity = atoi(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            column_count++;
        }

        // Add the product to the products array
        products[product_count - 1] = product;
        product_count++;
    }

    fclose(file);
}

void display_items() {
    printf("Items available:\n");
    for (int i = 0; i < product_count; i++) {
        printf("ID: %s, Name: %s, Price: $%.2f, Quantity: %d\n",
               products[i].product_id, products[i].name, products[i].price, products[i].quantity);
    }

    if (product_count == 0) {
        printf("No items available.\n");
    }
}

void add_to_cart(const char *name, int quantity) {
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        if (strcasecmp(products[i].name, name) == 0) {
            // Check if enough quantity is available
            if (products[i].quantity >= quantity) {
                // Check if the product is already in the cart
                int found = 0;
                for (int j = 0; j < cart_size; j++) {
                    if (strcmp(cart[j].product_id, products[i].product_id) == 0) {
                        cart[j].quantity += quantity;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    // If not found, add new item to cart
                    strcpy(cart[cart_size].product_id, products[i].product_id);
                    strcpy(cart[cart_size].name, products[i].name);
                    cart[cart_size].price = products[i].price;
                    cart[cart_size].quantity = quantity;
                    cart_size++;
                }
                // Reduce the quantity from the inventory
                products[i].quantity -= quantity;
                printf("Added %d of %s to the cart.\n", quantity, name);
                return;
            } else {
                printf("Insufficient quantity available for %s.\n", name);
                return;
            }
        }
    }
    printf("Product with name '%s' does not exist in inventory.\n", name);
}

void view_cart() {
    if (cart_size == 0) {
        printf("Cart is empty.\n");
        return;
    }

    printf("Cart Contents:\n");
    float total_price = 0.0;
    for (int i = 0; i < cart_size; i++) {
        float subtotal = cart[i].quantity * cart[i].price;
        printf("Product ID: %s, Name: %s, Quantity: %d, Price per item: $%.2f, Subtotal: $%.2f\n",
               cart[i].product_id, cart[i].name, cart[i].quantity, cart[i].price, subtotal);
        total_price += subtotal;
    }
    printf("Total Price: $%.2f\n", total_price);
}

void checkout() {
    printf("\nCheckout\n");
    printf("Confirm your order (yes/no): ");
    char confirmation[10];
    scanf("%s", confirmation);

    if (strcasecmp(confirmation, "yes") == 0) {
        float total_price = 0.0;
        for (int i = 0; i < cart_size; i++) {
            total_price += cart[i].quantity * cart[i].price;
        }

        printf("Total Price: $%.2f\n", total_price);
        printf("Order finalized. Thank you for your purchase!\n");

        // Clear the cart
        memset(cart, 0, sizeof(cart));
        cart_size = 0;
    } else {
        printf("Order canceled.\n");
    }
}

void exit_program() {
    printf("Exiting program.\n");
    exit(EXIT_SUCCESS);
}

void displayFormattedProducts() {
    printf("%-10s %-20s %-10s %s\n", "ID", "Name", "Price", "Quantity");
    for (int i = 0; i < product_count; i++) {
        printf("%-10s %-20s $%-10.2f %d\n", products[i].product_id, products[i].name, products[i].price, products[i].quantity);
    }
}

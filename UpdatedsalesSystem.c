#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_COLUMNS 10   // Maximum number of columns in the CSV file
#define MAX_LENGTH 100   // Maximum length of each field in the CSV file
#define MAX_PRODUCTS 100 // Maximum number of products in the cart

typedef struct {
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    double price;
    int quantity;
} Product;

typedef struct {
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    double price;
    int quantity;
} CartItem;

CartItem cart[MAX_PRODUCTS]; // Array to store cart items
int cart_size = 0; // Current number of items in the cart
Product products[MAX_PRODUCTS]; // Array to store products in the cart

// Function prototypes
void add_to_cart(const char *name, int quantity);
void display_items(const char *file_name);
void view_cart();
void checkout();
void exit_program();
void populate_products(const char *file_name); // Function prototype for loading product data

int main() {
    const char *file_name = "inventory.csv";  // Replace with your CSV file name
    int choice;

    // Initialize variables and data structures
    int productId, quantity; // Variables for user input

    // Load product data
    populate_products(file_name);

    // Display Main Menu
    do {
        printf("\nMenu:\n");
        printf("1. Display Items\n");
        printf("2. Add Product to Cart\n");
        printf("3. View Cart\n");
        printf("4. Checkout\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_items(file_name);
                break;
            case 2: {
                char name[MAX_LENGTH];
                printf("Enter the product name: ");
                scanf("%s", name);
                printf("Enter the quantity: ");
                scanf("%d", &quantity);
                add_to_cart(name, quantity);
                break;
            }
            case 3:
                view_cart();
                break;
            case 4:
                checkout();
                break;
            case 5:
                exit_program();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
    return 0;
}

// Function to read data from CSV and populate products array
void populate_products(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", file_name);
        exit(EXIT_FAILURE);
    }

    char line[MAX_COLUMNS * MAX_LENGTH];  // Allocate space for entire line
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line_count == 0) { // Skip header line
            line_count++;
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
        products[line_count - 1] = product;
        line_count++;
    }

    // Update the cart size
    cart_size = line_count - 1;

    fclose(file);
}

// Function to add a product to the cart
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

// Function to display items available
void display_items(const char *file_name) {
    printf("Items available:\n");
    for (int i = 0; i < cart_size; i++) {
        printf("ID: %s, Name: %s, Price: $%.2f, Quantity: %d\n", 
               products[i].product_id, products[i].name, products[i].price, products[i].quantity);
    }

    if (cart_size == 0) {
        printf("No items available.\n");
    }
}

// Function to view cart contents
void view_cart() {
    if (cart_size == 0) {
        printf("Cart is empty.\n");
        return;
    }

    printf("Cart Contents:\n");
    double total_price = 0.0;
    for (int i = 0; i < cart_size; i++) {
        double subtotal = cart[i].quantity * cart[i].price;
        printf("Product ID: %s, Name: %s, Quantity: %d, Price per item: $%.2f, Subtotal: $%.2f\n",
               cart[i].product_id, cart[i].name, cart[i].quantity, cart[i].price, subtotal);
        total_price += subtotal;
    }
    printf("Total Price: $%.2f\n", total_price);
}

// Function to checkout and finalize the order
void checkout() {
    printf("\nCheckout\n");
    printf("Confirm your order (yes/no): ");
    char confirmation[10];
    scanf("%s", confirmation);

    if (strcasecmp(confirmation, "yes") == 0) {
        double total_price = 0.0;
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

// Function to exit the program
void exit_program() {
    printf("Exiting program.\n");
    exit(EXIT_SUCCESS);
}
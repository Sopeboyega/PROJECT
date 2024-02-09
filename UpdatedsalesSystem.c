#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNS 10   // Maximum number of columns in the CSV file
#define MAX_LENGTH 100   // Maximum length of each field in the CSV file
#define MAX_PRODUCTS 100 // Maximum number of products in the inventory
#define MAX_CART_ITEMS 50 // Maximum number of products in the cart

typedef struct {
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    double price;
    int quantity;
} Product;

Product inventory[MAX_PRODUCTS]; // Array to store products in the inventory
int inventory_size = 0; // Current number of products in the inventory

Product cart[MAX_CART_ITEMS]; // Array to store products in the cart
int cart_size = 0; // Current number of products in the cart

// Function prototypes
void populate_inventory(const char *file_name);
void add_to_cart(const char *name, int quantity);
void display_inventory();
void view_cart();
void checkout();
void exit_program();

// Function to read data from CSV and populate inventory array
void populate_inventory(const char *file_name) {
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

        // Add the product to the inventory
        inventory[line_count - 1] = product;
        line_count++;
    }

    // Update the inventory size
    inventory_size = line_count - 1;

    fclose(file);
}

// Function to add a product to the cart
void add_to_cart(const char *name, int quantity) {
    // Find the product with the given name in the inventory (case-insensitive)
    for (int i = 0; i < inventory_size; i++) {
        if (strcasecmp(inventory[i].name, name) == 0) {
            // Product found in the inventory
            if (inventory[i].quantity >= quantity) {
                // Sufficient quantity available in the inventory
                // Add the product to the cart
                if (cart_size < MAX_CART_ITEMS) {
                    // Add the product to the cart
                    cart[cart_size] = inventory[i];
                    cart[cart_size].quantity = quantity;
                    cart_size++;
                    printf("Added %d %s to the cart.\n", quantity, inventory[i].name);

                    // Update the inventory quantity
                    inventory[i].quantity -= quantity;
                } else {
                    printf("Cart is full. Cannot add more items.\n");
                }
            } else {
                // Insufficient quantity available in the inventory
                printf("Insufficient quantity of %s available in the inventory.\n", inventory[i].name);
            }
            return;
        }
    }
    // Product not found in the inventory, display error message
    printf("Product with name '%s' does not exist in the inventory.\n", name);
}

// Function to display items available in the inventory
void display_inventory() {
    printf("Items available in the inventory:\n");
    for (int i = 0; i < inventory_size; i++) {
        printf("ID: %s, Name: %s, Price: $%.2f, Quantity: %d\n", 
               inventory[i].product_id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }

    if (inventory_size == 0) {
        printf("Inventory is empty.\n");
    }
}

// Function to view cart contents
void view_cart() {
    printf("Cart contents:\n");
    if (cart_size == 0) {
        printf("Cart is empty.\n");
        return;
    }
    for (int i = 0; i < cart_size; i++) {
        printf("Name: %s, Quantity: %d\n", cart[i].name, cart[i].quantity);
    }
}

// Function to checkout and finalize the order
void checkout() {
    printf("\nCheckout\n");
    printf("Confirm your order (yes/no): ");
    char confirmation[10];
    scanf("%s", confirmation);

    if (strcmp(confirmation, "yes") == 0) {
        // Calculate total price
        double total_price = 0.0;
        for (int i = 0; i < cart_size; i++) {
            total_price += cart[i].quantity * cart[i].price;
        }

        printf("Total Price: $%.2f\n", total_price);
        printf("Order finalized. Thank you for your purchase!\n");

        // Clear the cart
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

int main() {
    const char *file_name = "inventory.csv";  // Replace with your CSV file name
    int choice;

    // Populate the inventory array at the start of the program
    populate_inventory(file_name);

    // Display the menu
    do {
        printf("\nMenu:\n");
        printf("1. Display Inventory\n");
        printf("2. Add Product to Cart\n");
        printf("3. View Cart\n");
        printf("4. Checkout\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_inventory();
                break;
            case 2: {
                char name[MAX_LENGTH];
                int quantity;
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

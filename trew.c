void addProduct() {
    if (numProducts >= MAX_PRODUCTS) {
        printf("Error: Maximum number of products reached.\n");
        return;
    }

    printf("Enter product details:\n");
    char product_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    double price;
    int quantity;

    printf("Product ID: ");
    scanf("%s", product_id);
    printf("Name: ");
    scanf("%s", name);
    printf("Price: ");
    scanf("%lf", &price);
    printf("Quantity: ");
    scanf("%d", &quantity);

    // Open the inventory.csv file in append mode
    FILE *file = fopen("inventory.csv", "a");
    if (file == NULL) {
        printf("Error: Unable to open file 'inventory.csv'\n");
        return;
    }

    // Append the new product information to the file
    fprintf(file, "%s,%s,%.2lf,%d\n", product_id, name, price, quantity);
    fprintf(file, "\n");  // Start a new line for the next product
    fclose(file);

    printf("Product added successfully.\n");
}

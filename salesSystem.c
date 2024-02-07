int main() {
    int choice, taskNumber;
    char taskDesc[100];

    while (1) {
         printf("To-Do List program ");
        
        printf("\n1. Add Task\n2. Mark Task as Completed\n3. View To-Do List\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // to clear the newline character from the buffer

        switch (choice) {
            case 1:
                
                break;
            case 2:
                
            case 3:
                
            case 4:
                printf("Thank you for choosing Us!.");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

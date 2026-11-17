#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Loan {
    int book_id;
    int student_id;
    char issue_date[11];
    char return_date[11];
    int renewed;
    float fine;
};

void addLoan(struct Loan **loans, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity = (*capacity == 0) ? 5 : *capacity * 2;
        struct Loan *temp = realloc(*loans, (*capacity) * sizeof(struct Loan));
        if (temp == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        *loans = temp;
    }
    printf("Enter Book ID: ");
    scanf("%d", &(*loans)[*count].book_id);
    printf("Enter Student ID: ");
    scanf("%d", &(*loans)[*count].student_id);
    printf("Enter Issue Date (YYYY-MM-DD): ");
    scanf("%s", (*loans)[*count].issue_date);
    printf("Enter Return Date (YYYY-MM-DD): ");
    scanf("%s", (*loans)[*count].return_date);
    (*loans)[*count].renewed = 0;
    (*loans)[*count].fine = 0.0;
    (*count)++;
    printf("Loan record added successfully.\n");
}

void viewLoans(struct Loan *loans, int count) {
    if (count == 0) {
        printf("No loan records found.\n");
        return;
    }
    printf("\n--- All Loan Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("Index %d | Book ID: %d | Student ID: %d | Issue: %s | Return: %s | Renewed: %d | Fine: %.2f\n",
               i, loans[i].book_id, loans[i].student_id, loans[i].issue_date, loans[i].return_date, loans[i].renewed, loans[i].fine);
    }
}

void calculateFines(struct Loan *loans, int count) {
    int index, daysLate;
    printf("Enter index of the loan to calculate fine (0 to %d): ", count - 1);
    scanf("%d", &index);
    if (index < 0 || index >= count) {
        printf("Invalid index.\n");
        return;
    }
    printf("Enter number of days late: ");
    scanf("%d", &daysLate);
    if (daysLate > 0) {
        loans[index].fine = daysLate * 5.0;
        printf("Fine calculated: %.2f\n", loans[index].fine);
    } else {
        loans[index].fine = 0.0;
        printf("No fine.\n");
    }
}

void processRenewal(struct Loan *loans, int count) {
    int index;
    printf("Enter index of the loan to renew (0 to %d): ", count - 1);
    scanf("%d", &index);
    if (index < 0 || index >= count) {
        printf("Invalid index.\n");
        return;
    }
    if (loans[index].fine > 0) {
        printf("Cannot renew. Outstanding fine exists.\n");
    } else {
        loans[index].renewed = 1;
        printf("Book renewal successful.\n");
    }
}

void generateDefaulterReport(struct Loan *loans, int count) {
    printf("\n--- Defaulter Report ---\n");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (loans[i].fine > 0) {
            printf("Book ID: %d | Student ID: %d | Fine: %.2f\n", loans[i].book_id, loans[i].student_id, loans[i].fine);
            found = 1;
        }
    }
    if (!found) {
        printf("No defaulters found.\n");
    }
}

void generateRevenueReport(struct Loan *loans, int count) {
    float totalRevenue = 0.0;
    for (int i = 0; i < count; i++) {
        totalRevenue += loans[i].fine;
    }
    printf("\n--- Revenue Report ---\n");
    printf("Total Fine Revenue Collected: %.2f\n", totalRevenue);
}

void saveToFile(struct Loan *loans, int count) {
    FILE *file = fopen("library_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %d %s %s %d %.2f\n", loans[i].book_id, loans[i].student_id, loans[i].issue_date, loans[i].return_date, loans[i].renewed, loans[i].fine);
    }
    fclose(file);
    printf("Data saved to file successfully.\n");
}

void loadFromFile(struct Loan **loans, int *count, int *capacity) {
    FILE *file = fopen("library_data.txt", "r");
    if (file == NULL) {
        return;
    }
    int fileCount;
    if (fscanf(file, "%d", &fileCount) != 1) {
        fclose(file);
        return;
    }
    *capacity = fileCount > 5 ? fileCount : 5;
    *loans = realloc(*loans, (*capacity) * sizeof(struct Loan));
    if (*loans == NULL) {
        printf("Memory allocation failed during load.\n");
        fclose(file);
        return;
    }
    *count = 0;
    for (int i = 0; i < fileCount; i++) {
        fscanf(file, "%d %d %s %s %d %f", &(*loans)[i].book_id, &(*loans)[i].student_id, (*loans)[i].issue_date, (*loans)[i].return_date, &(*loans)[i].renewed, &(*loans)[i].fine);
        (*count)++;
    }
    fclose(file);
}

int main() {
    struct Loan *loans = NULL;
    int count = 0;
    int capacity = 0;
    int choice;

    loadFromFile(&loans, &count, &capacity);

    while (1) {
        printf("\n=== Library Fine and Renewal System ===\n");
        printf("1. Add Loan\n");
        printf("2. View All Loans\n");
        printf("3. Calculate Fines\n");
        printf("4. Process Renewal\n");
        printf("5. Generate Defaulter Report\n");
        printf("6. Generate Revenue Report\n");
        printf("7. Save and Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                addLoan(&loans, &count, &capacity);
                break;
            case 2:
                viewLoans(loans, count);
                break;
            case 3:
                calculateFines(loans, count);
                break;
            case 4:
                processRenewal(loans, count);
                break;
            case 5:
                generateDefaulterReport(loans, count);
                break;
            case 6:
                generateRevenueReport(loans, count);
                break;
            case 7:
                saveToFile(loans, count);
                free(loans);
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    free(loans);
    return 0;
}

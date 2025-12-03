#include <stdio.h>
#include <string.h>

// Structure to store employee information
struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};

// Function to display all employees in a table-like format
void displayEmployees(struct Employee emp[], int n) {
    int i;

    printf("\n-----------------------------------------------\n");
    printf("ID\tName\t\tDesignation\tSalary\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n",
               emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }

    printf("-----------------------------------------------\n");
}

// Function to find and display employee with highest salary
void findHighestSalary(struct Employee emp[], int n) {
    int i, index = 0;
    float highest = emp[0].salary;

    for (i = 1; i < n; i++) {
        if (emp[i].salary > highest) {
            highest = emp[i].salary;
            index = i;
        }
    }

    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\n", emp[index].id);
    printf("Name: %s\n", emp[index].name);
    printf("Designation: %s\n", emp[index].designation);
    printf("Salary: %.2f\n", emp[index].salary);
}

// Function to search employee by ID or Name
void searchEmployee(struct Employee emp[], int n) {
    int choice, id, i, found = 0;
    char name[50];

    printf("\nSearch Employee:\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Employee ID: ");
        scanf("%d", &id);

        for (i = 0; i < n; i++) {
            if (emp[i].id == id) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].designation);
                printf("Salary: %.2f\n", emp[i].salary);
                found = 1;
                break;
            }
        }
    }
    else if (choice == 2) {
        printf("Enter Employee Name: ");
        scanf("%s", name);

        for (i = 0; i < n; i++) {
            if (strcmp(emp[i].name, name) == 0) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].designation);
                printf("Salary: %.2f\n", emp[i].salary);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("\nEmployee Not Found.\n");
    }
}

int main() {
    struct Employee emp[100];
    int n, i;

    printf("Enter number of employees: ");
    scanf("%d", &n);

    // Input employee records
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d:\n", i + 1);

        printf("ID: ");
        scanf("%d", &emp[i].id);

        printf("Name: ");
        scanf("%s", emp[i].name);

        printf("Designation: ");
        scanf("%s", emp[i].designation);

        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }

    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    return 0;
}

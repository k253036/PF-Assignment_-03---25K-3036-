#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;
    char fullName[100];
    char batch[30];
    char membership[10];
    char regDate[11];
    char dob[11];
    char interest[10];
} Student;

Student *records = NULL;
int count = 0;

void loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    count = size / sizeof(Student);
    if (count > 0) {
        records = malloc(count * sizeof(Student));
        fread(records, sizeof(Student), count, fp);
    }
    fclose(fp);
}

void saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(records, sizeof(Student), count, fp);
    fclose(fp);
}

int existsID(int id) {
    for (int i = 0; i < count; i++)
        if (records[i].studentID == id)
            return 1;
    return 0;
}

void addStudent(const char *filename) {
    Student s;
    printf("Enter Student ID: ");
    scanf("%d", &s.studentID);
    if (existsID(s.studentID)) {
        printf("Duplicate ID. Not added.\n");
        return;
    }
    printf("Enter Full Name: ");
    getchar();
    fgets(s.fullName, 100, stdin);
    s.fullName[strcspn(s.fullName, "\n")] = 0;

    printf("Enter Batch (CS/SE/Cyber Security/AI): ");
    fgets(s.batch, 30, stdin);
    s.batch[strcspn(s.batch, "\n")] = 0;

    printf("Enter Membership (IEEE/ACM): ");
    fgets(s.membership, 10, stdin);
    s.membership[strcspn(s.membership, "\n")] = 0;

    printf("Enter Registration Date (YYYY-MM-DD): ");
    fgets(s.regDate, 11, stdin);
    s.regDate[strcspn(s.regDate, "\n")] = 0;

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    getchar();
    fgets(s.dob, 11, stdin);
    s.dob[strcspn(s.dob, "\n")] = 0;

    printf("Enter Interest (IEEE/ACM/Both): ");
    getchar();
    fgets(s.interest, 10, stdin);
    s.interest[strcspn(s.interest, "\n")] = 0;

    records = realloc(records, (count + 1) * sizeof(Student));
    records[count] = s;
    count++;

    saveDatabase(filename);

    printf("Student Added.\n");
}

void displayAll() {
    if (count == 0) {
        printf("No records.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("\nID: %d\nName: %s\nBatch: %s\nMembership: %s\nReg Date: %s\nDOB: %s\nInterest: %s\n",
               records[i].studentID, records[i].fullName, records[i].batch,
               records[i].membership, records[i].regDate, records[i].dob, records[i].interest);
    }
}

void updateStudent(const char *filename) {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (records[i].studentID == id) {
            printf("Enter new Batch: ");
            getchar();
            fgets(records[i].batch, 30, stdin);
            records[i].batch[strcspn(records[i].batch, "\n")] = 0;

            printf("Enter new Membership: ");
            fgets(records[i].membership, 10, stdin);
            records[i].membership[strcspn(records[i].membership, "\n")] = 0;

            saveDatabase(filename);
            printf("Record Updated.\n");
            return;
        }
    }
    printf("ID not found.\n");
}

void deleteStudent(const char *filename) {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < count; i++)
        if (records[i].studentID == id)
            index = i;

    if (index == -1) {
        printf("ID not found.\n");
        return;
    }

    for (int i = index; i < count - 1; i++)
        records[i] = records[i + 1];

    count--;
    records = realloc(records, count * sizeof(Student));
    saveDatabase(filename);

    printf("Record Deleted.\n");
}

void batchReport() {
    char batch[30];
    char interest[10];
    printf("Enter Batch: ");
    getchar();
    fgets(batch, 30, stdin);
    batch[strcspn(batch, "\n")] = 0;

    printf("Enter Interest (IEEE/ACM/Both): ");
    fgets(interest, 10, stdin);
    interest[strcspn(interest, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcasecmp(records[i].batch, batch) == 0 &&
            strcasecmp(records[i].interest, interest) == 0) {
            printf("\nID: %d\nName: %s\nMembership: %s\n",
                   records[i].studentID, records[i].fullName, records[i].membership);
            found = 1;
        }
    }
    if (!found) printf("No matching records.\n");
}

int main() {
    const char *filename = "members.dat";
    loadDatabase(filename);

    int choice;
    while (1) {
        printf("\n1. Register Student\n2. Update Student\n3. Delete Student\n4. View All\n5. Batch Report\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) addStudent(filename);
        else if (choice == 2) updateStudent(filename);
        else if (choice == 3) deleteStudent(filename);
        else if (choice == 4) displayAll();
        else if (choice == 5) batchReport();
        else if (choice == 6) {
            saveDatabase(filename);
            break;
        } else printf("Invalid.\n");
    }

    free(records);
    return 0;
}

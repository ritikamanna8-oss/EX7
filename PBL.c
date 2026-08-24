#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

typedef struct {
    char id[30];       
    char name[50];
    float fees;
    char course[10];
    char grade[15];
    int is_paid;
} Student;

Student students[MAX_STUDENTS];
int student_count = 0;

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void load_from_file() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return;
    student_count = fread(students, sizeof(Student), MAX_STUDENTS, fp);
    fclose(fp);
}

void save_to_file() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) return;
    fwrite(students, sizeof(Student), student_count, fp);
    fclose(fp);
}

void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("[ERROR] Student limit reached!\n");
        return;
    }

    Student s;
    printf("\n--- ADD NEW STUDENT ---\n");
    
    printf("Enter Student ID: ");
    scanf("%29s", s.id);
    clear_buffer();

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, s.id) == 0) {
            printf("[ERROR] Student ID %s already exists!\n", s.id);
            return;
        }
    }

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Fees Paid: ");
    scanf("%f", &s.fees);
    clear_buffer();

    s.is_paid = (s.fees > 0) ? 1 : 0;
    strcpy(s.course, "None");
    strcpy(s.grade, "Pending");

    students[student_count++] = s;
    save_to_file();
    printf("[SUCCESS] Student registered successfully!\n");
}

void allot_course() {
    char id[30];
    int choice, found = 0;
    printf("\n--- ALLOT COURSE ---\n");
    printf("Enter Student ID: ");
    scanf("%29s", id);
    clear_buffer();

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found = 1;
            if (!students[i].is_paid) {
                printf("[DENIED] Cannot allot course. Fees unpaid for %s.\n", students[i].name);
                return;
            }

            printf("\nSelect Course for %s:\n", students[i].name);
            printf("1. DS\n2. AI\n3. ML\n4. Web\nEnter choice (1-4): ");
            scanf("%d", &choice);
            clear_buffer();

            switch (choice) {
                case 1: strcpy(students[i].course, "DS"); break;
                case 2: strcpy(students[i].course, "AI"); break;
                case 3: strcpy(students[i].course, "ML"); break;
                case 4: strcpy(students[i].course, "Web"); break;
                default: printf("[ERROR] Invalid choice.\n"); return;
            }

            save_to_file();
            printf("[SUCCESS] Course '%s' allocated!\n", students[i].course);
            return;
        }
    }
    if (!found) printf("[ERROR] Student ID %s not found.\n", id);
}

void declare_result() {
    char id[30];
    int choice, found = 0;
    printf("\n--- DECLARE RESULT ---\n");
    printf("Enter Student ID: ");
    scanf("%29s", id);
    clear_buffer();

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found = 1;

            if (strcmp(students[i].course, "None") == 0) {
                printf("[DENIED] Course not allocated yet!\n");
                return;
            }

            printf("\nSelect Grade for %s [Course: %s]:\n", students[i].name, students[i].course);
            printf("1. Excellent\n2. Good\n3. Fail\nEnter choice (1-3): ");
            scanf("%d", &choice);
            clear_buffer();

            switch (choice) {
                case 1: strcpy(students[i].grade, "Excellent"); break;
                case 2: strcpy(students[i].grade, "Good"); break;
                case 3: strcpy(students[i].grade, "Fail"); break;
                default: printf("[ERROR] Invalid choice.\n"); return;
            }

            save_to_file();
            printf("[SUCCESS] Grade '%s' assigned!\n", students[i].grade);
            return;
        }
    }
    if (!found) printf("[ERROR] Student ID %s not found.\n", id);
}

// DELETE STUDENT FUNCTION (DSA Array Left Shift Algorithm)
void delete_student() {
    char id[30];
    int found = 0;

    if (student_count == 0) {
        printf("\n[INFO] No records available to delete.\n");
        return;
    }

    printf("\n--- DELETE STUDENT RECORD ---\n");
    printf("Enter Student ID to Delete: ");
    scanf("%29s", id);
    clear_buffer();

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found = 1;
            // Shifting elements to the left
            for (int j = i; j < student_count - 1; j++) {
                students[j] = students[j + 1];
            }
            student_count--;
            save_to_file(); // Save updated array to binary file
            printf("[SUCCESS] Record for Student ID '%s' deleted successfully!\n", id);
            break;
        }
    }

    if (!found) printf("[ERROR] Student ID %s not found.\n", id);
}

void display_students() {
    if (student_count == 0) {
        printf("\n[INFO] No records available.\n");
        return;
    }

    printf("\n========================================================================================\n");
    printf("%-20s %-20s %-12s %-10s %-12s\n", "ID", "Name", "Fees Paid", "Course", "Grade");
    printf("========================================================================================\n");

    for (int i = 0; i < student_count; i++) {
        printf("%-20s %-20s %-12.2f %-10s %-12s\n",
               students[i].id,
               students[i].name,
               students[i].fees,
               students[i].course,
               students[i].grade);
    }
    printf("========================================================================================\n");
}

int main() {
    load_from_file();
    int choice;
    while (1) {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Register Student\n");
        printf("2. Allot Course\n");
        printf("3. Declare Result\n");
        printf("4. Display Records\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        scanf("%d", &choice);
        clear_buffer();

        switch (choice) {
            case 1: add_student(); break;
            case 2: allot_course(); break;
            case 3: declare_result(); break;
            case 4: display_students(); break;
            case 5: delete_student(); break;
            case 6: exit(0);
            default: printf("[ERROR] Invalid choice!\n");
        }
    }
    return 0;
}
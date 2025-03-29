#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

typedef struct {
    char first_name[50];
    char last_name[50];
    char dob[20];
    char temp_address[100];
    char perm_address[100];
    char gender[10];
    char father_first_name[50];
    char father_last_name[50];
    char mother_first_name[50];
    char mother_last_name[50];
    char user_id[20];
    char password[20];
    char citizenship_type[50];
    char status[20];
} User;

typedef struct {
    char name[100];
    char dob[20];
    char education_level[50];
    char user_id[20];
    char password[20];
    char mobile_number[11];
} Admin;

User user;
Admin admin;

int validate_age(char dob[]);
void user_registration();
void admin_registration();
void user_login();
void admin_login();
void post_login_user();
void admin_dashboard();
void view_applications();
void approveorreject();
void apply_for_citizenship();
void view_application_status();
void trim_whitespace(char *str);
void display_welcome_banner();
void display_main_menu();
void display_colored_message(char *message, char *color);
void loading_animation();
void clear_screen();
void display_applications_table();
int confirm_action(char *message);
void display_goodbye_message();
void pause_program();

void trim_whitespace(char *str) {
    int length = strlen(str);
    while (length > 0 && (str[length - 1] == ' ' || str[length - 1] == '\n')) {
        str[length - 1] = '\0';
        length--;
    }
}

int validate_age(char dob[]) {
    int day, month, year;
    time_t t;
    struct tm tm;

    sscanf(dob, "%d-%d-%d", &day, &month, &year);

    t = time(NULL);
    tm = *localtime(&t);

    int age = tm.tm_year + 1900 - year;
    if (tm.tm_mon + 1 < month || (tm.tm_mon + 1 == month && tm.tm_mday < day)) {
        age--;
    }

    return (age >= 16);
}

void display_welcome_banner() {
    clear_screen();
    printf("\n");
    printf("==============================================\n");
    printf("          CITIZENSHIP APPLICATION\n");
    printf("                MANAGEMENT SYSTEM\n");
    printf("==============================================\n");
    printf("\n");
}

void display_main_menu() {
    clear_screen();
    printf("\n");
    printf("+-----------------------------+\n");
    printf("|         MAIN MENU           |\n");
    printf("+-----------------------------+\n");
    printf("| 1. User Registration        |\n");
    printf("| 2. Admin Registration       |\n");
    printf("| 3. User Login               |\n");
    printf("| 4. Admin Login              |\n");
    printf("| 5. Exit                     |\n");
    printf("+-----------------------------+\n");
    printf("Enter your choice: ");
}

void display_colored_message(char *message, char *color) {
    printf("%s%s%s\n", color, message, COLOR_RESET);
}

void loading_animation() {
	int i;
    printf("Loading ");
    for (i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void clear_screen() {
    system("cls");
}

void display_applications_table() {
    FILE *userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        display_colored_message("Error opening users file.", COLOR_RED);
        return;
    }

    printf("\n+---------------------+---------------------+---------------------+\n");
    printf("| %-20s | %-20s | %-20s |\n", "Name", "User ID", "Status");
    printf("+---------------------+---------------------+---------------------+\n");

    while (fscanf(userFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                  user.first_name, user.last_name, user.dob, user.temp_address,
                  user.perm_address, user.gender, user.father_first_name,
                  user.father_last_name, user.mother_first_name, user.mother_last_name,
                  user.user_id, user.password, user.citizenship_type, user.status) != EOF) {
        printf("| %-20s | %-20s | %-20s |\n",
               user.first_name, user.user_id, user.status);
    }

    printf("+---------------------+---------------------+---------------------+\n");
    fclose(userFile);
}

int confirm_action(char *message) {
	int i;
    char choice[10];
    printf("%s (yes/no): ", message);
    scanf("%s", choice);

    for (i = 0; choice[i]; i++) {
        choice[i] = tolower(choice[i]);
    }

    return (strcmp(choice, "yes") == 0);
}

void display_goodbye_message() {
    printf("\n");
    printf("+-----------------------------------------+\n");
    printf("| Thank you for using our system!         |\n");
    printf("| We hope to see you again soon.          |\n");
    printf("+-----------------------------------------+\n");
    printf("\n");
}

void pause_program() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear the input buffer
    getchar(); // Wait for Enter key
}

void user_registration() {
    FILE *userFile = fopen("users.txt", "a");

    if (userFile == NULL) {
        display_colored_message("Error opening users file.", COLOR_RED);
        pause_program();
        return;
    }

    char confirm[10];
    do {
        printf("Enter First Name: ");
        scanf("%s", user.first_name);
        printf("Enter Last Name: ");
        scanf("%s", user.last_name);
        printf("Enter Date of Birth (DD-MM-YYYY): ");
        scanf("%s", user.dob);

        if (!validate_age(user.dob)) {
            display_colored_message("You must be 16 years or older to register.", COLOR_RED);
            pause_program();
            fclose(userFile);
            return;
        }

        printf("Enter Temporary Address: ");
        scanf(" %[^\n]%*c", user.temp_address);
        printf("Enter Permanent Address: ");
        scanf(" %[^\n]%*c", user.perm_address);
        printf("Enter Gender: ");
        scanf("%s", user.gender);
        printf("Enter Father's First Name: ");
        scanf("%s", user.father_first_name);
        printf("Enter Father's Last Name: ");
        scanf("%s", user.father_last_name);

        // Validate that father's last name matches user's last name
        if (strcmp(user.father_last_name, user.last_name) != 0) {
            display_colored_message("Father's last name must match user's last name.", COLOR_RED);
            pause_program();
            fclose(userFile);
            return;
        }

        printf("Enter Mother's First Name: ");
        scanf("%s", user.mother_first_name);
        printf("Enter Mother's Last Name: ");
        scanf("%s", user.mother_last_name);

        printf("Enter User ID: ");
        scanf("%s", user.user_id);
        printf("Enter Password: ");
        scanf("%s", user.password);

        printf("Choose Citizenship Type (Birth / Descent / Naturalization): ");
        scanf("%s", user.citizenship_type);

        printf("\n+------------------------------+\n");
        printf("|       ENTERED DETAILS       |\n");
        printf("+------------------------------+\n");
        printf("| First Name: %-16s |\n", user.first_name);
        printf("| Last Name: %-17s |\n", user.last_name);
        printf("| Date of Birth: %-12s |\n", user.dob);
        printf("| Temporary Address: %-9s |\n", user.temp_address);
        printf("| Permanent Address: %-9s |\n", user.perm_address);
        printf("| Gender: %-19s |\n", user.gender);
        printf("| Father's Name: %-7s %-8s |\n", user.father_first_name, user.father_last_name);
        printf("| Mother's Name: %-7s %-8s |\n", user.mother_first_name, user.mother_last_name);
        printf("| User ID: %-18s |\n", user.user_id);
        printf("| Citizenship Type: %-9s |\n", user.citizenship_type);
        printf("+-----------------------------+\n");

        printf("\nIs all the information correct? (yes/no): ");
        scanf("%s", confirm);
    } while (strcmp(confirm, "yes") != 0);

    strcpy(user.status, "Pending");

    fprintf(userFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",
            user.first_name, user.last_name, user.dob, user.temp_address,
            user.perm_address, user.gender, user.father_first_name,
            user.father_last_name, user.mother_first_name, user.mother_last_name,
            user.user_id, user.password, user.citizenship_type, user.status);
    fclose(userFile);

    loading_animation();
    display_colored_message("Registration Successful!", COLOR_GREEN);
    pause_program();
}

void admin_registration() {
    display_colored_message("\nNOTE: This task is highly confidential and is of great importance to the Nepal government and its people.", COLOR_YELLOW);
    display_colored_message("Please ensure that all information provided is accurate and secure.\n", COLOR_YELLOW);

    FILE *adminFile = fopen("admins.txt", "a");

    if (adminFile == NULL) {
        display_colored_message("Error opening admins file.", COLOR_RED);
        pause_program();
        return;
    }

    char mobile_number[11];
    int valid_mobile = 0;

    printf("Enter Name: ");
    scanf(" %[^\n]%*c", admin.name);
    printf("Enter Date of Birth (DD-MM-YYYY): ");
    scanf("%s", admin.dob);
    printf("Enter Education Level: ");
    scanf(" %[^\n]%*c", admin.education_level);

    do {
        printf("Enter Mobile Number (10 digits, starting with 97 or 98): ");
        scanf("%s", mobile_number);

        if (strlen(mobile_number) == 10 && (strncmp(mobile_number, "97", 2) == 0 || strncmp(mobile_number, "98", 2) == 0)) {
            valid_mobile = 1;
        } else {
            display_colored_message("Invalid mobile number! It must be 10 digits and start with 97 or 98.", COLOR_RED);
        }
    } while (!valid_mobile);

    printf("Enter Admin User ID: ");
    scanf("%s", admin.user_id);
    printf("Enter Admin Password: ");
    scanf("%s", admin.password);

    fprintf(adminFile, "%s %s %s %s %s %s\n",
            admin.name, admin.dob, admin.education_level, admin.user_id, admin.password, mobile_number);
    fclose(adminFile);

    loading_animation();
    display_colored_message("Admin Registration Successful!", COLOR_GREEN);
    pause_program();
}

void user_login() {
    char id[20], password[20];
    int found = 0;
    FILE *userFile = fopen("users.txt", "r");

    if (userFile == NULL) {
        display_colored_message("Error opening users file.", COLOR_RED);
        pause_program();
        return;
    }

    printf("Enter User ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    while (fscanf(userFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                  user.first_name, user.last_name, user.dob, user.temp_address,
                  user.perm_address, user.gender, user.father_first_name,
                  user.father_last_name, user.mother_first_name, user.mother_last_name,
                  user.user_id, user.password, user.citizenship_type, user.status) != EOF) {

        trim_whitespace(user.user_id);
        trim_whitespace(user.password);

        if (strcmp(id, user.user_id) == 0 && strcmp(password, user.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(userFile);

    if (found) {
        loading_animation();
        display_colored_message("Login Successful!", COLOR_GREEN);
        pause_program();
        post_login_user();
    } else {
        display_colored_message("Invalid User ID or Password. Try again.", COLOR_RED);
        pause_program();
    }
}

void admin_login() {
    char id[20], password[20];
    int found = 0;
    FILE *adminFile = fopen("admins.txt", "r");

    if (adminFile == NULL) {
        display_colored_message("Error opening admins file.", COLOR_RED);
        pause_program();
        return;
    }

    printf("Enter Admin User ID: ");
    scanf("%s", id);
    printf("Enter Admin Password: ");
    scanf("%s", password);

    while (fscanf(adminFile, "%s %s %s %s %s",
                  admin.name, admin.dob, admin.education_level, admin.user_id, admin.password) != EOF) {

        trim_whitespace(admin.user_id);
        trim_whitespace(admin.password);

        if (strcmp(id, admin.user_id) == 0 && strcmp(password, admin.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(adminFile);

    if (found) {
        loading_animation();
        display_colored_message("Admin Login Successful!", COLOR_GREEN);
        pause_program();
        admin_dashboard();
    } else {
        display_colored_message("Invalid Admin ID or Password. Try again.", COLOR_RED);
        pause_program();
    }
}

void post_login_user() {
    int choice;
    while (1) {
        printf("\n+-----------------------------+\n");
        printf("|         USER DASHBOARD      |\n");
        printf("+-----------------------------+\n");
        printf("| 1. Apply for Citizenship    |\n");
        printf("| 2. View Application Status  |\n");
        printf("| 3. Go Back to Main Menu     |\n");
        printf("+-----------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                apply_for_citizenship();
                break;
            case 2:
                view_application_status();
                break;
            case 3:
                return;
            default:
                display_colored_message("Invalid choice! Try again.", COLOR_RED);
                pause_program();
        }
    }
}

void admin_dashboard() {
    int choice;
    while (1) {
        printf("\n+-----------------------------+\n");
        printf("|       ADMIN DASHBOARD       |\n");
        printf("+-----------------------------+\n");
        printf("| 1. View Applications        |\n");
        printf("| 2. Approve/Reject           |\n");
        printf("| 3. Go Back to Main Menu     |\n");
        printf("+-----------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_applications();
                break;
            case 2:
                approveorreject();
                break;
            case 3:
                return;
            default:
                display_colored_message("Invalid choice! Try again.", COLOR_RED);
                pause_program();
        }
    }
}

void view_applications() {
    display_applications_table();
    pause_program();
}

void approveorreject() {
    char user_id[20];
    char new_status[20];
    int found = 0;

    printf("Enter User ID to Approve/Reject: ");
    scanf("%s", user_id);

    FILE *userFile = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp_users.txt", "w");

    if (userFile == NULL || tempFile == NULL) {
        display_colored_message("Error opening files.", COLOR_RED);
        if (userFile) fclose(userFile);
        if (tempFile) fclose(tempFile);
        return;
    }

    while (fscanf(userFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                  user.first_name, user.last_name, user.dob, user.temp_address,
                  user.perm_address, user.gender, user.father_first_name,
                  user.father_last_name, user.mother_first_name, user.mother_last_name,
                  user.user_id, user.password, user.citizenship_type, user.status) != EOF) {

        if (strcmp(user.user_id, user_id) == 0) {
            found = 1;

            printf("\n+-----------------------------+\n");
            printf("|       USER DETAILS          |\n");
            printf("+-----------------------------+\n");
            printf("| First Name: %-15s |\n", user.first_name);
            printf("| Last Name: %-16s |\n", user.last_name);
            printf("| Date of Birth: %-12s |\n", user.dob);
            printf("| Temporary Address: %-9s |\n", user.temp_address);
            printf("| Permanent Address: %-9s |\n", user.perm_address);
            printf("| Gender: %-19s |\n", user.gender);
            printf("| Father's Name: %-7s %-8s |\n", user.father_first_name, user.father_last_name);
            printf("| Mother's Name: %-7s %-8s |\n", user.mother_first_name, user.mother_last_name);
            printf("| User ID: %-18s |\n", user.user_id);
            printf("| Citizenship Type: %-9s |\n", user.citizenship_type);
            printf("| Current Status: %-10s |\n", user.status);
            printf("+-----------------------------+\n");

            if (confirm_action("Is all the information correct and is the user valid for citizenship?")) {
                printf("Enter new status (Approved/Rejected): ");
                scanf("%s", new_status);
                strcpy(user.status, new_status);
                display_colored_message("Application status updated successfully!", COLOR_GREEN);
            } else {
                display_colored_message("No changes were made to the application status.", COLOR_YELLOW);
            }
        }

        fprintf(tempFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",
                user.first_name, user.last_name, user.dob, user.temp_address,
                user.perm_address, user.gender, user.father_first_name,
                user.father_last_name, user.mother_first_name, user.mother_last_name,
                user.user_id, user.password, user.citizenship_type, user.status);
    }

    fclose(userFile);
    fclose(tempFile);

    if (found) {
        remove("users.txt");
        rename("temp_users.txt", "users.txt");
    } else {
        display_colored_message("User ID not found!", COLOR_RED);
        remove("temp_users.txt");
    }
    pause_program();
}

void apply_for_citizenship() {
    printf("Applying for citizenship...\n");
    pause_program();
}

void view_application_status() {
    char user_id[20];
    int found = 0;

    printf("Enter your User ID to check status: ");
    scanf("%s", user_id);

    FILE *userFile = fopen("users.txt", "r");

    if (userFile == NULL) {
        display_colored_message("Error opening users file.", COLOR_RED);
        pause_program();
        return;
    }

    while (fscanf(userFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                  user.first_name, user.last_name, user.dob, user.temp_address,
                  user.perm_address, user.gender, user.father_first_name,
                  user.father_last_name, user.mother_first_name, user.mother_last_name,
                  user.user_id, user.password, user.citizenship_type, user.status) != EOF) {

        if (strcmp(user.user_id, user_id) == 0) {
            printf("\n+-----------------------------+\n");
            printf("|       APPLICATION STATUS    |\n");
            printf("+-----------------------------+\n");
            printf("| User ID: %-18s |\n", user.user_id);
            printf("| Status: %-20s |\n", user.status);
            printf("+-----------------------------+\n");
            found = 1;
            break;
        }
    }

    fclose(userFile);

    if (!found) {
        display_colored_message("Application not found!", COLOR_RED);
    }
    pause_program();
}

int main() {
    int choice;

    while (1) {
        display_welcome_banner();
        display_main_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                user_registration();
                break;
            case 2:
                admin_registration();
                break;
            case 3:
                user_login();
                break;
            case 4:
                admin_login();
                break;
            case 5:
                display_goodbye_message();
                exit(0);
            default:
                display_colored_message("Invalid choice! Try again.", COLOR_RED);
                pause_program();
        }
    }

    return 0;
}

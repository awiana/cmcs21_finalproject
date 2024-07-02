/*
CMSC21 FINAL PROJECT: Room for Rent
Submitted by: Edgar Alan Emmanuel B. Tiamzon III
Lab Instructor: Ma'am Erika Dominique Cunanan
ST5L
06 - 07 - 24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for a tenant
typedef struct tenant_tag {
    char firstName[50];
    char lastName[50];
    char mobileNum[12]; // easier to validate phone number format using char
    struct tenant_tag *nextTenant;
    struct tenant_tag *prevTenant;
} tenant;

// Structure for an apartment unit
typedef struct unit_tag {
    char unitName[50];
    int max_occp;
    int curr_occp;
    // head of tenants linked list
    // enables to access all the information directly inside the unit
    tenant *tenants; 
    struct unit_tag *nextUnit;
    struct unit_tag *prevUnit;
} unit;

// Function prototypes
void main_menu(unit *headUnit);             // prints all the functions of the program (user's interface).
void addUnit(unit **headUnit);              // function where the user can add a new unit. 
void addTenant(unit **headUnit);            // function where the user can add a new tenant in an existing unit.
void searchTenant(unit **headUnit);         // function that searches for a tenant in an existing unit.
void moveTenant(unit **headUnit);           // function that enables moving a tenant to another available apartment.
void removeTenant(unit **headUnit);         // function that removes a tenant if found in an existing unit. 
void editTenant(unit **headUnit);           // function that edits tenant information.
void master_list(unit **headUnit);          // function that displays the master list of occupants.
void occper_unit(unit **headUnit);          // function that displays the list of occupants per unit.
void removeUnit(unit **headUnit);           // function that removes a unit.
void exit_prog(unit **headUnit);            // function that frees all dynamically allocated memory for units and tenants.
void free_memory(unit **headUnit);
void load_tenants(unit **headUnit);         // function that loads tenants from a file.
void save_tenants(unit *headUnit);          // function that saves tenants to a file or another storage medium.

int main() {
    unit *headUnit = NULL;
    load_tenants(&headUnit);
    main_menu(headUnit);
    return 0;
}


// Function to show the main menu
void main_menu(unit *headUnit) {
    int choice;
    while (1) {
        printf("\n");
        printf("==========================================================\n");
        printf("|           APARTMENT COMPLEX MANAGEMENT SYSTEM          |\n");
        printf("|========================================================|\n");
        printf("| [1] Add Unit                                           |\n");
        printf("| [2] Add Tenant                                         |\n");
        printf("| [3] Search Tenant                                      |\n");
        printf("| [4] Move Tenant                                        |\n");
        printf("| [5] Remove Tenant                                      |\n");
        printf("| [6] Edit Tenant                                        |\n");
        printf("| [7] Master Lists of Occupants                          |\n");
        printf("| [8] Lists of Occupants per Unit                        |\n");
        printf("| [9] Remove Unit                                        |\n");
        printf("| [10] Exit                                              |\n");
        printf("==========================================================\n");
        printf("\nEnter Choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // clearing the input buffer

        switch (choice){
            case 1:
                addUnit(&headUnit);
                break;
            case 2:
                addTenant(&headUnit);
                break;
            case 3:
                searchTenant(&headUnit);
                break;
            case 4:
                moveTenant(&headUnit);
                break;
            case 5:
                removeTenant(&headUnit);
                break;
            case 6:
                editTenant(&headUnit);
                break;
            case 7:
                master_list(&headUnit);
                break;
            case 8:
                occper_unit(&headUnit);
                break;
            case 9:
                removeUnit(&headUnit);
                break;
            case 10:
                printf("Exiting the program...");
                save_tenants(headUnit);
                free_memory(&headUnit);
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again!\n");
        }
    }
}


void addUnit(unit **headUnit) {
    unit *new_unit = (unit *)malloc(sizeof(unit));
    if (new_unit == NULL) {
        printf("\nERROR: Memory Allocation Failed!");
        return;
    }

    printf("==========================================================");
    printf("\n|                       ADD UNIT                         |\n");
    printf("==========================================================\n");
    printf("Enter Unit Name: ");
    scanf(" %[^\n]", new_unit->unitName);

    // convert unit name to lowercase for case-insensitive comparison
    for (int i = 0; new_unit->unitName[i]; i++){
        new_unit->unitName[i] = tolower(new_unit->unitName[i]);
    }

    // checks for duplication of unit's name
    unit *current = *headUnit;
    while (current != NULL) {
        char currentName[50];
        strcpy(currentName, current->unitName);

        // convert tenant name to lowercase for case-insensitive comparison
        for (int i = 0; currentName[i]; i++) {
            currentName[i] = tolower(currentName[i]);
        }
        
        // prompt that the unit name is already exist
        if (strcmp(currentName, new_unit->unitName) == 0) {
            printf("\n[Unit's name already exists!]\n");
            free(new_unit);
            return;
        }
        current = current->nextUnit;
    }

    printf("Enter Maximum of Occupants: ");
    scanf("%d", &new_unit->max_occp);

    printf("\n[Unit's Information]\n");
    printf("\nUnit Name: %s\n", new_unit->unitName);
    printf("----------------------------------------------------------\n");
    printf("Maximum Occupants: %d\n", new_unit->max_occp);

    while (getchar() != '\n'); // clearing the input buffer

    new_unit->curr_occp = 0;
    new_unit->tenants = NULL;

    // check first if the list of units is empty 
    if (*headUnit == NULL) {
        new_unit->nextUnit = NULL;
        new_unit->prevUnit = NULL;
        *headUnit = new_unit;
    } else {
        current = *headUnit;
        unit *previous = NULL;
        
        // transversing the list to find the correct position of the new unit
        while (current != NULL && strcmp(current->unitName, new_unit->unitName) < 0) {
            previous = current;
            current = current->nextUnit;
        }
        
        // insertion of the new unit at the beginning of the list
        if (previous == NULL) { 
            new_unit->nextUnit = *headUnit;
            new_unit->prevUnit = NULL;
            (*headUnit)->prevUnit = new_unit;
            *headUnit = new_unit;
        
        // insertion of the new unit in the middle or at the end of the list
        } else { 
            new_unit->nextUnit = previous->nextUnit;
            new_unit->prevUnit = previous;
            if (previous->nextUnit != NULL) {
                previous->nextUnit->prevUnit = new_unit;
            }
            previous->nextUnit = new_unit;
        }
    }

    printf("\n[Unit Added Successfully!]\n");
}


void addTenant(unit **headUnit) {
    tenant *new_tenant = (tenant *)malloc(sizeof(tenant));
    if (new_tenant == NULL) {
        printf("\nERROR: Memory Allocation Failed!");
        return;
    }

    if (*headUnit == NULL){
        printf("[No units available.]\n");
        return;
    }

    printf("==========================================================");
    printf("\n|                      ADD TENANT                        |\n");
    printf("==========================================================");
    printf("\nEnter First Name: ");
    scanf(" %[^\n]", new_tenant->firstName);
    
    printf("Enter Last Name: ");
    scanf(" %[^\n]", new_tenant->lastName);

        // convert tenant's names to lowercase for case-insensitive comparison
    for (int i = 0; new_tenant->firstName[i]; i++){
        new_tenant->firstName[i] = tolower(new_tenant->firstName[i]);
    }
    for (int i = 0; new_tenant->lastName[i]; i++){
        new_tenant->lastName[i] = tolower(new_tenant->lastName[i]);
    }

    printf("Enter Mobile Number: ");
    scanf(" %[^\n]", new_tenant->mobileNum);

    printf("Enter Unit Name to add tenant: ");
    char unitName[50];
    scanf(" %[^\n]", unitName);

    while (getchar() != '\n'); // clearing the input buffer

    printf("\n[Tenant's Information]\n");
    printf("\nName: %c%s %c%s\n", toupper(new_tenant->firstName[0]), new_tenant->firstName + 1, // name should be printed in upper case
                                  toupper(new_tenant->lastName[0]), new_tenant->lastName + 1); // name should be printed in upper case
    printf("Mobile Number: %s\n", new_tenant->mobileNum);
    printf("Added to Unit %s\n", unitName);


    if (strlen(new_tenant->mobileNum) != 11) {
        printf("\n[Invalid Mobile Number. Must be 11 digits.]\n");
        free(new_tenant);
        return;
    }

    unit *init_unit = *headUnit;
    while (init_unit != NULL) {

        // convert current unit name to lowercase for case-insensitive comparison
        char currentName[50];
        strcpy(currentName, init_unit->unitName);
        for (int i = 0; currentName[i]; i++) {
            currentName[i] = tolower(currentName[i]);
        }

        // checks if the unit name matches the target unit name
        if (strcmp(currentName, unitName) == 0) {
            if (init_unit->curr_occp < init_unit->max_occp) {

                // checks for duplication of tenant's names within the same unit
                tenant *init_tenant = init_unit->tenants;
                while (init_tenant != NULL) {

                    // convert existing tenant's names to lowercase for comparison
                    char e_firstName[50];
                    char e_lastName[50];
                    strcpy(e_firstName, init_tenant->firstName);
                    strcpy(e_lastName, init_tenant->lastName);
                    for (int i = 0; e_firstName[i]; i++) {
                        e_firstName[i] = tolower(e_firstName[i]);
                    }
                    for (int i = 0; e_lastName[i]; i++) {
                        e_lastName[i] = tolower(e_lastName[i]);
                    }

                    // check if both first name and last name match the new tenant's names
                    if (strcmp(e_firstName, new_tenant->firstName) == 0 &&
                        strcmp(e_lastName, new_tenant->lastName) == 0) {
                        printf("\n[Tenant's name already exists!]\n");
                        free(new_tenant);
                        return;
                    }
                    init_tenant = init_tenant->nextTenant;
                }

                // add the tenant to the unit in sorted order
                tenant **headTenant = &(init_unit->tenants);
                tenant *prev = NULL;
                init_tenant = *headTenant;

                // traverse the tenant list to find the correct position to insert the new tenant
                while (init_tenant != NULL && 
                      (strcmp(init_tenant->lastName, new_tenant->lastName) < 0 || 
                      (strcmp(init_tenant->lastName, new_tenant->lastName) == 0 && 
                       strcmp(init_tenant->firstName, new_tenant->firstName) < 0))) {
                    prev = init_tenant;
                    init_tenant = init_tenant->nextTenant;
                }

                if (prev == NULL) {
                    // insert at the beginning
                    new_tenant->nextTenant = *headTenant;
                    if (*headTenant != NULL) {
                        // if there is already a tenant at the head, update its previous pointer
                        (*headTenant)->prevTenant = new_tenant;
                    }
                    *headTenant = new_tenant;
                    new_tenant->prevTenant = NULL;
                } else {
                    // insert after prev
                    new_tenant->nextTenant = prev->nextTenant;
                    if (prev->nextTenant != NULL) {
                        // If there is a next tenant, update its previous pointer
                        prev->nextTenant->prevTenant = new_tenant;
                    }
                    prev->nextTenant = new_tenant;
                    new_tenant->prevTenant = prev;
                }

                // Increment the current occupancy of the unit
                init_unit->curr_occp++;
                printf("\n[Tenant Added Successfully!]\n");
                return;
            } else {
                printf("\n[Unit is Full!]\n");
                free(new_tenant);
                return;
            }
        }
        init_unit = init_unit->nextUnit;
    }
    printf("\n[Unit not found!]\n");
    free(new_tenant);
}


// using singly linked list much more doable
void searchTenant(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    char firstName[50], lastName[50];
    printf("==========================================================\n");
    printf("|                      SEARCH TENANT                     |\n");
    printf("==========================================================\n");
    
    printf("Enter First Name: ");
    scanf(" %[^\n]", firstName);

    printf("Enter Last Name: ");
    scanf(" %[^\n]", lastName);

    unit *init_unit = *headUnit;
    int tenantFound = 0;  // Flag to check if the tenant was found

    while (init_unit != NULL) {
        tenant *init_tenant = init_unit->tenants;
        while (init_tenant != NULL) {
            if (strcmp(init_tenant->firstName, firstName) == 0 &&
                strcmp(init_tenant->lastName, lastName) == 0) {
                
                // print all the information once the tenant found
                printf("\nUnit Name: %s\n", init_unit->unitName);
                printf("----------------------------------------------------------\n");
                printf("[Tenant's Information]\n");
                printf("\nFirst Name: %c%s\n", toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
                printf("Last Name: %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1);
                printf("Mobile Number: %s\n", init_tenant->mobileNum);
                printf("----------------------------------------------------------\n");

                
                tenantFound = 1;  // set flag to indicate tenant found
            }
            init_tenant = init_tenant->nextTenant;
        }
        init_unit = init_unit->nextUnit;
    }

    if (!tenantFound) {
        printf("\n[Tenant not found!]\n");
    }
}


void moveTenant(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    char firstName[50], lastName[50];
    printf("==========================================================\n");
    printf("|                       MOVE TENANT                      |\n");
    printf("==========================================================\n");

    printf("Enter First Name: ");
    scanf(" %[^\n]", firstName);

    printf("Enter Last Name: ");
    scanf(" %[^\n]", lastName);

    unit *init_unit = *headUnit;
    int tenantFound = 0;
    while (init_unit != NULL) {
        tenant *init_tenant = init_unit->tenants;
        while (init_tenant != NULL) {
            if (strcmp(init_tenant->firstName, firstName) == 0 &&
                strcmp(init_tenant->lastName, lastName) == 0) {

                tenantFound = 1;

                // Tenant found - print the information
                printf("\nUnit Name: %s\n", init_unit->unitName);
                printf("----------------------------------------------------------\n");
                printf("[Tenant's Information]\n");
                printf("\nFirst Name: %c%s\n", toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
                printf("Last Name: %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1);
                printf("Mobile Number: %s\n", init_tenant->mobileNum);
                printf("----------------------------------------------------------\n");

                // Display available units
                unit *availUnit = *headUnit;
                printf("\n==========================================================\n");
                printf("|                  LIST OF AVAILABLE UNIT                |\n");
                printf("==========================================================\n");
                while (availUnit != NULL) {
                    if (availUnit->curr_occp < availUnit->max_occp) {
                        printf("\nUnit Name: %s\n", availUnit->unitName);
                        printf("Current Occupants: %d / %d\n", availUnit->curr_occp, availUnit->max_occp);
                        printf("----------------------------------------------------------\n");
                    }
                    availUnit = availUnit->nextUnit;
                }

                char unit_occ[50];
                printf("Choose unit to be occupied: ");
                scanf(" %[^\n]", unit_occ);
                while (getchar() != '\n');
                
                // sets the condition where if the unitName has already have maximum occupants
                unit *newUnit = *headUnit;
                while (newUnit != NULL) {
                    if (strcmp(newUnit->unitName, unit_occ) == 0) {
                        if (newUnit->curr_occp < newUnit->max_occp) {

                            // Remove tenant from the current unit
                            if (init_tenant->prevTenant != NULL) {
                                init_tenant->prevTenant->nextTenant = init_tenant->nextTenant;
                            } 
                            
                            else {
                                init_unit->tenants = init_tenant->nextTenant;
                            }
                            if (init_tenant->nextTenant != NULL) {
                                init_tenant->nextTenant->prevTenant = init_tenant->prevTenant;
                            }
                            init_unit->curr_occp--;

                            // add tenant to the new unit in sorted order
                            // the tenants is included in the structure of the unit so that it can easily access all the information of each tenant. 
                            // consequently, the head of the tenants linked list is already sorted alphabetically, as it is traversed during the insertion of new tenants in the tenant list, 
                            // where the process already implemented in the addTenant function
                            // which then the list of tenants for the specified unit is traversed starting from the head of the tenant list (newUnit->tenants)
                            init_tenant->nextTenant = newUnit->tenants;
                            if (newUnit->tenants != NULL) {
                                newUnit->tenants->prevTenant = init_tenant;
                            }
                            newUnit->tenants = init_tenant;
                            init_tenant->prevTenant = NULL;

                            newUnit->curr_occp++;
                            printf("\n[Tenant moved successfully!]\n");
                            return;
                        } else {
                            printf("\n[Selected unit is full!]\n");
                            return;
                        }
                    }
                    newUnit = newUnit->nextUnit;
                }
                printf("\n[Selected unit not found!]\n");
                return;
            }
            init_tenant = init_tenant->nextTenant;
        }
        init_unit = init_unit->nextUnit;
    }
    if (!tenantFound) {
        printf("\n[Tenant not found.]\n");
    }
}


void removeTenant(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    char firstName[50], lastName[50];
    printf("==========================================================\n");
    printf("|                      REMOVE TENANT                     |\n");
    printf("==========================================================\n");

    printf("Enter First Name: ");
    scanf(" %[^\n]", firstName);

    printf("Enter Last Name: ");
    scanf(" %[^\n]", lastName);

    unit *init_unit = *headUnit;
    int tenantFound = 0;

    while (init_unit != NULL) {
        tenant *init_tenant = init_unit->tenants;

        while (init_tenant != NULL) {
            if (strcmp(init_tenant->firstName, firstName) == 0 &&
                strcmp(init_tenant->lastName, lastName) == 0) {

                // Tenant found
                printf("\nUnit Name: %s\n", init_unit->unitName);
                printf("----------------------------------------------------------\n");
                printf("[Tenant's Information]\n");
                printf("\nFirst Name: %c%s\n", toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
                printf("Last Name: %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1);
                printf("Mobile Number: %s\n", init_tenant->mobileNum);
                printf("----------------------------------------------------------\n");

                // Making sure the user wants to remove the tenant
                // for confirmation
                int ans = 0;
                printf("\nAre you sure you want to remove the tenant?");
                printf("\n[1] - Yes");
                printf("\n[2] - No\n");
                printf("\nEnter your choice: ");
                scanf("%d", &ans);
                printf("\n----------------------------------------------------------\n");

                if (ans == 1) {
                    // Remove the tenant from the list
                    if (init_tenant->prevTenant != NULL) {
                        init_tenant->prevTenant->nextTenant = init_tenant->nextTenant;
                    } else {
                        init_unit->tenants = init_tenant->nextTenant; // Tenant is head of the list
                    }

                    if (init_tenant->nextTenant != NULL) {
                        init_tenant->nextTenant->prevTenant = init_tenant->prevTenant;
                    }

                    // deletes all the information of the specified tenant
                    free(init_tenant);
                    init_unit->curr_occp--;
                    tenantFound = 1; 
                    printf("\n[Tenant removed successfully!]\n");
                    break; // exit the inner while loop

                } else if (ans == 2) {
                    printf("\nRest assured, the tenant is not removed!\n");
                    printf("\n[Thank you for your response!]\n");
                    tenantFound = 1;
                    break; // exit the inner while loop
                }
            }
            init_tenant = init_tenant->nextTenant;
        }
        if (tenantFound) {
            break; // Exit the outer while loop if the tenant was found and handled
        }
        init_unit = init_unit->nextUnit;
    }

    if (!tenantFound) {
        printf("\n[Tenant not found.]\n"); 
    }
}

// using singly linked list as it much doable
// as it only searches one node to print all of the information of the tenant 
void editTenant(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    char firstName[50], lastName[50];
    printf("==========================================================\n");
    printf("|                      EDIT TENANT                       |\n");
    printf("==========================================================\n");
    
    printf("Enter First Name: ");
    scanf(" %[^\n]", firstName);

    printf("Enter Last Name: ");
    scanf(" %[^\n]", lastName);

    unit *init_unit = *headUnit;
    while (init_unit != NULL) {
        tenant *init_tenant = init_unit->tenants;
        while (init_tenant != NULL) {
            if (strcmp(init_tenant->firstName, firstName) == 0 &&
                strcmp(init_tenant->lastName, lastName) == 0) {
                
                // Tenant found
                printf("\nUnit Name: %s\n", init_unit->unitName);
                printf("----------------------------------------------------------\n");
                printf("[Tenant's Information]\n");
                printf("\nFirst Name: %c%s\n", toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
                printf("Last Name: %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1);
                printf("Mobile Number: %s\n", init_tenant->mobileNum);
                printf("----------------------------------------------------------\n");


                // Input new mobile number
                char newMobileNum[12];
                printf("\nEnter New Mobile Number: ");
                scanf(" %[^\n]", newMobileNum);

                // sets the condition for inputing new mobile number must be 11 digits
                if (strlen(newMobileNum) != 11) {
                    printf("\n[Invalid Mobile Number. Must be 11 digits.]\n");
                } else {
                    strcpy(init_tenant->mobileNum, newMobileNum);
                    printf("\n[Mobile Number Updated Successfully!]\n");
                }
                return;
            }
            init_tenant = init_tenant->nextTenant;
        }
        init_unit = init_unit->nextUnit;
    }

    printf("\n[Tenant not found!]\n");
}


void master_list(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    printf("==========================================================\n");
    printf("|                   MASTER LIST OF OCCUPANTS             |\n");
    printf("==========================================================\n");

    unit *init_unit = *headUnit;
    while (init_unit != NULL) {
        printf("\nUnit Name: %s\n", init_unit->unitName);
        printf("----------------------------------------------------------\n");


        // prints tenant in each unit
        // trint all the information in alphabetical format 
        // the tenants is included in the structure of the unit so that it can easily access all the information of each tenant
        // consequently, the head of the tenants linked list is already sorted alphabetically, as it is traversed during the insertion of new tenants in the tenant list, 
        // where the process already implemented in the addTenant function
        // which then the list of tenants for the specified unit is traversed starting from the head of the tenant list (init_unit->tenants)
        tenant *init_tenant = init_unit->tenants;
        while (init_tenant != NULL) {
            printf("Tenant's Name: %c%s, %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1,
                                                  toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
            init_tenant = init_tenant->nextTenant;
        }

        printf("\n");
        init_unit = init_unit->nextUnit;
    }
}

void occper_unit(unit **headUnit) {
    if (*headUnit == NULL) {
        printf("[No units available.]\n");
        return;
    }

    printf("==========================================================\n");
    printf("|                   OCCUPANTS PER UNIT                   |\n");
    printf("==========================================================\n");
    
    printf("Enter Unit Name: ");
    char unitName[50];
    scanf(" %[^\n]", unitName);
    while (getchar() != '\n'); // for clearing input buffer

    unit *init_unit = *headUnit;
    while (init_unit != NULL) {
        if (strcmp(init_unit->unitName, unitName) == 0) {
            printf("\nUnit Name: %s\n", init_unit->unitName);
            printf("----------------------------------------------------------\n");


            // prints tenant in the specific unit
            // print all the information in alphabetical format 
            // the tenants is included in the structure of the unit so that it can easily access all the information of each tenant
            // consequently, the head of the tenants linked list is already sorted alphabetically, as it is traversed during the insertion of new tenants in the tenant list, 
            // where the process already implemented in the addTenant function
            // which then the list of tenants for the specified unit is traversed starting from the head of the tenant list (init_unit->tenants)
            tenant *init_tenant = init_unit->tenants;
            while (init_tenant != NULL) {
                printf("Tenant's Name: %c%s, %c%s\n", toupper(init_tenant->lastName[0]), init_tenant->lastName + 1,
                                                      toupper(init_tenant->firstName[0]), init_tenant->firstName + 1);
                init_tenant = init_tenant->nextTenant;
            }
            printf("\n");
            return;
        }
        init_unit = init_unit->nextUnit;
    }

    printf("\n[Unit not found!]\n");
}

void removeUnit(unit **headUnit) {
    if (*headUnit == NULL){
        printf("[No units available.]\n");
        return;
    }

    char unitName[50];
    printf("==========================================================\n");
    printf("|                       REMOVE UNIT                      |\n");
    printf("==========================================================\n");
    
    printf("Unit Name: ");
    scanf(" %[^\n]", unitName);

    unit *removeUnit = *headUnit;
    while (removeUnit != NULL) {
        if (strcmp(removeUnit->unitName, unitName) == 0) {
            if (removeUnit->tenants == NULL) {
                printf("[No tenant in this unit.]\n");
                return;
            }

            // If it is occupied, print all the tenants in the unit
            printf("\nUnit Name: %s\n", removeUnit->unitName);
            printf("----------------------------------------------------------\n");

            // Print tenants in the specific unit
            // Prints in alphabetical format based on the addTenant function
            tenant *init_tenant = removeUnit->tenants;
            while (init_tenant != NULL) {
                printf("Tenant's Name: %s, %s\n", init_tenant->lastName, init_tenant->firstName);
                init_tenant = init_tenant->nextTenant;
            }

            // Prompt if the user wants to remove the unit
            char choose;
            printf("\n----------------------------------------------------------\n");
            printf("\nDo you want to remove the unit [ Y | N ]? ");
            scanf(" %c", &choose);

            if (choose == 'y' || choose == 'Y') {
                int option;
                // Prompt if the tenants should be moved or removed
                printf("\nAre the tenants to be moved or removed?");
                printf("\n[1] - Move");
                printf("\n[2] - Remove\n");
                printf("\nEnter choice: ");
                scanf("%d", &option);
                printf("\n----------------------------------------------------------\n");

                tenant *moveTenant = removeUnit->tenants;
                if (option == 1) { 
                    // Algorithm for move
                    // List all the occupants per unit
                    unit *init_unit = *headUnit;
                    while (init_unit != NULL) {
                        // condition where the entered unit only will list the tenant per unit
                        printf("\nUnit Name: %s\n", init_unit->unitName);
                        printf("----------------------------------------------------------\n");

                        // Print tenants in each unit in alphabetical order
                        int tenantIndex = 1; // Initialize the tenant index
                        tenant *init_tenant = init_unit->tenants;
                        while (init_tenant != NULL) {
                            printf("[%d] %s, %s\n", tenantIndex, init_tenant->lastName, init_tenant->firstName);
                            init_tenant = init_tenant->nextTenant;
                            tenantIndex++; // Increment the tenant index
                        }

                        init_unit = init_unit->nextUnit;
                    }

                    // Display available units
                    unit *availUnit = *headUnit;
                    printf("\n==========================================================\n");
                    printf("|                  LIST OF AVAILABLE UNITS               |\n");
                    printf("==========================================================\n");
                    while (availUnit != NULL) {
                        if (availUnit->curr_occp < availUnit->max_occp) {
                            printf("\nUnit Name: %s\n", availUnit->unitName);
                            printf("Current Occupants: %d / %d\n", availUnit->curr_occp, availUnit->max_occp);
                            printf("----------------------------------------------------------\n");
                        }
                        availUnit = availUnit->nextUnit;
                    }

                    char unit_occ[50];
                    int num_tenant;
                    printf("Enter the tenant number to move: ");
                    scanf("%d", &num_tenant);

                    printf("Enter the unit name to move the tenant to: ");
                    scanf(" %[^\n]", unit_occ);
                    while (getchar() != '\n'); // clear the input buffer

                    unit *newUnit = *headUnit;
                    while (newUnit != NULL) {
                        // checks if the chosen unit name exist and if its already occupied
                        if (strcmp(newUnit->unitName, unit_occ) == 0) {  
                            if (newUnit->curr_occp < newUnit->max_occp) {
                                tenant *init_tenant = removeUnit->tenants;
                                
                                // loop until the desired tenant is found
                                int tenantIndex = 1;
                                while (init_tenant != NULL && tenantIndex < num_tenant) {
                                    init_tenant = init_tenant->nextTenant;
                                    tenantIndex++;
                                }
                                // condition that validates if the tenant number exist
                                if (init_tenant == NULL || tenantIndex != num_tenant) {
                                    printf("\n[Invalid tenant number.]\n");
                                    return;
                                }

                                // Remove tenant from the current unit
                                if (init_tenant->prevTenant != NULL) {
                                    init_tenant->prevTenant->nextTenant = init_tenant->nextTenant;
                                } else {
                                    removeUnit->tenants = init_tenant->nextTenant;
                                }
                                if (init_tenant->nextTenant != NULL) {
                                    init_tenant->nextTenant->prevTenant = init_tenant->prevTenant;
                                }
                                removeUnit->curr_occp--;

                                // Add tenant to the new unit in sorted order
                                init_tenant->nextTenant = newUnit->tenants;
                                if (newUnit->tenants != NULL) {
                                    newUnit->tenants->prevTenant = init_tenant;
                                }
                                newUnit->tenants = init_tenant;
                                init_tenant->prevTenant = NULL;

                                newUnit->curr_occp++;
                                printf("\n[Tenant moved successfully!]\n");
                                return;
                            } else {
                                printf("\n[Selected unit is full!]\n");
                                return;
                            }
                        }
                        newUnit = newUnit->nextUnit;
                    }
                    printf("\n[Selected unit not found!]\n");
                    return;
                } else if (option == 2) { 
                    // Algorithm for remove
                    // Remove tenants and unit
                    while (removeUnit->tenants != NULL) {
                        tenant *tempTenant = removeUnit->tenants;
                        removeUnit->tenants = removeUnit->tenants->nextTenant;
                        free(tempTenant);
                    }

                    if (removeUnit->prevUnit != NULL) {
                        removeUnit->prevUnit->nextUnit = removeUnit->nextUnit;
                    } else {
                        *headUnit = removeUnit->nextUnit; // unit is head of the list
                    }

                    if (removeUnit->nextUnit != NULL) {
                        removeUnit->nextUnit->prevUnit = removeUnit->prevUnit;
                    }

                    free(removeUnit);
                    printf("\n[Unit removed successfully!]\n");
                    return;
                }
            } else {
                printf("\n[Invalid choice!]\n");
                return;
            }
        }
        removeUnit = removeUnit->nextUnit;
    }

    printf("[Unit not found.]\n");
}

void exit_prog(unit **headUnit) {
    save_tenants(*headUnit);
    free_memory(headUnit);
    exit(0);
}

// Function to free memory
void free_memory(unit **headUnit){
    unit *currentUnit = *headUnit;
    unit *nextUnit;

    while (currentUnit != NULL) {
        nextUnit = currentUnit->nextUnit;

        while (currentUnit->tenants != NULL) {
            tenant *tempTenant = currentUnit->tenants;
            currentUnit->tenants = currentUnit->tenants->nextTenant;
            free(tempTenant);
        }

        free(currentUnit);
        currentUnit = nextUnit;
    }

    *headUnit = NULL; // Set headUnit to NULL after freeing all memory
}

void load_tenants(unit **headUnit) {
    FILE *file = fopen("tenant.txt", "r");
    if (file == NULL) {
        printf("[Error opening file for reading.\n]");
        return;
    }

    // variables to store unit and tenant information
    char unitName[50], firstName[50], lastName[50], mobileNum[12];
    int curr_occp, max_occp;
    unit *lastUnit = NULL;

    // read unit and tenant information from the file
    while (fscanf(file, "%s %d %d", unitName, &curr_occp, &max_occp) == 3) {

        // allocate memory for a new unit
        unit *newUnit = (unit *)malloc(sizeof(unit));
        // copy the unit's information
        strcpy(newUnit->unitName, unitName);
        newUnit->curr_occp = curr_occp;
        newUnit->max_occp = max_occp;
        newUnit->tenants = NULL;  // initializing tenant list for this unit
        newUnit->nextUnit = NULL;
        newUnit->prevUnit = lastUnit;

        // update previous unit's next pointer if this is not the first unit
        if (lastUnit != NULL) {
            lastUnit->nextUnit = newUnit;
        } else {
            *headUnit = newUnit;  
        }

        // this update lastUnit pointer
        lastUnit = newUnit;

        // read tenant information for this unit
        while (fscanf(file, "%s", lastName) == 1 && strcmp(lastName, "#") != 0) {
            fscanf(file, "%s %s", firstName, mobileNum);

            // allocate memory for a new tenant
            tenant *newTenant = (tenant *)malloc(sizeof(tenant));
            // copy tenant's information
            strcpy(newTenant->lastName, lastName);
            strcpy(newTenant->firstName, firstName);
            strcpy(newTenant->mobileNum, mobileNum);
            newTenant->prevTenant = NULL;
            newTenant->nextTenant = NULL;

            // add newTenant to the tenant list of this unit
            if (newUnit->tenants == NULL) {
                newUnit->tenants = newTenant;  // add newTenant as the head of the tenant list
            } else {
                // traverse the tenant list to find the last tenant
                tenant *temp = newUnit->tenants;
                while (temp->nextTenant != NULL) {
                    temp = temp->nextTenant;
                }
                // add newTenant after the last tenant
                temp->nextTenant = newTenant;
                newTenant->prevTenant = temp;
            }
        }
    }
    fclose(file);
}

void save_tenants(unit *headUnit) {
    FILE *file = fopen("tenant.txt", "w");
    if (file == NULL) {
        printf("[Error opening file for writing.\n]");
        return;
    }

    // traverse the list of units and save unit and tenant information to the file
    unit *init_unit = headUnit;
    while (init_unit != NULL) {
        // this write the unit information to the file
        fprintf(file, "%s %d %d\n", init_unit->unitName, init_unit->curr_occp, init_unit->max_occp);

        // traverse the list of tenants for this unit and write their information to the file
        tenant *init_tenant = init_unit->tenants;
        while (init_tenant != NULL) {
            fprintf(file, "%s %s %s\n", init_tenant->lastName, init_tenant->firstName, init_tenant->mobileNum);
            init_tenant = init_tenant->nextTenant;
        }

        // an indicator for the end of the tenant list for this unit
        fprintf(file, "#\n");

        // moving to the next unit in the list
        init_unit = init_unit->nextUnit;
    }

    fclose(file);
}

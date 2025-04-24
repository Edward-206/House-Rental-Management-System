#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROPERTIES 50
#define FILENAME "rentals.txt"

/* =============================================
   Data Structure (Shared by all members)
   ============================================= */
typedef struct {
    int id;
    char type[20]; // "House" or "Apartment"
    char address[100];
    float price;
    int bedrooms;
    bool isRented;
    char tenantName[50];
} Property;

Property properties[MAX_PROPERTIES];
int propertyCount = 0;

/* =============================================
   Member 4: File I/O Operations
   ============================================= */
void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    for (int i = 0; i < propertyCount; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d,%d,%s\n",
                properties[i].id,
                properties[i].type,
                properties[i].address,
                properties[i].price,
                properties[i].bedrooms,
                properties[i].isRented,
                properties[i].tenantName);
    }
    fclose(file);
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return;

    propertyCount = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%f,%d,%d,%[^\n]\n",
                  &properties[propertyCount].id,
                  properties[propertyCount].type,
                  properties[propertyCount].address,
                  &properties[propertyCount].price,
                  &properties[propertyCount].bedrooms,
                  &properties[propertyCount].isRented,
                  properties[propertyCount].tenantName) != EOF) {
        propertyCount++;
        if (propertyCount >= MAX_PROPERTIES) break;
    }
    fclose(file);
}

/* =============================================
   Member 2: Property Management
   ============================================= */
void addProperty() {
    if (propertyCount >= MAX_PROPERTIES) {
        printf("Cannot add more properties. Storage full!\n");
        return;
    }

    Property newProperty;

    printf("\nEnter Property ID: ");
    scanf("%d", &newProperty.id);

    printf("Enter Property Type (House/Apartment): ");
    scanf("%s", newProperty.type);

    printf("Enter Address: ");
    scanf(" %[^\n]", newProperty.address);

    printf("Enter Monthly Rent Price: ");
    scanf("%f", &newProperty.price);

    printf("Enter Number of Bedrooms: ");
    scanf("%d", &newProperty.bedrooms);

    newProperty.isRented = false;
    strcpy(newProperty.tenantName, "None");

    properties[propertyCount++] = newProperty;
    saveToFile();
    printf("\nProperty Added Successfully!\n");
}

void displayProperties() {
    if (propertyCount == 0) {
        printf("\nNo Properties Available!\n");
        return;
    }

    printf("\n=== Available Properties ===\n");
    for (int i = 0; i < propertyCount; i++) {
        printf("\nProperty ID: %d", properties[i].id);
        printf("\nType: %s", properties[i].type);
        printf("\nAddress: %s", properties[i].address);
        printf("\nPrice: $%.2f", properties[i].price);
        printf("\nBedrooms: %d", properties[i].bedrooms);
        printf("\nStatus: %s", properties[i].isRented ? "Rented" : "Available");
        printf("\nTenant: %s", properties[i].tenantName);
        printf("\n--------------------------\n");
    }
}

void searchProperty() {
    if (propertyCount == 0) {
        printf("\nNo Properties Available!\n");
        return;
    }

    int searchId;
    printf("\nEnter Property ID to Search: ");
    scanf("%d", &searchId);

    bool found = false;
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].id == searchId) {
            found = true;
            printf("\n=== Property Found ===\n");
            printf("ID: %d\nType: %s\nAddress: %s\nPrice: $%.2f\nBedrooms: %d\nStatus: %s\nTenant: %s\n",
                   properties[i].id, properties[i].type, properties[i].address,
                   properties[i].price, properties[i].bedrooms,
                   properties[i].isRented ? "Rented" : "Available",
                   properties[i].tenantName);
            break;
        }
    }

    if (!found) {
        printf("\nProperty Not Found!\n");
    }
}

/* =============================================
   Member 3: Rental & Editing Operations
   ============================================= */
void rentProperty() {
    if (propertyCount == 0) {
        printf("\nNo Properties Available!\n");
        return;
    }

    int rentId;
    printf("\nEnter Property ID to Rent: ");
    scanf("%d", &rentId);

    bool found = false;
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].id == rentId) {
            found = true;
            if (properties[i].isRented) {
                printf("\nProperty is Already Rented!\n");
            } else {
                printf("Enter Tenant Name: ");
                scanf(" %[^\n]", properties[i].tenantName);
                properties[i].isRented = true;
                saveToFile();
                printf("\nProperty Rented Successfully to %s!\n", properties[i].tenantName);
            }
            break;
        }
    }

    if (!found) {
        printf("\nProperty Not Found!\n");
    }
}

void editProperty() {
    if (propertyCount == 0) {
        printf("\nNo Properties Available!\n");
        return;
    }

    int editId;
    printf("\nEnter Property ID to Edit: ");
    scanf("%d", &editId);

    bool found = false;
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].id == editId) {
            found = true;
            if (properties[i].isRented) {
                printf("\nCannot edit a rented property!\n");
                return;
            }

            printf("\nCurrent Details:\n");
            printf("ID: %d\nType: %s\nAddress: %s\nPrice: $%.2f\nBedrooms: %d\n",
                   properties[i].id, properties[i].type, properties[i].address,
                   properties[i].price, properties[i].bedrooms);

            printf("\nEnter New Details:\n");
            printf("Property Type (House/Apartment): ");
            scanf("%s", properties[i].type);

            printf("Address: ");
            scanf(" %[^\n]", properties[i].address);

            printf("Monthly Rent Price: ");
            scanf("%f", &properties[i].price);

            printf("Number of Bedrooms: ");
            scanf("%d", &properties[i].bedrooms);

            saveToFile();
            printf("\nProperty Updated Successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("\nProperty Not Found!\n");
    }
}

/* =============================================
   Member 1: Main Program & Menu System
   ============================================= */
int main() {
    int choice;
    loadFromFile(); // Load data at startup

    printf("\n=== House Rental Management System ===\n");

    while (1) {
        printf("\n1. Add Property\n2. Display Properties\n3. Search Property\n");
        printf("4. Rent Property\n5. Edit Property\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProperty(); break;
            case 2: displayProperties(); break;
            case 3: searchProperty(); break;
            case 4: rentProperty(); break;
            case 5: editProperty(); break;
            case 6:
                saveToFile();
                printf("\nExiting the System. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid Choice! Try Again.\n");
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // For string formatting

// # COMMON section
int lenStr;
char tmpStr[20];
int choice;

void manageCandidates(); // In C we have to declare the function before calling, So this will help to avoid that (called function prototype)
int main();

// Capitalize the first letter of the string
void capitalize(char *str)
{
    // TODO: lowercase other letters other than first character
    // printf("DEBUG: Capitalize\n");

    if (str != NULL && strlen(str) > 0)
    {
        str[0] = toupper(str[0]);
    }

    return;
}

// Removes the trailing new line character '\n'
void removeNewLine(char *str)
{
    // printf("DEBUG: Remove New Line\n");

    str[strcspn(str, "\n")] = 0;

    // lenStr = strlen(str);

    // if (str != NULL && lenStr > 0 && str[lenStr - 1] == '\n')
    // {
    //     str[lenStr - 1] = '\0';
    // }

    return;
}

// display the names with the index
void displayNames(int number, char names[][20])
{

    if (number < 1)
    {
        printf("\nNo candidates to display\n");
        return;
    }

    for (int i = 0; i < number; i++)
    {
        printf("    [%d] %s\n", i + 1, names[i]);
    }

    return;
}

// # VOTERS section
void castVote()
{
    printf("DEBUG: Vote casted\n");
}

void voter()
{
    printf("DEBUG: Voter\n");
}

// ADMIN section
int numberOfCandidates = 0;  // To keep track of number of candidates
char candidateNames[10][20]; // Can add 10 candidates with 20 characters each

// remove candidates
void removeCandidate()
{
    int toDelete;

    printf("\nRemove Candidate\n");

    if (numberOfCandidates == 0)
    {
        printf("\nERROR - No candidates to remove\n");
        manageCandidates();
    }

    displayNames(numberOfCandidates, candidateNames);

    printf("[ ] Enter candidate ID to remove: ");

    if (scanf("%d", &toDelete) != 1)
    {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n')
            ; // Important to prevent infinite loops

        removeCandidate();

        return;
    }

    if (toDelete < 1 || toDelete > numberOfCandidates)
    {
        printf("\nERROR - Invalid input: Out of range.\n");

        removeCandidate();

        return;
    }

    toDelete--; // To match the index of the array

    for (int i = toDelete; i < numberOfCandidates - 1; i++)
    {
        strcpy(candidateNames[i], candidateNames[i + 1]);
    }

    numberOfCandidates--;

    printf("\nCandidate removed successfully\n");

    // removeCandidate();

    return;
}

// add candidates
void addCandidate()
{
    // printf("DEBUG: Add candidate\n");

    printf("\nAdd Candidate\n");

    if (numberOfCandidates >= 10) // Because we can only add 10 candidates
    {
        printf("\nERROR!!! Maximum number of candidates reached\n");
        manageCandidates();
    }

    char name[20];
    printf("Enter the name of the candidate: ");
    // scanf("%s", name);
    fgets(name, 20, stdin);

    removeNewLine(name);

    capitalize(name);

    strcpy(candidateNames[numberOfCandidates++], name);
    printf("Candidate \'%s\' added successfully\n");

    manageCandidates();
}

void manageCandidates()
{
    printf("\nManage Candidates\n    [1] View candidates\n    [2] Add candidate\n    [3] Remove candidate\n    [4] Go to main menu\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n')
            ; // Important to prevent infinite loops

        manageCandidates();

        return;
    }

    while (getchar() != '\n')
        ; // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 5)
    {
        printf("\nERROR - Invalid input: Out of range.\n");

        manageCandidates();

        return;
    }

    switch (choice)
    {
    case 1:
        printf("\nCandidates\n");
        displayNames(numberOfCandidates, candidateNames);
        manageCandidates();
        break;

    case 2:
        addCandidate();
        manageCandidates();
        break;

    case 3:
        removeCandidate();
        manageCandidates();
        break;

    case 4:
        // TODO: change this to where default options are shown
        main();
        manageCandidates();
        break;

    case 0:
        printf("\nExiting...\n");
        exit(0);
        break;
    }
}

void admin()
{
    printf("DEBUG: Admin\n");
}

int main()
{
    // addCandidate();
    // removeCandidate();
    manageCandidates();
    return 0;
}

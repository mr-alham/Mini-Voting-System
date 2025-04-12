#include <ctype.h>  // For string formatting
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// # COMMON section
int lenStr;
char tmpStr[20];
int choice;

#define maxNumOfCandidates 10  // To keep track of total number of candidates
#define maxNumOfVoters 40      // To keep track of total number of voters

// voter
int registeredVoters = 0;             // To keep track of number of voters
int voteCount[maxNumOfCandidates];    // To keep track of votes for each candidate
char voterNames[maxNumOfVoters][20];  // Can add 40 voters with 20 characters each

// candidate
int registeredCandidates = 0;                 // To keep track of number of candidates
char candidateNames[maxNumOfCandidates][20];  // Can add 10 candidates with 20 characters each
int numOfVotes[maxNumOfCandidates];           // To keep track of votes for each candidate

// In C we have to declare the function before calling, So this will help to avoid that (called function prototype)
int main();
void admin();
void mainMenu();
void viewResults();
void manageVoters();
void manageCandidates();

// Capitalize the first letter of the string
void capitalize(char *str) {
    // TODO: lowercase other letters other than first character

    if (str != NULL && strlen(str) > 0) {
        str[0] = toupper(str[0]);
    }

    return;
}

// Removes the trailing new line character '\n'
void removeNewLine(char *str) {
    str[strcspn(str, "\n")] = 0;

    return;
}

// display the names with the index
void displayNames(int number, char names[][20]) {
    if (number < 1) {
        printf("\n    - Not registered anyone yet!\n");
        return;
    }

    for (int i = 0; i < number; i++) {
        printf("    [%d] %s\n", i + 1, names[i]);
    }

    return;
}

// #VOTERS section
void castVote() {
    // printf("DEBUG: Vote casted\n");
    char studentID[20];

    printf("\nCast your Vote\n");
    printf("Enter your Student ID to proceed Or press enter to go back: ");
    fgets(studentID, sizeof(studentID), stdin);

    if (studentID[0] == '\n') {
        mainMenu();
        return;
    }

    removeNewLine(studentID);
    capitalize(studentID);

    // TODO: Check if the student ID is valid and registered
    // student ID should be unique for each voter
    // student ID format: StdXXX where XXX is a number eg: Std005
    // 005 means fifth name in the array in the voterNames

    // if the student ID is valid

    printf("\nCandidates\n");

    displayNames(registeredCandidates, candidateNames);

    printf("[ ] Enter candidate ID to vote Or press enter to go back: ");

    fgets(tmpStr, sizeof(tmpStr), stdin);

    // Exit if the user presses Enter without input
    if (tmpStr[0] == '\n') {
        mainMenu();
        return;
    }

    // Convert input to integer
    if (sscanf(tmpStr, "%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");
        castVote();
        return;
    }

    if (choice < 1 || choice > registeredCandidates) {
        printf("\nInvalid input: Out of range.\n");

        castVote();

        return;
    }

    while (getchar() != '\n');

    voteCount[choice - 1]++;

    printf("\n\'%s\', Voted successfully for \'%s\'", studentID, candidateNames[choice - 1]);
    printf("\nThank you for voting!\n");

    castVote();
}

// Voters panel: where the voters can vote
void voter() {
    // TODO: can directly use castVote() instead of this function
    // this will increase the complexity of the application
    // printf("DEBUG: Voter\n");
    printf("\nVoter's Panel\n    [1] Cast the Vote\n    [2] View Results\n    [3] Go to main menu\n    [0] Exit\n    [ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        voter();

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    switch (choice) {
        case 1:
            castVote();
            // voter();
            break;

        case 2:
            viewResults();
            // voter();
            castVote();
            break;

        case 3:
            main();
            // TODO: change this to where default options are shown
            break;

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;
        default:
            printf("\nInvalid input: Out of range.\n");
            // voter();
            break;
    }
}

// #ADMIN section

// remove candidates
void removeCandidate() {
    int toDelete;

    printf("\nRemove Candidate\n");

    if (registeredCandidates == 0) {
        printf("\nERROR - No candidates to remove\n");
        manageCandidates();
    }

    displayNames(registeredCandidates, candidateNames);

    printf("[ ] Enter candidate ID to remove: ");

    if (scanf("%d", &toDelete) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        removeCandidate();

        return;
    }

    if (toDelete < 1 || toDelete > registeredCandidates) {
        printf("\nERROR - Invalid input: Out of range.\n");

        removeCandidate();

        return;
    }

    toDelete--;  // To match the index of the array

    for (int i = toDelete; i < registeredCandidates - 1; i++) {
        strcpy(candidateNames[i], candidateNames[i + 1]);
    }

    registeredCandidates--;

    printf("\nCandidate removed successfully\n");

    // removeCandidate();

    return;
}

// add candidates
void addCandidate() {
    printf("\nAdd Candidate\n");

    if (registeredCandidates >= maxNumOfCandidates)  // Because we can only add 10 candidates
    {
        printf("\nERROR!!! Maximum number of candidates reached\n");
        manageCandidates();
    }

    char name[20];
    printf("Enter the name of the candidate or press enter to go back: ");
    // scanf("%s", name);
    fgets(name, 20, stdin);

    if (name[0] == '\n') {
        return;
    }

    removeNewLine(name);

    capitalize(name);

    strcpy(candidateNames[registeredCandidates++], name);

    printf("Candidate \'%s\' added successfully\n");

    // while (getchar() != '\n');
    if (strlen(name) == sizeof(name) - 1 && name[strlen(name) - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // Clear the input buffer
    }

    addCandidate();
}

// manage candidates
void manageCandidates() {
    printf("\nManage Candidates\n    [1] View candidates\n    [2] Add candidate\n    [3] Remove candidate\n    [4] Go to main menu\n    [5] Go back\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        manageCandidates();

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 5) {
        printf("\nERROR - Invalid input: Out of range.\n");

        manageCandidates();

        return;
    }

    switch (choice) {
        case 1:
            printf("\nCandidates\n");
            displayNames(registeredCandidates, candidateNames);
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
            // DONE: change this to where default options are shown
            // main();
            mainMenu();
            manageCandidates();
            break;
        case 5:
            admin();
            break;

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;
    }
}

// add voters
void addVoter() {
    printf("\nAdd Voters:");

    while (registeredVoters < maxNumOfVoters) {
        char name[20];
        printf("\nEnter the name of voter (or press Enter to stop): ");
        fgets(name, 20, stdin);

        // Exit the loop if the user presses Enter without input
        if (name[0] == '\n') {
            break;
        }

        removeNewLine(name);
        capitalize(name);

        strcpy(voterNames[registeredVoters++], name);
        printf("Voter \'%s\' added successfully\n");

        if (strlen(name) == sizeof(name) - 1 && name[strlen(name) - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Clear the input buffer
        }
    }

    if (registeredVoters >= maxNumOfVoters) {
        printf("\nERROR!!! Maximum number of voters reached\n");
    }

    manageVoters();
}

// remove voters
void removeVoter() {
    choice = 0;

    printf("\nRemove Voter\n");

    if (registeredVoters == 0) {
        printf("\nERROR - No voters to remove\n");
        manageVoters();
    }

    displayNames(registeredVoters, voterNames);

    printf("[ ] Enter voter ID to remove OR Press enter to go back: ");

    char input[10];
    fgets(input, sizeof(input), stdin);

    // Exit if the user presses Enter without input
    if (input[0] == '\n') {
        manageVoters();
        return;
    }

    // Convert input to integer
    if (sscanf(input, "%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");
        removeVoter();
        return;
    }

    if (choice < 1 || choice > registeredVoters) {
        printf("\nERROR - Invalid input: Out of range.\n");

        removeVoter();

        return;
    }

    choice--;  // To match the index of the array

    for (int i = choice; i < registeredVoters - 1; i++) {
        strcpy(voterNames[i], voterNames[i + 1]);
    }

    registeredVoters--;

    printf("\nVoter removed successfully\n");

    if (registeredVoters <= 0) {
        manageVoters();
    }

    removeVoter();

    return;
}

// in this function voters are managed by admin
void manageVoters() {
    printf("\nManage Voters\n    [1] View voters\n    [2] Add Voters\n    [3] Remove Voters\n    [4] Go to main menu\n    [5] Go Back\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        manageVoters();

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 5) {
        printf("\nERROR - Invalid input: Out of range.\n");

        manageVoters();

        return;
    }

    switch (choice) {
        case 1:
            printf("\nVoters:\n");
            displayNames(registeredVoters, voterNames);
            manageVoters();
            break;

        case 2:
            addVoter();
            manageVoters();
            break;

        case 3:
            removeVoter();
            manageVoters();
            break;

        case 4:
            // DONE: change this to where default options are shown
            // main();
            mainMenu();
            manageCandidates();
            break;

        case 5:
            admin();
            break;

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;
    }
}

// function to view the voting results
void viewResults() {
    // printf("\nDEBUG: View Results\n");
    printf("\nVoting Results:\n");

    if (registeredCandidates == 0) {
        printf("\nNo candidates are registered to show results.\nContact Admin\n");
        manageCandidates();
    }

    for (int i = 0; i < registeredCandidates; i++) {
        printf("    %s: %d\n", candidateNames[i], voteCount[i]);
    }
}

// reset votes
void clearVotes() {
}

// admin panel
void admin() {
    printf("\nAdmin Panel\n    [1] Manage Candidates\n    [2] Manage Voters\n    [3] View Results\n    [4] Clear Votes\n    [5] Go to main menu\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        // main();
        mainMenu();
        // DONE: change this to where default options are shown

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 6) {
        printf("\nERROR - Invalid input: Out of range.\n");

        // main();
        mainMenu();
        // DONE: change this to where default options are shown

        return;
    }

    switch (choice) {
        case 1:
            manageCandidates();
            // main();
            mainMenu();
            // DONE: change this to where default options are shown
            break;

        case 2:
            manageVoters();
            // main();
            mainMenu();
            // DONE: change this to where default options are shown

            break;

        case 3:
            viewResults();
            // main();
            mainMenu();
            // DONE: change this to where default options are shown

            break;

        case 4:
            clearVotes();
            // main();
            mainMenu();
            // DONE: change this to where default options are shown

            break;

        case 5:
            // main();
            mainMenu();
            // DONE: change this to where default options are shown

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;
    }
}

// function to show the help menu
void help() {
    // TODO: update the help menu
    printf("\nHelp Menu\n");
    printf("This is a mini voting system.\n");
    printf("You can vote for your favorite candidate.\n");
    printf("You can also manage candidates and voters as an admin.\n");
    printf("Press 0 to exit the program.\n");

    mainMenu();
}

// The main menu of the program
void mainMenu() {
    printf("\nMain Menu\n");
    printf("    [1] Vote\n    [2] Admin\n    [3] Help Menu\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\nERROR - Invalid input: Not an integer.\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        mainMenu();

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 4) {
        printf("\nERROR - Invalid input: Out of range.\n");

        mainMenu();

        return;
    }

    switch (choice) {
        case 1:
            if (registeredVoters <= 0) {
                printf("\nNo registered voters to vote for \nContact admin\n");
                mainMenu();
            } else if (registeredCandidates <= 0) {
                printf("\nNo registered candidates to vote for \nContact admin\n");
                mainMenu();
            }

            // voter();
            castVote();
            break;

        case 2:
            admin();
            break;

        case 3:
            help();
            break;

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;
    }
}

int main() {
    // printf("\nMini Voting System\n");
    printf("Welcome to the Mini Voting System");
    // addCandidate();
    // removeCandidate();
    // manageCandidates();
    // manageVoters();
    // admin();
    // voter();
    mainMenu();

    // addVoter();
    // removeVoter();
    return 0;
}

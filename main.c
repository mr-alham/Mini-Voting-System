#include <ctype.h>  // For string formatting
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// # COMMON section

// File paths for saving and loading data
#define VOTES_FILE "votes.txt"
#define VOTERS_FILE "voters.txt"
#define CANDIDATES_FILE "candidates.txt"
#define HAS_VOTED_FILE "hasVoted.txt"

int choice;
int lenStr;
char tmpStr[20];

#define maxNumOfVoters 40      // To keep track of total number of voters
#define maxNumOfCandidates 10  // To keep track of total number of candidates

// voter
int doContinue = 0;                   // To keep track of the voting process
int registeredVoters = 0;             // To keep track of number of voters
int voteCount[maxNumOfCandidates];    // To keep track of votes for each candidate
int hasVoted[maxNumOfVoters] = {0};   // Array to track if a voter has voted
char voterNames[maxNumOfVoters][20];  // Can add 40 voters with 20 characters each

// candidate
int registeredCandidates = 0;                 // To keep track of number of candidates
int numOfVotes[maxNumOfCandidates];           // To keep track of votes for each candidate
char candidateNames[maxNumOfCandidates][20];  // Can add 10 candidates with 20 characters each

// In C we have to declare the function before calling, So this will help to avoid that (called function prototype)
int main();
void admin();
void mainMenu();
void viewResults();
void manageVoters();
void manageCandidates();

// Some common functions

// Removes the trailing new line character '\n'
void removeNewLine(char *str) {
    str[strcspn(str, "\n")] = 0;

    return;
}

// Function to save data to files
void saveData() {
    // Save candidates
    FILE *file = fopen(CANDIDATES_FILE, "w");
    if (file) {
        for (int i = 0; i < registeredCandidates; i++) {
            fprintf(file, "%s\n", candidateNames[i]);
        }
        fclose(file);
    }

    // Save voters
    file = fopen(VOTERS_FILE, "w");
    if (file) {
        for (int i = 0; i < registeredVoters; i++) {
            fprintf(file, "%s\n", voterNames[i]);
        }
        fclose(file);
    }

    // Save votes
    file = fopen(VOTES_FILE, "w");
    if (file) {
        for (int i = 0; i < registeredCandidates; i++) {
            fprintf(file, "%d\n", voteCount[i]);
        }
        fclose(file);
    }

    // Save hasVoted array
    file = fopen(HAS_VOTED_FILE, "w");
    if (file) {
        for (int i = 0; i < registeredVoters; i++) {
            fprintf(file, "%d\n", hasVoted[i]);
        }
        fclose(file);
    }
}

// Function to load data from files
void loadData() {
    // Load candidates
    FILE *file = fopen(CANDIDATES_FILE, "r");
    if (file) {
        registeredCandidates = 0;
        while (fgets(candidateNames[registeredCandidates], sizeof(candidateNames[registeredCandidates]), file)) {
            removeNewLine(candidateNames[registeredCandidates]);
            registeredCandidates++;
        }
        fclose(file);
    } else {
        printf("\n!!! Error: Could not open candidates file. !!!\n");
    }

    // Load voters
    file = fopen(VOTERS_FILE, "r");
    if (file) {
        registeredVoters = 0;
        while (fgets(voterNames[registeredVoters], sizeof(voterNames[registeredVoters]), file)) {
            removeNewLine(voterNames[registeredVoters]);
            registeredVoters++;
        }
        fclose(file);
    } else {
        printf("\n!!! Error: Could not open voters file. !!!\n");
    }

    // Load votes
    file = fopen(VOTES_FILE, "r");
    memset(voteCount, 0, sizeof(voteCount));  // Reset before loading
    if (file) {
        for (int i = 0; i < registeredCandidates; i++) {
            if (fscanf(file, "%d", &voteCount[i]) != 1) {
                printf("\n!!! Warning: Incomplete or corrupted votes data. !!!\n");
                break;
            }
        }
        fclose(file);
    } else {
        printf("\n!!! Error: Could not open votes file. !!!\n");
    }

    // Load hasVoted array
    file = fopen(HAS_VOTED_FILE, "r");
    memset(hasVoted, 0, sizeof(hasVoted));  // Reset before loading
    if (file) {
        for (int i = 0; i < registeredVoters; i++) {
            if (fscanf(file, "%d", &hasVoted[i]) != 1) {
                printf("\n!!! Warning: Incomplete or corrupted hasVoted data. !!!\n");
                break;
            }
        }
        fclose(file);
    } else {
        printf("\n!!! Error: Could not open hasVoted file. !!!\n");
    }
}

// Capitalize the first letter of the string
void capitalize(char *str) {
    if (str != NULL && strlen(str) > 0) {
        str[0] = toupper(str[0]);  // Convert the first character to uppercase

        for (int i = 1; str[i] != '\0'; i++) {
            str[i] = tolower(str[i]);  // Convert the rest to lowercase
        }
    }

    return;
}

// display the names with the index
void displayNames(int number, char names[][20]) {
    if (number < 1) {
        printf("    [ ] !!! No registered entries !!!\n");
        return;
    }

    for (int i = 0; i < number; i++) {
        printf("    [%d] %s\n", i + 1, names[i]);
    }

    return;
}

// #VOTERS section
// This function handles the voting process
// It allows the user to cast their vote for a candidate
// It checks if the user is registered and if they have already voted
// It also handles invalid inputs and prompts the user to try again
void castVote() {
    char studentID[20];
    int voterIndex;

    if (doContinue == 0) {
        printf("\n[~] Proceed to Vote\n");
        printf("[ ] Enter your Student ID Or press enter to go back: ");

        fgets(studentID, sizeof(studentID), stdin);

        if (studentID[0] == '\n') {
            mainMenu();
            return;
        }

        removeNewLine(studentID);
        capitalize(studentID);

        // Check if the student ID is valid and registered
        if (strlen(studentID) != 6 || strncmp(studentID, "Std", 3) != 0 || !isdigit(studentID[3]) || !isdigit(studentID[4]) || !isdigit(studentID[5])) {
            printf("\n!!! Invalid Student ID format; The format is 'StdXXX'. !!!\n");

            castVote();

            return;
        }

        voterIndex = atoi(&studentID[3]) - 1;  // Extract the number part and convert to index

        if (voterIndex < 0 || voterIndex >= registeredVoters) {
            printf("\n!!! Student ID not registered. !!!\n");

            castVote();

            return;
        }

        // Check if the voter has already voted
        if (hasVoted[voterIndex] == 1) {
            printf("\n!!! You have already voted; Voting is allowed only once. !!!\n");
            mainMenu();
            return;
        }
    }

    doContinue = 1;

    printf("\n~ Hello! %s, Vote for your favorite candidate.\n", voterNames[voterIndex]);

    printf("\n[~] Registered Candidates\n");

    displayNames(registeredCandidates, candidateNames);

    printf("[ ] Candidate ID Or press enter to go back: ");

    fgets(tmpStr, sizeof(tmpStr), stdin);

    // Clear the input buffer to avoid leftover characters
    if (tmpStr[strlen(tmpStr) - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // Exit if the user presses Enter without input
    if (tmpStr[0] == '\n') {
        mainMenu();
        return;
    }

    // Convert input to integer
    if (sscanf(tmpStr, "%d", &choice) != 1) {
        printf("\n!!! Invalid input: Not an integer. !!!\n");
        castVote();
        return;
    }

    if (choice < 1 || choice > registeredCandidates) {
        printf("\n!!! Invalid input: Out of range. !!!\n");

        castVote();

        return;
    }

    while (getchar() != '\n' && getchar() != EOF);

    if (choice - 1 >= 0 && choice - 1 < maxNumOfCandidates) {
        voteCount[choice - 1]++;
    } else {
        printf("\n!!! Error: Invalid candidate index. !!!\n");
        castVote();
        return;
    }
    hasVoted[voterIndex] = 1;  // Mark the voter as having voted

    doContinue = 0;

    printf("~ Thank your valuable Vote, You voted successfully!\n");

    saveData();

    mainMenu();
}

// #ADMIN section

// remove candidates
void removeCandidate() {
    int toDelete;

    printf("\n[~] Remove Candidate\n");

    if (registeredCandidates == 0) {
        printf("\n!!! No candidates to remove !!!\n");
        manageCandidates();
        return;
    }

    displayNames(registeredCandidates, candidateNames);

    printf("[ ] Enter candidate ID to remove or press Enter to go back: ");

    char input[10];
    fgets(input, sizeof(input), stdin);

    // Exit if the user presses Enter without input
    if (input[0] == '\n') {
        manageCandidates();
        return;
    }

    // Convert input to integer
    if (sscanf(input, "%d", &toDelete) != 1) {
        printf("\n!!! Invalid input; Not an integer. !!!\n");
        removeCandidate();
        return;
    }

    if (toDelete < 1 || toDelete > registeredCandidates) {
        printf("\n!!! Invalid input; Out of range. !!!\n");
        removeCandidate();
        return;
    }

    toDelete--;  // To match the index of the array

    for (int i = toDelete; i < registeredCandidates - 1; i++) {
        // Reset the hasVoted array
        for (int i = 0; i < registeredVoters; i++) {
            hasVoted[i] = 0;
        }

        saveData();  // Save the updated data to the file

        candidateNames[i][sizeof(candidateNames[i]) - 1] = '\0';  // Ensure null-termination
    }

    registeredCandidates--;

    saveData();  // Save the updated data to the file

    printf("\n ~ Successfully removed candidate.\n");

    return;
}

// add candidates
void addCandidate() {
    printf("\n[~] Register a new Candidate\n");

    if (registeredCandidates >= maxNumOfCandidates)  // Because we can only add 10 candidates
    {
        printf("\n!!! Maximum number of candidates reached. !!!\n");
        manageCandidates();
    }

    char name[20];
    printf("[ ] Enter the name of the candidate or press enter to go back: ");

    fgets(name, 20, stdin);

    if (name[0] == '\n') {
        return;
    }

    removeNewLine(name);

    capitalize(name);

    strcpy(candidateNames[registeredCandidates++], name);

    saveData();  // Save the updated data to the file

    printf("\n~ Candidate \'%s\' added successfully\n", name);

    // while (getchar() != '\n');
    if (strlen(name) == 19 && name[strlen(name) - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // Clear the input buffer
    }

    addCandidate();
}

// manage candidates
void manageCandidates() {
    printf("\n[~] Manage Candidates\n    [1] View candidates\n    [2] Register new candidate\n    [3] Remove candidate\n    [4] Go to main menu\n    [5] Go back\n    [0] Exit\n[ ] Enter your choice: ");

    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL || sscanf(input, "%d", &choice) != 1) {
        printf("\n!!! Invalid input; Not an integer. !!!\n");

        // while (getchar() != '\n');  // Important to prevent infinite loops

        manageCandidates();

        return;
    }

    // while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 5) {
        printf("\n!!! Invalid input; Out of range. !!!\n");

        manageCandidates();

        return;
    }

    switch (choice) {
        case 1:
            printf("\n[~] Registered Candidates\n");
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
            mainMenu();
            manageCandidates();
            break;
        case 5:
            admin();
            break;

        case 0:
            printf("\n!!! Exiting... !!!\n");
            exit(0);
            break;
    }
}

// add voters
void addVoter() {
    printf("\n[~] Register new Voters:");

    while (registeredVoters < maxNumOfVoters) {
        char name[20];
        printf("\n[ ] Enter the name Or press enter to go back: ");
        fgets(name, 20, stdin);

        // Exit the loop if the user presses Enter without input
        if (name[0] == '\n') {
            break;
        }

        removeNewLine(name);
        capitalize(name);

        strcpy(voterNames[registeredVoters++], name);

        saveData();  // Save the updated data to the file

        printf("\n~ Successfully registered voter, \'%s\'.\n", name);

        if (strlen(name) == sizeof(name) - 1 && name[strlen(name) - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Clear the input buffer
        }
    }

    if (registeredVoters >= maxNumOfVoters) {
        printf("\n!!! Maximum number of voters reached !!!\n");
    }

    manageVoters();
}

// remove voters
void removeVoter() {
    choice = 0;

    printf("\n[~] Remove Voter\n");

    if (registeredVoters == 0) {
        printf("\n!!!No registered voters to remove !!!\n");

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
        printf("\n!!! Invalid input; Not an integer. !!!\n");
        removeVoter();
        return;
    }

    if (choice < 1 || choice > registeredVoters) {
        printf("\n!!! Invalid input; Out of range. !!!\n");

        removeVoter();

        return;
    }

    choice--;  // To match the index of the array

    for (int i = choice; i < registeredVoters - 1; i++) {
        strncpy(voterNames[i], voterNames[i + 1], sizeof(voterNames[i]));
        voterNames[i][sizeof(voterNames[i]) - 1] = '\0';  // Ensure null termination
    }

    registeredVoters--;

    saveData();  // Save the updated data to the file

    printf("\n~ Successfully removed the registered voter.\n");

    if (registeredVoters <= 0) {
        manageVoters();
    }

    removeVoter();

    return;
}

// in this function voters are managed by admin
void manageVoters() {
    printf("\n[~] Manage Voters\n    [1] View voters\n    [2] Add Voters\n    [3] Remove Voters\n    [4] Go to main menu\n    [5] Go Back\n    [0] Exit\n[ ] Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("\n!!! Invalid input; Not an integer. !!!\n");

        while (getchar() != '\n');  // Important to prevent infinite loops

        manageVoters();

        return;
    }

    while (getchar() != '\n');  // Clear the input buffer to avoid issues with fgets

    if (choice < 0 || choice > 5) {
        printf("\n!!! Invalid input; Out of range. !!!\n");

        manageVoters();

        return;
    }

    switch (choice) {
        case 1:
            printf("\n[~] Registered Voters:\n");
            displayNames(registeredVoters, voterNames);

            printf("... Press Enter to continue ...\n");

            getchar();  // Wait for user feedback
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
            mainMenu();
            manageCandidates();
            break;

        case 5:
            admin();
            break;

        case 0:
            printf("\n!!! Exiting... !!!\n");
            exit(0);
            break;
    }
}

// function to view the voting results
void viewResults() {
    printf("\n[~] Voting Results:\n");

    if (registeredCandidates == 0) {
        printf("\n!!! No registered candidates. !!!\n");
        printf("... Press Enter to continue ...\n");

        getchar();  // Wait for user feedback

        manageCandidates();
    }

    for (int i = 0; i < registeredCandidates; i++) {
        printf("    %d. %s:- %d\n", i + 1, candidateNames[i], voteCount[i]);
    }

    printf("\n... Press Enter to continue ...\n");

    getchar();  // Wait for user feedback

    admin();
}

// reset votes
void clearVotes() {
    printf("\nClearing Votes:\n");

    if (registeredCandidates == 0) {
        // printf("\nNo registered candidates.\n");
        admin();
    }

    for (int i = 0; i < registeredCandidates; i++) {
        voteCount[i] = 0;
    }

    saveData();  // Save the updated data to the file

    printf("\n~ Successfully reset votes.\n");
    printf("... Press Enter to continue ...\n");

    getchar();  // Wait for user feedback

    admin();
}

// admin panel
void admin() {
    printf("\n\n[~] Admin Panel\n    [1] Manage Candidates\n    [2] Manage Voters\n    [3] View Results\n    [4] Clear Votes\n    [5] Go to main menu\n    [0] Exit\n[ ] Enter your choice: ");

    char input[10];

    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d", &choice) != 1) {
        printf("\n!!! Invalid input; Not an integer. !!!\n");
        printf("... Press Enter to continue ...\n");

        getchar();  // Wait for user feedback

        admin();

        return;
    }

    // Remove the extra getchar() call to avoid requiring an additional Enter press
    if (choice < 0 || choice > 5) {
        printf("\n!!! Invalid input; Out of range. !!!\n");
        printf("... Press Enter to continue ...\n");

        getchar();  // Wait for user feedback

        admin();

        return;
    }

    switch (choice) {
        case 1:
            manageCandidates();
            mainMenu();
            break;

        case 2:
            manageVoters();
            mainMenu();
            break;

        case 3:
            viewResults();
            mainMenu();
            break;

        case 4:
            clearVotes();
            mainMenu();
            break;

        case 5:
            mainMenu();
            break;

        case 0:
            printf("\n!!! Exiting... !!!\n");
            exit(0);
            break;
    }
}

// function to show the help menu
void help() {
    printf("\n\n=================================\n");
    printf(" Help Menu - Mini Voting System\n");
    printf("=================================\n");

    printf("\nOptions:\n");
    printf("  [1] Vote\n");
    printf("      - Cast your vote for a registered candidate.\n");
    printf("  [2] Admin\n");
    printf("      - Access the admin panel to manage candidates and voters.\n");
    printf("  [3] Help Menu\n");
    printf("      - Display this help menu.\n");
    printf("  [0] Exit\n");
    printf("      - Exit the program.\n");

    printf("\nDescription:\n");
    printf("  This is a mini voting system where users can vote for their favorite candidates.\n");
    printf("  Admins can manage candidates and voters, view results, and reset votes.\n");

    printf("\nExamples:\n");
    printf("  - To vote, select option [1] from the main menu.\n");

    printf("  - This is a group project written by Alham, Thahida, Nasweer and Safra\n");
    printf("\n\n... Press Enter to continue ...\n");

    getchar();  // Wait for user feedback

    mainMenu();
}

// The main menu of the program
void mainMenu() {
    printf("\n[~] Main Menu\n");
    printf("    [1] Vote\n    [2] Admin\n    [3] Help Menu\n    [0] Exit\n[ ] Enter your choice: ");

    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL || sscanf(input, "%d", &choice) != 1) {
        printf("\n!!! Invalid input: Not an integer. !!!\n");
        printf("... Press Enter to continue ...\n");

        getchar();  // Wait for user feedback

        mainMenu();

        return;
    }

    switch (choice) {
        case 1:
            if (registeredVoters <= 0) {
                printf("\n!!! No registered voters; Contact admin !!!\n");

                mainMenu();

            } else if (registeredCandidates <= 0) {
                printf("\n!!! No registered candidates; Contact admin !!!\n");
                printf("... Press Enter to continue ...\n");

                getchar();  // Wait for user feedback

                mainMenu();
            }

            castVote();
            break;

        case 2:
            admin();
            break;

        case 3:
            help();
            break;

        case 0:
            printf("\n!!! Exiting... !!!\n");
            exit(0);
            break;
    }
}

int main() {
    loadData();  // Load data from files at the start

    printf("\n=====================\n");
    printf("  Mini Voting System");
    printf("\n=====================");

    mainMenu();

    return 0;
}

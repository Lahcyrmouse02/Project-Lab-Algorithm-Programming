#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void displayHangman(int tries) {
    switch (tries) {
        case 6:
            printf("\n  -----\n  |   |\n      |\n      |\n      |\n      |\n=========");
            break;
        case 5:
            printf("\n  -----\n  |   |\n  O   |\n      |\n      |\n      |\n=========");
            break;
        case 4:
            printf("\n  -----\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========");
            break;
        case 3:
            printf("\n  -----\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========");
            break;
        case 2:
            printf("\n  -----\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========");
            break;
        case 1:
            printf("\n  -----\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========");
            break;
        case 0:
            printf("\n  -----\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========");
            break;
        default:
            break;
    }
}

void displayWord(char word[], char guessedLetters[], int wordLength) {
    for (int i = 0; i < wordLength; i++) {
        printf("%c ", guessedLetters[i]);
    }
    printf("\n");
}

int isLetterInWord(char letter, char word[], int wordLength) {
    for (int i = 0; i < wordLength; i++) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

void updateScore(const char* playerName, int score) {
    FILE* file = fopen("Players.txt", "r+");
    if (!file) {
        file = fopen("Players.txt", "w"); // Create the file if it doesn't exist
        if (!file) {
            printf("Error opening file!\n");
            return;
        }
    }

    char players[100][50];
    int scores[100];
    int count = 0;
    int found = 0;

    // Read players and scores from file
    while (fscanf(file, "%49s %d", players[count], &scores[count]) == 2) {
        if (strcmp(players[count], playerName) == 0) {
            scores[count] += score;
            found = 1;
        }
        count++;
    }

    if (!found) {
        strcpy(players[count], playerName);
        scores[count] = score;
        count++;
    }

    // Rewrite the file with updated scores
    freopen("Players.txt", "w", file);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", players[i], scores[i]);
    }

    fclose(file);
}

void playGame() {
    char* words[] = {"apple", "orange", "watermelon", "dragonfruit", "grape"};
    int numWords = sizeof(words) / sizeof(words[0]);
    srand(time(NULL));
    int wordIndex = rand() % numWords;
    char* word = words[wordIndex];
    char guess;
    int i;
    int correctGuesses = 0;
    int wordLength = strlen(word);
    char guessed[wordLength + 1];
    int attempts = 6;
    int alreadyGuessed[256] = {0}; // Array to keep track of guessed letters

    // Initialize guessed array with underscores
    for (i = 0; i < wordLength; i++) {
        guessed[i] = '_';
    }
    guessed[wordLength] = '\0';  // Null-terminate the string

    printf("\nWelcome to Hangman!\n");

    while (attempts > 0 && correctGuesses < wordLength) {
        printf("\nWord: %s\n", guessed);
        printf("Attempts remaining: %d\n", attempts);
        displayHangman(attempts);
        printf("Enter your guess: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        // Check if the letter has already been guessed
        if (alreadyGuessed[(int)guess]) {
            printf("You've already guessed '%c'. Try a different letter.\n", guess);
            continue;
        }

        alreadyGuessed[(int)guess] = 1; // Mark this letter as guessed

        int found = 0;
        for (i = 0; i < wordLength; i++) {
            if (tolower(word[i]) == guess) {
                if (guessed[i] == '_') {
                    guessed[i] = word[i];
                    correctGuesses++;
                    found = 1;
                }
            }
        }

        if (!found) {
            attempts--;
            printf("Wrong guess!\n");
        }
    }

    char playerName[50];
    printf("Enter your name: ");
    scanf(" %49[^\n]", playerName);

    if (correctGuesses == wordLength) {
        printf("\nCongratulations! You've guessed the word: %s\n", word);
        printf("You earned 100 points!\n");
        updateScore(playerName, 100);
    } else {
        displayHangman(attempts);
        printf("\nGame Over! The word was: %s\n", word);
        printf("You earned 0 points.\n");
        updateScore(playerName, 0);
    }

    printf("\nYour current score has been updated.\n");
}

void newPlayer() {
    FILE *file = fopen("Players.txt", "a"); 
    if (file == NULL) {                    
        printf("Error opening file!\n");
        return;
    }

    char playerName[50];
    printf("Enter player name: ");
    scanf(" %49[^\n]", playerName);        

    fprintf(file, "%s 0\n", playerName);     
    fclose(file);
    printf("Player '%s' has been added and saved to Players.txt!\n", playerName);
}

void removePlayer() {
    FILE* file = fopen("Players.txt", "r");
    if (file == NULL) {
        printf("Error opening file or no players found!\n");
        return;
    }

    char players[100][50];
    int scores[100];
    int count = 0;

    // Read players and scores from file
    while (fscanf(file, "%49s %d", players[count], &scores[count]) == 2) {
        count++;
    }
    fclose(file);

    char playerName[50];
    printf("Enter the player name to remove: ");
    scanf(" %49[^\n]", playerName);

    file = fopen("Players.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(players[i], playerName) != 0) {
            fprintf(file, "%s %d\n", players[i], scores[i]);
        } else {
            found = 1;
        }
    }
    fclose(file);

    if (found) {
        printf("Player '%s' has been removed.\n", playerName);
    } else {
        printf("Player '%s' not found.\n", playerName);
    }
}

void scoreboard() {
    FILE *file = fopen("Players.txt", "r"); // Open file in read mode
    if (file == NULL) {
        printf("Error opening file or no players found!\n");
        return;
    }

    char players[100][50]; // Array to store player names
    int scores[100] = {0}; // Array to store scores
    int count = 0;

    // Read players and scores from file
    while (fscanf(file, "%49s %d", players[count], &scores[count]) == 2) {
        count++;
    }
    fclose(file);

    // Sort players alphabetically using qsort
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(players[i], players[j]) > 0) {
                char tempName[50];
                int tempScore;

                strcpy(tempName, players[i]);
                tempScore = scores[i];

                strcpy(players[i], players[j]);
                scores[i] = scores[j];

                strcpy(players[j], tempName);
                scores[j] = tempScore;
            }
        }
    }

    // Print the sorted scoreboard
    printf("\nScoreboard:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s %d\n", i + 1, players[i], scores[i]);
    }

    // Search feature
    char searchName[50];
    printf("\nDo you want to search for a player's score? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Enter the player's name: ");
        scanf(" %49[^\n]", searchName);

        // Search for the player
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(players[i], searchName) == 0) {
                printf("\nPlayer found: %s, Score: %d\n", players[i], scores[i]);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("\nPlayer '%s' not found in the scoreboard.\n", searchName);
        }
    }
}

int main() {
    int choice;

    do {
        printf("--- Hangman Menu ---\n");
        printf("1. New Player\n");
        printf("2. Play Game\n");
        printf("3. Scoreboard\n");
        printf("4. Remove Player\n");
        printf("5. Exit\n");
        printf("--------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                newPlayer();
                break;
            case 2:
                playGame();
                break;
            case 3:
                scoreboard();
                break;
            case 4:
                removePlayer();
                break;
            case 5:
                printf("Exiting the game. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    return 0;
}


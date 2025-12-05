// Core game logic and functions
#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "BST.h"
#include "Display.h"

using namespace std;

// External references to global variables (defined in main.cpp)
extern DoublyLinkedList currentGrid;
extern DoublyLinkedList targetGrid;
extern DoublyLinkedList savedGrid;
extern Stack moveHistory;
extern BST leaderboard;
extern int gridSize;
extern int emptyRow, emptyCol;
extern int moves;
extern int currentTheme;
extern string themes[5];
extern string emojis[5][25];

void clearScreen() {
    system("cls");
}

// Move cursor up and clear previous line
void clearPreviousLine() {
    cout << "\033[A\033[2K\r";
    cout << "\033[A\033[2K\r";
}

// Randomize array order using Fisher-Yates shuffle algorithm
void shuffleEmojis(string arr[], int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        string temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// GRID MANAGEMENT FUNCTIONS

// Initialize the game grid with emojis
// newPattern: true = generate new puzzle, false = reuse saved puzzle (for retry)
void initializeGrid(bool newPattern) {
    currentGrid.clear();
    targetGrid.clear();

    if (newPattern) {
        // Select random theme based on grid size
        // 3x3: all themes available
        // 4x4/5x5: exclude Moon Phases theme (not enough emojis)
        if (gridSize == 3) {
            currentTheme = rand() % 5;
        } else {
            int themeOptions[] = {0, 1, 3, 4}; // Emotion, Fruits, Foods, Animals
            currentTheme = themeOptions[rand() % 4];
        }

        // Select required number of emojis
        int needed = gridSize * gridSize - 1;  // -1 for empty space
        string selected[25];
        for (int i = 0; i < needed; i++) {
            selected[i] = emojis[currentTheme][i];
        }
        selected[needed] = "";  // Empty space

        // Randomize emoji positions
        shuffleEmojis(selected, needed);

        // Store initial pattern in all three grids
        savedGrid.clear();
        int index = 0;
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                targetGrid.insert(i, j, selected[index]);
                savedGrid.insert(i, j, selected[index]);
                currentGrid.insert(i, j, selected[index]);

                // Track empty space position
                if (selected[index] == "") {
                    emptyRow = i;
                    emptyCol = j;
                }
                index++;
            }
        }
    } else {
        // Reuse saved pattern for retry
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                string emoji = savedGrid.getEmoji(i, j);
                targetGrid.insert(i, j, emoji);
                currentGrid.insert(i, j, emoji);

                if (emoji == "") {
                    emptyRow = i;
                    emptyCol = j;
                }
            }
        }
    }
}

// Shuffle the grid using random valid moves
// This ensures the puzzle is always solvable (every shuffle move can be reversed)
void shuffleGrid() {
    int shuffles = gridSize * gridSize * 20;  // More shuffles for larger grids

    for (int i = 0; i < shuffles; i++) {
        int dir = rand() % 4;
        int newRow = emptyRow, newCol = emptyCol;

        // Try to move in random direction
        if (dir == 0 && emptyRow > 0) newRow--;                     // Up
        else if (dir == 1 && emptyRow < gridSize - 1) newRow++;     // Down
        else if (dir == 2 && emptyCol > 0) newCol--;                // Left
        else if (dir == 3 && emptyCol < gridSize - 1) newCol++;     // Right

        // Swap empty space with adjacent emoji
        string emptyEmoji = currentGrid.getEmoji(emptyRow, emptyCol);
        string newEmoji = currentGrid.getEmoji(newRow, newCol);

        currentGrid.setEmoji(emptyRow, emptyCol, newEmoji);
        currentGrid.setEmoji(newRow, newCol, emptyEmoji);

        emptyRow = newRow;
        emptyCol = newCol;
    }
}

// DISPLAY FUNCTIONS

// Display the current game state
// Shows: target pattern (top), control instructions, current grid (bottom)
void displayGrid() {
    clearScreen();
    displayGameHeader();
    displayDifficultyHeader(gridSize);
    
    cout << " Theme: " << themes[currentTheme] << "\n";
    cout << " Moves: " << moves;

    // Display target pattern (what player needs to match)
    cout << "\n\n\033[0m";    
    for (int i = 0; i < gridSize; i++) {
        if (gridSize == 3) {
            cout << "\t\t\t\t";
        } else if (gridSize == 4) {
            cout << "\t\t\t       ";
        } else if (gridSize == 5) {
            cout << "\t\t\t     ";
        }

        for (int j = 0; j < gridSize; j++) {
            string emoji = targetGrid.getEmoji(i, j);
            
            if (emoji == "") {
                cout << "   ";
            } else {
                cout << emoji << " ";
            }
        }
        cout << "\n";
    }

    setDifficultyColor(gridSize);

    // Display current puzzle state (with boxes)
    for (int i = 0; i < gridSize; i++) {
        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            cout << "┌────┐ ";
        }

        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            string emoji = currentGrid.getEmoji(i, j);
            
            if (emoji == "") {
                cout << "│" << left << setw(4) << "" << "│ ";
            } else {
                cout << "│ " << left << setw(2) << emoji << " │ ";
            }
        }

        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            cout << "└────┘ ";
        }
    }
    cout << "\n";
}

// Display leaderboard screen
void displayLeaderboard() {
    clearScreen();
    displayLeaderboardHeader();

    if (leaderboard.isEmpty()) {
        displayEmptyLeaderboard();
    } else {
        displayLeaderboardTableHeader();
        leaderboard.display();
        displayLeaderboardTableFooter();
    }

    cout << endl << endl;
    cout << "                            Press any key to return to menu.\n";                       
    _getch();
}

// GAME LOGIC FUNCTIONS

// Check if puzzle is solved
// Compares current grid with target pattern
bool isSolved() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (currentGrid.getEmoji(i, j) != targetGrid.getEmoji(i, j)) {
                return false;
            }
        }
    }
    return true;
}

// Execute a move in the puzzle
// direction: Arrow key code from _getch() (72=Up, 80=Down, 75=Left, 77=Right)
// Swaps empty space with adjacent emoji if move is valid
void makeMove(char direction) {
    int newRow = emptyRow, newCol = emptyCol;

    // Calculate new empty position based on arrow key
    if (direction == 72 && emptyRow < gridSize - 1) newRow++;      // Up arrow
    else if (direction == 80 && emptyRow > 0) newRow--;            // Down arrow
    else if (direction == 75 && emptyCol < gridSize - 1) newCol++; // Left arrow
    else if (direction == 77 && emptyCol > 0) newCol--;            // Right arrow
    else return;  // Invalid move

    // Swap empty space with target emoji
    string emptyEmoji = currentGrid.getEmoji(emptyRow, emptyCol);
    string newEmoji = currentGrid.getEmoji(newRow, newCol);

    currentGrid.setEmoji(emptyRow, emptyCol, newEmoji);
    currentGrid.setEmoji(newRow, newCol, emptyEmoji);

    // Save move to history for undo functionality
    moveHistory.push(direction, ++moves);

    emptyRow = newRow;
    emptyCol = newCol;
}

// Undo the last move
// Pops direction from stack and reverses the move
void undoMove() {
    if (moveHistory.isEmpty()) return;

    char lastDirection = moveHistory.pop();
    moves--;

    int newRow = emptyRow, newCol = emptyCol;

    // Reverse the last move
    if (lastDirection == 72) newRow--;
    else if (lastDirection == 80) newRow++;
    else if (lastDirection == 75) newCol--;
    else if (lastDirection == 77) newCol++;

    // Swap back
    string emptyEmoji = currentGrid.getEmoji(emptyRow, emptyCol);
    string newEmoji = currentGrid.getEmoji(newRow, newCol);

    currentGrid.setEmoji(emptyRow, emptyCol, newEmoji);
    currentGrid.setEmoji(newRow, newCol, emptyEmoji);

    emptyRow = newRow;
    emptyCol = newCol;
}

// UI SCREEN FUNCTIONS

// Display win screen and get player's choice
char showWinScreen() {
    clearScreen();
    logo();
    displayCongratulations();
    Sleep(2000);
    clearScreen();

    displayWinHeader();

    // Display solved grid
    for (int i = 0; i < gridSize; i++) {
        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            cout << "┌────┐ ";
        }

        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            string emoji = currentGrid.getEmoji(i, j);
            
            if (emoji == "") {
                cout << "│" << left << setw(4) << "" << "│ ";
            } else {
                cout << "│ " << left << setw(2) << emoji << " │ ";
            }
        }

        centerGrid(gridSize);
        for (int j = 0; j < gridSize; j++) {
            cout << "└────┘ ";
        }
    }

    // Display statistics
    displayStatisticsHeader();
    cout << "\t ║ Theme: " << left << setw(44) << themes[currentTheme] << "║\n";
    displayDifficultyInStats(gridSize);
    cout << "\t ║ Total Moves: " << left << setw(38) << moves << "║\n";
    displayEfficiencyRating(moves, gridSize);
    displayStatisticsFooter();
    
    // Ask to save score
    cout << "\t\t  💾 Save to Leaderboard? " Y "[Y]" C " / " G "[N]" C ": ";
    char saveChoice = _getch();
    cout << (char)toupper(saveChoice) << "\n";

    if (saveChoice == 'y' || saveChoice == 'Y') {
        clearPreviousLine();
        string playerName;
        cout << "\n                Enter your name (max 17 chars): ";
        getline(cin, playerName);

        //Only letters and spaces allowed for the player name
        while (true) {
            bool validName = true;

            for (char c : playerName) {
                if (!isalpha(c) && c != ' ') { 
                    validName = false;
                    break;
                }
            }

            if (!validName) {
                clearPreviousLine();
                cout << "\n                Enter your name (max 17 chars): ";
                getline(cin, playerName);
                continue;
            }

            break; // input is valid
        }

        if (playerName.empty()) playerName = "Anonymous";
        if (playerName.length() > 17) playerName = playerName.substr(0, 17);

        leaderboard.insert(playerName, moves, gridSize, themes[currentTheme]);
        leaderboard.saveToFile("leaderboard.txt");

        clearPreviousLine();
        cout << "\n\t ✅ 𝐒 𝐂 𝐎 𝐑 𝐄   𝐒 𝐀 𝐕 𝐄 𝐃   𝐓 𝐎   𝐋 𝐄 𝐀 𝐃 𝐄 𝐑 𝐁 𝐎 𝐀 𝐑 𝐃 \n\n";
        Sleep(1500);
    }

    // Show options
    clearPreviousLine();
    cout << "        [N] Next Level      [R] Retry Level      [B] Back to Menu \n";

    while (true) {
        char choice = _getch();
        if (choice == 'n' || choice == 'N') return 'N';
        if (choice == 'r' || choice == 'R') return 'R';
        if (choice == 'b' || choice == 'B') return 'B';
    }
}

// Show splash/tutorial screen
void showSplash() {
    clearScreen();
    displaySplashScreen();
    _getch();
}

// Show main menu and get user choice
// Returns: 0=Exit, 1=Easy, 2=Medium, 3=Hard, 4=Leaderboard
int showMenu() {
    clearScreen();
    displayMainMenu();

    while (true) {
        char choice = _getch();
        if (choice >= '0' && choice <= '4') {
            return choice - '0';
        }
    }
}

// Main game loop
void playGame(int difficulty) {
    // Set grid size based on difficulty
    if (difficulty == 1) gridSize = 3;
    else if (difficulty == 2) gridSize = 4;
    else if (difficulty == 3) gridSize = 5;

    bool keepPlaying = true;
    bool samePattern = false;  // Track if retrying same puzzle

    while (keepPlaying) {
        // Initialize new puzzle or retry current one
        initializeGrid(!samePattern);
        shuffleGrid();
        moves = 0;
        moveHistory.clear();

        // Main gameplay loop
        while (true) {
            displayGrid();

            // Check win condition
            if (isSolved()) {
                char choice = showWinScreen();

                if (choice == 'N') {
                    samePattern = false;  // Generate new puzzle
                    break;
                } else if (choice == 'R') {
                    samePattern = true;   // Retry same puzzle
                    break;
                } else if (choice == 'B') {
                    keepPlaying = false;  // Return to menu
                    break;
                }
            }

            // Get player input
            char key = _getch();

            // Handle arrow keys (require two _getch() calls)
            if (key == -32 || key == 0) {
                key = _getch();
                makeMove(key);
            } 
            // Handle special keys
            else if (key == 'u' || key == 'U') {
                undoMove();
            } else if (key == 'r' || key == 'R') {
                samePattern = true;
                break;
            } else if (key == 'q' || key == 'Q') {
                keepPlaying = false;
                break;
            }
        }
    }
}

#endif
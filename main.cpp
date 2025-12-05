#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "BST.h"
#include "Display.h"
#include "GameFunctions.h"

using namespace std;

// GLOBAL VARIABLES
DoublyLinkedList currentGrid;       // Current puzzle state (what player sees)
DoublyLinkedList targetGrid;        // Target pattern to match
DoublyLinkedList savedGrid;         // Saved initial pattern (for retry)
Stack moveHistory;                  // History of moves (for undo)
BST leaderboard;                    // High scores storage

// Game state variables
int gridSize = 3;                   // Current grid dimension (3, 4, or 5)
int emptyRow = 0, emptyCol = 0;     // Position of empty space
int moves = 0;                      // Current move count
int currentTheme = 0;               // Current theme index

string themes[] = {"Emotion", "Fruits", "Moon Phases", "Foods", "Animals"};

string emojis[5][25] = {
    {"😙", "😆", "😑", "😮", "😢", "🤨", "🤪", "😍", "🙃",
     "😧", "😁", "😷", "😌", "😱", "😤", "😶", "😨", "😭",
     "🤭", "🤤", "🤮", "🤒", "🙄", "😋", "🤩"},

    {"🍋", "🍈", "🍐", "🥕", "🍒", "🌶️", "🍏", "🥝", 
     "🥑", "🍆", "🥭", "🥒", "🌽", "🍍", "🍌", "🍇", 
     "🌰", "🍅", "🍓", "🍊", "🥥", "🍎", "🍉", "🍑"},

    {"🌑", "🌒", "🌓", "🌕", "🌖", "🌗", "🌘", "🌔", "🌙"},

    {"🥪", "🥔", "🍪", "🍩", "🍤", "🥜", "🍞", "🍮", 
     "🥐", "🥖", "🌮", "🍝", "🥞", "🍰", "🥧", "🍦", 
     "🥠", "🍔", "🌭", "🥨", "🍕", "🥘", "🍗", "🍟"},

    {"🦇", "🐫", "🦔", "🐒", "🐅", "🐿️", "🦕", "🦅", 
     "🐂", "🦈", "🐧", "🐕", "🐎", "🦌", "🐆", "🐖", 
     "🦒", "🐃", "🦉", "🦃", "🐀", "🐡", "🐌", "🦍", "🐻"}
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    // Seed random number generator for shuffling
    srand(time(0));

    // Load saved high scores from file
    leaderboard.loadFromFile("leaderboard.txt");

    // Show tutorial/controls screen
    showSplash();

    // Main menu
    while (true) {
        int choice = showMenu();

        if (choice == 0) {
            clearScreen();
            displayExitScreen();        // Exit game
            break;
        } else if (choice == 4) {
            displayLeaderboard();       // View Leaderboard
        } else if (choice >= 1 && choice <= 3) {
            playGame(choice);           // Start game with selected difficulty
        }
    }
    return 0;
}
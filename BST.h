// Binary Search Tree: Maintains sorted leaderboard scores
#ifndef BST_H
#define BST_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

// Color Codes
#define C "\033[0m"
#define Y "\033[38;5;226m"
#define G "\033[38;5;118m"

// Used to store player scores in sorted order
struct BSTNode {
    string name;     
    int moves;           
    int difficulty;      
    string theme;        
    BSTNode* right;       // right child (better scores)
    BSTNode* left;      // left child (worse scores)
};

class BST {
    BSTNode* root;    
    int totalScores;

public:
    BST() : root(NULL), totalScores(0) {}

    ~BST() {
        destroyTree(root);
    }
    
    // Add a new score to leaderboard
    // Sorts by: difficulty then moves
    void insert(string name, int moves, int difficulty, string theme) {
        if (root == NULL) {
            root = new BSTNode();
            root -> name = name;
            root -> moves = moves;
            root -> difficulty = difficulty;
            root -> theme = theme;
            root -> right = NULL;
            root -> left = NULL;
            totalScores++;
            return;
        }

        BSTNode* current = root;
        while (true) {
            bool goRight = false;
            
            if (difficulty > current -> difficulty) {
                goRight = true;
            } else if (difficulty == current -> difficulty && moves < current -> moves) {
                goRight = true;
            }

            if (goRight) {
                if (current -> right == NULL) {
                    current -> right = new BSTNode();
                    current -> right -> name = name;
                    current -> right -> moves = moves;
                    current -> right -> difficulty = difficulty;
                    current -> right -> theme = theme;
                    current -> right -> right = NULL;
                    current -> right -> left = NULL;
                    totalScores++;
                    return;
                } else {
                    current = current -> right;
                }
            } else {
                if (current -> left == NULL) {
                    current -> left = new BSTNode();
                    current -> left -> name = name;
                    current -> left -> moves = moves;
                    current -> left -> difficulty = difficulty;
                    current -> left -> theme = theme;
                    current -> left -> right = NULL;
                    current -> left -> left = NULL;
                    totalScores++;
                    return;
                } else {
                    current = current -> left;
                }
            }
        }
    }
    
    // Display top 10 scores in order
    void display() {
        int rank = 1; 
        displayInOrder(root, rank);
    }
    
    // Save all scores to file
    void saveToFile(string filename) {
        ofstream file(filename.c_str());
        if (file.is_open()) {
            saveToFileHelper(root, file);
            file.close();
        }
    }
    
    // Load scores from file
    void loadFromFile(string filename) {
        ifstream file(filename.c_str());
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;

                string name = "", moves = "", difficulty = "", theme = "";
                int part = 0;

                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '|') {
                        part++;
                    } else {
                        if (part == 0) name += line[i];
                        else if (part == 1) moves += line[i];
                        else if (part == 2) difficulty += line[i];
                        else if (part == 3) theme += line[i];
                    }
                }

                if (!name.empty() && !moves.empty()) {
                    insert(name, atoi(moves.c_str()), atoi(difficulty.c_str()), theme);
                }
            }
            file.close();
        }
    }
    
    // Check if leaderboard is empty
    bool isEmpty() {
        return root == NULL;
    }
    
    // Get total number of scores
    int getSize() {
        return totalScores;
    }

private:
    // Recursively display scores in-order
    void displayInOrder(BSTNode* node, int& rank) {
        if (node == NULL || rank > 10) return;

        displayInOrder(node -> right, rank);

        if (rank <= 10) {
            string diffText;
            if (node -> difficulty == 3)
                diffText = "Easy (3x3)";
            else if (node -> difficulty == 4)
                diffText = "Med (4x4)";
            else
                diffText = "Hard (5x5)";

            cout << G " ║ " C << setw(4) << right << rank << G " ║ " C;

            string name = node -> name;
            if (name.length() > 20) name = name.substr(0, 20);
            cout << setw(20) << right << name << G " ║ " C;

            cout << setw(15) << right << diffText << G " ║ " C;

            cout << setw(6) << right << node -> moves << G " ║ " C; 

            string theme = node -> theme;
            if (theme.length() > 25) theme = theme.substr(0, 25);
            cout << setw(25) << right << theme << G " ║\n";

            rank++;
        }

        displayInOrder(node -> left, rank);
    }
    
    // Recursively save scores to file
    void saveToFileHelper(BSTNode* node, ofstream& file) {
        if (node == NULL) return;
        
        saveToFileHelper(node -> right, file);
        file << node -> name << "|" << node -> moves << "|" 
             << node -> difficulty << "|" << node -> theme << "\n";
        saveToFileHelper(node -> left, file);
    }

    // Recursively delete all nodes
    void destroyTree(BSTNode* node) {
        if (node == NULL) return;
        destroyTree(node -> right);
        destroyTree(node -> left);
        delete node;
    }
};

#endif
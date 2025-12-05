// Doubly Linked List: Stores grid positions and emojis
#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <string>
using namespace std;

// Used to store emoji grid data with position information
struct DNode {
    string emoji;
    int row;       
    int col;
    DNode* next;
    DNode* prev;
};

// Store the game grid (target pattern and current state)
// Stores each emoji with its row/column position
class DoublyLinkedList {
    DNode* head;
    DNode* tail;
    int size;

public:
    DoublyLinkedList() : head(NULL), tail(NULL), size(0) {}
    
    ~DoublyLinkedList() {
        clear();
    }

    // Insert a new emoji at specified position
    void insert(int row, int col, string emoji) {
        DNode* newNode = new DNode();
        newNode -> emoji = emoji;
        newNode -> row = row;
        newNode -> col = col;
        newNode -> next = NULL;
        newNode -> prev = tail;

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail -> next = newNode;
            tail = newNode;
        }
        size++;
    }
    
    // Get emoji at specified position
    string getEmoji(int row, int col) {
        DNode* current = head;
        while (current != NULL) {
            if (current -> row == row && current -> col == col) {
                return current -> emoji;
            }
            current = current -> next;
        }
        return "";
    }
    
    // Update emoji at specified position
    void setEmoji(int row, int col, string emoji) {
        DNode* current = head;
        while (current != NULL) {
            if (current -> row == row && current -> col == col) {
                current -> emoji = emoji;
                return;
            }
            current = current -> next;
        }
    }
    
    // Remove all nodes from list
    void clear() {
        while (head != NULL) {
            DNode* temp = head;
            head = head -> next;
            delete temp;
        }
        head = tail = NULL;
        size = 0;
    }
    
    // Get total number of nodes
    int getSize() {
        return size;
    }
};

#endif
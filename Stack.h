// Stack: Manages move history for undo functionality
#ifndef STACK_H
#define STACK_H

// Used to store move history for undo functionality
struct StackNode {
    char direction;     // Arrow key code (72=Up, 80=Down, 75=Left, 77=Right)
    int moveNum;        // Move number when this action was performed
    StackNode* next;    // Pointer to next node
};

// Each push stores a direction and move number
// Pop retrieves the last move for undo operation
class Stack {
    StackNode* top;  
    int count;

public:
    Stack() : top(NULL), count(0) {}
    
    ~Stack() {
        clear();
    }

    // Add a move to history
    void push(char direction, int moveNum) {
        StackNode* newNode = new StackNode();
        newNode -> direction = direction;
        newNode -> moveNum = moveNum;
        newNode -> next = top;
        top = newNode;
        count++;
    }
    
    // Remove and return last move (for undo)
    char pop() {
        if (isEmpty()) {
            return ' ';
        }

        StackNode* temp = top;
        char direction = top -> direction;
        top = top -> next;
        delete temp;
        count--;
        return direction;
    }
    
    // Check if stack is empty
    bool isEmpty() {
        return top == NULL;
    }
    
    // Get number of moves in history
    int getSize() {
        return count;
    }
    
    // Remove all moves from history
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

#endif
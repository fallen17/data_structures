#ifndef AVL_H
#define AVL_H

#include <string>

namespace AVL{
    struct Node{
        int value;
        int height;
        Node* left;
        Node* right;

        Node(int val) : value(val), left(nullptr), right(nullptr), height(1){}
};

    int getHeight(Node* node);
    int getBalance(Node* node);
    void updateHeight(Node* node);
    Node* findMin(Node* node);

    Node* rotateLeft(Node* x);
    Node* rotateRight(Node* y);
    Node* balance(Node* node);
    Node* insert(Node* node, int value);
    Node* search(Node* node, int value);
    Node* remove(Node* node, int value);
    void print(Node* root, const std::string& prefix = "", bool isLeft = true);
}


#endif
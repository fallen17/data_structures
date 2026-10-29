#include"AVL.h"
#include<iostream>

namespace AVL{
    int getHeight(Node* node){
        return node ? node->height : 0;
    }

    int getBalance(Node* node){
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(Node* node){
        if(node){
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    Node* findMin(Node* node){
        while (node->left) node = node->left;
        return node;
    }

    Node* rotateLeft(Node* x){
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rotateRight(Node* y){
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* balance(Node* node){
        if (!node) return node;

        updateHeight(node);
        int balanceFactor = getBalance(node);

        if(balanceFactor > 1 && getBalance(node->left) >= 0){
            return rotateRight(node);
        }
        if(balanceFactor < -1 && getBalance(node->right) <= 0){
            return rotateLeft(node);
        }
        if(balanceFactor > 1 && getBalance(node->left) < 0){
            node->left = rotateLeft(node->left);
            return rotateRight(node);    
        }
        if(balanceFactor < -1 && getBalance(node->right) > 0){
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, int value){
        if(!node) return new Node(value);

        if(value < node->value){
            node->left = insert(node->left, value);
        }
        else if (value > node->value){
            node->right = insert(node->right, value);
        }
        else{
            return node;
        }
        return balance(node);
    }

    Node* search(Node* node, int value){
        if (!node || node->value == value){
            return node;
        }

        if (value < node->value){
            return search(node->left, value);
        }
        return search(node->right, value);
    }

    Node* remove(Node* node, int value){
        if (!node) return nullptr;

        if(value < node->value){
            node->left = remove(node->left, value);
        }
        else if(value > node->value){
            node->right = remove(node->right, value);
        }
        else{
            if(!node->left || !node->right){
                Node* temp = node->left ? node->left : node->right;

                if(!temp){
                    temp = node;
                    node = nullptr;
                }
                else{
                    *node = *temp;
                }
                delete temp;
            }
            else{
                Node* temp = findMin(node->right);
                node->value = temp->value;
                node->right = remove(node->right, temp->value);
            }
        }

        if(!node) return nullptr;
        return balance(node);
    }

    void print(Node* root, const std::string& prefix, bool isLeft) {
        if (root == nullptr) {
            return;
        }

        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << root->value << " (h:" << root->height << ")\n";

        print(root->left, prefix + (isLeft ? "│   " : "    "), true);
        print(root->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}
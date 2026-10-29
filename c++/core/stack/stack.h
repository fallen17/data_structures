#ifndef STACK_H
#define STACK_H

#include <string>

namespace stack{
    struct Node {
        std::string data;
        Node* next;
    };

    struct Stack {
        Node* head;

        void init();
        void push(const std::string& value);
        void pop();
        void print() const;
    };
}

#endif
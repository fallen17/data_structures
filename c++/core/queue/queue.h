#ifndef QUEUE_H
#define QUEUE_H

#include <string>

namespace queue{
    struct Node {
        std::string data;
        Node* next;
    };

    struct Queue {
        Node* head;
        Node* tail;

        void init();
        void push(const std::string& value);
        void pop();
        void print() const;
    };
}
#endif
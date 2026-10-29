#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <string>

namespace singly{
    struct Node {
        std::string data;
        Node* next;
    };

    struct SinglyLinkedList {
        Node* head;

        void init();

        void push_front(const std::string& value);
        void push_back(const std::string& value);
        void insert_after(Node* target, const std::string& value);
        void insert_before(Node* target, const std::string& value);

        void pop_front();
        void pop_back();
        void remove_after(Node* target);
        void remove_before(Node* target);

        Node* find_by_value(const std::string& value) const;
        bool remove_by_value(const std::string& value);

        void print() const;
        std::string get_front() const;
        std::string get_back() const;
    };
}
#endif
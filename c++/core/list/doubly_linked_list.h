#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <string>
namespace doubly{
    struct Node {
        std::string data;
        Node* next;
        Node* prev;
    };

    struct DoublyLinkedList {
        Node* head;
        Node* tail;

        void init();

        void push_front(const std::string& value);
        void push_back(const std::string& value);
        void insert_after(Node* target, const std::string& value);
        void insert_before(Node* target, const std::string& value);

        void pop_front();
        void pop_back();
        void remove_after(Node* target);
        void remove_before(Node* target);

        Node* find(const std::string& value) const;
        bool remove_by_value(const std::string& value);

        void print_forward() const;
        void print_backward() const;
        std::string get_front() const;
        std::string get_back() const;
    };
};

#endif
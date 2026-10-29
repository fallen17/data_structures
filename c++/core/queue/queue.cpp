#include "queue.h"
#include <iostream>
#include <stdexcept>

namespace queue{
    void Queue::init() {
        head = nullptr;
        tail = nullptr;
    }

    void Queue::push(const std::string& value) {
        Node* new_node = new Node{value, nullptr};
        
        if (tail == nullptr) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void Queue::pop() {
        if (head == nullptr) {
            throw std::underflow_error("Cannot pop from empty queue");
        }

        Node* temp = head;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        delete temp;
    }

    void Queue::print() const {
        std::cout << "Queue: [ ";
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << "\"" << curr->data << "\" ";
            curr = curr->next;
        }
        std::cout << " ]\n";
    }
}
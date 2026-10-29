#include "stack.h"
#include <iostream>
#include <stdexcept>

namespace stack{
    void Stack::init() {
        head = nullptr;
    }

    void Stack::push(const std::string& value) {
        Node* new_node = new Node;
        new_node->data = value;
        new_node->next = head;
        head = new_node;
    }

    void Stack::pop() {
        if (head == nullptr) {
            throw std::underflow_error("Cannot pop from empty stack");
        }

        Node* temp = head;
        head = head->next;
        std::cout << temp->data << " was deleted" << std::endl;
        delete temp;
    }

    void Stack::print() const {
        std::cout << "Stack: [ ";
        Node* current = head;
        while (current != nullptr) {
            std::cout << "\"" << current->data << "\" ";
            current = current->next;
        }
        std::cout << "]\n";
    }
}
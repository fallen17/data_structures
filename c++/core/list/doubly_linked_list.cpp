#include "doubly_linked_list.h"
#include <iostream>
#include <stdexcept>

namespace doubly{
    void DoublyLinkedList::init() {
        head = nullptr;
        tail = nullptr;
    }

    void DoublyLinkedList::push_front(const std::string& value) {
        Node* new_node = new Node{value, head, nullptr};
        if (head != nullptr) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }
        head = new_node;
    }

    void DoublyLinkedList::push_back(const std::string& value) {
        Node* new_node = new Node{value, nullptr, tail};
        if (tail != nullptr) {
            tail->next = new_node;
        } else {
            head = new_node;
        }
        tail = new_node;
    }

    void DoublyLinkedList::insert_after(Node* target, const std::string& value) {
        if (target == nullptr) return;
        
        if (target == tail) {
            push_back(value);
            return;
        }

        Node* new_node = new Node{value, target->next, target};
        target->next->prev = new_node;
        target->next = new_node;
    }

    void DoublyLinkedList::insert_before(Node* target, const std::string& value) {
        if (target == nullptr) return;

        if (target == head) {
            push_front(value);
            return;
        }

        Node* new_node = new Node{value, target, target->prev};
        target->prev->next = new_node;
        target->prev = new_node;
    }


    void DoublyLinkedList::pop_front() {
        if (head == nullptr) return;

        Node* temp = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }

        delete temp;
    }

    void DoublyLinkedList::pop_back() {
        if (tail == nullptr) return;

        Node* temp = tail;
        tail = tail->prev;

        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }

        delete temp;
    }

    void DoublyLinkedList::remove_after(Node* target) {
        if (target == nullptr || target->next == nullptr) return;

        Node* temp = target->next;
        target->next = temp->next;

        if (temp->next != nullptr) {
            temp->next->prev = target;
        } else {
            tail = target;
        }

        delete temp;
    }

    void DoublyLinkedList::remove_before(Node* target) {
        if (target == nullptr || target->prev == nullptr) return;

        Node* temp = target->prev;
        target->prev = temp->prev;

        if (temp->prev != nullptr) {
            temp->prev->next = target;
        } else {
            head = target;
        }

        delete temp;
    }


    Node* DoublyLinkedList::find(const std::string& value) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == value) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    bool DoublyLinkedList::remove_by_value(const std::string& value) {
        Node* target = find(value);
        if (target == nullptr) return false;

        if (target == head) {
            pop_front();
        } else if (target == tail) {
            pop_back();
        } else {
            target->prev->next = target->next;
            target->next->prev = target->prev;
            delete target;
        }
        return true;
    }


    void DoublyLinkedList::print_forward() const {
        std::cout << "List (forward): [ ";
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << "\"" << curr->data << "\" ";
            curr = curr->next;
        }
        std::cout << "]\n";
    }

    void DoublyLinkedList::print_backward() const {
        std::cout << "List (backward): [ ";
        Node* curr = tail;
        while (curr != nullptr) {
            std::cout << "\"" << curr->data << "\" ";
            curr = curr->prev;
        }
        std::cout << "]\n";
    }

    std::string DoublyLinkedList::get_front() const {
        if (head == nullptr) throw std::underflow_error("List is empty");
        return head->data;
    }

    std::string DoublyLinkedList::get_back() const {
        if (tail == nullptr) throw std::underflow_error("List is empty");
        return tail->data;
    }
}
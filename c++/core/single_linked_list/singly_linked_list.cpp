#include "singly_linked_list.h"
#include <iostream>
#include <stdexcept>

namespace singly{
    void SinglyLinkedList::init() {
        head = nullptr;
    }

    void SinglyLinkedList::push_front(const std::string& value) {
        Node* new_node = new Node{value, head};
        head = new_node;
    }

    void SinglyLinkedList::push_back(const std::string& value) {
        Node* new_node = new Node{value, nullptr};
        if (head == nullptr) {
            head = new_node;
            return;
        }
        Node* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = new_node;
    }

    void SinglyLinkedList::insert_after(Node* target, const std::string& value) {
        if (target == nullptr) return;
        Node* new_node = new Node{value, target->next};
        target->next = new_node;
    }

    void SinglyLinkedList::insert_before(Node* target, const std::string& value) {
        if (target == nullptr || head == nullptr) return;
        
        if (head == target) {
            push_front(value);
            return;
        }

        Node* curr = head;
        while (curr->next != nullptr && curr->next != target) {
            curr = curr->next;
        }

        if (curr->next == target) {
            Node* new_node = new Node{value, target};
            curr->next = new_node;
        }
    }


    void SinglyLinkedList::pop_front() {
        if (head == nullptr) return;
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    void SinglyLinkedList::pop_back() {
        if (head == nullptr) return;
        
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            return;
        }

        Node* curr = head;
        while (curr->next->next != nullptr) {
            curr = curr->next;
        }
        delete curr->next;
        curr->next = nullptr;
    }

    void SinglyLinkedList::remove_after(Node* target) {
        if (target == nullptr || target->next == nullptr) return;
        Node* temp = target->next;
        target->next = temp->next;
        delete temp;
    }

    void SinglyLinkedList::remove_before(Node* target) {
        if (target == nullptr || head == nullptr || head == target) return;

        if (head->next == target) {
            pop_front();
            return;
        }

        Node* curr = head;
        while (curr->next != nullptr && curr->next->next != target) {
            curr = curr->next;
        }

        if (curr->next != nullptr && curr->next->next == target) {
            Node* temp = curr->next;
            curr->next = target;
            delete temp;
        }
    }

    Node* SinglyLinkedList::find_by_value(const std::string& value) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == value) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    bool SinglyLinkedList::remove_by_value(const std::string& value) {
        if (head == nullptr) return false;

        if (head->data == value) {
            pop_front();
            return true;
        }

        Node* curr = head;
        while (curr->next != nullptr && curr->next->data != value) {
            curr = curr->next;
        }

        if (curr->next != nullptr) {
            Node* temp = curr->next;
            curr->next = temp->next;
            delete temp;
            return true;
        }
        return false;
    }

    void SinglyLinkedList::print() const {
        std::cout << "List: [ ";
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << "\"" << curr->data << "\" ";
            curr = curr->next;
        }
        std::cout << "]\n";
    }

    std::string SinglyLinkedList::get_front() const {
        if (head == nullptr) throw std::underflow_error("List is empty");
        return head->data;
    }

    std::string SinglyLinkedList::get_back() const {
        if (head == nullptr) throw std::underflow_error("List is empty");
        Node* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        return curr->data;
    }
}
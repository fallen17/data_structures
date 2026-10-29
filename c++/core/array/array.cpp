#include "array.h"
#include <iostream>
#include <stdexcept>

void Array::init(size_t fixed_capacity) {
    size = 0;
    capacity = fixed_capacity;
    data = new std::string[capacity];
}

void Array::push_back(const std::string& value) {
    if (size >= capacity) {
        throw std::overflow_error("Array overflow: cannot add to full array");
    }
    data[size++] = value;
}

std::string Array::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

void Array::insert_at(size_t index, const std::string& value){
    if(index > size){
        throw std::out_of_range("Index out of range");
    }
    if(size >= capacity){
        throw std::overflow_error("Array overflow : cannot insert into full array");
    }

    for(size_t i = size; i > index; --i){
        data[i] = data[i - 1];
    }

    data[index] = value;
    size++;
}

void Array::remove_at(size_t index){
    if(index >= size){
        throw std::out_of_range("Index out of range");
    }
    for(size_t i = index; i < size; ++i){
        data[i] = data[i + 1];
    }

    data[size - 1] = "";
    size--;
}

void Array::replace(size_t index, const std::string& value) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    data[index] = value;
}

size_t Array::length() const {
    return size;
}

void Array::print() const {
    std::cout << "[";
    for (size_t i = 0; i < size; ++i) {
        std::cout << "\"" << data[i] << "\"" << (i + 1 < size ? ", " : " ");
    }
    std::cout << "] (size: " << size << ", max_capacity: " << capacity << ")\n";
}
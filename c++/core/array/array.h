#ifndef ARRAY_H
#define ARRAY_H

#include <string>
#include <cstddef>

struct Array {
    std::string* data;
    size_t size;
    size_t capacity;

    void init(size_t fixed_capacity);
    void push_back(const std::string& value);
    std::string get(size_t index) const;
    void insert_at(size_t index, const std::string& value);
    void remove_at(size_t index);
    void replace(size_t index, const std::string& value);
    size_t length() const;
    void print() const;
};

#endif
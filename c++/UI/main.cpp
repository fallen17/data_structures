#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <exception>
#include <system_error>

#include "../core/array/array.h"
#include "../core/AVL-tree/AVL.h"
#include "../core/list/doubly_linked_list.h"
#include "../core/queue/queue.h"
#include "../core/single_linked_list/singly_linked_list.h"
#include "../core/stack/stack.h"


using namespace std;

Array currentArray;
singly::SinglyLinkedList singlyList;
doubly::DoublyLinkedList doublyList;
queue::Queue currentQueue;
stack::Stack currentStack;
AVL::Node* avlRoot = nullptr;

struct Query {
    string command;
    string name;
    int number = 0;
    string value = "";
    string value2 = "";
};

bool parseArguments(int argc, char* argv[], string& filename, string& text) {
    if (argc != 5) return false;
    if (string(argv[1]) != "--file" || string(argv[3]) != "--query") return false;
    filename = argv[2];
    text = argv[4];
    return !filename.empty() && !text.empty();
}

int argumentCount(const string& command) {
    if (command == "PRINT") return 0;
    if (command.empty()) return -1;
    char type = command[0];
    string operation = command.substr(1);
    
    if (operation == "INIT") return 0;

    if (type == 'M') {
        if (operation == "PUSH" || operation == "GET" || operation == "REMOVE") return 1;
        if (operation == "SET" || operation == "REPLACE" || operation == "INSERT") return 2;
        if (operation == "LENGTH" || operation == "PRINT") return 0;
    } else if (type == 'F' || type == 'L') {
        if (operation == "PUSH_FRONT" || operation == "PUSH_BACK" || operation == "FIND" || 
            operation == "REMOVE" || operation == "REMOVE_AFTER" || operation == "REMOVE_BEFORE") return 1;
        if (operation == "INSERT_AFTER" || operation == "INSERT_BEFORE") return 2;
        if (operation == "POP_FRONT" || operation == "POP_BACK" || operation == "PRINT" || 
            operation == "PRINT_F" || operation == "PRINT_B" || operation == "GET_FRONT" || operation == "GET_BACK") return 0;
    } else if (type == 'Q') {
        if (operation == "PUSH") return 1;
        if (operation == "POP" || operation == "PRINT") return 0;
    } else if (type == 'S') {
        if (operation == "PUSH") return 1;
        if (operation == "POP" || operation == "PRINT") return 0;
    } else if (type == 'A') {
        if (operation == "INSERT" || operation == "REMOVE" || operation == "FIND") return 1;
        if (operation == "PRINT" || operation == "INIT") return 0;
    }
    return -1;
}

bool firstArgumentIsNumber(const string& command) {
    if (command.empty()) return false;
    
    return command == "MGET" || command == "MSET" || command == "MREPLACE" || 
           command == "MINSERT" || command == "MREMOVE" || 
           command == "AINSERT" || command == "AREMOVE" || command == "AFIND";
}

bool parseQuery(const string& text, Query& query) {
    query = Query{};
    istringstream input(text);
    if (!(input >> query.command >> query.name)) return false;
    int count = argumentCount(query.command);
    if (count == -1) return false;
    
    if (count >= 1) {
        string token;
        if (!(input >> token)) return false;
        if (firstArgumentIsNumber(query.command)) {
            istringstream valStream(token);
            if (!(valStream >> query.number)) return false;
        } else {
            query.value = token;
        }
    }
    if (count == 2) {
        if (firstArgumentIsNumber(query.command)) {
            if (!(input >> query.value)) return false;
        } else {
            if (!(input >> query.value2)) return false;
        }
    }
    string extra;
    return !(input >> extra);
}

bool canCreate(const string& command) {
    return command.find("PUSH") != string::npos || 
           command.find("INIT") != string::npos || 
           command.find("INSERT") != string::npos;
}

void clearAVL(AVL::Node* node) {
    if (!node) return;
    clearAVL(node->left);
    clearAVL(node->right);
    delete node;
}

bool readFile(const string& filename, const Query& query, vector<string>& lines, vector<string>& values, int& position, char& type) {
    position = -1;
    type = query.command == "PRINT" || query.command == "APRINT" ? '\0' : query.command[0];
    ifstream input(filename);
    error_code error;
    bool exists = filesystem::exists(filename, error);
    if (!input && (exists || error)) return false;

    string line;
    while (getline(input, line)) {
        istringstream row(line);
        string savedType, savedName;
        if (row >> savedType >> savedName && savedName == query.name) {
            if (position != -1 || savedType.size() != 1) return false;
            if (query.command != "PRINT" && query.command != "APRINT" && savedType[0] != type) return false;
            type = savedType[0];
            position = static_cast<int>(lines.size());
            string value;
            while (row >> value) values.push_back(value);
        }
        lines.push_back(line);
    }

    if (input.bad()) return false;
    if (position == -1 && !canCreate(query.command)) return false;
    if (string("MFLQSA").find(type) == string::npos) return false;
    return true;
}

bool loadStructure(char type, const vector<string>& values) {
    switch (type) {
        case 'M':
            currentArray.init(100);
            for (const auto& v : values) currentArray.push_back(v);
            break;
        case 'F':
            singlyList.init();
            for (const auto& v : values) singlyList.push_back(v);
            break;
        case 'L':
            doublyList.init();
            for (const auto& v : values) doublyList.push_back(v);
            break;
        case 'Q':
            currentQueue.init();
            for (const auto& v : values) currentQueue.push(v);
            break;
        case 'S':
            currentStack.init();
            for (const auto& v : values) currentStack.push(v);
            break;
        case 'A':
            avlRoot = nullptr;
            for (const auto& v : values) {
                avlRoot = AVL::insert(avlRoot, stoi(v));
            }
            break;
        default:
            return false;
    }
    return true;
}

bool executeCommand(char type, const Query& query, bool& changed) {
    changed = false;
    const string& cmd = query.command;

    try {
        if (type == 'M') {
            if (cmd == "MINIT") { currentArray.init(100); changed = true; }
            else if (cmd == "MPUSH") { currentArray.push_back(query.value); changed = true; }
            else if (cmd == "MGET") { cout << "-> " << currentArray.get(query.number) << "\n"; }
            else if (cmd == "MSET" || cmd == "MREPLACE") { currentArray.replace(query.number, query.value); changed = true; }
            else if (cmd == "MLENGTH") { cout << "-> " << currentArray.length() << "\n"; }
            else if (cmd == "MPRINT") { currentArray.print(); }
            else if (cmd == "MINSERT") { currentArray.insert_at(query.number, query.value); changed = true;}
            else if (cmd == "MREMOVE") { currentArray.remove_at(query.number); changed = true;}

        }
        else if (type == 'F') {
            if (cmd == "FINIT") { singlyList.init(); changed = true; }
            else if (cmd == "FPUSH_FRONT") { singlyList.push_front(query.value); changed = true; }
            else if (cmd == "FPUSH_BACK") { singlyList.push_back(query.value); changed = true; }
            else if (cmd == "FPOP_FRONT") { singlyList.pop_front(); changed = true; }
            else if (cmd == "FPOP_BACK") { singlyList.pop_back(); changed = true; }
            else if (cmd == "FFIND") { 
                singly::Node* found = singlyList.find_by_value(query.value);
                cout << (found ? "-> Found" : "-> Not found") << "\n";
            }
            else if (cmd == "FREMOVE") { 
                changed = singlyList.remove_by_value(query.value); 
            }
            else if (cmd == "FINSERT_AFTER") {
                singly::Node* target = singlyList.find_by_value(query.value);
                if (target) { singlyList.insert_after(target, query.value2); changed = true; }
            }
            else if (cmd == "FINSERT_BEFORE") {
                singly::Node* target = singlyList.find_by_value(query.value);
                if (target) { singlyList.insert_before(target, query.value2); changed = true; }
            }
            else if (cmd == "FREMOVE_AFTER") {
                singly::Node* target = singlyList.find_by_value(query.value);
                if (target) { singlyList.remove_after(target); changed = true; }
            }
            else if (cmd == "FREMOVE_BEFORE") {
                singly::Node* target = singlyList.find_by_value(query.value);
                if (target) { singlyList.remove_before(target); changed = true; }
            }
            else if (cmd == "FGET_FRONT") { 
                try { cout << "-> " << singlyList.get_front() << "\n"; } catch(const exception& e) { cout << "-> " << e.what() << "\n"; }
            }
            else if (cmd == "FGET_BACK") { 
                try { cout << "-> " << singlyList.get_back() << "\n"; } catch(const exception& e) { cout << "-> " << e.what() << "\n"; }
            }
            else if (cmd == "FPRINT") { singlyList.print(); }
        }
        else if (type == 'L') {
            if (cmd == "LINIT") { doublyList.init(); changed = true; }
            else if (cmd == "LPUSH_FRONT") { doublyList.push_front(query.value); changed = true; }
            else if (cmd == "LPUSH_BACK") { doublyList.push_back(query.value); changed = true; }
            else if (cmd == "LPOP_FRONT") { doublyList.pop_front(); changed = true; }
            else if (cmd == "LPOP_BACK") { doublyList.pop_back(); changed = true; }
            else if (cmd == "LFIND") { 
                doubly::Node* found = doublyList.find(query.value);
                cout << (found ? "-> Found" : "-> Not found") << "\n";
            }
            else if (cmd == "LREMOVE") { 
                changed = doublyList.remove_by_value(query.value); 
            }
            else if (cmd == "LINSERT_AFTER") {
                doubly::Node* target = doublyList.find(query.value);
                if (target) { doublyList.insert_after(target, query.value2); changed = true; }
            }
            else if (cmd == "LINSERT_BEFORE") {
                doubly::Node* target = doublyList.find(query.value);
                if (target) { doublyList.insert_before(target, query.value2); changed = true; }
            }
            else if (cmd == "LREMOVE_AFTER") {
                doubly::Node* target = doublyList.find(query.value);
                if (target) { doublyList.remove_after(target); changed = true; }
            }
            else if (cmd == "LREMOVE_BEFORE") {
                doubly::Node* target = doublyList.find(query.value);
                if (target) { doublyList.remove_before(target); changed = true; }
            }
            else if (cmd == "LGET_FRONT") { 
                try { cout << "-> " << doublyList.get_front() << "\n"; } catch(const exception& e) { cout << "-> " << e.what() << "\n"; }
            }
            else if (cmd == "LGET_BACK") { 
                try { cout << "-> " << doublyList.get_back() << "\n"; } catch(const exception& e) { cout << "-> " << e.what() << "\n"; }
            }
            else if (cmd == "LPRINT_F") { doublyList.print_forward(); }
            else if (cmd == "LPRINT_B") { doublyList.print_backward(); }
        }
        else if (type == 'Q') {
            if (cmd == "QINIT") { currentQueue.init(); changed = true; }
            else if (cmd == "QPUSH") { currentQueue.push(query.value); changed = true; }
            else if (cmd == "QPOP") { 
                currentQueue.pop(); 
                changed = true; 
            }
            else if (cmd == "QPRINT") { currentQueue.print(); }
        }
        else if (type == 'S') {
            if (cmd == "SINIT") { currentStack.init(); changed = true; }
            else if (cmd == "SPUSH") { currentStack.push(query.value); changed = true; }
            else if (cmd == "SPOP") { 
                currentStack.pop(); 
                changed = true; 
            }
            else if (cmd == "SPRINT") { currentStack.print(); }
        }
        else if (type == 'A') {
            if (cmd == "AINIT") { 
                clearAVL(avlRoot); 
                avlRoot = nullptr; 
                changed = true; 
            }
            else if (cmd == "AINSERT") { 
                avlRoot = AVL::insert(avlRoot, query.number); 
                changed = true; 
            }
            else if (cmd == "AREMOVE") { 
                avlRoot = AVL::remove(avlRoot, query.number); 
                changed = true; 
            }
            else if (cmd == "AFIND") { 
                AVL::Node* found = AVL::search(avlRoot, query.number);
                cout << (found ? "-> Found" : "-> Not found") << "\n";
            }
            else if (cmd == "APRINT") { 
                AVL::print(avlRoot); 
            }
        }
        else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
    return true;
}

void serializeAVL(AVL::Node* node, ostringstream& out) {
    if (!node) return;
    serializeAVL(node->left, out);
    out << node->value << " ";
    serializeAVL(node->right, out);
}

string structureToString(char type) {
    ostringstream out;
    if (type == 'M') {
        for (size_t i = 0; i < currentArray.length(); ++i) {
            out << currentArray.get(i) << " ";
        }
    } else if (type == 'F') {
        singly::Node* curr = singlyList.head;
        while (curr != nullptr) {
            out << curr->data << " ";
            curr = curr->next;
        }
    } else if (type == 'L') {
        doubly::Node* curr = doublyList.head;
        while (curr != nullptr) {
            out << curr->data << " ";
            curr = curr->next;
        }
    } else if (type == 'Q') {
        queue::Node* curr = currentQueue.head;
        while (curr != nullptr) {
            out << curr->data << " ";
            curr = curr->next;
        }
    } else if (type == 'S') {
        stack::Node* curr = currentStack.head;
        while (curr != nullptr) {
            out << curr->data << " ";
            curr = curr->next;
        }
    } else if (type == 'A') {
        serializeAVL(avlRoot, out);
    }
    return out.str();
}

bool writeFile(const string& filename, vector<string>& lines, int position, char type, const string& name) {
    string updated = string(1, type) + " " + name + " " + structureToString(type);
    if (position == -1) {
        lines.push_back(updated);
    } else {
        lines[position] = updated;
    }
    ofstream output(filename);
    if (!output) return false;
    for (const auto& line : lines) {
        output << line << "\n";
    }
    return true;
}

int main(int argc, char* argv[]) {
    string filename, text;
    Query query;

    if (!parseArguments(argc, argv, filename, text)) {
        cerr << "Формат: ./dbms --file <file.data> --query '<COMMAND> <name> [args]'\n";
        return 1;
    }

    if (!parseQuery(text, query)) {
        cerr << "Ошибка: Неизвестная команда или неверные аргументы.\n";
        return 1;
    }

    vector<string> lines;
    vector<string> values;
    int position;
    char type;

    if (!readFile(filename, query, lines, values, position, type)) {
        cerr << "Ошибка чтения файла данных.\n";
        return 1;
    }

    if (!loadStructure(type, values)) {
        cerr << "Ошибка инициализации структуры данных.\n";
        return 1;
    }

    bool changed = false;
    bool success = executeCommand(type, query, changed);

    if (success && changed) {
        success = writeFile(filename, lines, position, type, query.name);
    }

    if (!success) {
        cerr << "Ошибка выполнения команды или записи в файл.\n";
        return 1;
    }

    return 0;
}
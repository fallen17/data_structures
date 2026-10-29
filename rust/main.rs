use std::env;
use std::fs;
use std::io::{self, Write};
use std::path::Path;

mod array;
mod forward_list;
mod list;
mod queue;
mod stack;

use array::Array;
use forward_list::SinglyLinkedList;
use list::DoublyLinkedList;
use queue::Queue;
use stack::Stack;

#[derive(Debug, Default)]
struct Query {
    command: String,
    name: String,
    number: usize,
    value: String,
}

fn parse_arguments() -> Option<(String, String)> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 5 {
        return None;
    }
    if args[1] != "--file" || args[3] != "--query" {
        return None;
    }
    let filename = args[2].clone();
    let text = args[4].clone();
    if filename.is_empty() || text.is_empty() {
        return None;
    }
    Some((filename, text))
}

fn argument_count(command: &str) -> i32 {
    if command == "PRINT" {
        return 0;
    }
    if command.is_empty() {
        return -1;
    }
    
    let type_char = command.chars().next().unwrap();
    let operation = &command[1..];
    
    if operation == "INIT" {
        return 0;
    }

    match type_char {
        'M' => {
            if operation == "PUSH" || operation == "GET" || operation == "REMOVE" { 1 }
            else if operation == "SET" || operation == "REPLACE" { 1 }
            else if operation == "LENGTH" || operation == "PRINT" { 0 }
            else { -1 }
        }
        'F' | 'L' => {
            if operation == "PUSH_FRONT" || operation == "PUSH_BACK" || operation == "FIND" || operation == "REMOVE" { 1 }
            else if operation == "POP_FRONT" || operation == "POP_BACK" || operation == "PRINT" { 0 }
            else { -1 }
        }
        'Q' | 'S' => {
            if operation == "PUSH" { 1 }
            else if operation == "POP" || operation == "PRINT" { 0 }
            else { -1 }
        }
        _ => -1,
    }
}

fn first_argument_is_number(command: &str) -> bool {
    if command.is_empty() {
        return false;
    }
    let operation = &command[1..];
    if operation == "PUSH" || operation == "PUSH_FRONT" || operation == "PUSH_BACK" || operation == "FIND" || operation == "REMOVE" {
        false
    } else {
        true
    }
}

fn parse_query(text: &str) -> Option<Query> {
    let mut parts = text.split_whitespace();
    let command = parts.next()?.to_string();
    let name = parts.next()?.to_string();

    let count = argument_count(&command);
    if count == -1 {
        return None;
    }

    let mut query = Query {
        command,
        name,
        number: 0,
        value: String::new(),
    };

    if count >= 1 {
        let token = parts.next()?;
        if first_argument_is_number(&query.command) {
            query.number = token.parse().ok()?;
        } else {
            query.value = token.to_string();
        }
    }
    if count == 2 {
        query.value = parts.next()?.to_string();
    }

    if parts.next().is_some() {
        return None;
    }

    Some(query)
}

fn can_create(command: &str) -> bool {
    command.contains("PUSH") || command.contains("INIT")
}

fn read_file(filename: &str, query: &Query) -> Result<(Vec<String>, Vec<String>, i32, char), ()> {
    let mut position = -1;
    let mut type_char = if query.command == "PRINT" { '\0' } else { query.command.chars().next().unwrap() };
    
    let path = Path::new(filename);
    let mut lines = Vec::new();
    let mut values = Vec::new();

    if path.exists() {
        let content = fs::read_to_string(path).map_err(|_| ())?;
        for (idx, line) in content.lines().enumerate() {
            let mut row = line.split_whitespace();
            if let (Some(saved_type), Some(saved_name)) = (row.next(), row.next()) {
                if saved_name == query.name {
                    if position != -1 || saved_type.len() != 1 {
                        return Err(());
                    }
                    let s_char = saved_type.chars().next().unwrap();
                    if query.command != "PRINT" && s_char != type_char {
                        return Err(());
                    }
                    type_char = s_char;
                    position = idx as i32;
                    for val in row {
                        values.push(val.to_string());
                    }
                }
            }
            lines.push(line.to_string());
        }
    }

    if position == -1 && !can_create(&query.command) {
        return Err(());
    }
    if !"MFLQS".contains(type_char) {
        return Err(());
    }

    Ok((lines, values, position, type_char))
}

fn load_structure(type_char: char, values: &[String], arr: &mut Array, singly: &mut SinglyLinkedList, doubly: &mut DoublyLinkedList, queue: &mut Queue, stack: &mut Stack) -> bool {
    match type_char {
        'M' => {
            *arr = Array::init(100);
            for v in values {
                if arr.push_back(v.clone()).is_err() {
                    return false;
                }
            }
        }
        'F' => {
            *singly = SinglyLinkedList::init();
            for v in values {
                singly.push_back(v.clone());
            }
        }
        'L' => {
            *doubly = DoublyLinkedList::init();
            for v in values {
                doubly.push_back(v.clone());
            }
        }
        'Q' => {
            *queue = Queue::init();
            for v in values {
                queue.push(v.clone());
            }
        }
        'S' => {
            *stack = Stack::init();
            for v in values {
                stack.push(v.clone());
            }
        }
        _ => return false,
    }
    true
}

fn execute_command(type_char: char, query: &Query, arr: &mut Array, singly: &mut SinglyLinkedList, doubly: &mut DoublyLinkedList, queue: &mut Queue, stack: &mut Stack) -> Result<bool, String> {
    let cmd = &query.command;
    let mut changed = false;

    match type_char {
        'M' => {
            if cmd == "MINIT" { *arr = Array::init(100); changed = true; }
            else if cmd == "MPUSH" { arr.push_back(query.value.clone()).map_err(|e| e.to_string())?; changed = true; }
            else if cmd == "MGET" { 
                let val = arr.get(query.number).map_err(|e| e.to_string())?;
                println!("-> {}", val); 
            }
            else if cmd == "MSET" || cmd == "MREPLACE" { 
                arr.replace(query.number, query.value.clone()).map_err(|e| e.to_string())?; 
                changed = true; 
            }
            else if cmd == "MLENGTH" { println!("-> {}", arr.length()); }
            else if cmd == "MPRINT" { arr.print(); }
            else { return Err("Unknown command".into()); }
        }
        'F' => {
            if cmd == "FINIT" { *singly = SinglyLinkedList::init(); changed = true; }
            else if cmd == "FPUSH_FRONT" { singly.push_front(query.value.clone()); changed = true; }
            else if cmd == "FPUSH_BACK" { singly.push_back(query.value.clone()); changed = true; }
            else if cmd == "FPOP_FRONT" { singly.pop_front(); changed = true; }
            else if cmd == "FPOP_BACK" { singly.pop_back(); changed = true; }
            else if cmd == "FPRINT" { singly.print(); }
            else { return Err("Unknown command".into()); }
        }
        'L' => {
            if cmd == "LINIT" { *doubly = DoublyLinkedList::init(); changed = true; }
            else if cmd == "LPUSH_FRONT" { doubly.push_front(query.value.clone()); changed = true; }
            else if cmd == "LPUSH_BACK" { doubly.push_back(query.value.clone()); changed = true; }
            else if cmd == "LPOP_FRONT" { doubly.pop_front(); changed = true; } // Примечание: убедитесь, что pop_front реализован в doubly_linked_list.rs при необходимости
            else if cmd == "LPOP_BACK" { doubly.pop_back(); changed = true; }   // Аналогично для pop_back
            else if cmd == "LPRINT" { doubly.print_forward(); }
            else { return Err("Unknown command".into()); }
        }
        'Q' => {
            if cmd == "QINIT" { *queue = Queue::init(); changed = true; }
            else if cmd == "QPUSH" { queue.push(query.value.clone()); changed = true; }
            else if cmd == "QPOP" { 
                queue.pop().map_err(|e| e.to_string())?; 
                changed = true; 
            }
            else if cmd == "QPRINT" { queue.print(); }
            else { return Err("Unknown command".into()); }
        }
        'S' => {
            if cmd == "SINIT" { *stack = Stack::init(); changed = true; }
            else if cmd == "SPUSH" { stack.push(query.value.clone()); changed = true; }
            else if cmd == "SPOP" { 
                stack.pop().map_err(|e| e.to_string())?; 
                changed = true; 
            }
            else if cmd == "SPRINT" { stack.print(); }
            else { return Err("Unknown command".into()); }
        }
        _ => return Err("Unknown type".into()),
    }

    Ok(changed)
}

fn structure_to_string(type_char: char, arr: &Array, singly: &SinglyLinkedList, doubly: &DoublyLinkedList, queue: &Queue, stack: &Stack) -> String {
    let mut out = String::new();
    match type_char {
        'M' => {
            for i in 0..arr.length() {
                if let Ok(val) = arr.get(i) {
                    out.push_str(&format!("{} ", val));
                }
            }
        }
        'F' => {
            let mut curr = singly.head.as_ref();
            while let Some(node) = curr {
                out.push_str(&format!("{} ", node.data));
                curr = node.next.as_ref();
            }
        }
        'L' => {
            let mut curr = doubly.head.clone();
            while let Some(node) = curr {
                out.push_str(&format!("{} ", node.borrow().data));
                curr = node.borrow().next.clone();
            }
        }
        'Q' => {
            let mut curr = queue.head.as_ref();
            while let Some(node) = curr {
                out.push_str(&format!("{} ", node.data));
                curr = node.next.as_ref();
            }
        }
        'S' => {
            let mut curr = stack.head.as_ref();
            while let Some(node) = curr {
                out.push_str(&format!("{} ", node.data));
                curr = node.next.as_ref();
            }
        }
        _ => {}
    }
    out.trim_end().to_string()
}

fn write_file(filename: &str, lines: &mut Vec<String>, position: i32, type_char: char, name: &str, struct_data: &str) -> io::Result<()> {
    let updated = format!("{} {} {}", type_char, name, struct_data);
    if position == -1 {
        lines.push(updated);
    } else {
        lines[position as usize] = updated;
    }
    
    let mut output = fs::File::create(filename)?;
    for line in lines {
        writeln!(output, "{}", line)?;
    }
    Ok(())
}

fn main() {
    let (filename, text) = match parse_arguments() {
        Some(args) => args,
        None => {
            eprintln!("Формат: ./dbms --file <file.data> --query '<COMMAND> <name> [args]'");
            std::process::exit(1);
        }
    };

    let query = match parse_query(&text) {
        Some(q) => q,
        None => {
            eprintln!("Ошибка: Неизвестная команда или неверные аргументы.");
            std::process::exit(1);
        }
    };

    let (mut lines, values, position, type_char) = match read_file(&filename, &query) {
        Ok(res) => res,
        Err(_) => {
            eprintln!("Ошибка чтения файла данных.");
            std::process::exit(1);
        }
    };

    let mut current_array = Array::init(100);
    let mut singly_list = SinglyLinkedList::init();
    let mut doubly_list = DoublyLinkedList::init();
    let mut current_queue = Queue::init();
    let mut current_stack = Stack::init();

    if !load_structure(type_char, &values, &mut current_array, &mut singly_list, &mut doubly_list, &mut current_queue, &mut current_stack) {
        eprintln!("Ошибка инициализации структуры данных.");
        std::process::exit(1);
    }

    let changed = match execute_command(type_char, &query, &mut current_array, &mut singly_list, &mut doubly_list, &mut current_queue, &mut current_stack) {
        Ok(res) => res,
        Err(e) => {
            eprintln!("Error: {}", e);
            std::process::exit(1);
        }
    };

    if changed {
        let struct_str = structure_to_string(type_char, &current_array, &singly_list, &doubly_list, &current_queue, &current_stack);
        if write_file(&filename, &mut lines, position, type_char, &query.name, &struct_str).is_err() {
            eprintln!("Ошибка выполнения команды или записи в файл.");
            std::process::exit(1);
        }
    }
}
use std::rc::{Rc, Weak};
use std::cell::RefCell;

pub struct Node {
    pub data: String,
    pub next: Option<Rc<RefCell<Node>>>,
    pub prev: Option<Weak<RefCell<Node>>>,
}

pub struct DoublyLinkedList {
    pub head: Option<Rc<RefCell<Node>>>,
    pub tail: Option<Rc<RefCell<Node>>>,
}

impl DoublyLinkedList {
    pub fn init() -> Self {
        DoublyLinkedList {
            head: None,
            tail: None,
        }
    }

    pub fn push_front(&mut self, value: String) {
        let new_node = Rc::new(RefCell::new(Node {
            data: value,
            next: self.head.clone(),
            prev: None,
        }));

        if let Some(old_head) = &self.head {
            old_head.borrow_mut().prev = Some(Rc::downgrade(&new_node));
        } else {
            self.tail = Some(new_node.clone());
        }
        self.head = Some(new_node);
    }

    pub fn push_back(&mut self, value: String) {
        let new_node = Rc::new(RefCell::new(Node {
            data: value,
            next: None,
            prev: self.tail.as_ref().map(Rc::downgrade),
        }));

        if let Some(old_tail) = &self.tail {
            old_tail.borrow_mut().next = Some(new_node.clone());
        } else {
            self.head = Some(new_node.clone());
        }
        self.tail = Some(new_node);
    }

    pub fn insert_after(&mut self, target: &Rc<RefCell<Node>>, value: String) {
        let is_tail = match &self.tail {
            Some(t) => Rc::ptr_eq(t, target),
            None => false,
        };

        if is_tail {
            self.push_back(value);
            return;
        }

        let next_node_opt = target.borrow().next.clone();
        if let Some(next_node) = next_node_opt {
            let new_node = Rc::new(RefCell::new(Node {
                data: value,
                next: Some(next_node.clone()),
                prev: Some(Rc::downgrade(target)),
            }));

            next_node.borrow_mut().prev = Some(Rc::downgrade(&new_node));
            target.borrow_mut().next = Some(new_node);
        }
    }

    pub fn insert_before(&mut self, target: &Rc<RefCell<Node>>, value: String) {
        let is_head = match &self.head {
            Some(h) => Rc::ptr_eq(h, target),
            None => false,
        };

        if is_head {
            self.push_front(value);
            return;
        }

        let prev_weak_opt = target.borrow().prev.clone();
        if let Some(prev_weak) = prev_weak_opt {
            if let Some(prev_node) = prev_weak.upgrade() {
                let new_node = Rc::new(RefCell::new(Node {
                    data: value,
                    next: Some(target.clone()),
                    prev: Some(Rc::downgrade(&prev_node)),
                }));

                prev_node.borrow_mut().next = Some(new_node.clone());
                target.borrow_mut().prev = Some(Rc::downgrade(&new_node));
            }
        }
    }

    pub fn pop_front(&mut self) {
        if let Some(old_head) = self.head.take() {
            let next_opt = old_head.borrow().next.clone();
            self.head = next_opt;

            if let Some(new_head) = &self.head {
                new_head.borrow_mut().prev = None;
            } else {
                self.tail = None;
            }
        }
    }

    pub fn pop_back(&mut self) {
        if let Some(old_tail) = self.tail.take() {
            let prev_weak_opt = old_tail.borrow().prev.clone();
            
            if let Some(prev_weak) = prev_weak_opt {
                if let Some(prev_node) = prev_weak.upgrade() {
                    prev_node.borrow_mut().next = None;
                    self.tail = Some(prev_node);
                } else {
                    self.head = None;
                }
            } else {
                self.head = None;
            }
        }
    }

    pub fn find(&self, value: &str) -> Option<Rc<RefCell<Node>>> {
        let mut curr = self.head.clone();
        while let Some(node) = curr {
            if node.borrow().data == value {
                return Some(node);
            }
            curr = node.borrow().next.clone();
        }
        None
    }

    pub fn remove_by_value(&mut self, value: &str) -> bool {
        let target = match self.find(value) {
            Some(t) => t,
            None => return false,
        };

        let is_head = self.head.as_ref().map_or(false, |h| Rc::ptr_eq(h, &target));
        let is_tail = self.tail.as_ref().map_or(false, |t| Rc::ptr_eq(t, &target));

        if is_head {
            self.pop_front();
        } else if is_tail {
            self.pop_back();
        } else {
            let prev_opt = target.borrow().prev.as_ref().and_then(|w| w.upgrade());
            let next_opt = target.borrow().next.clone();

            if let (Some(prev), Some(next)) = (prev_opt, next_opt) {
                prev.borrow_mut().next = Some(next.clone());
                next.borrow_mut().prev = Some(Rc::downgrade(&prev));
            }
        }
        true
    }

    pub fn print_forward(&self) {
        print!("List (forward): [ ");
        let mut curr = self.head.clone();
        while let Some(node) = curr {
            print!("\"{}\" ", node.borrow().data);
            curr = node.borrow().next.clone();
        }
        println!("]");
    }

    pub fn print_backward(&self) {
        print!("List (backward): [ ");
        let mut curr = self.tail.clone();
        while let Some(node) = curr {
            print!("\"{}\" ", node.borrow().data);
            curr = node.borrow().prev.as_ref().and_then(|w| w.upgrade());
        }
        println!("]");
    }

    pub fn get_front(&self) -> Result<String, &'static str> {
        match &self.head {
            Some(node) => Ok(node.borrow().data.clone()),
            None => Err("List is empty"),
        }
    }

    pub fn get_back(&self) -> Result<String, &'static str> {
        match &self.tail {
            Some(node) => Ok(node.borrow().data.clone()),
            None => Err("List is empty"),
        }
    }
}
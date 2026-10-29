pub struct Node {
    pub data: String,
    pub next: Option<Box<Node>>,
}

pub struct SinglyLinkedList {
    pub head: Option<Box<Node>>,
}

impl SinglyLinkedList {
    pub fn init() -> Self {
        SinglyLinkedList { head: None }
    }

    pub fn push_front(&mut self, value: String) {
        let new_node = Box::new(Node {
            data: value,
            next: self.head.take(),
        });
        self.head = Some(new_node);
    }

    pub fn push_back(&mut self, value: String) {
        let new_node = Box::new(Node {
            data: value,
            next: None,
        });

        if self.head.is_none() {
            self.head = Some(new_node);
            return;
        }

        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            if node.next.is_none() {
                node.next = Some(new_node);
                break;
            }
            curr = node.next.as_mut();
        }
    }

    pub fn pop_front(&mut self) {
        if let Some(old_head) = self.head.take() {
            self.head = old_head.next;
        }
    }

    pub fn pop_back(&mut self) {
        if self.head.is_none() {
            return;
        }

        if self.head.as_ref().unwrap().next.is_none() {
            self.head = None;
            return;
        }

        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            if let Some(ref next_node) = node.next {
                if next_node.next.is_none() {
                    node.next = None;
                    break;
                }
            }
            curr = node.next.as_mut();
        }
    }

    pub fn print(&self) {
        print!("List: [ ");
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            print!("\"{}\" ", node.data);
            curr = node.next.as_ref();
        }
        println!("]");
    }

    pub fn get_front(&self) -> Result<&String, &'static str> {
        match &self.head {
            Some(node) => Ok(&node.data),
            None => Err("List is empty"),
        }
    }

    pub fn get_back(&self) -> Result<&String, &'static str> {
        if self.head.is_none() {
            return Err("List is empty");
        }
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            if node.next.is_none() {
                return Ok(&node.data);
            }
            curr = node.next.as_ref();
        }
        Err("List is empty")
    }
}
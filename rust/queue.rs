pub struct Node {
    pub data: String,
    pub next: Option<Box<Node>>,
}

pub struct Queue {
    pub head: Option<Box<Node>>,
}

impl Queue {
    pub fn init() -> Self {
        Queue { head: None }
    }

    pub fn push(&mut self, value: String) {
        let new_node = Some(Box::new(Node {
            data: value,
            next: None,
        }));

        if self.head.is_none() {
            self.head = new_node;
        } else {
            let mut curr = self.head.as_mut();
            while let Some(node) = curr {
                if node.next.is_none() {
                    node.next = new_node;
                    break;
                }
                curr = node.next.as_mut();
            }
        }
    }

    pub fn pop(&mut self) -> Result<(), &'static str> {
        if let Some(old_head) = self.head.take() {
            self.head = old_head.next;
            Ok(())
        } else {
            Err("Cannot pop from empty queue")
        }
    }

    pub fn print(&self) {
        print!("Queue: [ ");
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            print!("\"{}\" ", node.data);
            curr = node.next.as_ref();
        }
        println!(" ]");
    }
}
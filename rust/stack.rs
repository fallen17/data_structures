pub struct Node {
    pub data: String,
    pub next: Option<Box<Node>>,
}

pub struct Stack {
    pub head: Option<Box<Node>>,
}

impl Stack {
    pub fn init() -> Self {
        Stack { head: None }
    }

    pub fn push(&mut self, value: String) {
        let new_node = Box::new(Node {
            data: value,
            next: self.head.take(),
        });
        self.head = Some(new_node);
    }

    pub fn pop(&mut self) -> Result<String, &'static str> {
        match self.head.take() {
            Some(old_head) => {
                self.head = old_head.next;
                println!("{} was deleted", old_head.data);
                Ok(old_head.data)
            }
            None => Err("Cannot pop from empty stack"),
        }
    }

    pub fn print(&self) {
        print!("Stack: [ ");
        let mut current = self.head.as_ref();
        while let Some(node) = current {
            print!("\"{}\" ", node.data);
            current = node.next.as_ref();
        }
        println!("]");
    }
}
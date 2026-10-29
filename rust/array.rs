pub struct Array {
    data: Vec<String>,
    size: usize,
    capacity: usize,
}

impl Array {
    pub fn init(fixed_capacity: usize) -> Self {
        Array {
            data: Vec::with_capacity(fixed_capacity),
            size: 0,
            capacity: fixed_capacity,
        }
    }

    pub fn push_back(&mut self, value: String) -> Result<(), &'static str> {
        if self.size >= self.capacity {
            return Err("Array overflow: cannot add to full array");
        }
        if self.data.len() <= self.size {
            self.data.push(value);
        } else {
            self.data[self.size] = value;
        }
        self.size += 1;
        Ok(())
    }

    pub fn get(&self, index: usize) -> Result<&String, &'static str> {
        if index >= self.size {
            return Err("Index out of range");
        }
        Ok(&self.data[index])
    }

    pub fn replace(&mut self, index: usize, value: String) -> Result<(), &'static str> {
        if index >= self.size {
            return Err("Index out of range");
        }
        self.data[index] = value;
        Ok(())
    }

    pub fn length(&self) -> usize {
        self.size
    }

    pub fn max_size(&self) -> usize {
        self.capacity
    }

    pub fn print(&self) {
        print!("[");
        for i in 0..self.size {
            print!("\"{}\"{}", self.data[i], if i + 1 < self.size { ", " } else { " " });
        }
        println!("] (size: {}, max_capacity: {})", self.size, self.capacity);
    }
}
fn find(haystack: &str, needle: char) -> Option<usize> {
    for (offset, c) in haystack.char_indices() {
        if c == needle {
            return Some(offset);
        }
    }
    None
}

fn guess(n: i32) -> bool {
    if n < 1 || n > 10 {
        panic!("invalid number: {}", n);
    }
    n == 5
}

fn main() {
    let file_name = "foobar.rs";
    match find(file_name, '.') {
        None => println!("No file extension found."),
        Some(i) =>  println!("File extension: {}", &file_name[i+1..]),
    }
    guess(11);
}

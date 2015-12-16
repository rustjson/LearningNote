extern crate language;

use language::{english, french, chinese};

fn main() {
    println!("Hello in English: {}", english::greetings::hello());
    println!("Hello in French: {}", french::greetings::hello());
    println!("Hello in Chinese: {}", chinese::greetings::hello());

    println!("Goodbye in English: {}", english::farewells::goodbye());
    println!("Goodbye in French: {}", french::farewells::goodbye());
    println!("Goodbye in Chinese: {}", chinese::farewells::goodbye());
}

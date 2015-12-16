extern crate phrases;

fn main() {
    println!("Hello in English: {}", phrases::english::greetings::hello());
    println!("Hello in French: {}", phrases::french::greetings::hello());
    println!("Hello in Chinese: {}", phrases::chinese::greetings::hello());

    println!("Goodbye in English: {}", phrases::english::farewells::goodbye());
    println!("Goodbye in French: {}", phrases::french::farewells::goodbye());
    println!("Goodbye in Chinese: {}", phrases::chinese::farewells::goodbye());
}

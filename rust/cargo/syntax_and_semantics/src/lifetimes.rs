pub fn main() {
    println!("---------------------lifetimes-------------------");

    let a = 14;
    bar(&a);
    foo(&a);

    let y = &5;
    let f = Foo{ x: y};

    println!("x is :{}", f.x());

    let some_string = "Hello, world";
    let s = &some_string;
    //some_string[0..1] = 'x';
    println!("{}", &some_string[0..1]);
    let chx = 'x';
    println!("{}", chx.is_digit(10));
}

// implicit
fn foo(x: &i32) {

}

// explicit
//The 'a reads ‘the lifetime a’.
//Technically, every reference has some lifetime associated with it
fn bar<'a>(x: &'a i32) {

}

struct Foo<'a> {
    x: &'a i32,
}

impl<'a> Foo<'a> {
    fn x (&self) -> &'a i32 { self.x}
}


struct Ab<'b> {
    x: &'b i32,
}

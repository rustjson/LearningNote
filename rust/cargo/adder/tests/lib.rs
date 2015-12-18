extern crate adder;

#[test]
fn it_works() {
    assert!(true);
}

#[test]
#[should_panic]
fn it_panic() {
    assert!(false);
}

#[test]
#[should_panic]
fn it_equals() {
    assert_eq!("Hello", "world");
}

#[test]
#[should_panic(expected = "assertion failed")]
fn it_panic_with_assert_fail() {
    assert_eq!("Hello", "world");
}

#[test]
fn test_add_two() {
    assert_eq!(4, adder::add_two(2));
}

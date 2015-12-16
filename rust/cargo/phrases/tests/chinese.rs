
//use language::chinese;
use language::{english, french, chinese};

#[test]
fn test_hello() {
    assert_eq!("你好!", chinese::greetings::hello());
}

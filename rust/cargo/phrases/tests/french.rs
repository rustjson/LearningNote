
//use language::chinese;
use language::{english, french, chinese};

#[test]
fn test_hello() {
    assert_eq!("bonjour!", french::greetings::hello());
}

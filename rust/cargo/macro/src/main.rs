
macro_rules! vec  {
    ( $( $y:expr ),* )  => (
        {
            let mut temp_vec = Vec::new();
            $(
                temp_vec.push($y + 1);
            )*
            temp_vec
        }
    );
}

macro_rules! foo {
    //(x => $x:expr, y => $y:expr) => (println!("mode X: {}, Y: {}", $x, $y));
    //(y => $e:expr) => (println!("mode Y: {}", $e));
    ( $( $var:expr => $val:expr ), * ) => (
        {
            $(
                println!("var {:?} is {:?}", $var, $val);
            )*
        }
    )
}


macro_rules! o_O {
    (
        $( $x:expr; [$($y:expr),*]).*
    ) =>
    {
        &[ $( $($x + $y),*),*];
    }

}

macro_rules! write_html {
    ($w:expr, ) => (());

    ($w:expr, $e:tt) => (write!($w, "{}", $e));

    ($w:expr, $tag:ident [ $($inner:tt)* ] $($rest:tt)*) => {{
        write!($w, "<{}>", stringify!($tag));
        write_html!($w, $($inner)*);
        write!($w, "</{}>", stringify!($tag));
        write_html!($w, $($rest)*);
    }};
}

fn main() {
    use std::fmt::Write;
    let mut out = String::new();

    //let x: Vec<u64> = vec!(1, 2, 33333333333333);
    //println!("x is {:?}", x);

    //foo!('x' => 3 : 'u' => 4);

    let a: &[i32]
        = o_O!(10; [1, 2, 3].
               20; [4, 5, 6].
               20; [7, 8, 9]);

    println!("{:?}", a);



    write_html!(&mut out,
        html[
            head[title["Macros guide"]]
            body[h1["Macros are the best!"]]
        ]);

    println!("{}", out);
}

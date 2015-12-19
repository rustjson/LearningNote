pub fn main() {
    let v = vec!(1, 2, 3);
    //the vector also allocates space on *the heap*
    //when v goes out of sccope at the the of this function,
    //Rust will clean up everything relateed to the vector, even
    //the heap-allocated memory.

    let v2 = &v;//Withou this borrow. it will panic
    println!("$v[0] is {}", v[0]);

    take(v2);
    take(&v);
    //using v2 or &v, otherwise, panic too;

    println!("$v[0] is {}", v[0]);

    //But
    let vint = 1;

    let vint2 = vint;//No need for borrowing

    println!("vint is: {}", vint);

    mut_example();
    iterator_invalidation();
}

fn take(v: &Vec<i32>) {

}

fn mut_example() {
    let mut x = 5;
    {
        let y = &mut x;
        *y += 1;

        //let z = &x;//panic cannot borrow x as immutable bacause ....
        //let z = x;//panic cannot use x bcause it was borrowed
    }
    x += 4;
    //you may have one or the other of these two kinds of borrows, but not both at the same time:

    //one or more references (&T) to a resource,
    //exactly one mutable reference (&mut T).

    println!("mutable x is {}", x);
}

#[derive(Debug)]
struct People {
    size: i32,
    height: i32,
}

fn iterator_invalidation() {

    let mut v = vec!(People{size:0,height:1}, People{size:0,height:2}, People{size:0,height:3});

    for i in &v {

        //v.push(People{size:1,height:8});
        //panic cannot borrow v as mutable because it is also
        //borrowed as immutable

        println!("{:?}", i);
    }

    {
        let mut x = &mut v[1];
        x.size = 100;
        //Why cannot change if v = vec![0,2,3];
        println!("x is {:?}", x);
    }
    v.push(People{size:0,height:4});
    println!("now v is {:?}", v);
    //v[2]
}

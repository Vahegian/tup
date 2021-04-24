#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use]
extern crate rocket;

use rocket::State;
use std::{
    env,
    sync::{Arc, Mutex},
};

mod coord_gen;
#[path = "utils/json_parser.rs"]
mod json_parser;
#[derive(Debug)]
struct TupStateString {
    pub coords: Arc<Mutex<String>>,
}

#[get("/tup")]
fn index(coords: State<TupStateString>) -> String {
    let counter = Arc::clone(&coords.coords);
    let data = counter.lock().unwrap();
    data.clone()
}

#[tokio::main]
async fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len().ne(&2) {
        println!("Please specify the path to config.json file");
        return;
    }

    let coins = json_parser::parse_tup_config(&args[1]);

    let state = TupStateString {
        coords: Arc::new(Mutex::new(String::from(""))),
    };
    let counter = Arc::clone(&state.coords);
    tokio::spawn(async move {
        // println!("{:?}", counter.lock().unwrap());
        coord_gen::coin_update_loop(&coins, counter).await;
    });

    rocket::ignite()
        .manage(state)
        .mount("/", routes![index])
        .launch();
}

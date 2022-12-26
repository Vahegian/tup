#[macro_use] extern crate rocket;

use rocket::State;
use std::{
    env,
    sync::{Arc, Mutex}, collections::HashMap,
};

mod coord_gen;
#[path = "utils/json_parser.rs"]
mod json_parser;
#[derive(Debug)]
struct TupStateString {
    pub coords: Arc<Mutex<String>>,
    pub coins: HashMap<String, String>
}

#[get("/tup")]
fn index(coords: &State<TupStateString>) -> String {
    let counter = Arc::clone(&coords.coords);
    let data = counter.lock().unwrap();
    data.clone()
}

#[rocket::main]
async fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len().ne(&2) {
        println!("Please specify the path to config.json file");
        return;
    }

    let coins = json_parser::parse_tup_config(&args[1]);

    let state = TupStateString {
        coords: Arc::new(Mutex::new(String::from(""))),
        coins
    };

    let _ = rocket::build()
        .manage(state)
        .mount("/", routes![index])
        .attach(coord_gen::CoordGen)
        .launch().await;
}

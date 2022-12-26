#[macro_use]
extern crate rocket;

use rocket::State;
use std::{
    collections::HashMap,
    env,
    sync::{Arc, Mutex},
};

mod coord_gen;
mod cors;

mod json_parser;
#[derive(Debug)]
struct TupStateString {
    pub coords: Arc<Mutex<String>>,
    pub coins: HashMap<String, String>,
}

#[get("/tup")]
fn index(coords: &State<TupStateString>) -> String {
    let counter = Arc::clone(&coords.coords);
    let data = counter.lock().unwrap();
    data.clone()
}

#[get("/hi")]
async fn hi() -> String {
    "Hi back".to_owned()
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
        coins,
    };

    let _ = rocket::build()
        .mount("/", routes![index, hi])
        .manage(state)
        .attach(coord_gen::CoordGen)
        .attach(cors::CORS)
        .launch()
        .await;
}

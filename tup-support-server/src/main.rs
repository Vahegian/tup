#[macro_use]
extern crate rocket;

use rocket::{tokio::sync::Mutex, State};
use std::{collections::HashMap, env, sync::Arc};

mod coord_gen;
mod cors;

mod json_parser;
#[derive(Debug)]
struct TupStateString {
    pub coords: Arc<Mutex<String>>,
    pub coins: HashMap<String, String>,
}

#[get("/tup")]
async fn index(coords: &State<TupStateString>) -> String {
    let counter = coords.coords.clone();
    let data = counter.lock().await;
    data.clone()
}

#[get("/ping")]
async fn ping() -> &'static str {
    "Alive"
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
        .mount("/", routes![index, ping])
        .manage(state)
        .attach(coord_gen::CoordGen)
        .attach(cors::CORS)
        .launch()
        .await;
}

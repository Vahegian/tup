#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use] extern crate rocket;

// use tokio::time::sleep;
use rocket::State;
use std::{env, fmt::format, sync::{Arc, Mutex}, thread, time::{Duration}};
#[path ="utils/json_parser.rs"] mod json_parser;

#[derive(Debug)]
struct TUPSTRING{
    pub coords: Arc<Mutex<String>>
}

// struct CoinGeckoResp{
//     pub 
// }

// static TUP_STRING:TUPSTRING = TUPSTRING{coords:Arc::new(Mutex::new((String::from(""))))};


#[get("/")]
fn index(coords: State<TUPSTRING>) -> String {
    let counter = Arc::clone(&coords.coords);
    let data = counter.lock().unwrap();
    data.clone()
}

async fn coin_update_loop(config_path: &String, counter: Arc<Mutex<String>>){
    // let mut i = 0;
    let client = reqwest::Client::builder()
        .build().expect("Error creating a client");
    loop{
        thread::sleep(Duration::from_secs(65));
        // i+=1;
        
        let res = client
            .get(format!("https://api.coingecko.com/api/v3/simple/price?ids={}&vs_currencies=usd&include_24hr_change=true", config_path))
            .send()
            .await.expect("Error getting coingecko response");

        let ip = res
            .json::<serde_json::Value>()
            .await.expect("Error Deserializing coingecko response");
        let mut data = counter.lock().unwrap();
        *data = format!("{}-{:#?}", config_path, ip);
        println!("{}", data);
    }
}

#[tokio::main]
async fn main() {

    let args: Vec<String> = env::args().collect();
    if args.len().ne(&2) {
        println!("Please specify the path to config.json file");
        return;
    }

    let coins = json_parser::parse_tup_config(&args[1]);

    let state = TUPSTRING{coords:Arc::new(Mutex::new(String::from("")))};
    let counter = Arc::clone(&state.coords);
    tokio::spawn( async move {
        // println!("{:?}", counter.lock().unwrap());
        coin_update_loop(&coins.coin_tickers, counter).await;
    });

    rocket::ignite()
    .manage(state)
    .mount("/", routes![index]).launch();
}

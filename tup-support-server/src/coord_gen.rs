use std::{
    collections::HashMap,
    sync::{Arc, Mutex},
    thread,
    time::Duration,
};
use reqwest::Error;
use rocket::{fairing::{Fairing, Kind, Info}, Rocket, Orbit};

async fn get_one_day_changes(coin_string: &String) -> Result<serde_json::Value, Error> {
    let client = reqwest::Client::builder()
        .build()
        .expect("Error creating a client");

    let res = client
            .get(format!("https://api.coingecko.com/api/v3/simple/price?ids={}&vs_currencies=usd&include_24hr_change=true", coin_string))
            .send()
            .await?;

    res.json::<serde_json::Value>().await
}

fn update_tup_state(
    selected_coins: &HashMap<String, String>,
    coin_data: serde_json::Value,
    counter: &Arc<Mutex<String>>,
) {
    let mut coords = String::from("");
    for key in selected_coins.keys() {
        let mut coord = selected_coins[key].clone();
        if coin_data[key]["usd_24h_change"].is_f64() {
            let one_day_change = match coin_data[key]["usd_24h_change"]
                .as_f64(){
                    Some(val) => val,
                    None => 0.0
                };
                
            if one_day_change < 0.0 {
                coord = format!("{},{}", coord, 0);
            } else if one_day_change > 0.0{
                coord = format!("{},{}", coord, 1);
            } else {
                coord = format!("{},{}", coord, 2);
            }

            coords = format!("{}{}:", coords, coord);
        }
    }
    let mut data = counter.lock().unwrap();
    *data = coords;
}

pub async fn coin_update_loop(
    selected_coins: &HashMap<String, String>,
    counter: Arc<Mutex<String>>,
) {
    let mut coin_string = String::from("");
    for (i, key) in selected_coins.keys().enumerate() {
        if i == 0 {
            coin_string = key.to_string();
            continue;
        }
        coin_string = format!("{},{}", coin_string, key);
    }
    loop {
        let coin_data = get_one_day_changes(&coin_string).await;
        let coin_data = match coin_data {
            Ok(val) => val,
            Err(e) => {
                println!("{e}");
                continue
            }
        };
        update_tup_state(selected_coins, coin_data, &counter);
        // println!("Updated prices {}", Utc::now());
        thread::sleep(Duration::from_secs(60 * 5));
    }
}

#[derive(Debug, Clone)]
pub struct CoordGen;

#[rocket::async_trait]
impl Fairing for CoordGen {
    fn info(&self) -> Info {
        Info {
            name: "Get token coords",
            kind: Kind::Liftoff,
        }
    }

    async fn on_liftoff(&self, rocket: &Rocket<Orbit>) {
        // let shutdown = rocket.shutdown();
        let providers = rocket.state::<crate::TupStateString>().unwrap().clone();
        coin_update_loop(&providers.coins, providers.coords.clone()).await

    }
}

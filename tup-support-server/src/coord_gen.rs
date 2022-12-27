use reqwest::Error;
use rocket::{
    fairing::{Fairing, Info, Kind},
    tokio::{self, sync::Mutex},
    Orbit, Rocket,
};
use std::{collections::HashMap, sync::Arc};

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

async fn update_tup_state(
    selected_coins: &HashMap<String, String>,
    coin_data: serde_json::Value,
    counter: &Arc<Mutex<String>>,
) {
    let mut coords = String::from("");
    for key in selected_coins.keys() {
        let mut coord = selected_coins[key].clone();
        if coin_data[key]["usd_24h_change"].is_f64() {
            let one_day_change = match coin_data[key]["usd_24h_change"].as_f64() {
                Some(val) => val,
                None => 0.0,
            };

            if one_day_change < 0.0 {
                coord = format!("{},{}", coord, 0);
            } else if one_day_change > 0.0 {
                coord = format!("{},{}", coord, 1);
            } else {
                coord = format!("{},{}", coord, 2);
            }

            coords = format!("{}{}:", coords, coord);
        }
    }
    {
        let mut data = counter.lock().await;
        *data = coords;
    }
    // println!("release")
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
                error!("{e}");
                continue;
            }
        };
        update_tup_state(selected_coins, coin_data, &counter).await;
        // println!("Updated prices {}", coin_string);
        tokio::time::sleep(tokio::time::Duration::from_secs(60 * 5)).await;
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
        let (coins, coords) = (providers.coins.clone(), providers.coords.clone());
        tokio::spawn(async move { coin_update_loop(&coins, coords).await });
    }
}

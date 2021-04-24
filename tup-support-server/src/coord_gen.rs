use std::{
    collections::HashMap,
    sync::{Arc, Mutex},
    thread,
    time::Duration,
};

async fn get_one_day_changes(client: &reqwest::Client, coin_string: &String) -> serde_json::Value {
    let res = client
            .get(format!("https://api.coingecko.com/api/v3/simple/price?ids={}&vs_currencies=usd&include_24hr_change=true", coin_string))
            .send()
            .await.expect("Error getting coingecko response");

    res.json::<serde_json::Value>()
        .await
        .expect("Error Deserializing coingecko response")
}

fn update_tup_state(
    selected_coins: &HashMap<String, String>,
    coin_data: serde_json::Value,
    counter: &Arc<Mutex<String>>,
) {
    let mut coords = String::from("");
    for key in selected_coins.keys() {
        let one_day_change = coin_data[key]["usd_24h_change"]
            .as_f64()
            .expect("Error in Coingecko response");
        let mut coord = selected_coins[key].clone();
        if one_day_change < 0.0 {
            coord = format!("{},{}", coord, 0);
        } else {
            coord = format!("{},{}", coord, 1);
        }

        coords = format!("{}{}:", coords, coord);
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

    let client = reqwest::Client::builder()
        .build()
        .expect("Error creating a client");
    loop {
        let coin_data = get_one_day_changes(&client, &coin_string).await;
        update_tup_state(selected_coins, coin_data, &counter);
        thread::sleep(Duration::from_secs(60*5));
    }
}

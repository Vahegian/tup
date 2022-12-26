use serde::Deserialize;
use std::{collections::HashMap, io::Read};

#[derive(Debug, Deserialize)]
pub struct TupConfig {
    pub coin_tickers: HashMap<String, String>,
}

fn get_file_content(path: &String) -> String {
    let mut f =
        std::fs::File::open(path).expect(format!("error reading the file: {}", path).as_str());
    let mut content: String = String::new();
    f.read_to_string(&mut content)
        .expect("Unable to read file data");
    content
}

pub fn parse_tup_config(path: &String) -> HashMap<String, String> {
    let content = get_file_content(path);
    let deserialized_point: HashMap<String, String> =
        serde_json::from_str(&content).expect("error parsing json");
    // println!("{:?}", deserialized_point)
    deserialized_point
}

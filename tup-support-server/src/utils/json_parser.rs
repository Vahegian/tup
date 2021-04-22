use serde::{Deserialize};
use std::{io::Read};

#[derive(Debug, Deserialize)]
pub struct TupConfig {
    pub coin_tickers: String,
}

fn get_file_content(path: &String)->String{
    let mut f = std::fs::File::open(path).expect("error reading the json file");
    let mut content: String = String::new();
    f.read_to_string(&mut content).expect("Unable to read json data"); 
    content
}

pub fn parse_tup_config(path: &String) -> TupConfig{
    let content = get_file_content(path);
    let deserialized_point: TupConfig = serde_json::from_str(&content).expect("error parsing json");
    // println!("{:?}", deserialized_point)
    deserialized_point 
}
For Rocket use ```rustup override set nightly``` otherwise code will not compile

In config.json file you can specify the coins you want and their coordinates on Tup.

To run the service just type ```cargo run config.json```.
To run service in docker first execute ```./build_prod.sh``` than change to directory build and run ```./build_run_docker.sh``` result will be available at ```0.0.0.0:5000/tup```
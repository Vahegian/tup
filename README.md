# Tup

Tup is displaying 24 hour price change of top cryptos in USD.

<image src="demo/demo.gif" width="480px"/>

ESP32 is the microcontroller used for controlling the LEDs.

Shift registers are used for switching the RGB LEDs on/off. Sorry for the messy schematic, will put better one together soon. 

<image src="demo/circuit.png" width="480px"/>

The code is [here](tup-controller).

Coingecko API is used for getting the data. The API response is formatted to make it easier for the microcontroller to process the data. Code [here](tup-support-server)

All the 3d Models are available [here](3d-Models)

To print the logos correctly the front [panel](3d-Models/stl/front_panel.STL) is printed on a semi-transparent sheet.

<image src="demo/print_demo.jpg" width="480px"/>

```TODO```

* Add support for beaconcha.in API to show the states of the ETH2 validators
* Tup is a 3d object, add LEDs to the sides and the top.






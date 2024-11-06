# Question 3
#### Emmett Fitzharris | R00222357

What is the esp8266 WiFi module? 

- Give an overview of the IoT and smart home devices based on the esp8266 WiFi module. 

- Refer to firmware for such devices in your answer. e.g. 
  - tasmota,
  - esphome, 
  - mongoose OS

--- 

The ESP (Espressif) WiFi Module is a system on a chip (SOC) which can be used to add Wi-Fi to a microcontroller. It has a built int TCP/IP stack allowing the integrated microcontroller to connect to the internet. 

The controller is very low cost, (approx $7.50 on Sparkfun sold in bulk [1]). This board comes with 4MB of flash memory allowing for the storage of firmware and other data. 

The chip is based on the Tensilica Xtensa LX106 processor, which is build on the RISC architecture. This standard is used in many microcontrollers. 

The board has a number of standard GPIO pins to access features like the UART, Flash data and several Serial Peripheral Interface (SPI) pins to access specific control features[2].

As can be seen on the ESPHome (a web based control system for ESP devices) website, the ESP8266 is a very popular platform for DIY projects[3], especially when building smart home devices. Examples include, air quality sensors, Weather monitors, Cameras, Displays, and controllers for aquaponics systems.

Tasmota offers open source firmware for ESP devices such as the ESP8266. This is a community driven project that allows for the control of smart home devices. There are extensive guides on how to get started with Tasmota on their documentation page[4]. Tasmota is also capable of integrating with third party solutions, which can make it easy to tie into existing smart home systems.

Their GitHub page has a discussions page dedicated to showcasing project examples, with recent examples such as Wi-Fi connected seven segment displays, Geiger counters and smart home devices such as light switches and power outlets[5].

Mongoose OS is yet another firmware platform for the IOT devices, which supports the ESP8266. This platform notably supports not only the C programming language, but also Javascript, making the process of developing for this architecture much more accessible to a wider range of developers[6].




#### References:
1. [Sparkfun | WiFi Module - ESP8266 (4MB Flash)](https://www.sparkfun.com/products/17146)
2. [esphome | ESP8266 Platform](https://esphome.io/components/esp8266.html)
3. [esphome | DIY Examples](https://esphome.io/guides/diy.html)
4. [Tasmota | Open Source Firmware](https://tasmota.github.io/docs/Getting-Started/)
5. [Tasmota | Github Show and Tell](https://github.com/arendst/Tasmota/discussions/categories/show-and-tell?discussions_q=is%3Aopen+category%3A%22Show+and+tell%22+sort%3Atop)
6. [Mongoose OS | ESP8266](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md)
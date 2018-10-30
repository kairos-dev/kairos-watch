# Introduction

## Getting started

### BOM

### Connections

#### Display - SSD1306

* ESP32 - I2C 2
    * SCL - GPIO_4
    * SDA - GPIO_0

#### Sensors 

* ESP32 - I2C 1
    * SCL - GPIO_2
    * SDA - GPIO_15

#### Keypad

* Left Key - GPIO_14
* Enter/Home Key - GPIO_27
* Right Key - GPIO_32

#### Battery Monitor

* Charging - ?
* Read Battery Level - GPIO_33 (ADC1_CHANNEL_5)
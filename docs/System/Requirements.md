
# KairOS Watch Requirements

This file provides information about the system functionality (requirements are the statements of what a system should do).

| 	|	|
|:-:|:-:| 
| Target release | 1.0 |
| Document status | **DRAFT** |
| Document owner | [Calebe94](github.com/Calebe94) | 
| Developers | [Calebe94](github.com/Calebe94) | 

## Backgroud and strategic fit

We all know that smartwatchs is on the rise. Most of these smartwathes are proprietary, expensive and not custumizable. So our goal is to develop a smartwatch full open-source and open-hardware.

## Requirements

| # | User Story title | User story description | Priority | Notes |
|:--:|:--:|:--:|:--:|:--:|
| 1 | Watchfaces | Smartwatch needs to show watchfaces | Must Have | |
| 2 | Power On/Off| Smartwatch needs to be powered on/off | Must Have | After battery is connected, the power/home button needs to be pressed for 2 seconds to be powerd on/off |
| 3 | Smartwatch connection | Smartwatch needs to share info with a smartphone | Must Have | Bluetooth Connection with a smartphone to share info between devices | 
| 4 | Gestures Support | Gestures with accelerometer and gyroscope | Should Have | After a certain gesture the screen must be on | 
| 5 | Read Vital Signals | Read some vital signals such as Heart Beat and SPO2 with MAX30100 sensor | Must Have | Maybe use the MAX30102 sensor and have an app to read it. | 
| 6 | Read Steps | Incorporate a steps algorithm | Should Have | Must have an app to read it|
| 7 | Read Calories burned | Incorporate a calories burned algorithm | Should Have | Must have an app to read it |


## User interaction and design

| Description | Home Screen | Menu Screen |
|:-----------:|:-----------:|:-----------:|
| 	Mockup	  | ![Home](https://github.com/kairos-dev/KairosWatch/blob/KairOSWatch_Test/resources/screens/home.png?raw=true)		| ![Menu](https://raw.githubusercontent.com/kairos-dev/KairosWatch/KairOSWatch_Test/resources/screens/menu.png)		  |

## Questions

Below is a list of questions to be addressed as a result of this requirements document:


 1. **What about third party apps?**
 	* We Think this is important, but not for version one. 
	* We can look at this at a later stage.

## Not Doing

* Third party apps support - out of scope, see above for details.

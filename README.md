Money box counter

Hello, In this video We are going to make DIY Coin Counting Bank with Arduino nano. You will easy know how much coin you have and how many of each coin you have. Hope You enjoy the video. #howto #coinbox #moneybox #arduino #arduinoproject

<a href="https://www.youtube.com/watch?v=6BF9copnfS4">
<img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/coinbox.youtubelogo.jpg" />
</a>

This project requires the following parts:

Arduino nano, https://amzn.to/3hB8Pwf<br>
Infrared pair tube, https://amzn.to/3hi7zP8<br>
20x4 I2C LCD or 16x2 I2C LCD, https://amzn.to/3gTMZnW<br>
HC-SR501, https://amzn.to/3aJAOHX<br>
220Ω resistor, https://amzn.to/2OSGlBW<br>
10K resistor, https://amzn.to/2OSGlBW<br>
Two buttons, https://amzn.to/3hKNL7N<br>
Jumper wires, https://amzn.to/3jCHhZd<br>
Breadboard, https://amzn.to/33yEavN<br>



Note
Before starting this project, we must first test the address of the 20X4 I2C LCD or 16x2 I2C LCD. The character address of each LCD is different, it may be 0x27, it may be 0x3F. Only if the character address of your LCD is correct, the LCD screen will display characters. Otherwise, no characters will appear on the LCD screen. 

<img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/note.jpg">

In order to prevent the program from being disordered, after burning the firmware, please do not unplug the serial cable, connect the serial cable to the computer usb port, press and hold the reset button and do not let go, press the reset button on the arduino nano, and press Set the button for 3 seconds, the number of coins and coin value are reset. (Please use the power supply provided by the computer to reset the coin)

Production

1. Install the library file: Open "Sketch"-"Include Library" in the Arduino development software, then "Add Zip Library...", then add the compressed files containing EEPROMex.h and LCD_1602_RUS.h
 <img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/1.jpg">
 <img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/2.jpg">
Select the development board as Arduino Nano, this is to choose the right.
 <img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/3.jpg">

Select the processor as ATmega328P(Old Bootloader), this is to choose the right.
 <img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/4.jpg">

select the port, you can burn the code into the development board.
 <img src="http://dkardu.oss-cn-hongkong.aliyuncs.com/coinbox/5.jpg">


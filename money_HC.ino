#include "EEPROMex.h"
#include "LCD_1602_RUS.h"


#define coin_amount 4    // the number of coins to be recognized
float coin_value[coin_amount] = {0.5, 1.0, 2.0, 10.0};  // coin value
String currency = "HKD"; // Currency name
int stb_time = 10000;    // idle time after which the system goes to sleep (milliseconds)


int coin_signal[coin_amount];    // here the signal value for each coin size is stored
int coin_quantity[coin_amount];  // number of coins
byte empty_signal;               // store the level of the empty signal
unsigned long standby_timer, reset_timer; // timers
float summ_money = 0;            // the amount of coins in the piggy bank


LCD_1602_RUS lcd(0x27, 20, 4); // LCD_1602_RUS lcd(0x27, 16, 2);         


boolean recogn_flag, sleep_flag = true;   // flags


#define button 2         // Display the total number of coins
#define calibr_button 3  // hidden calibration and reset button
#define disp_power 12    // display power
#define LEDpin 11        // LED power supply
#define IRpin 17         // phototransistor power supply
#define IRsens 14        // phototransistor signal
#define haha 6           // Wake up the screen

int sens_signal, last_sens_signal;
boolean coin_flag = false;

void setup() {


  Serial.begin(9600);                   // open port for communication with PC for debugging

  
  pinMode(haha,INPUT);

  // pull up the buttons
  pinMode(button, INPUT_PULLUP);
  pinMode(calibr_button, INPUT_PULLUP);

  
  // power pins as outputs
  pinMode(disp_power, OUTPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, OUTPUT);

  

  // supply power to display and sensor
  digitalWrite(disp_power, 1);
  digitalWrite(LEDpin, 1);
  digitalWrite(IRpin, 1);


  empty_signal = analogRead(IRsens);  // read empty (reference) signal

  // display initialization
  lcd.init();
  lcd.backlight();


  if (!digitalRead(calibr_button)) {  // if the calibration button is pressed at startup
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(L"Service");
    delay(500);
    reset_timer = millis();
    while (1) {                                   // endless cycle
      if (millis() - reset_timer > 3000) {        // if the button is still held down and 3 seconds have passed
        // очистить количество монет
        for (byte i = 0; i < coin_amount; i++) {
          coin_quantity[i] = 0;
          EEPROM.writeInt(20 + i * 2, 0);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(L"Memory cleared");
        delay(100);
      }
      if (digitalRead(calibr_button)) {   // if the button is released, go to calibration
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(L"Calibration");
        break;
      }
    }
    while (1) {
      for (byte i = 0; i < coin_amount; i++) {
        lcd.setCursor(0, 1); lcd.print(coin_value[i]);  // отобразить цену монеты, размер которой калибруется
        lcd.setCursor(17, 1); lcd.print(currency);      // display currency
        last_sens_signal = empty_signal;
        while (1) {
          sens_signal = analogRead(IRsens);                                    // read sensor
          if (sens_signal > last_sens_signal) last_sens_signal = sens_signal;  // if the current value is greater than the previous one
          if (sens_signal - empty_signal > 3) coin_flag = true;                // if the value dropped almost to "empty", consider that the coin flew away
          if (coin_flag && (abs(sens_signal - empty_signal)) < 2) {            // if the coin flew away for sure
            coin_signal[i] = last_sens_signal;                                 // write the maximum value to memory
            EEPROM.writeInt(i * 2, coin_signal[i]);
            coin_flag = false;
            break;
          }
        }
      }
      break;
    }
  }

  // at the start of the system, read the signals of coins from memory for further work, as well as their number in the bank
  for (byte i = 0; i < coin_amount; i++) {
    coin_signal[i] = EEPROM.readInt(i * 2);
    coin_quantity[i] = EEPROM.readInt(20 + i * 2);
    summ_money += coin_quantity[i] * coin_value[i];  // well, and immediately calculate the amount, as the product of the coin price by the amount
  }
  /*
    // для отладки, вывести сигналы монет в порт
    for (byte i = 0; i < coin_amount; i++) {
      Serial.println(coin_signal[i]);
    }
  */
  

  standby_timer = millis();  // reset sleep timer

}

void loop() {
  
 // if you woke up after sleep, initialize the display and display text, amount and currency
    if(sleep_flag){
    delay(500);
    lcd.init();
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(L"Account");
    lcd.setCursor(0, 1); lcd.print(summ_money);
    lcd.setCursor(17, 1); lcd.print(currency);     
    lcd.setCursor(0, 3); lcd.print("----Become  rich----");    //If you use 16x2 I2C lcd, please delete or comment out this line of code
    empty_signal = analogRead(IRsens);
    sleep_flag = false;
    }

  // далее работаем в бесконечном цикле
  last_sens_signal = empty_signal;
  while (1) {
    sens_signal = analogRead(IRsens);  
    //further the same algorithm as in the calibration
    if (sens_signal > last_sens_signal) last_sens_signal = sens_signal;
    if (sens_signal - empty_signal > 3) coin_flag = true;
    if (coin_flag && (abs(sens_signal - empty_signal)) < 2) {
      recogn_flag = false;  
      // in general, we found the maximum for the coin, wrote it down in last_sens_signal
      // then we start to compare with the values for coins stored in memory
      for (byte i = 0; i < coin_amount; i++) {
        int delta = abs(last_sens_signal - coin_signal[i]);   
        // the value of the difference between the received signal and our values from memory
        if (delta < 30) {   // and here, if this difference falls within the range, then we consider the coin recognized
          summ_money += coin_value[i];  // to the amount we stupidly add the price of the coin (dada, the amount is calculated in two different ways. At the start of the system, the sum of all coins, and here the addition
          lcd.setCursor(0, 1); lcd.print(summ_money);
          coin_quantity[i]++;  // for the recognized coin number, add the amount
          recogn_flag = true;
          break;
        }
      }
      coin_flag = false;
      standby_timer = millis();  // reset timer
      break;
    }

    // if we didn’t do anything, the time of the aimer is up, sleep
    if (millis() - standby_timer > stb_time) {
      //good_night();
        for (byte i = 0; i < coin_amount; i++) {
        EEPROM.updateInt(20 + i * 2, coin_quantity[i]);
        }
        sleep_flag = true;
        int montion = digitalRead(haha);
        Serial.println(montion);
        if(montion == 1){
        wake_up();
        }else{
         // cut off power from all displays and sensors
        digitalWrite(disp_power, 0);
        digitalWrite(LEDpin, 0);
        digitalWrite(IRpin, 0);
        delay(100);
      }
      continue;
    }

    // if the coin is inserted (closes the contacts) and held for 2 seconds
    while (!digitalRead(button)) {
      if (millis() - standby_timer > 2000) {
        lcd.clear();

       // show on the display: above the prices of coins (rounded up to whole numbers), below their number
        for (byte i = 0; i < coin_amount; i++) {
          lcd.setCursor(i * 3, 0); lcd.print((int)coin_value[i]);
          lcd.setCursor(i * 3, 1); lcd.print(coin_quantity[i]);
        }
      }
    }
  }
  
}


void wake_up() {
  digitalWrite(disp_power, 1);
  digitalWrite(LEDpin, 1);
  digitalWrite(IRpin, 1);
  standby_timer = millis();  // and reset the timer
}

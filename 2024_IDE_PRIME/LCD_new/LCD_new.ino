#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); 

byte customChar[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b11111,
	0b11111,
	0b10101,
	0b00000,
	0b00000
};

byte customChar1[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b01110,
	0b00100,
	0b00000,
	0b00000
};

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);

}

void loop()
{
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Landslide Ahead");
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(1, 1);
  lcd.write((byte)1);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
  lcd.setCursor(14, 1);
  lcd.write((byte)1);


  delay(2000);                 // display the above for two seconds


}
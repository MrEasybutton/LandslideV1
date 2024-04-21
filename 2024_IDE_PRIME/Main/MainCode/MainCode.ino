
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); 

const int AirValue = 620;  
const int WaterValue = 310; 
int soilMoistureValue = 0;
int soilmoisturepercent=0;

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
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);

}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Landslide Ahead");
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(1, 1);
  lcd.write((byte)1);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
  lcd.setCursor(14, 1);
  lcd.write((byte)1);
  lcd.setCursor(7, 1);
  SoilMoisture_Capacitive();

  

}

void SoilMoisture_Capacitive() {
  soilMoistureValue = analogRead(A0);
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  lcd.setCursor(7, 1);
  if(soilmoisturepercent >= 100)
  {
    Serial.print("Moisture Percentage: ");
    Serial.println("100 %");
    lcd.print("FULL");

  }
  else if(soilmoisturepercent <=0)
  {
    Serial.print("Moisture Percentage: ");
    Serial.println("0 %");
    lcd.print("NULL");
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    Serial.print("Moisture Percentage: ");
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    lcd.print(soilmoisturepercent);
  }
  lcd.print("%");
  delay(1000); 

}
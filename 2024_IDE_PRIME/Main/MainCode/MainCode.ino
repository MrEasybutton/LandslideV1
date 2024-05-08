// Include all libraries used in this project.
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h"

// Defining Variables Pt 1 (pre-compilation)
#define PIN_RED    3 
#define PIN_YELLOW 4
#define PIN_GREEN  5 

#define RAIN_PIN 8  
#define AO_PIN A1 

#define BUZZER_PIN 2

// Call LiquidCrystal address
LiquidCrystal_I2C lcd(0x3F, 16, 2); 

// Defining Variables Pt 2 (post-compilation)

// Capacitive Moisture Sensor Variables
const int AirValue = 620;  
const int WaterValue = 310; 
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
// Clock
const unsigned long interval= 3000;
unsigned long previousMillis;
unsigned long currentMillis;
// Servo Motor Variables
Servo servo;
int ax = 30;
// Rain Sensor Variables
int rainPotency;
// Vibration Sensor Variable
int vib_val;
int VIB_PIN = 7;
// Bluetooth String Sent
String sent_info;
// LCD Custom Characters
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

int melody[] = {
  NOTE_B2, NOTE_B2, NOTE_B2,
  NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_C2, NOTE_B2, NOTE_C2, NOTE_B2,

};

int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
};


void VibrationSensor() {
  vib_val = digitalRead(VIB_PIN);
  
}

void PiezoAlert() {
  // iterate over the notes of the melody:
  int size = sizeof(noteDurations) / sizeof(int);

  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void RainSensor() {
  digitalWrite(RAIN_PIN, HIGH);  
  int rainValue = analogRead(AO_PIN);
  rainPotency = 400 - rainValue;
  digitalWrite(RAIN_PIN, LOW);

}

void Servo_Code() {

  for (int pos = 0; pos != ax; pos += 2) {
    servo.write(pos);
  }
}

void Traffic_Red() {
  digitalWrite(PIN_RED, HIGH);   // turn on
  digitalWrite(PIN_YELLOW, LOW); // turn off
  digitalWrite(PIN_GREEN, LOW);  // turn off
}

void Traffic_Yellow() {
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_YELLOW, HIGH);
  digitalWrite(PIN_GREEN, LOW);
}

void Traffic_Green() {
  // green light on
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_YELLOW, LOW);
  digitalWrite(PIN_GREEN, HIGH);
}

void LCD_baseprint() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(1, 1);
  lcd.write((byte)1);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
  lcd.setCursor(14, 1);
  lcd.write((byte)1);
  lcd.setCursor(0, 0);
}

void SoilMoisture_Capacitive() {
  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

}

void setup() {
  // Initialisation of Serial Monitor Output [Debug + Info]
  Serial.begin(9600);
  // Initialisation of LCD component + Toggle backlight ON
  lcd.init();
  lcd.backlight();
  // Create the custom characters defined previously
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);
  // Attach Servo Output Pin
  servo.attach(9);
  servo.write(90);
  // Set Vibration Sensor PinMode
  pinMode(VIB_PIN,INPUT);
  // Set Rain Sensor PinMode
  pinMode(RAIN_PIN, OUTPUT);
  // Set Traffic Lights PinMode
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  

}

// Main Program Loop
void loop()
{

  LCD_baseprint();
  VibrationSensor();
  RainSensor();
  SoilMoisture_Capacitive();

  if (vib_val == 1) 
  {

    if (soilmoisturepercent >= 95)
    {
      if (rainPotency >= 200)
      {
        // Worst Case Scenario: All 3 Detected Severe Results, display do_not + red_light
        Serial.println("WORST CASE SCENARIO!");
        lcd.print("LANDSLIDE RISK");
        Traffic_Red();
        Servo_Code();
        PiezoAlert();

      }
      else
      {
        // Penultimate Case: Report High Danger, display do_not + red_light
        Serial.println("BAD SCENARIO!");
        lcd.print("LANDSLIDE RISK");
        Traffic_Red();
        Servo_Code();
        PiezoAlert();
      }

    }
    else if (soilmoisturepercent >= 20 && soilmoisturepercent < 95)
    {
      if (rainPotency >= 250)
      {
        // Moderate Scenario A: Report Moderate Danger, display warning + yellow light
        Serial.println("MODERATE SCENARIO!");
        lcd.print("Caution!");
        Traffic_Yellow();
        Servo_Code();
      }
      else
      {
        // Moderate Scenario B: Report Low Danger, display good_to_go + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);
      }

    }
    else if (soilmoisturepercent < 20)
    {
      if (rainPotency >= 325)
      {
        // OK Scenario A: Report Low Danger, display good_highrain + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);
      }
      else
      {
        // OK Scenario B: Report Low Danger, display good_to_go + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);
      }

    }
    else 
    {
      Serial.print("Soilmoisturepercent may be bugged: ");
    }
  }
  else 
  {
    if (soilmoisturepercent >= 95)
    {
      if (rainPotency >= 200)
      {
        // High Risk Scenario: Report High Danger, display warning + red light
        Serial.println("HIGH RISK SCENARIO!");
        lcd.print("LANDSLIDE RISK");
        Traffic_Red();
        Servo_Code();
        PiezoAlert();
      }
      else
      {
        // Penultimate Case: Report High Danger, display warning + red light
        Serial.println("HIGH RISK SCENARIO!");
        lcd.print("LANDSLIDE RISK");
        Traffic_Red();
        Servo_Code();
        PiezoAlert();
        
      }


    }
    else if (soilmoisturepercent >= 20 && soilmoisturepercent < 95)
    {
      if (rainPotency >= 250)
      {
        // Moderate Scenario A: Report Moderate Danger, display warning + green light
        Serial.println("MODERATE SCENARIO!");
        lcd.print("Caution!");
        Traffic_Yellow();
        Servo_Code();

      }
      else
      {
        // Moderate Scenario B: Report Low Danger, display good_to_go + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);
      }

    }
    else if (soilmoisturepercent < 20)
    {
      if (rainPotency >= 325)
      {
        // OK Scenario A: Report Low Danger, display good_highrain + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);

      }
      else
      {
        // OK Scenario B: Report Low Danger, display good_to_go + green light
        Serial.println("LOW SCENARIO!");
        lcd.print("Please Proceed");
        Traffic_Green();
        servo.write(90);
      }

    }
    else 
    {
      Serial.print("Soilmoisturepercent may be bugged: ");
    }
  }
  
  delay(1000);
}


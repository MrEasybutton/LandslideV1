const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}
void loop() {
soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
Serial.print("Soil Moisture Value: ");
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
  Serial.print("Moisture Percentage: ");
  Serial.println("100 %");
}
else if(soilmoisturepercent <=0)
{
  Serial.print("Moisture Percentage: ");
  Serial.println("0 %");
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Serial.print("Moisture Percentage: ");
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  
}
  delay(1000);
}
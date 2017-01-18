#include "blynk/blynk.h"
#include "blynk/BlynkSimpleParticle.h"
#include "SparkCorePolledTimer/SparkCorePolledTimer.h"
#include "spark-dallas-temperature/spark-dallas-temperature.h"
#include "OneWire/OneWire.h"
#define ONE_WIRE_BUS D0 // Data wire is plugged into pin D0 on the particle
#define TEMPERATURE_PRECISION 12         // DS18B20 Thermometer Stuff
OneWire oneWire(ONE_WIRE_BUS);          // DS18B20 Thermometer Stuff
DallasTemperature sensors(&oneWire);    // DS18B20 Thermometer Stuff

SparkCorePolledTimer updateTimer(5000); //Create a timer object and set it's timeout in milliseconds
void OnTimer(void); //Prototype for timer callback method

float main, mainRaw, reading, voltage, humidity;

char auth[] = "yourauthkeyhere";

//temp stuff
double InTempC = -1;
void update18B20Temp(DeviceAddress deviceAddress, double &tempC);
//end temp stuff

void setup(){
Serial.begin(9600);
updateTimer.SetCallback(OnTimer);

// DS18B20 initialization
sensors.begin();



delay(5000); // Allow board to settle

pinMode(A0, INPUT);

Blynk.begin(auth);
}

void loop()
{


Blynk.run();
updateTimer.Update();
}

void OnTimer(void) { //Handler for the timer, will be called automatically



    // DS18B20
    sensors.requestTemperatures();
    update18B20Temp(Thermometer1, InTempC);



updateTimer.Update();    // new stuff
}

void update18B20Temp(DeviceAddress deviceAddress, double &tempC)
{
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

if ( tempC < 60 && tempC > -30) {
    Blynk.virtualWrite(0, tempC);
    Blynk.virtualWrite(1, tempC);
}

/*

// read analog ports
mainRaw = analogRead(A0);
main = map(mainRaw, 0, 4096, 0, 1865);
main = (main / 100);

*/
  // Keep reading the sensor value so when we make an API
  // call to read its value, we have the latest one
    reading = analogRead(A0);

  // The returned value from the Core is going to be in the range from 0 to 4095
  // Calculate the voltage from the sensor reading
    voltage = (reading * 5) / 4095; //5 volts reference

  // Calculate the temperature and update our static variable
    humidity = (161*voltage/5)-25.8; //5 volts reference

Blynk.virtualWrite(10, humidity);
Blynk.virtualWrite(11, humidity);

}

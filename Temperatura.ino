/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
const int oneWireBus2 = 5;
const int oneWireBus3 = 12; //La idea seria utilizar los siguientes pines :  GPIO 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33.     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
OneWire oneWire2(oneWireBus2);
OneWire oneWire3(oneWireBus3);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

DeviceAddress tempDeviceAddress;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  sensors2.begin();
  sensors3.begin();
  sensors.getAddress(tempDeviceAddress, 0);
  sensors2.getAddress(tempDeviceAddress, 0);
  sensors3.getAddress(tempDeviceAddress, 0);
  //Aqui estaba haciendo pruebas y basicamente el sensor viene por defecto con la resolucion en 12 que es la maxima que permite
  sensors.setResolution(tempDeviceAddress, 9);
}

void loop() {
  sensors.requestTemperatures();
  sensors2.requestTemperatures(); 
  sensors3.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureC2 = sensors2.getTempCByIndex(0);
  float temperatureC3 = sensors3.getTempCByIndex(0);

  Serial.print(temperatureC);
  Serial.println("ºC");

  delay(5000);
}


#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
const int oneWireBus2 = 5;
const int oneWireBus3 = 18; //La idea seria utilizar los siguientes pines :  GPIO 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33.
int numberOfDevices1;   
int numberOfDevices2;
int numberOfDevices3;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
OneWire oneWire2(oneWireBus2);
OneWire oneWire3(oneWireBus3);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

DeviceAddress tempDeviceAddress1;
DeviceAddress tempDeviceAddress2;
DeviceAddress tempDeviceAddress3;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  sensors2.begin();
  sensors3.begin();
  numberOfDevices1 = sensors.getDeviceCount();
  numberOfDevices2 = sensors2.getDeviceCount();
  numberOfDevices3 = sensors3.getDeviceCount();
}

void loop() {
  sensors.requestTemperatures();
  sensors2.requestTemperatures(); 
  sensors3.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureC2 = sensors2.getTempCByIndex(0);
  float temperatureC3 = sensors3.getTempCByIndex(0);
  //Serial.println("Primero");
  //Serial.println(temperatureC);
  //Serial.println("Segundo");
  //Serial.println(temperatureC2);
  //Serial.println("Tercero");
  //Serial.println(temperatureC3);

  for(int i=0;i<numberOfDevices1; i++){
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress1, i)){

      // Output the device ID
      Serial.print("Canal1: ");
      Serial.println(i,DEC);
      // Print the data
      float tempC = sensors.getTempC(tempDeviceAddress1);

      Serial.println(tempC);
      printAddress(tempDeviceAddress1);
      Serial.println();


  
    }
  }
  for(int i=0;i<numberOfDevices2; i++){
    // Search the wire for address
    if(sensors2.getAddress(tempDeviceAddress2, i)){

      // Output the device ID
      Serial.print("Canal2: ");
      Serial.println(i,DEC);
      // Print the data
      float tempC = sensors2.getTempC(tempDeviceAddress2);

      Serial.println(tempC);
      printAddress(tempDeviceAddress2);
      Serial.println();


  
    }
  }
  for(int i=0;i<numberOfDevices3; i++){
    // Search the wire for address
    if(sensors3.getAddress(tempDeviceAddress3, i)){

      // Output the device ID
      Serial.print("Canal3 ");
      Serial.println(i,DEC);
      // Print the data
      float tempC = sensors3.getTempC(tempDeviceAddress3);

      Serial.println(tempC);
      printAddress(tempDeviceAddress3);
      Serial.println();


  
    }
  }
  delay(1000);
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}



#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <ESP32Time.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;
const int oneWireBus2 = 14;
const int oneWireBus3 = 25; //La idea seria utilizar los siguientes pines :  GPIO 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33.
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

const int numSensores = 45;

float pendientes[numSensores] = {
  1.0079, 0.9811, 1.0012, 0.9862, 0.9887, 0.9798, 1.0081, 1.0193, 1.0082,
  0.9958, 1.0061, 1.0003, 0.9995, 1.0226, 1.0108, 1.0129, 1.0031, 1.0071,
  1.0195, 0.0,     1.0252, 1.0219, 1.0142, 1.0011, 1.0063, 1.0076, 1.0155,
  1.0219, 1.0107, 1.0085, 0.9988, 1.0098, 1.0177, 1.0121, 1.0027, 0.9991,
  0.9985, 0.9977, 1.0031, 1.0126, 1.0247, 1.0143, 1.0138, 1.0148, 1.0387
};

float offsets[numSensores] = {
  0.0598, 0.6282, -0.2868, 0.4137, 0.5653, 0.8954, -0.045, 0.0072, 0.1004,
  0.4079, -0.2006, 0.2178, 0.5976, -0.6423, 0.4226, -0.5323, -0.1532, -0.2585,
  -0.3789, 0.0,    -1.0456, -1.0712, -0.9136, 0.0696, -0.2966, 0.1352, -0.3999,
  -0.9143, 0.3317, 0.2313, 0.3908, 0.3008, -0.0525, -0.6488, 0.1470, 0.4868,
  0.8680, 0.0296, 0.7519, -0.7378, -1.11,   -0.3526, -0.7724, -1.0788, -1.3206
};

String direcciones[numSensores] = {
  "28D0E936000000B2", "282D473900000081", "28DA0037000000DB", "28636935000000AF",
  "285BD33300000018", "283D3A38000000D4", "288EAD3600000079", "28E04939000000F6",
  "28CE9A3500000051", "28DB6E5000000049", "28166C350000000E", "28091E3400000010",
  "280F67380000002E", "28046738000000D6", "28ADED380000006A", "281C2B3500000079",
  "28C7A3340000003F", "28C60F3900000087", "28D4E63600000001", "28E80F880000003C",
  "28958038000000DB", "2848F696F0013C4C", "2890AD3600000031", "2895B83800000061",
  "28FEC938000000FA", "28CEF134000000BF", "28221838000000D0", "286A523800000050",
  "28D34E3600000089", "282AD134000000B2", "28F3563800000068", "28153E38000000DC",
  "28F6AF38000000D6", "282ADF3400000010", "2800B3380000004B", "28CC2C3800000071",
  "2829B136000000FD", "285F143800000023", "28DA463800000034", "28A65D36000000C8",
  "28639238000000E4", "280E4338000000FE", "288C5A34000000E7", "28D44436000000B1",
  "28610B3900000041"
};

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      } 
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){

    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void SdSave(String h,String T,String Hour,String Date,String Address);

ESP32Time rtc(0);

String Date;
String DateActual;


String Hora;




void setup() {

  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  rtc.setTime(0, 49, 17, 9, 7, 2025); /// Aqui toca setear la fecha del dia, que digamos para tener una referencia ahi tenemos 17:49:00 7/9/2025

  DateActual=rtc.getDate(); 

  Hora=rtc.getTime();

  SdNSave(DateActual);

  sensors.begin(); /// Como cualquier otro sensor o periferico se debe inicializar, pues esta es la funcion
  sensors2.begin();
  sensors3.begin();
  numberOfDevices1 = sensors.getDeviceCount(); /// como tenemos 3 canales, en esta libreria se pide que se debe de tener cuantos dispositivos estan asociados a este canal, por lo que
  numberOfDevices2 = sensors2.getDeviceCount(); /// esta funcion me dice cuantos dispositivos tengo en el canal, para asi poder despues buscar esa direccion de cada canal.
  numberOfDevices3 = sensors3.getDeviceCount();
}

void loop() {
  sensors.requestTemperatures(); /// Esta funcion es para ir por decirlo de alguna manera ir pidiendo los datos de temperatura de los sensores, que despues con getTemp podemos obtener la temperatura
  sensors2.requestTemperatures(); 
  sensors3.requestTemperatures();
  Date=rtc.getDate();
  Hora=rtc.getTime();

  for(int i=0;i<numberOfDevices1; i++){
    if(sensors.getAddress(tempDeviceAddress1, i)){
      float tempC = sensors.getTempC(tempDeviceAddress1);
      String Address=getAddressAsString(tempDeviceAddress1);
      int location=buscarIndicePorDireccion(Address);
      double m=pendientes[location];
      double offset=offsets[location];
      float temperaturaC=m*tempC+offset;
      SdSave("20",String(temperaturaC),Hora,Date);
      Serial.println(Address);
      Serial.println(m);
      Serial.println(offset);
    }
  }
  for(int i=0;i<numberOfDevices2; i++){
    // Search the wire for address
    if(sensors2.getAddress(tempDeviceAddress2, i)){
      float tempC = sensors2.getTempC(tempDeviceAddress2);
      String Address=getAddressAsString(tempDeviceAddress2);
      int location=buscarIndicePorDireccion(Address);
      double m=pendientes[location];
      double offset=offsets[location];
      float temperaturaC=m*tempC+offset;
      SdSave("20",String(temperaturaC),Hora,Date);
      Serial.println(Address);
      Serial.println(m);
      Serial.println(offset);
    }
  }
  for(int i=0;i<numberOfDevices3; i++){
    // Search the wire for address
    if(sensors3.getAddress(tempDeviceAddress3, i)){
      float tempC = sensors3.getTempC(tempDeviceAddress3);
      String Address=getAddressAsString(tempDeviceAddress2);
      int location=buscarIndicePorDireccion(Address);
      double m=pendientes[location];
      double offset=offsets[location];
      float temperaturaC=m*tempC+offset;
      SdSave("20",String(temperaturaC),Hora,Date);
      Serial.println(Address);
      Serial.println(m);
      Serial.println(offset);
    }
  }
  delay(100000);

  if(Date==DateActual)
  {
  }
  else
  {
    DateActual=Date;
    SdNSave(Date);

  }
}

void printAddress(DeviceAddress deviceAddress) {  /// Hace lo mismo que la de abajo, a diferencia de la de abajo esta pues imprime la direccion y no la concatena con si se esta haciendo en la de abajo
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
String getAddressAsString(DeviceAddress deviceAddress) { /// Basicamente con esa funcion ingresamos la direccion que esta en un arreglo con 8 posiciones, y la idea es pasarlo a string para poderlo comparar
  String address = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) address += "0";
    address += String(deviceAddress[i], HEX);
  }
  address.toUpperCase(); /// Poner en mayuscula
  return address;
}

int buscarIndicePorDireccion(String direccion) { /// La idea de esta funciones es que yo tengo las direcciones de todos los sensores listada en un arreglo, y segun esa direccion
  for (int i = 0; i < numSensores; i++) {       /// tengo tambien los valores de calibracion para los sensores que son el offset y la pendiente, por ejemplo si la direccion del sensor
    if (direcciones[i] == direccion) {          /// es "28D0E936000000B2" sus valores seran los primeros de los arreglos es decir, offset[0], pendiente[0]
      return i; // Retorna la posición (desde 0)
    }
  }
  return -1; // Si no la encuentra
}

void SdNSave(String Date)    /// Esta funcion lo que hace es generar un archivo de inicio para el dia, es decir cada dia genera un archivo .txt con profundidad hora temperatura
{                           
  String NameArchive="/";
  NameArchive+=Date;
  NameArchive+=".txt";
  writeFile(SD, NameArchive.c_str(),"Profundidad Hora Temperatura");
  appendFile(SD, NameArchive.c_str(),"\n");
  
}
void SdSave(String h,String T,String Hour,String Date)   /// En esta funcion la idea es escribir sobre el archivo previamente creado la informacion que es ingresada, es necesario
{                                                        /// utilizar .c_str() para que esa variable String, me la pueda pasar a una variable de tipo puntero, pues la libreria pide
  String NameArchive="/";                                /// que se debe de poner la direccion a la que apunta.
  NameArchive+=Date;
  NameArchive+=".txt";
  appendFile(SD, NameArchive.c_str(),"\n");
  appendFile(SD, NameArchive.c_str(),h.c_str());
  appendFile(SD, NameArchive.c_str()," ");
  appendFile(SD, NameArchive.c_str(),Hour.c_str());
  appendFile(SD, NameArchive.c_str()," ");
  appendFile(SD, NameArchive.c_str(),T.c_str());
  appendFile(SD, NameArchive.c_str(),"\n");
  
  }


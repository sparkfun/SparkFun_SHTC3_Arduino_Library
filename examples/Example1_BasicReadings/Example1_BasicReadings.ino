/*
  Take humidity and temperature readings with the SHTC3 using I2C
  By: Owen Lyke
  SparkFun Electronics
  Date: August 24 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Example1_BasicReadings
  To connect the sensor to an Arduino:
  This library supports the sensor using the I2C protocol
  On Qwiic enabled boards simply connnect the sensor with a Qwiic cable and it is set to go
  On non-qwiic boards you will need to connect 4 wires between the sensor and the host board
  (Arduino pin) = (Display pin)
  SCL = SCL on display carrier
  SDA = SDA
  GND = GND
  3.3V = 3.3V
*/

#include "SparkFun_SHTC3.h" // Click here to get the library: http://librarymanager/All#SparkFun_SHTC3

#define SERIAL_PORT Serial  // #define-ing the SERIAL_PORT allows you to easily change your target port - for example if you are using a SAMD21 board you can change "Serial" to "SerialUSB"

SHTC3 mySHTC3;              // Declare an instance of the SHTC3 class

void setup() {
  SERIAL_PORT.begin(115200);                                  // Begin Serial 
  while(SERIAL_PORT == false){};                                   // Wait for the serial connection to start up
  SERIAL_PORT.println("SHTC3 Example 1 - Basic Readings");    // Title
  SERIAL_PORT.println();

  SERIAL_PORT.print("Beginning sensor. Result = ");           // Most SHTC3 functions return a variable of the type "SHTC3_Status_TypeDef" to indicate the status of their execution 
  errorDecoder(mySHTC3.begin());                              // To start the sensor you must call "begin()", the default settings use Wire (default Arduino I2C port)
  Wire.setClock(400000);                                      // The sensor is listed to work up to 1 MHz I2C speed, but the I2C clock speed is global for all sensors on that bus so using 400kHz or 100kHz is recommended
  SERIAL_PORT.println();

  if(mySHTC3.passIDcrc)                                       // Whenever data is received the associated checksum is calculated and verified so you can be sure the data is true
  {                                                           // The checksum pass indicators are: passIDcrc, passRHcrc, and passTcrc for the ID, RH, and T readings respectively
    SERIAL_PORT.print("ID Passed Checksum. ");
    SERIAL_PORT.print("Device ID: 0b"); 
    SERIAL_PORT.print(mySHTC3.ID, BIN);                       // The 16-bit device ID can be accessed as a member variable of the object
  }
  else
  {
    SERIAL_PORT.print("ID Checksum Failed. ");
  }
  SERIAL_PORT.println("\n\n");
  SERIAL_PORT.println("Waiting for 5 seconds so you can read this info ^^^");

  delay(5000);                                                // Give time to read the welcome message and device ID. 
}

void loop() {
  SHTC3_Status_TypeDef result = mySHTC3.update();             // Call "update()" to command a measurement, wait for measurement to complete, and update the RH and T members of the object
  printInfo();                                                // This function is used to print a nice little line of info to the serial port
  delay(190);                                                 // Delay for the data rate you want - note that measurements take ~10 ms so the fastest data rate is 100 Hz (when no delay is used)
}



///////////////////////
// Utility Functions //
///////////////////////
void printInfo()
{
  if(mySHTC3.lastStatus == SHTC3_Status_Nominal)              // You can also assess the status of the last command by checking the ".lastStatus" member of the object
  {
    SERIAL_PORT.print("RH = "); 
    SERIAL_PORT.print(mySHTC3.toPercent());                   // "toPercent" returns the percent humidity as a floating point number
    SERIAL_PORT.print("% (checksum: "); 
    if(mySHTC3.passRHcrc)                                     // Like "passIDcrc" this is true when the RH value is valid from the sensor (but not necessarily up-to-date in terms of time)
    {
      SERIAL_PORT.print("pass");
    }
    else
    {
      SERIAL_PORT.print("fail");
    }
    SERIAL_PORT.print("), T = "); 
    SERIAL_PORT.print(mySHTC3.toDegF());                        // "toDegF" and "toDegC" return the temperature as a flaoting point number in deg F and deg C respectively 
    SERIAL_PORT.print(" deg F (checksum: "); 
    if(mySHTC3.passTcrc)                                        // Like "passIDcrc" this is true when the T value is valid from the sensor (but not necessarily up-to-date in terms of time)
    {
      SERIAL_PORT.print("pass");
    }
    else
    {
      SERIAL_PORT.print("fail");
    }
    SERIAL_PORT.println(")");
  }
  else
  {
    SERIAL_PORT.print("Update failed, error: "); 
    errorDecoder(mySHTC3.lastStatus);
    SERIAL_PORT.println();
  }
}

void errorDecoder(SHTC3_Status_TypeDef message)                             // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch(message)
  {
    case SHTC3_Status_Nominal : SERIAL_PORT.print("Nominal"); break;
    case SHTC3_Status_Error : SERIAL_PORT.print("Error"); break;
    case SHTC3_Status_CRC_Fail : SERIAL_PORT.print("CRC Fail"); break;
    default : SERIAL_PORT.print("Unknown return code"); break;
  }
}

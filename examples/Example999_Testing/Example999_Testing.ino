#include "SparkFun_SHTC3.h"

SHTC3 mySHT; // Why is it picky about not using parentheses when there are no arguments to the constructor???? 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("SHTC3 Example 999 - Testing");
  Serial.println();

  Serial.print("Beginning sensor. Result = ");
  errorDecoder(mySHT.begin(Wire, 400000));
  Serial.println();

  Serial.print("Device ID: 0b"); 
  Serial.print(mySHT.ID, BIN);
  Serial.print(" (checksum: ");
  if(mySHT.passIDcrc)
  {
    Serial.print("pass");
  }
  else
  {
    Serial.print("fail");
  }
  Serial.println(")\n\n");

  delay(3000);          // Give time to read the 
}

void loop() {
  // put your main code here, to run repeatedly:
  SHTC3_Status_TypeDef result = mySHT.update();
  if(result == SHTC3_Status_Nominal)
  {
//    Serial.print("RH = 0x"); Serial.print(mySHT.RH, HEX); Serial.print(", T = 0x"); Serial.print(mySHT.T, HEX); Serial.println();
    Serial.print("RH = "); 
    Serial.print(mySHT.toPercent()); 
    Serial.print("% (checksum: "); 
    if(mySHT.passRHcrc)
    {
      Serial.print("pass");
    }
    else
    {
      Serial.print("fail");
    }
    Serial.print("), T = "); 
    Serial.print(mySHT.toDegF()); 
    Serial.print(" deg F (checksum: "); 
    if(mySHT.passTcrc)
    {
      Serial.print("pass");
    }
    else
    {
      Serial.print("fail");
    }
    Serial.println(")");
  }
  else
  {
    Serial.print("Update failed, error: "); 
    errorDecoder(result);
    Serial.println();
  }

  delay(200);
}

void errorDecoder(SHTC3_Status_TypeDef message)
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
}


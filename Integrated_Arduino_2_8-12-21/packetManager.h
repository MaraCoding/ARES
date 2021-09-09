
//===Serial Comm Data
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void sendDataPacket(){
  
  Serial.print('<');
//  Serial.println("AMPS");
  Serial.print(systemAmps); //System current float
  Serial.print(",");
  Serial.print(drillAmps); //Drill current float
  Serial.print(",");
  Serial.print(rpmInt); //int
  Serial.print(",");
  Serial.print(averageTemp); //ThermoCouple Output int
//  Serial.println("HEATER TEMP");
  Serial.println('>');


  
}
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}
void showAndExecuteCommand() {
  if (newData == true) {
    Serial.println(receivedChars);
    if (receivedChars[0] == 's')stopDrilling();
    else if (receivedChars[0] == 'a')SpeedUp();
    else if (receivedChars[0] == 'f')SlowDown();
    else if (receivedChars[0] == 'i')startPump();
    else if(receivedChars[0] == 'o')stopPump();
    else if(receivedChars[0] == 'k')turnOnHeater();
    else if(receivedChars[0] == 'l')turnOffHeater();
    else if(receivedChars[0] == 'q')heaterMidTemp();
    else if(receivedChars[0] == 'w')heaterMaxTemp();
    else if(receivedChars[0] == 'e')incrementTempByTen();
    else if(receivedChars[0] == 'r')decreaseTempByTen();
    newData = false;
    }
}

    
  
 

 

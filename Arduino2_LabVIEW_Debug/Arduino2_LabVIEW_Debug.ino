#include <avr/io.h>
#include "purge.h"
//#include <SPI.h>
#include "ArduinoTimersSetup.h"
#include "remoteControls.h"
#include "currentSense.h"
#include "HeaterPumpThermo.h"
#include "drillSpeed.h"
#include "RPMSensor.h"
#include "packetManager.h"

#define PI 3.1415926535897932384626433832795
/*
 * Current sense init changed to start counting up from 1 and resets at 1000
 * rpmInt changes to start counting up from 1 and resets at 1000
 * heaterpumpthermo changes to start counting up from 1 and resets at 1000
 * commented out rpmInt = rpmDouble
 */
int comPortInitialization;

void setup() {
                
        //Start Serial Port
        cli();
        Serial.begin(115200); 
        //========Initialize Purge Valve & PSI sensor===============================================
        purgeInit();
        
        //============Initialize Current Sensors=======================================================
        current_sense_init();
        
        //========Initialize Drill======================================================================
        fastp_init(); //set registers for fast pwm, mode 7, non-inverting
        drill_init(); //Set top value for 65Hz operation, initialize at 0% duty cycle, 
        rpmConfig();
        //========Initialize Pump=======================================================================
        pumpInit();
        //=====================Initialize Heater and Thermocouple========================================
        heaterAndThermoInit();
        sei();
        }

void loop() {
        recvWithStartEndMarkers();
        showAndExecuteCommand();
        current = speedCorrect(current, newSpeed);
//        rpmInt = rpmDouble;
        sendDataPacket();
        purgeControl();
        }

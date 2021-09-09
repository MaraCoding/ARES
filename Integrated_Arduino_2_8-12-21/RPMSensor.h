#define pulseThreshold 400
#define maxCount 400
int countInt = 0; //this variable keeps track of the number of zeros that the RPM sensor has detected
int hist, sensorValue = 0; //these variables keep track of the most recent analogRead and the past to determine if a pulse has been detected
double rpmDouble, frequencyHz, countDouble;
int pulseCount = 0;
int rpmInt = 0;
// count will have a maximum, come back to it
void rpmConfig(void)
{
  frequencyHz = 200; // sensor read rate
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  OCR1A = 1250;                  //Top count is 16M/(64*200), where 200 is 200Hz and 64 is the pre-scale value
  TIMSK1 |= (1<<OCIE1A);            //enable timer mask interrupts
  TCCR1A |= (1<<COM1A1);            //Clear on compare match mode
  TCCR1B |= (1<<WGM12);             //CTC mode FOR TIMER 1
  TCCR1B |= (1<<CS11) | (1<<CS10);  //Prescale value 64
}
//use Timer 1 for RPM 2
ISR(TIMER1_COMPA_vect){
  sensorValue = analogRead(A5);
  if(sensorValue < pulseThreshold)
    {
      countInt = countInt + 1; // if value read is smaller than pulsethreshold, a 0 is read so count increases by 1
      if(countInt >= maxCount)
      {
        countInt = 0; //200Hz times 60 per second equals a maximum of 12,000 reads in a minute
        rpmDouble = 0;
      }
      hist = sensorValue;
    }
  else if(sensorValue > pulseThreshold && hist < pulseThreshold) // pulse detected, count the number of zeros and use math to calculate RPM
    {
      pulseCount += 1;
      //RPM Formula goes here
      countDouble = countInt;
      rpmDouble = frequencyHz*(1/countDouble)*60;
      countInt = 0;
      hist = sensorValue;
    }
}

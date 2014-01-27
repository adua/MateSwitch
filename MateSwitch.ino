/* Sleep Serial
 * -----------------
 * Updated code for SwitchMate to put the Arduino to sleep 
 * and only wake up on USART activity
 */
 
#include <Servo.h>

#include <avr/power.h>
#include <avr/sleep.h>

#define SERVO_DOWN  540
#define SERVO_MID   1460
#define SERVO_UP    2400

Servo servo;

int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter for sleeping

int servoPin = 13; //If you don't understand this, kill yourself

void setup()
{
  /** Servo config **/
  servo.attach(servoPin);
  servo.writeMicroseconds(SERVO_MID);
  delay(500);
  servo.detach();
  

  Serial.begin(9600);
}

void sleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     *  the power reduction management <avr/power.h>  is described in 
     *  http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
     */  
     
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 
  
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  
  sleep_mode();            // here the device is actually put to sleep!!
 
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep:
                            // disable sleep...

  power_all_enable();
   
}


void loop()
{
  // display information about the counter
  Serial.print("Awake for ");
  Serial.print(count);
  Serial.println("sec");
  count++;
  delay(1000);                           // waits for a second

  // compute the serial input
  if (Serial.available()) {
    int val = Serial.read();
    switch(val)
    {
      case 'U':
        flipUp();
      break;
      case 'D':
        flipDown();
      break;
    }
   
   
    /**
    if (val == 'S') {
      Serial.println("Serial: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep 
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
    }
    if (val == 'A') {
      Serial.println("Waddup gangsta"); // classic dummy message
    }
    **/
  }

  // check if it should go asleep because of time
  if (count >= 10) {
      Serial.println("Timer: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep 
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
  }
}

/** Commutate the servo to flip the switch up **/
void flipUp()
{
  servo.attach(3);
  servo.writeMicroseconds(SERVO_UP);
  delay(350);
  servo.writeMicroseconds(SERVO_MID);
  delay(350);
  servo.detach();
}

/** Commutate the servo to flip the switch up **/
void flipDown()
{
  servo.attach(3);
  servo.writeMicroseconds(SERVO_DOWN);
  delay(350);
  servo.writeMicroseconds(SERVO_MID);
  delay(350);
  servo.detach();
}


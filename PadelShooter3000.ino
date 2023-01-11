#include <TimerOne.h>
#include <SoftwareSerial.h>

#define TIMER_100MS 100000                         // 100mS set timer duration in microseconds 

#define TIMER_500MS_TICK_COUNTS 5                  // 500MS worth of timer ticks
#define TIMER_1S_TICK_COUNTS 10                   // 1S worth of timer ticks

volatile long timer500MSTickCount = TIMER_500MS_TICK_COUNTS;
volatile bool in500MSRoutineISR = false;

volatile long timer1STickCount = TIMER_1S_TICK_COUNTS;
volatile bool in1SRoutineISR = false;

// ----------------- END TIMER ----------------- //

#define MODE_BUTTON_PIN 13

// ----------------- END MODE BUTTONS ----------------- //

#define  MOTOR_ENABLE_PIN 4
#define  MOTOR_1_POTMETER A0
#define  MOTOR_1_PWM_PIN 5

#define  MOTOR_2_POTMETER A1
#define  MOTOR_2_PWM_PIN 6

int motor1Speed = 0;
int motor2Speed = 0;

// ----------------- END MOTOR ----------------- //

#define  DISTANCE_SENSOR_ECHO_PIN 7
#define  DISTANCE_SENSOR_TRIGGER_PIN 8

long distanceSensorDuration;
int distanceSensorDistance;

// ----------------- END DISTANCE ----------------- //

#define SIGNAL_RED_LED_PIN 11
#define SIGNAL_GREEN_LED_PIN 10
#define SIGNAL_BLUE_LED_PIN 9

// ----------------- END SIGNAL LEDS ----------------- //


void setup() {
 initBluetooth();
 
 Timer1.initialize(TIMER_100MS);                  // Initialise timer 1
 Timer1.attachInterrupt( timerISR );           // attach the ISR routine here

 setupDistanceSensor();
 setupMotors();
 setupSignalLeds();
 setupModeButton();
}

void loop() {
  checkMode();

  if(isAnalogMode()) {    
    motor1Speed = map(analogRead(MOTOR_1_POTMETER), 0, 1024, 0, 255);
    motor2Speed = map(analogRead(MOTOR_2_POTMETER), 0, 1024, 0, 255); 
  } 

  if(isBTMode()) {
    if(isCommandSet() && hasCommandValue()) {
      if(getCommandState() == "motor1") {
        motor1Speed = getCommandValue().toInt();
      }

      if(getCommandState() == "motor2") {
        motor2Speed = getCommandValue().toInt();
      }
    }
  }

  analogWrite(MOTOR_1_PWM_PIN, motor1Speed);
  analogWrite(MOTOR_2_PWM_PIN, motor2Speed);
}

// --------------------------
// timerISR() 100 milli second interrupt ISR()
// Called every time the hardware timer 1 times out.
// --------------------------
void timerISR()
{    
    if (!(--timer500MSTickCount)) {
      timer500MSTickCount = TIMER_500MS_TICK_COUNTS;
      routineISR_500MS();
    }
    
    if (!(--timer1STickCount)) {
      timer1STickCount = TIMER_1S_TICK_COUNTS;
      routineISR_1S();
    }
}

// --------------------------
// routineISR_1S() 1 second routine
// Called every time the count gets to 1S
// --------------------------
void routineISR_500MS()
{
    if (in500MSRoutineISR) {
      return;
    }
    
    in500MSRoutineISR = true;

    interrupts();

    if(isAnalogMode()) {
      signalColor(255, 255, 0);  
    }

    if(isBTMode()) {
      signalColor(0, 0, 255);
    }

    updateSerial(); 
    
    noInterrupts();
    in500MSRoutineISR = false;
}

// --------------------------
// routineISR_1S() 1 second routine
// Called every time the count gets to 1S
// --------------------------
void routineISR_1S()
{
    if (in1SRoutineISR) {
      return;
    }
    
    in1SRoutineISR = true;

    interrupts();

    isDangerClose();
   
    noInterrupts();
    in1SRoutineISR = false;
}

void isDangerClose() {
  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, LOW);

  distanceSensorDuration = pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH);
  distanceSensorDistance = distanceSensorDuration * 0.034 / 2;

  if (distanceSensorDistance < 10) {
    signalColor(255, 0, 0);
    
    toggleMotor(LOW); // toggle both motors
  } else {
    signalColor(0, 255, 0);
    
    toggleMotor(HIGH); // toggle both motors
  }  
}

void setupMotors() {
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
 
  pinMode(MOTOR_1_PWM_PIN, OUTPUT);
  pinMode(MOTOR_2_PWM_PIN, OUTPUT);
}


void setupDistanceSensor() {
  pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);
  pinMode(DISTANCE_SENSOR_TRIGGER_PIN, OUTPUT);
}

void setupSignalLeds() {
  pinMode(SIGNAL_RED_LED_PIN, OUTPUT);
  pinMode(SIGNAL_GREEN_LED_PIN, OUTPUT);
  pinMode(SIGNAL_BLUE_LED_PIN, OUTPUT);
  
}

void toggleMotor(int state) {
  digitalWrite(MOTOR_ENABLE_PIN, state);
  digitalWrite(MOTOR_ENABLE_PIN, state);
}

void signalColor(int red, int green, int blue) {
  analogWrite(SIGNAL_RED_LED_PIN, red);
  analogWrite(SIGNAL_GREEN_LED_PIN, green);
  analogWrite(SIGNAL_BLUE_LED_PIN, blue);
}

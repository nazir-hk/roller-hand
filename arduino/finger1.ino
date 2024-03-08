#include <Servo.h>

#define ENCA 2 // YELLOW
#define ENCB 3 // WHITE
#define PWM 5
#define IN2 6
#define IN1 7

Servo myservo;  // create servo object to control a servo

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;


const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;

int servotarget = 0;             // new for this version
int dctarget = 0;             // new for this version


void setup() {
  
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);

  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
}

void loop() {
  recvWithEndMarker();
  showNewNumber();

  myservo.write(servotarget); // tell servo to go to position in variable 'pos'

  // PID constants
  float kp = 2.2;
  float kd = 1.4;
  float ki = 1.2;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  // Read the position
  int pos = 0; 
  noInterrupts(); // disable interrupts temporarily while reading
  pos = posi;
  interrupts(); // turn interrupts back on

  // error, derivative, integral
  int e = pos - dctarget;
  float dedt = (e-eprev)/(deltaT);
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > 255 ){
    pwr = 255;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir,pwr,PWM,IN1,IN2);

  // store previous error
  eprev = e;

  // SerialUSB.print(dctarget);
  // SerialUSB.print(" ");
  // SerialUSB.print(pos);
  // SerialUSB.println();
  // Serial.println(pos);


}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }  
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewNumber() {
    if (newData == true) {
        if (receivedChars[0] == 'S')
        {
          servotarget = 0;             // new for this version
          char receivedChars_s[sizeof(receivedChars) - 1];
          strcpy(receivedChars_s, receivedChars + 1);
          servotarget = atoi(receivedChars_s);   // new for this version
          newData = false;
        }
        if (receivedChars[0] == 'D')
        {
          dctarget = 0;             // new for this version
          char receivedChars_dc[sizeof(receivedChars) - 1];
          strcpy(receivedChars_dc, receivedChars + 1);
          dctarget = atoi(receivedChars_dc);   // new for this version
          newData = false;
        }
    }
}


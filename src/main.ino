#include <Arduino.h>
#include "NeuralNetwork.h"
#include "fslp.h"

NeuralNetwork *nn;

#define fslp1BotR0 25   //yellow
#define fslp1SenseLine 26 //orange -->Green
#define fslp1DriveLine1 27 //red
#define fslp1DriveLine2 14 //blue
#define fslp2BotR0 18 //yellow
#define fslp2SenseLine 4 //green
#define fslp2DriveLine1 2 //blue
#define fslp2DriveLine2 15 //red


// !!!!!I PUT 13, BUT NEEDS TO BE VERIFIED WITH THE ACTUAL ESP32 !!!!!!!
// ------------------------------------------------------------ //
#define close 13 
#define open 12 
#define controlDriver1 11 
#define controlDriver2 10 
#define enable1 9
// ------------------------------------------------------------ //

//FSLP
FSLP FSLP1(fslp1SenseLine,fslp1DriveLine1,fslp1DriveLine2,fslp1BotR0);
FSLP FSLP2(fslp2SenseLine,fslp2DriveLine1,fslp2DriveLine2,fslp2BotR0);
int pressure1 = 0;
int pressure2 = 0;
int position1 = 0;
int position2 = 0;
int slipping = 0;
bool flagClose = false;
int pwm = 0;

void setup()
{
  Serial.begin(115200);
  nn = new NeuralNetwork();
  // !!!!! NEED TO CHECK IF HIGH OR LOW )!!!!!!!
  // ------------------------------------------------------------ //
  attachInterrupt(digitalPinToInterrupt(close), &signalClose,RISING);
  attachInterrupt(digitalPinToInterrupt(open), &signalOpen,RISING);
  // ------------------------------------------------------------ //

}

void loop()
{

  pressure1 = int(FSLP1.fslpGetPressure());
  position1 = FSLP1.fslpGetPosition();
  pressure2 = FSLP2.fslpGetPressure();
  position2 = FSLP2.fslpGetPosition();
   
  // Assign the mesurements of the FSLP sensors as inputs to the model (VERIFY POSITIONS), and get result 
  // ------------------------------------------------------------ //
  nn->getInputBuffer()[0] = pressure1;
  nn->getInputBuffer()[1] = position1;
  nn->getInputBuffer()[2] = pressure2;
  nn->getInputBuffer()[3] = position2;

  int slipping = nn->predict();
  // ------------------------------------------------------------ //
  
  // Change the PWM values when the model detect slipping (PLAY WITH VALUES OF INCREMENT)
  // ------------------------------------------------------------ //
  pwm = slipping and pwm < 256 ? pwm+5 : pwm;
  analogWrite(enable1, pwm);
  // ------------------------------------------------------------ //
  
}

// Interruptions to close and open the gripper
// ------------------------------------------------------------ //
void signalClose() {
  digitalWrite(controlDriver1, HIGH);
  digitalWrite(controlDriver2, LOW);
  flagClose = true;
  pwm = 0.5 * 255;
}

void signalOpen() {
  digitalWrite(controlDriver2, HIGH);
  digitalWrite(controlDriver1, LOW);
  flagClose = false;
  pwm = 0.5 * 255;
}
// ------------------------------------------------------------ //
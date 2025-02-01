class FSLP {
  int fslpBotR0;
  int fslpSenseLine;
  int fslpDriveLine1;
  int fslpDriveLine2;
  
  public: 
    FSLP(int sl, int dl1, int dl2, int R0){
      fslpBotR0=R0;
      fslpSenseLine=sl;
      fslpDriveLine1=dl1;
      fslpDriveLine2=dl2;
    }

    double fslpGetPressure(){
      // Step 1 - Set up the appropriate drive line voltages.
      pinMode(fslpDriveLine1, OUTPUT);
      digitalWrite(fslpDriveLine1, HIGH);
      pinMode(fslpBotR0, OUTPUT);
      digitalWrite(fslpBotR0, LOW);
      pinMode(fslpSenseLine, INPUT);
      pinMode(fslpDriveLine2, INPUT);

      // Step 2 - Wait for the voltage to stabilize.
      delayMicroseconds(10);
      double v1 = analogRead(fslpDriveLine2);
      double v2 = analogRead(fslpSenseLine);

      // Step 4 - Calculate the pressure.
      // Detailed information about this formula can be found in the
      // FSLP Integration Guide.
      if (v1 == v2)
      {
        // Avoid dividing by zero, and return maximum reading.
        return 32 * 1023;
      }
      return 32 * v2 / (v1 - v2);
    }

    int fslpGetPosition(){
      // Step 1 - Clear the charge on the sensor.
      pinMode(fslpSenseLine, OUTPUT);
      digitalWrite(fslpSenseLine, LOW);

      pinMode(fslpDriveLine1, OUTPUT);
      digitalWrite(fslpDriveLine1, LOW);

      pinMode(fslpDriveLine2, OUTPUT);
      digitalWrite(fslpDriveLine2, LOW);

      pinMode(fslpBotR0, OUTPUT);
      digitalWrite(fslpBotR0, LOW);

      // Step 2 - Set up appropriate drive line voltages.
      digitalWrite(fslpDriveLine1, HIGH);
      pinMode(fslpBotR0, INPUT);
      pinMode(fslpSenseLine, INPUT);

      // Step 3 - Wait for the voltage to stabilize.
      delayMicroseconds(100);

      // Step 4 - Take the measurement.
      return analogRead(fslpSenseLine);
    }
};

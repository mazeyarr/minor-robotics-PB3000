#define SWITCH_MODE_BUTTON 12

int modeButtonState;            // the current reading from the input pin
int lastModeButtonState = LOW;  // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


currentMode = "analog";

void initModeButton() {
  pinMode(SWITCH_MODE_BUTTON, INPUT);
}

void checkMode() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(SWITCH_MODE_BUTTON);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastModeButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != modeButtonState) {
      modeButtonState = reading;

      if (modeButtonState == HIGH) {
        toggleMode();
      }
    }
  }
}

void toggleMode() {
  if (currentMode == "analog") {
    currentMode = "bluetooth"
  }

  if (currentMode == "bluetooth") {
    currentMode = "analog"
  }
}

String currentMode = "ANALOG";

int modeButtonState;            // the current currentButtonReading from the input pin
int lastModemodeButtonState = LOW;  // the previous currentButtonReading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

void setupModeButton() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);  
}

void checkMode() {
  int currentButtonReading = digitalRead(MODE_BUTTON_PIN);
  
  if(currentButtonReading == HIGH && lastModemodeButtonState == LOW && millis() - lastDebounceTime > debounceDelay)
  {
    lastDebounceTime = millis();

    toggleMode();
  }
  
  lastModemodeButtonState = currentButtonReading;
}

bool isAnalogMode() {
  return currentMode == "ANALOG";
}

bool isBTMode() {
  return currentMode == "BT";
}

void toggleMode() {
  if(isAnalogMode()) {
    currentMode = "BT";
  } else if(isBTMode()) {
    resetBluetooth();
    currentMode = "ANALOG";
  }
}

int ledPins[4] = { 2, 3, 4, 5 };
int buttonPins[4] = { 33, 34, 35, 36 };
boolean lastButtonState[4] = { LOW, LOW, LOW, LOW };
boolean buttonState[4] = { LOW, LOW, LOW, LOW };
boolean on[4] = { false, false, false, false };

int tempoVal = 0;
int buttonSwitchOrder = 39;

int currentStep = 0;
unsigned long lastStepTime = 0;

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=147,169
AudioOutputAnalog        dac1;           //xy=328,170
AudioConnection          patchCord1(waveform1, dac1);
// GUItool: end automatically generated code

void setup() {
  AudioMemory(12);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.4);
  waveform1.frequency(262);

  Serial.begin(9600);
  Serial.println(millis());

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
  }

  pinMode(buttonSwitchOrder, INPUT);
}

void loop () {
  tempoVal = 150; //analogRead(A19);
  checkButtons();
  setLeds();
  sequence();
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[i] == false) {
        on[i] = true;
      } else if (on[i] == true) {
        on[i] = false;
      }
    }
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (on[i] == true || currentStep == i) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void sequence() {
  if (digitalRead(buttonSwitchOrder) == HIGH) {
    sequenceForwards();
  }
  if (digitalRead(buttonSwitchOrder) == LOW) {
    sequenceBackwards();
  }
}

void sequenceForwards() {

  if (millis() > lastStepTime + tempoVal) {

    //set stuff off from last step
    digitalWrite(ledPins[currentStep], LOW);

    incrementUp();
    lastStepTime = millis();

    //turn stuff on for currentStep
    digitalWrite(ledPins[currentStep], HIGH);

    if (on[currentStep] == true) {
      usbMIDI.sendNoteOff(60, 0, 1);
      usbMIDI.sendNoteOn(60, 127, 1);
    }
  }
}

void sequenceBackwards() {

  if (millis() > lastStepTime + tempoVal) {

    //set stuff off from last step
    digitalWrite(ledPins[currentStep], LOW);

    incrementDown();
    lastStepTime = millis();

    //turn stuff on for currentStep
    digitalWrite(ledPins[currentStep], HIGH);

    if (on[currentStep] == true) {
      usbMIDI.sendNoteOff(60, 0, 1);
      usbMIDI.sendNoteOn(60, 127, 1);
    }
  }
}

void incrementUp() {
  currentStep = currentStep + 1;
  if (currentStep > 3) {
    currentStep = 0;
  }
}

void incrementDown() {
  currentStep = currentStep - 1;
  if (currentStep < 0) {
    currentStep = 3;
  }
}

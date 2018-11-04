// ////////////////////////////// TECHNICAL STUF ////////////////////////////////////////////////////////////

// Libraries for Audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

// Shift Register
#include <ShiftRegister74HC595.h>
const int shiCount = 5;
const int shiSTCP = 4; // latchPin
const int shiSHCP = 3; // clockPin
const int shiDS = 2;// serial Data Pin
ShiftRegister74HC595 shiftRegister (shiCount, shiDS, shiSHCP, shiSTCP); 

// Encoder
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>
long encPos  = -999;
const int encPin0 = 20;
const int encPin1 = 17;
Encoder enc(encPin0, encPin1);
const int encBtn = 16;
Bounce encBtnPush = Bounce();
byte enBtnLast = HIGH;

// Clock
unsigned long currentMillis = 0;

// 7 Segment
int segmentDigit[14][7] = {
  {1, 1, 1, 0, 1, 1, 1},  // 0
  {0, 0, 1, 0, 1, 0, 0},  // 1
  {0, 1, 0, 1, 1, 1, 1},  // 2
  {0, 1, 1, 1, 1, 0, 1},  // 3
  {1, 0, 1, 1, 1, 0, 0},  // 4
  {1, 1, 1, 1, 0, 0, 1},  // 5
  {1, 1, 1, 1, 0, 1, 1},  // 6
  {0, 0, 1, 0, 1, 0, 1},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 1, 0, 1},  // 9
  {0, 0, 0, 0, 0, 0, 1},  // Oberer Strich
  {0, 0, 0, 1, 0, 0, 0},  // Mittlerer Strich
  {0, 1, 0, 0, 0, 0, 0},  // Unterer Strich
  {0, 1, 0, 1, 0, 0, 1}   // Alle Querstriche
};
unsigned long segmentPlayPreviousMillis = 0;
unsigned int segmentPlayClock = 500;
int segmentPlayCounter = 0;
int segmentPlayMax = 3;

// stepValue
const int stepValueMax[] = {4, 8, 9, 7, 2};
// Steps
int stepSelected = 0;
const int stepCount = 5;
int stepValue[5] = {0, 0, 0, 0, 0};

// ////////////////////////////// SEGMENTS ////////////////////////////////////////////////////////////

// /////////////// INTRODUCTION ///////////////

const int introDeciFrequencies[] = {440, 400, 360, 320, 280, 220, 180};
int introDeciCounter = 0;
const int introNotesCount[] = {40, 57, 39, 35};
unsigned long introNotePreviousMillis = 0;
unsigned int introNoteClock = 400;
boolean introPlayTone = false;
int introCounter = 0;
const int introSequences[][57] = {
  {
    // PrimeNumbers
    1, 0, // 1
    1, 1, 0, // 2
    1, 1, 1, 0, // 3
    1, 1, 1, 1, 1, 0, // 5
    1, 1, 1, 1, 1, 1, 1, 0, // 7
    1, 2, 0, // 11
    1, 1, 1, 2, 0, // 13
    1, 1, 1, 1, 1, 1, 1, 2, 0, // 17    
  },
  {
    // SquareNumbers
    1, 0, // 1
    1, 1, 0, // 2
    1, 1, 1, 1, 0, // 4
    1, 1, 1, 1, 1, 1, 1, 1, 0, // 8
    1, 1, 1, 1, 1, 1, 2, 0, // 16
    1, 1, 2, 2, 2, 0, // 32
    1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, // 64
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 0 // 128  
  },
  {
    // PI
    1, 1, 1, 0, // 3
    1, 0, // 1
    1, 1, 1, 1, 0, // 4
    1, 0, // 1
    1, 1, 1, 1, 1, 0, // 5
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // 9
    1, 1, 0, // 2
    1, 1, 1, 1, 1, 1, 0 // 6
  },
  {
    // Fibonacci
    1, 0, // 1
    1, 0, // 1,
    1, 1, 0, // 2
    1, 1, 1, 0, // 3
    1, 1, 1, 1, 1, 0, // 5
    1, 1, 1, 1, 1, 1, 1, 1, 0, // 8
    1, 1, 1, 2, 0, // 13
    1, 2, 2, 0 // 21
  }
};

// /////////////// REFERENCE AND FORM ///////////////

unsigned int referenceTime = 2000;
unsigned long referPreviousMillis;
boolean referSetPreviousMillis = false;
unsigned int formTime = 2000;
unsigned long formPreviousMillis;
boolean formSetPreviousMillis = false;
unsigned long formNotePreviousMillis = 0;
unsigned int formNoteClock = 2000;
int formCounter = 0;
boolean formPlayTone = false;

// /////////////// QUALITY ///////////////

unsigned long qualNotePreviousMillis = 0;
unsigned int qualNoteClock = 2000;
boolean qualPlayTone = false;
int qualDurFrequencies[] = {262, 330, 392};
int qualMollFrequencies[] = {196, 233, 294};

// /////////////// QUANTITY ///////////////

unsigned long quantNotePreviousMillis = 0;
unsigned int quantNoteClock = introNoteClock;
int quantCounter = 0;
boolean quantPlayTone = false;
int quantValue = 0;
int quantValueCounter = 0;
int quantValueBitCounter = 0;
boolean quantValueSet = false;
unsigned long quantValuePreviousMillis;
boolean quantValuePreviousSet = false;
const unsigned int quantValueSetTiming[] = {400, 50};

// ////////////////////////////// STRUCTURE ////////////////////////////////////////////////////////////

boolean messagePlay = false;
int messageState = 0;

// ////////////////////////////// AUDIO ////////////////////////////////////////////////////////////

// /////////////// AUDIO ELEMENTS ///////////////

AudioMixer4              masterMixer0;
AudioMixer4              masterMixer1;
AudioMixer4              masterMixer;
AudioOutputI2S           LineOut;
AudioControlSGTL5000     audioShield;

// /////////////// INTRODUCTION ///////////////

AudioSynthWaveformSine  introSine;
AudioEffectEnvelope     introEnv;
// Patches
AudioConnection         patchIntro00(introSine, introEnv);
AudioConnection         patchIntro01(introEnv, 0, masterMixer0, 0);

// /////////////// REFERENCE AND FORM ///////////////

AudioSynthNoiseWhite    referNoise;
AudioFilterStateVariable referFilter;
AudioSynthWaveformSine   formSine;
AudioEffectEnvelope      formEnv;

// Patches
AudioConnection         patchWho00(referNoise, referFilter);
AudioConnection         patchWho01(referFilter, 1, masterMixer0, 1);
AudioConnection         patchForm00(formSine, formEnv);
AudioConnection         patchForm01(formEnv, 0, masterMixer0, 2);

// /////////////// QUALITY ///////////////

AudioSynthWaveformSine  qualSine0;
AudioSynthWaveformSine  qualSine1;
AudioSynthWaveformSine  qualSine2;
AudioSynthWaveform      qualSawtooth0;
AudioSynthWaveform      qualSawtooth1;
AudioSynthWaveform      qualSawtooth2;

AudioMixer4             qualDurMixer;
AudioMixer4             qualMollMixer;
AudioMixer4             qualMixer;

// Patches
AudioConnection         patchQual00(qualSine0, 0, qualDurMixer, 0);
AudioConnection         patchQual01(qualSine1, 0, qualDurMixer, 1);
AudioConnection         patchQual02(qualSine2, 0, qualDurMixer, 2);
AudioConnection         patchQual03(qualSawtooth0, 0, qualMollMixer, 0);
AudioConnection         patchQual04(qualSawtooth1, 0, qualMollMixer, 1);
AudioConnection         patchQual05(qualSawtooth2, 0, qualMollMixer, 2);

AudioConnection         patchQual06(qualDurMixer, 0, qualMixer, 0);
AudioConnection         patchQual07(qualMollMixer, 0, qualMixer, 1);

AudioConnection         patchQual08(qualMixer, 0, masterMixer0, 3);

// /////////////// QUANTITY ///////////////

AudioSynthSimpleDrum    quantTimeDrum;
AudioSynthKarplusStrong quantTimeString;

// Patches
AudioConnection         patchQuant00(quantTimeDrum, 0, masterMixer1, 0);

// /////////////// INTERSTELAR OBJECTS ///////////////

AudioMixer4              planetMixer;

// Gas Planet (Jupiter)
// Noise Generator
AudioSynthNoisePink      planetGasNoise;
// Sinus
AudioSynthWaveformSine   planetGasSine;
// Chorus
AudioEffectChorus        planetGasChorus;
#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
short delayline[CHORUS_DELAY_LENGTH];
// Filter
AudioFilterStateVariable planetGasFilterNoise;
AudioFilterStateVariable planetGasFilterSine;
// Mixer
AudioMixer4              planetGasMixer;
// Patches
AudioConnection          patchPlanetGas00(planetGasNoise, 0, planetGasFilterNoise, 0);
AudioConnection          patchPlanetGas01(planetGasSine, planetGasFilterSine);
AudioConnection          patchPlanetGas02(planetGasFilterSine, planetGasChorus);
AudioConnection          patchPlanetGas03(planetGasChorus, 0, planetGasMixer, 0);
AudioConnection          patchPlanetGas04(planetGasFilterNoise, 0, planetGasMixer, 1);
AudioConnection          patchPlanetGas05(planetGasMixer, 0, planetMixer, 0);

// Rockplanet (Venus)
AudioSynthWaveformSine   planetRockSine00;
AudioFilterStateVariable planetRockFilterSine;
AudioMixer4              planetRockSineMixer03;

float planetRockSineFreq = 43.6;
AudioSynthWaveform       planetRockWaveformTriangle00;
AudioSynthWaveform       planetRockWaveformTriangle01;
AudioSynthWaveform       planetRockWaveformTriangle02;
AudioSynthWaveform       planetRockWaveformTriangle03;
AudioEffectMultiply      planetRockMultiply03;
AudioEffectMultiply      planetRockMultiply04;
AudioEffectMultiply      planetRockMultiply05;
AudioMixer4              planetRockSineMixer00;

AudioSynthWaveformSine   planetRockSine03;
AudioSynthWaveformSine   planetRockSine04;
AudioSynthWaveformSine   planetRockSine05;
AudioSynthWaveformSine   planetRockSine06;
AudioEffectChorus        planetRockChorus01;
AudioMixer4              planetRockSineMixer01;

AudioSynthWaveform       planetRockWaveform04;
AudioSynthWaveform       planetRockWaveform05;
AudioSynthWaveform       planetRockWaveform06;
AudioSynthWaveform       planetRockWaveform07;
AudioEffectMultiply      planetRockMultiply00;
AudioEffectMultiply      planetRockMultiply01;
AudioEffectMultiply      planetRockMultiply02;
AudioFilterStateVariable planetRockFilterWaveform;
AudioMixer4              planetRockSineMixer02;

// Mixer
AudioFilterStateVariable planetRockFilterMaster;
AudioMixer4              planetRockMixer;

// Patches
AudioConnection          patchPlanetRock00(planetRockSine00, planetRockFilterSine);
AudioConnection          patchPlanetRock03(planetRockFilterSine, 0, planetRockSineMixer03, 0);
AudioConnection          patchPlanetRock04(planetRockSineMixer03, 0, planetRockMixer, 0);

AudioConnection          patchPlanetRock05(planetRockWaveformTriangle00, 0, planetRockMultiply03, 0);
AudioConnection          patchPlanetRock07(planetRockWaveformTriangle01, 0, planetRockMultiply03, 1);
AudioConnection          patchPlanetRock08(planetRockWaveformTriangle02, 0, planetRockMultiply04, 2);
AudioConnection          patchPlanetRock09(planetRockWaveformTriangle03, 0, planetRockMultiply04, 3);
AudioConnection          patchPlanetRock06(planetRockMultiply03, 0, planetRockMultiply05, 0);
AudioConnection          patchPlanetRock10(planetRockMultiply04, 0, planetRockMultiply05, 1);
AudioConnection          patchPlanetRock11(planetRockMultiply05, 0, planetRockMixer, 1);

AudioConnection          patchPlanetRock12(planetRockSine03, 0, planetRockSineMixer01, 0);
AudioConnection          patchPlanetRock13(planetRockSine04, 0, planetRockSineMixer01, 1);
AudioConnection          patchPlanetRock14(planetRockSine05, 0, planetRockSineMixer01, 2);
AudioConnection          patchPlanetRock15(planetRockSine06, 0, planetRockSineMixer01, 3);
AudioConnection          patchPlanetRock16(planetRockSineMixer01, planetRockChorus01);
AudioConnection          patchPlanetRock17(planetRockChorus01, 0, planetRockMixer, 2);

AudioConnection          patchPlanetRock18(planetRockWaveform04, 0, planetRockMultiply00, 0);
AudioConnection          patchPlanetRock19(planetRockWaveform05, 0, planetRockMultiply00, 1);
AudioConnection          patchPlanetRock20(planetRockWaveform06, 0, planetRockMultiply01, 0);
AudioConnection          patchPlanetRock21(planetRockWaveform07, 0, planetRockMultiply01, 1);
AudioConnection          patchPlanetRock22(planetRockMultiply00, 0, planetRockMultiply02, 0);
AudioConnection          patchPlanetRock23(planetRockMultiply01, 0, planetRockMultiply02, 1);
AudioConnection          patchPlanetRock24(planetRockMultiply02, planetRockFilterWaveform);
AudioConnection          patchPlanetRock25(planetRockFilterWaveform, 0, planetRockMixer, 3);

AudioConnection          patchPlanetRock26(planetRockMixer, 0, planetRockFilterMaster, 0);
AudioConnection          patchPlanetRock27(planetRockFilterMaster, 0, planetMixer, 1);

// PlanteMixer

AudioConnection          patchPlanetMaster(planetMixer, 0, masterMixer1, 1);

// /////////////// MASTER ///////////////

AudioConnection          patchXX00(masterMixer0, 0, masterMixer, 0);
AudioConnection          patchXX01(masterMixer1, 0, masterMixer, 1);
AudioConnection          patchXX02(masterMixer, 0, LineOut, 0);
AudioConnection          patchXX03(masterMixer, 0, LineOut, 1);

// ////////////////////////////// PROGRMM ////////////////////////////////////////////////////////////

// /////////////// SETUP ///////////////

void setup() {
  // Memory Space
  AudioMemory(16);

  // AudioShield
  audioShield.enable();
  audioShield.volume(0.7);

  // Intro
  introSine.frequency(440);
  introEnv.attack(50);
  introEnv.hold(50);
  introEnv.decay(50);
  introEnv.sustain(50);
  introEnv.release(50);

  // Reference
  referNoise.amplitude(0.8);
  referFilter.frequency(600);
  referFilter.resonance(7);
  masterMixer0.gain(1, 0);

  // Form
  formSine.frequency(440);
  formEnv.attack(50);
  formEnv.hold(50);
  formEnv.decay(50);
  formEnv.sustain(50);
  formEnv.release(50);

  // Quality
  qualSine0.frequency(qualDurFrequencies[0]);
  qualSine1.frequency(qualDurFrequencies[1]);
  qualSine2.frequency(qualDurFrequencies[2]);
  qualSawtooth0.begin(1, qualMollFrequencies[0], WAVEFORM_SAWTOOTH);
  qualSawtooth1.begin(1, qualMollFrequencies[1], WAVEFORM_SAWTOOTH);
  qualSawtooth2.begin(1, qualMollFrequencies[2], WAVEFORM_SAWTOOTH);

  // Gasplanet (Jupiter)
  planetGasNoise.amplitude(0.5);
  planetGasChorus.begin(delayline, CHORUS_DELAY_LENGTH, 8);
  planetGasFilterNoise.frequency(300);
  planetGasFilterNoise.resonance(5);
  planetGasSine.amplitude(0.4);
  planetGasSine.frequency(590);
  planetGasFilterSine.frequency(1800);
  planetGasFilterSine.resonance(2);
  planetGasMixer.gain(0, 1);
  planetGasMixer.gain(1, 1);

  // Rockplanet (Venus)
  planetRockSine00.frequency(880);
  planetRockFilterSine.frequency(1000);
  planetRockFilterSine.resonance(0.7);
  planetRockWaveformTriangle00.begin(1, 55, WAVEFORM_TRIANGLE);
  planetRockWaveformTriangle01.begin(1, 69.2, WAVEFORM_TRIANGLE);
  planetRockWaveformTriangle02.begin(1, 82.4, WAVEFORM_TRIANGLE);
  planetRockWaveformTriangle03.begin(1, 80, WAVEFORM_TRIANGLE); 
  planetRockSine03.frequency(73.4 * 2);
  planetRockSine04.frequency(87.3 * 2);
  planetRockSine05.frequency(43.6 * 2);
  planetRockSine06.frequency(51.9 * 2);
  planetRockChorus01.begin(delayline, CHORUS_DELAY_LENGTH, 1);  
  planetRockWaveform04.begin(1, planetRockSineFreq + 29.8, WAVEFORM_SQUARE);
  planetRockWaveform05.begin(1, planetRockSineFreq + 43.7, WAVEFORM_SQUARE);
  planetRockWaveform06.begin(1, planetRockSineFreq, WAVEFORM_SQUARE);
  planetRockWaveform07.begin(1, planetRockSineFreq + 8.3, WAVEFORM_SQUARE);
  planetRockFilterWaveform.frequency(1800);
  planetRockFilterWaveform.resonance(0.7);

  planetRockFilterMaster.frequency(800);
  planetRockFilterMaster.resonance(0);

  planetRockMixer.gain(0, 1);
  planetRockMixer.gain(1, 1);
  planetRockMixer.gain(2, 1);
  planetRockMixer.gain(3, 0.4);

  // Encoder
  pinMode(encBtn, INPUT);
  // Activate internal pull-up (optional) 
  digitalWrite(encBtn, HIGH);
  // After setting up the button, setup the object
  encBtnPush.attach(encBtn);
  encBtnPush.interval(10);

  Serial.begin(9600);
}

// /////////////// LOOP ///////////////

void loop() {
  currentMillis = millis();

  encoder();
  shiftregister();
  

  if (messagePlay) {
    if (messageState == 0) {
      if (stepValue[0] != 0) introduction();
      else messageState++;
    }
    else if (messageState > 0) {
      interstellarObject();
      if (messageState == 1) {
        if (stepValue[1] != 0) referenceAndForm();
        else messageState++;
      }
      else if (messageState == 2) {
        if (stepValue[2] != 0) quality();
        else messageState++;
      }
      else if (messageState == 3) {
        if (stepValue[3] != 0) quantity();
        else messageState++;
      }
    }
    if (messageState >= 4) messageState = 1;
  }
  else {
    introEnv.noteOff();
    masterMixer0.gain(1, 0);
    masterMixer0.gain(3, 0);
    masterMixer1.gain(1, 0);
    formEnv.noteOff();
  }

}

void resetAll() {
  messagePlay = false;
  stepSelected = 0;
  messageState = 0;
  segmentPlayCounter = 0;
  quantValue = 0;
  for (int i = 0; i < stepCount; i++) stepValue[i] = 0;
}

boolean stepValuesAllZero() {
  for (int i = 0; i < stepCount; i++) {
    if (stepValue[i] != 0) {
      return false;
      break;
    }
  }
  return true;
}

// ////////////////////////////// ENOCDER //////////////////////////////

void encoder() {
  if (!messagePlay) {
    long encNew;
    encNew = enc.read()/4;
  
    if (encNew != encPos) {
      if (quantValueSet) {
        if (currentMillis - quantValuePreviousMillis <= quantValueSetTiming[1]) quantValue = quantValue + 100 * (encPos - encNew);
        else if (currentMillis - quantValuePreviousMillis <= quantValueSetTiming[0]) quantValue = quantValue + 10 * (encPos - encNew);
        else quantValue = quantValue + 1 * (encPos - encNew);
        quantValuePreviousMillis = currentMillis;
        Serial.println(encPos);
      }
      encPos = encNew;
    }

    if (encPos > 0) {
      enc.write(0);
      encPos = 0;
    }
  
    if (!quantValueSet) {
      stepValue[stepSelected] = -encPos;
      if (encPos < -stepValueMax[stepSelected]) {
        stepValue[stepSelected] = stepValueMax[stepSelected];
        enc.write(-stepValueMax[stepSelected] * 4);
        encPos = -stepValueMax[stepSelected];
      }
    }
    else {
      if (quantValue > 99999) quantValue = 99999;
      else if (quantValue < 0) quantValue = 0;
    }
  }

  if (encBtnPush.update()) {
    if (encBtnPush.read() == LOW) {
      if (!messagePlay) {
        enc.write(0);
        if (stepSelected != 3 || quantValueSet || (stepSelected == 3 && (stepValue[3] == 0 || stepValue[3] == 1))) {
          quantValueSet = false;
          stepSelected++;
        }
        else if (!quantValueSet) quantValueSet = true;

        if (stepSelected == stepCount) {
          stepSelected = 0;
          if (!stepValuesAllZero()) messagePlay = true;
        }
      }
      else {
        messagePlay = false;
        resetAll();
      }
    }
  }
}

// ////////////////////////////// SHIFTREGISTER //////////////////////////////

void shiftregister() {
  if (!messagePlay) {
    for (int j = 0; j < stepCount; j++) {
      for (int i = 1; i < 8; i++) {
        if (!quantValueSet) shiftRegister.set(i + (8 * j), segmentDigit[stepValue[j]][i - 1]);
        else shiftRegister.set(i + (8 * ((map((j + 1) * 100, 100, stepCount * 100, stepCount * 100, 100)/100) - 1)), segmentDigit[(quantValue / int(pow(10U, j))) % 10][i - 1]);
      }
    }
  
    for (int i = 0; i < stepCount; i++) {
      if (i == stepSelected && !quantValueSet) shiftRegister.set(i * 8, HIGH);
      else shiftRegister.set(i * 8, LOW);
    }
  }

  else {
    for (int i = 0; i < stepCount; i++) shiftRegister.set(i * 8, LOW);
    if (currentMillis - segmentPlayPreviousMillis >= segmentPlayClock) {
      segmentPlayPreviousMillis = currentMillis;
      for (int j = 0; j < stepCount; j++) {
        for (int i = 1; i < 8; i++) {
          if (j == messageState) shiftRegister.set(i + (8 * j), segmentDigit[segmentPlayCounter + 10][i - 1]);
          else shiftRegister.set(i + (8 * j), segmentDigit[13][i - 1]);
        }
      }
      segmentPlayCounter++;
      if (segmentPlayCounter == segmentPlayMax) segmentPlayCounter = 0;
    }
  }
}

// ////////////////////////////// INTRODUCTION //////////////////////////////

void introduction() {
  if (introDeciCounter < 73) {
    if (currentMillis - introNotePreviousMillis >= introNoteClock) {
      introNotePreviousMillis = currentMillis;
      if (!introPlayTone) {
        Serial.println(introDeciCounter);

        if (introDeciCounter % 12 == 0 && introDeciCounter != 0) {
          introSine.frequency(introDeciFrequencies[int(introDeciCounter/12)]);
          Serial.println(introDeciFrequencies[int(introDeciCounter/12)]);
        }
        if (introDeciCounter % 12 != 1) introEnv.noteOn();

        introPlayTone = true;
      }
      else {
        introEnv.noteOff();
        introPlayTone = false;
        introDeciCounter++;
      }
    }
  }
  else if (stepValue[0] == 1) messageState++;
  else {
    if (currentMillis - introNotePreviousMillis >= introNoteClock) {
      introNotePreviousMillis = currentMillis;
      if (!introPlayTone) {
        if (introSequences[stepValue[0] - 2][introCounter] != 0) {
          introSine.frequency(introDeciFrequencies[introSequences[stepValue[0] - 2][introCounter] - 1]);
          introEnv.noteOn();
          introPlayTone = true;
        }
        introCounter++;
      }
      else {
        introEnv.noteOff();
        introPlayTone = false;
      }
    }

    if (introCounter >= introNotesCount[stepValue[0] - 2]) messageState++;    
  }
}

// ////////////////////////////// REFERENCE AND FORM //////////////////////////////

void referenceAndForm() {
  if (stepValue[1] == 0) messageState++;
  else if (stepValue[1] == 1 || stepValue[1] == 2) reference();
  else if (stepValue[1] == 3 || stepValue[1] == 4) form();
  else {
    reference();
    form();
  }
}

void reference() {
  if (!referSetPreviousMillis) {
    referPreviousMillis = currentMillis;
    referSetPreviousMillis = true;
  }

  if (stepValue[1] == 1 || stepValue[1] == 5 || stepValue[1] == 6) {
    masterMixer0.gain(1, map(currentMillis - referPreviousMillis, 0, referenceTime, 0, 1000)/1000.0);
  }
  else if (stepValue[1] == 2 || stepValue[1] == 7 || stepValue[1] == 8) {
    masterMixer0.gain(1, map(currentMillis - referPreviousMillis, 0, referenceTime, 1000, 0)/1000.0);
  }

  if (currentMillis - referPreviousMillis >= referenceTime) {
    if (stepValue[1] < 5) messageState++;
    referSetPreviousMillis = false;
  }
}

void form() {
  if (!formSetPreviousMillis) {
    formPreviousMillis = currentMillis;
    formEnv.noteOn();
    formSetPreviousMillis = true;
  }

  if (stepValue[1] == 3 || stepValue[1] == 5 || stepValue[1] == 7) {
    formSine.frequency(map(currentMillis - formPreviousMillis, 0, formTime, 220, 440));
  }
  else if (stepValue[1] == 4 || stepValue[1] == 6 || stepValue[1] == 8) {
    formSine.frequency(map(currentMillis - formPreviousMillis, 0, formTime, 440, 220));
  }

  if (currentMillis - formPreviousMillis >= formTime) {
    formEnv.noteOff();
    formSetPreviousMillis = false;
    messageState++;
  }
}

// ////////////////////////////// QUALITY //////////////////////////////

void quality() {
  if (stepValue[2] == 0) messageState++;
  else {
    if (stepValue[2] < 5) {
      qualMixer.gain(0, map(stepValue[2], 1, 5, 0, 1));
      qualMixer.gain(1, 1);
    }
    else if (stepValue[2] == 5) {
      qualMixer.gain(0, 0.5);
      qualMixer.gain(1, 0.5);
    }
    else if (stepValue[2] > 5) {      
      qualMixer.gain(0, 1);
      qualMixer.gain(1, map(stepValue[2], 5, 9, 1, 0));     
    }
  }

  if (currentMillis - qualNotePreviousMillis >= qualNoteClock) {
    qualNotePreviousMillis = currentMillis;
    if (!qualPlayTone) {
      // Note On
      qualPlayTone = true;
    }
    else {
      // Note off
      qualPlayTone = false;
      messageState++;
    }
  }
}

// ////////////////////////////// QUANTITY //////////////////////////////

void quantity() {
  if (currentMillis - quantNotePreviousMillis >= quantNoteClock) {
    quantNotePreviousMillis = currentMillis;

    if (quantValueBitCounter < 5) {
      while (bitRead(quantValue - int(quantValue/32) * 32, quantValueBitCounter) == 0) quantValueBitCounter++;
      // frequency(quantFrequencies[quantValueBitCounter]);
    }
    // else frequency(quantFrequencies[5]);
    
    if (!quantPlayTone) {
      // Note on
      quantPlayTone = true;
    }
    else {
      // Note off
      quantPlayTone = false;
      // if (stepValue[3] == 4 || stepValue[3] == 5) // TimeNoteOn
      if (quantValueBitCounter >= 5) quantValueCounter++;
      else quantValueBitCounter++;
    }

    if (quantValueCounter >= int(quantValue/32)) {
      quantValueBitCounter = 0;
      quantValueCounter = 0;
      messageState++;
    }
  }

  if (stepValue[3] == 2 || stepValue[3] == 3) {
    /*
    if (quantValueCounter % 8 >= 4) map(quantValueCounter % 4, 0, 4, 0, 1);
    else map(quantValueCounter % 4, 0, 4, 1, 0);
    */
  }
}

// ////////////////////////////// INTERSTELLAR OBJECTS //////////////////////////////

void interstellarObject() {
  if (stepValue[4] == 0) masterMixer1.gain(1, 0);
  else masterMixer1.gain(1, 1);

  if (stepValue[4] == 1) {
    planetGas();
    planetMixer.gain(0, 1);
    planetMixer.gain(1, 0);
  }
  else if (stepValue[4] == 2) {
    planetRock();
    planetMixer.gain(0, 0);
    planetMixer.gain(1, 1);
  }
}

float pulse(int intervalTime, float pulseMin, float pulseMax) {
  return sin(((currentMillis * 1000.0) / 57296.0) / intervalTime)  * ((pulseMax - pulseMin)/2) + (pulseMin + pulseMax)/2;
}

void planetGas() {
  planetGasNoise.amplitude(pulse(11, 0.5, 0.9));
  planetGasFilterNoise.frequency(pulse(30, 200, 400));
  planetGasMixer.gain(0, pulse(5, 0.1, 0.2));
}

void planetRock() {

  planetRockSineFreq = pulse(10, 43.6, 54.4);

  planetRockWaveform04.frequency(planetRockSineFreq + 29.8);
  planetRockWaveform05.frequency(planetRockSineFreq + 43.7);
  planetRockWaveform06.frequency(planetRockSineFreq);
  planetRockWaveform07.frequency(planetRockSineFreq + 8.3);

  planetRockSine00.frequency(pulse(32, 880, 940));
}
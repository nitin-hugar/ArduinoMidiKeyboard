//MIDI KEYS WITH buttons DRUMS,POT and Theremin with Midi out
//Theremin Switch
int thereminSwitch = A5;
// Theremin
int pingPin = 13;
int inPin = 13;
long duration;
int cm_new = 20;
int cm_old = 20;
int sens = 5;

//Drums
int buttonPin1 = 2;
int buttonPin2 = 3;
int buttonPin3 = 4;
int buttonPin4 = 12;
boolean currentState1 = LOW;//stroage for current button state
boolean lastState1 = LOW;//storage for last button state
boolean currentState2 = LOW;//stroage for current button state
boolean lastState2 = LOW;//storage for last button state
boolean currentState3 = LOW;//stroage for current button state
boolean lastState3 = LOW;//storage for last button state
boolean currentState4 = LOW;//stroage for current button state
boolean lastState4 = LOW;//storage for last button state
//Pot
int potval = 0; 
int lastVal = 0;
int potval2 = 0;
int lastVal2 = 0;
int potval3 = 0; 
int lastVal3 = 0;
int potval4 = 0; 
int lastVal4 = 0;

//Keyboard

//const int threshold = 50;  
const int row1 = 5;
const int row2 = 6;
const int row3 = 7;
const int row4 = 8;

// The 74HC595 uses a serial communication 
// link which has three pins
const int clock = 9;
const int latch = 10;
const int data = 11;


uint8_t keyToMidiMap[32];

boolean keyPressed[32];

int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000 };


// 74HC595 shift to next column
void scanColumn(int value) {
  digitalWrite(latch, LOW); //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH); //Pulls the latch high displaying the data


void setup() {
//Drums
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);

 
//KEYBOARD
  // Map scan matrix buttons/keys to actual Midi note number. Lowest num 41 corresponds to F MIDI note.
  keyToMidiMap[0] = 48;
  keyToMidiMap[1] = 41;
  keyToMidiMap[2] = 42;
  keyToMidiMap[3] = 43;
  keyToMidiMap[4] = 44;
  keyToMidiMap[5] = 45;
  keyToMidiMap[6] = 46;
  keyToMidiMap[7] = 47;

  keyToMidiMap[8] = 56;
  keyToMidiMap[1 + 8] = 49;
  keyToMidiMap[2 + 8] = 50;
  keyToMidiMap[3 + 8] = 51;
  keyToMidiMap[4 + 8] = 52;
  keyToMidiMap[5 + 8] = 53;
  keyToMidiMap[6 + 8] = 54;
  keyToMidiMap[7 + 8] = 55;

  keyToMidiMap[16] = 64;
  keyToMidiMap[1 + 16] = 57;
  keyToMidiMap[2 + 16] = 58;
  keyToMidiMap[3 + 16] = 59;
  keyToMidiMap[4 + 16] = 60;
  keyToMidiMap[5 + 16] = 61;
  keyToMidiMap[6 + 16] = 62;
  keyToMidiMap[7 + 16] = 63;

  keyToMidiMap[24] = 72;
  keyToMidiMap[1 + 24] = 65;
  keyToMidiMap[2 + 24] = 66;
  keyToMidiMap[3 + 24] = 67;
  keyToMidiMap[4 + 24] = 68;
  keyToMidiMap[5 + 24] = 69;
  keyToMidiMap[6 + 24] = 70;
  keyToMidiMap[7 + 24] = 71;

  // setup pins output/input mode
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);

  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);

    Serial.begin(31250);

  delay(1000);

}

void loop() {

//Theremin

int  thereminOn = analogRead( thereminSwitch ) ;
  if ( thereminOn == 0 ) 
  {

  for (int col = 0; col < 8; col++) {
    
    // shift scan matrix to following column
    scanColumn(bits[col]);

    // check if any keys were pressed - rows will have HIGH output in this case corresponding
    int groupValue1 = digitalRead(row1);
    int groupValue2 = digitalRead(row2);
    int groupValue3 = digitalRead(row3);
    int groupValue4 = digitalRead(row4);

    // process if any combination of keys pressed
    if (groupValue1 != 0 || groupValue2 != 0 || groupValue3 != 0
        || groupValue4 != 0) {

      if (groupValue1 != 0 && !keyPressed[col]) {
        keyPressed[col] = true;
        noteOn(0x91, keyToMidiMap[col], noteVelocity);
      }

      if (groupValue2 != 0 && !keyPressed[col + 8]) {
        keyPressed[col + 8] = true;
        noteOn(0x91, keyToMidiMap[col + 8], noteVelocity);
      }

      if (groupValue3 != 0 && !keyPressed[col + 16]) {
        keyPressed[col + 16] = true;
        noteOn(0x91, keyToMidiMap[col + 16], noteVelocity);
      }

      if (groupValue4 != 0 && !keyPressed[col + 24]) {
        keyPressed[col + 24] = true;
        noteOn(0x91, keyToMidiMap[col + 24], noteVelocity);
      }

    }

    //  process if any combination of keys released
    if (groupValue1 == 0 && keyPressed[col]) {
      keyPressed[col] = false;
      noteOn(0x91, keyToMidiMap[col], 0);
    }

    if (groupValue2 == 0 && keyPressed[col + 8]) {
      keyPressed[col + 8] = false;
      noteOn(0x91, keyToMidiMap[col + 8], 0);
    }

    if (groupValue3 == 0 && keyPressed[col + 16]) {
      keyPressed[col + 16] = false;
      noteOn(0x91, keyToMidiMap[col + 16], 0);
    }

    if (groupValue4 == 0 && keyPressed[col + 24]) {
      keyPressed[col + 24] = false;
      noteOn(0x91, keyToMidiMap[col + 24], 0);
    }

  }

//POT LOOP

 potval = analogRead(A0)/8;   // Divide by 8 to get range of 0-127 for midi
   if (potval != lastVal) // If the value does not = the last value the following command is made. This is because the pot has been turned. Otherwise the pot remains the same and no midi message is output.
   {
   noteOn(176,3,potval);}         // 176 = CC command (channel 1 control change), 1 = Which Control, val = value read from Potentionmeter 1 NOTE THIS SAYS VAL not VA1 (lowercase of course)
   lastVal = potval;

   potval2 = analogRead(A1)/8;   // Divide by 8 to get range of 0-127 for midi
   if (potval2 != lastVal2) 
   {
   noteOn(176,4,potval2);}         // 176 = CC command, 2 = Which Control, val = value read from Potentionmeter 2
   lastVal2 = potval2;

potval3 = analogRead(A2)/8;   // Divide by 8 to get range of 0-127 for midi
   if (potval3 != lastVal3) // If the value does not = the last value the following command is made. This is because the pot has been turned. Otherwise the pot remains the same and no midi message is output.
   {
   noteOn(176,5,potval3);}         // 176 = CC command (channel 1 control change), 1 = Which Control, val = value read from Potentionmeter 1 NOTE THIS SAYS VAL not VA1 (lowercase of course)
   lastVal3 = potval3;

   potval4 = analogRead(A3)/8;   // Divide by 8 to get range of 0-127 for midi
   if (potval4 != lastVal4) // If the value does not = the last value the following command is made. This is because the pot has been turned. Otherwise the pot remains the same and no midi message is output.
   {
   noteOn(176,6,potval4);}         // 176 = CC command (channel 1 control change), 1 = Which Control, val = value read from Potentionmeter 1 NOTE THIS SAYS VAL not VA1 (lowercase of course)
   lastVal4 = potval4;


   delay(10);
   
//Drums

currentState1 = digitalRead(buttonPin1);
  if (currentState1 == HIGH && lastState1 == LOW){//if button has just been pressed
    noteOn(0x91, 60, 127);//turn note 60 on with 127 velocity
    delay(2);//crude form of button debouncing
  } else if(currentState1 == LOW && lastState1 == HIGH){
    noteOn(0x91, 60, 0);//turn note 60 off
    delay(2);//crude form of button debouncing
  }
  lastState1 = currentState1;

  currentState2 = digitalRead(buttonPin2);
  if (currentState2 == HIGH && lastState2 == LOW){//if button has just been pressed
    noteOn(0x91, 61, 127);//turn note 60 on with 127 velocity
    delay(2);//crude form of button debouncing
  } else if(currentState2 == LOW && lastState2 == HIGH){
    noteOn(0x91, 61, 0);//turn note 60 off
    delay(2);//crude form of button debouncing
  }
  lastState2 = currentState2;

  currentState3 = digitalRead(buttonPin3);
  if (currentState3 == HIGH && lastState3 == LOW){//if button has just been pressed
    noteOn(0x91, 62, 127);//turn note 60 on with 127 velocity
    delay(2);//crude form of button debouncing
  } else if(currentState3 == LOW && lastState3 == HIGH){
    noteOn(0x91, 62, 0);//turn note 60 off
    delay(2);//crude form of button debouncing
  }
  lastState3 = currentState3;

  currentState4 = digitalRead(buttonPin4);
  if (currentState4 == HIGH && lastState4 == LOW){//if button has just been pressed
    noteOn(0x91, 63, 127);//turn note 60 on with 127 velocity
    delay(2);//crude form of button debouncing
  } else if(currentState4 == LOW && lastState4 == HIGH){
    noteOn(0x91, 63, 0);//turn note 60 off
    delay(2);//crude form of button debouncing
  }
  lastState4 = currentState4;
  }

  else
  {
//Theremin

pinMode(pingPin, OUTPUT); // setup the Pins
 
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);

pinMode(inPin, INPUT); // setup the Pins

duration = pulseIn(inPin, HIGH); // the singal from the Sensor is measured. The length of the pulse describes the distance form an object to the sensor.
 
cm_new = map(constrain(duration, 20, 3000), 50, 2000, 96, 48); // contrain --> set a threshold to cut values of the sensor that are to far away of to short (20 .. 3000) MAP --> Map the sensor range to a note values vom 96 (A high C) to 48 (a low C)

if (cm_new != cm_old) // only if a new sensor value is detected, a new nite will be send. IF the object stays stable infront of the sensor, no note will be send.
{
 
 noteOn(0x91,cm_old,0);   // Stop the old note
 noteOn(0x91,cm_new,127);  // Send a new Note (pitch , velocity 127, on channel 1)
cm_old = cm_new;
}
delay(30);
  }
}

  


void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

/*
 * JP1971 Step Sequencer
 * read 1 of 8 analog values into single analog input pin with 4051 multiplexer
 */

// array of pins used to select 1 of 8 inputs on multiplexer
const int select[] = {2,3,4}; // pins connected to the 4051 input select lines
const int analogPin = 0;      // the analog pin connected to multiplexer output

//Define the pins for the 8 step LEDs
const int led[] = {5,6,7,8,9,10,11,12};

//Define trigger outputs for drum pad
const int tom = 13;

//The stpAct array is used to toggle steps on and off
int stpAct[] = {0,0,0,0,0,0,0,0};

//Define current and previous value arrays
int curVal[] = {0,0,0,0,0,0,0,0};
int prvVal[] = {0,0,0,0,0,0,0,0};

unsigned long currentTime;
unsigned long previousTime;
unsigned long interval = 667;

int x = 0;

// Returns the analog value for the given button and sets the state of the stp
int isActive(int stp)
{
  // set the selector pins HIGH and LOW to match the binary value of step
  for(int bit = 0; bit < 3; bit++)
  {
    int pin = select[bit]; // the pin wired to the multiplexer select bit
    int isBitSet =  bitRead(stp, bit); // true if given bit set in channel
    digitalWrite(pin, isBitSet);
  }

  curVal[stp] = analogRead(analogPin);

  if(curVal[stp] > 0 && prvVal[stp] == 0){
     stpAct[stp] = 1 - stpAct[stp];
     delay(10);
   } 

   prvVal[stp] = curVal[stp];

    if (stpAct[stp] == 1) {
      digitalWrite(led[stp], HIGH);
    } else {
      digitalWrite(led[stp],LOW);
    }   
}

void trg(){
  digitalWrite(13, HIGH);  
  digitalWrite(13, LOW);
  Serial.println('trg hit!');
}

void setup()
{
  //Set the select pins to output
  for(int bit = 0; bit < 3; bit++){
    pinMode(select[bit], OUTPUT);
  }

  //Set the step LED pins to output
  for(int pin = 0; pin < 8; pin++){
    pinMode(led[pin], OUTPUT);
  }
  
  //Set the trigger pins to output
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);
}

void loop () {

  for(int stp = 0; stp < 8; stp++){
    isActive(stp);
  }
  
  currentTime = millis();
  if(currentTime - previousTime > interval){ 
    previousTime = currentTime;
    if(x>=8){
      x=0;
    }
    if(stpAct[x]){
      digitalWrite(13, HIGH);  
      digitalWrite(13, LOW);
      Serial.println('trg hit!');
    }
  }
}


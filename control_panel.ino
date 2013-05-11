//pins for the common anode rgb LED
const int rgbPins[] = {9,10,11};

#define ON LOW
#define OFF HIGH

boolean RED[] = {ON, OFF, OFF};
boolean GREEN[] = {OFF, ON, OFF};
boolean BLUE[] = {OFF, OFF, ON};

void setup(){
  Serial.begin(9600);
  
  for(int i=0; i<3; i++){
    pinMode(rgbPins[i],OUTPUT);
  }
  
}

void loop(){
  setColor(RED);
  delay(1000);
  setColor(GREEN);
  delay(100);
  setColor(BLUE);
  delay(5000);
}

void setColor(boolean color[]){
  for(int i=0; i<3; i++){
    digitalWrite(rgbPins[i],color[i]);
  }
}

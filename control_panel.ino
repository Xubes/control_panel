//pins for the common anode rgb LED
const int rgbPins[] = {9,10,11};
double RED[] = {0, 1, 1};
double GREEN[] = {1, 0, 1};
double BLUE[] = {1, 1, 0};
double TEST[] = {1, 0, 0};

/* Vars for the dial */
#define potPin A0
int currentLevel = 1;
int lastLR = 1;
int potLims[7][2] = {{0,6},{10,16},{32,45},{64,100},{128,210},{256,460},{512,1024}};

/* Vars for button. */
int buttonPin = 3;

/* Vars for serial comm. */
String inputString = "";
boolean inputComplete = false;

void setup(){
  // Setup serial
  Serial.begin(9600);
  inputString.reserve(128);
  
  // setup reset button
  pinMode(buttonPin,INPUT);
  
  // setup led
  for(int i=0; i<3; i++){
    pinMode(rgbPins[i],OUTPUT);
  }
  setColor(TEST);
  delay(1000);
  setColor(GREEN);
  
  pinMode(potPin,INPUT);
  currentLevel = getLevel(analogRead(potPin));
  Serial.println("Current level :\t" + (String)currentLevel);
   
}

void loop(){
  /* Check button. */
  int bVal = digitalRead(buttonPin);
  Serial.println("Button state: " + (String)bVal);
  
  /* Check pot. */
  int potVal = analogRead(potPin);
  int nlevel = getLevel(potVal);
  if(!(nlevel==currentLevel || nlevel==lastLR || nlevel==0)){
    setColor(BLUE);
    requestLevelChange(nlevel);
    lastLR = nlevel;
  }
}

void setColor(double* color){
  for(int i=0; i<3; i++){
    analogWrite(rgbPins[i],*color * 255);
    color++;
  }
}

/* Sends request to change level. */
void requestLevelChange(int nl){
  Serial.println("Requesting level change to " + (String)nl);
}

/* Maps the specified pot value to one of the dial levels. */
int getLevel(int p){
  for(int i=0; i<7; i++){
    if(p>=potLims[i][0] && p<=potLims[i][1]){
      return i+1;
    }
  }
  return 0;
}

/* Processes an input code. */
void processIn(String rs){
  int code = str2Int(rs);
  Serial.println("recieved code: " + String(code));
  if(code==lastLR){
    currentLevel = lastLR;
    setColor(GREEN);
    return;
  }
  switch(code){
    default:
      setColor(BLUE);
  }
}
/* Serial event to receive data from Serial Port. */
void serialEvent() {
  while(Serial.available()){
    //get the new byte:
    char inChar = (char)Serial.read();
    
    //check if line is complete
    if(inChar=='\n'){
      inputComplete = true;
      processIn(inputString);
      inputString = "";
    }
    else{
      inputString += inChar;
    }
  }
}

// split a string into 3 doubles
double* splitCodes(String s){
  double ret[3];
  int len = s.length();
  int sidx = 0;
  int counter = 0;
  while(sidx<len){
    int eidx = sidx+1;
    while(s.charAt(eidx)!=',' && eidx<len){
      eidx++;
    }
    ret[counter] = str2Float(s.substring(sidx,eidx));
    sidx = eidx+1;
  }
  return ret;
}

double str2Float(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atof(carray);
}

int str2Int(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atoi(carray);
}
//cycle through all colors
void test(){
  double ss = 0.1;
  for(double r=1.0; r>0; r-=ss){
    for(double g=1.0; g>0; g-=ss){
      for(double b=1.0; b>0; b-=ss){
        TEST[0] = r;
        TEST[1] = g;
        TEST[2] = b;
        setColor(TEST);
        delay(50);
      }
    }
  } 
}

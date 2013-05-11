//pins for the common anode rgb LED
const int rgbPins[] = {9,10,11};
double RED[] = {0, 1, 1};
double GREEN[] = {1, 0, 1};
double BLUE[] = {1, 1, 0};
double TEST[] = {1, 1, 1};

/* Vars for the dial */
#define potPin A0
int currentLevel = 1;
int lastLR = 1;
int potLims[] = {0,20,40,80,160,320,640,1280};

/* Vars for serial comm. */
String inputString = "";
boolean inputComplete = false;

void setup(){
  Serial.begin(9600);
  inputString.reserve(128);
  
  for(int i=0; i<3; i++){
    pinMode(rgbPins[i],OUTPUT);
  }
  setColor(RED);
  delay(1000);
  setColor(BLUE);
  delay(1000);
  setColor(GREEN);
  delay(1000);
  
  pinMode(potPin,INPUT);
  currentLevel = getLevel(analogRead(potPin));
   
}

void loop(){
  if(false){
    //Serial.println(inputString);
    double* color = splitCodes(inputString);
    setColor(color);
    inputComplete = false;
    inputString = "";
  }

  int potVal = analogRead(potPin);
  int nlevel = getLevel(potVal);
  if(!(nlevel==currentLevel || nlevel==lastLR)){
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
  int idx=0;
  while(p>potLims[idx] && idx<8){
    idx++;
  }
  return idx+1;
}

/* Processes a request code. */
void processRequest(String rs){
  int code = str2Int(rs);
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
      processRequest(inputString);
    }
    else{
      inputString += inChar;
    }
  }
}

// get the split strings
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

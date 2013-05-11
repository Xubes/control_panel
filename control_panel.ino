//pins for the common anode rgb LED
const int rgbPins[] = {9,10,11};

#define ON LOW
#define OFF HIGH

double RED[] = {0, 1, 1};
double GREEN[] = {1, 0, 1};
double BLUE[] = {1, 1, 0};
double TEST[] = {1, 1, .8};

void setup(){
  Serial.begin(9600);
  
  for(int i=0; i<3; i++){
    pinMode(rgbPins[i],OUTPUT);
  }
   
}

void loop(){
  setColor(TEST);
  delay(1000);
  setColor(GREEN);
  delay(100);
}

void setColor(double color[]){
  for(int i=0; i<3; i++){
    analogWrite(rgbPins[i],255*color[i]);
  }
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

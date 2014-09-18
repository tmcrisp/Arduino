const int redLED = 11;
const int greenLED = 9;
const int blueLED = 10;
const int buttonPin = 2;
const int debugPin = 13;

const int setColors[][3] = { //set pre-defined colors
  {
    255, 0, 0  }
  ,      // Red
  {
    0, 255, 0  }
  ,      // Green
  {
    0, 0, 255  }
  ,      // Blue
  {
    216, 254, 110   }
  ,  // Purple
  {
    255, 215, 0  }
  ,    // Orange
  {
    0, 255, 100  }
  ,    // Blue-Green
  {
    243, 13, 35  }
  ,    // Pink
}; 
const int numSetColors = ((sizeof(setColors)/sizeof(int))/3); //get the number of pre-defined colors
const char colorNames[numSetColors][20] = { //set the name of each pre-defined color
  "Red", 
  "Green", 
  "Blue",
  "Purple", 
  "Orange", 
  "Blue-Green", 
  "Pink", 
};

int fadeColors[] = { //default color on power-on (white)
  255,255,255};
int led[] = {  
  redLED, greenLED, blueLED};
int nextColor = 0;  //counter for switching between pre-defined colors

int lastButtonState = LOW; //button for switching between colors and fade/static modes
const int longPressDelay = 500;
int longPressTimer;

//for fading
boolean isFading = true;
unsigned long lastFadeTime;
const int fadeDelay = 5000;



void setup() {
  //Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(debugPin, OUTPUT);

  for(int i = 0; i < 3; i++) { //set initial LED colors
    analogWrite(led[i], fadeColors[i]);
  }

}

void loop() {

  int buttonState = digitalRead(buttonPin); 

  //when in fading mode, switch to static mode. current color is saved in fadeColors for cycling.
  if (buttonState != lastButtonState) {
    if(buttonState == HIGH) { 
      if(isFading == true) { 
        isFading = false;  
      }
      else { 
        if(nextColor == numSetColors) { 
          nextColor = 0;
        }
        else{
          nextColor++;
        }
      }
    }
  }
  //when in static mode, switch to fading mode on long press (0.5s)
  else if(buttonState == lastButtonState) {
    if(buttonState == HIGH) {
      if(isFading == false) {
        longPressTimer++;
        if(longPressTimer > longPressDelay) {
          isFading = true;
          longPressTimer = 0;
        }
      }
    }
  }

  lastButtonState = buttonState;

  //color fading method (smooth stepping)

  if(isFading == true) {

    digitalWrite(debugPin, HIGH);

    if (millis() - lastFadeTime > fadeDelay) {

      int newColors[] = {
        random(256), random(256), random(256)
        };

        for(int i = 0; i < 3; i++) {
          if(fadeColors[i] > newColors[i]) {
            while(fadeColors[i] > newColors[i]) {
              fadeColors[i] = fadeColors[i]--;
              analogWrite(led[i], fadeColors[i]);
              delay(5);
            }

          }
          else if(fadeColors[i] < newColors[i]) {
            while(fadeColors[i] < newColors[i]){
              fadeColors[i] = fadeColors[i]++;
              analogWrite(led[i], fadeColors[i]);
              delay(5);
            }
          }

          lastFadeTime = millis();

        }
    }
    for(int i = 0; i < 3; i++) {
    //  Serial.print(fadeColors[i]);
    //  Serial.print("\t");
    }
    //Serial.println("");
  }
  else {
    //cycle through pre-defined colors
    
    digitalWrite(debugPin, LOW);

    if(nextColor == 0) {
      for(int i = 0; i < 3; i++) {
        analogWrite(led[i], fadeColors[i]);
      //  Serial.print(fadeColors[i]);
      //  Serial.print("\t");
      }
     // Serial.println("");

    }
    else {
      for(int i = 0; i < 3; i++) {
        analogWrite(led[i], setColors[nextColor-1][i]);
      }
     // Serial.println(colorNames[nextColor-1]);
    }
  }
 delay(5);
}


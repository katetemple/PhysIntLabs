// Piranha RGB LED: smooth rainbow fade
// Board: Arduino Uno
//
// Wiring:
//   LED red   -> 220 ohm resistor -> pin 9
//   LED green -> 220 ohm resistor -> pin 10
//   LED blue  -> 220 ohm resistor -> pin 11
//   LED common leg -> 5V (common anode) or GND (common cathode)
//
// How it works:
//   The rainbow is split into three stages, each 256 steps long:
//     Stage 1: red   fades to green  (passing through yellow)
//     Stage 2: green fades to blue   (passing through cyan)
//     Stage 3: blue  fades to red    (passing through purple)
//   That makes 768 steps in total, and then it starts again.

const int RED_PIN   = 11;    // LED pins must be PWM pins (marked ~)
const int GREEN_PIN = 10;
const int BLUE_PIN  = 9;

int potValue = 0; // 0 to 1023 
int redLevel = 0;
int blueLevel = 0;

int ldrDark = 500;
int ldrBright = 810;
int lightValue;

int buttonPin = 2;
const int NUM_COLOURS = 10;

int buttonState = HIGH; 
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

const int colours[NUM_COLOURS][3] = { 
    {255, 0, 0},     // 0 Red
    {255, 80, 0},    // 1 Orange
    {0, 0, 255},     // 2 Blue
    {0, 255, 0},     // 3 Green
    {255, 255, 0},   // 4 Yellow
    {128, 0, 255},   // 5 Purple
    {255, 0, 255},   // 6 Magenta
    {0, 255, 255},   // 7 Cyan
    {255, 255, 255}, // 8 White
    {255, 105, 180}  // 9 Pink
};

// Change this to match your LED:
// true  = common anode   (common leg connected to 5V)
// false = common cathode (common leg connected to GND)
const bool COMMON_ANODE = true;

// Time between steps in milliseconds.
// 10 gives one full rainbow roughly every 8 seconds.
// Smaller = faster, larger = slower.
const int FADE_DELAY = 10;

int position = 0;   // where we are in the rainbow: 0 to 767

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(A5));
}

void loop() {
  int reading = digitalRead(buttonPin);
  Serial.println(reading);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        int currentColour = random(NUM_COLOURS);

        setColour(
          colours[currentColour][0],
          colours[currentColour][1],
          colours[currentColour][2]
    );
      }
    }

    lastButtonState = reading;
  }

 
  // lightValue = constrain(analogRead(A2), ldrDark, ldrBright);

  // potValue = analogRead(A2);
  // // blueLevel = map(potValue, 0, 1023, 0, 255); 
  // blueLevel = map(lightValue, ldrDark, ldrBright, 0, 255);
  // redLevel = 255 - blueLevel; // red falls as blue rises
  // Serial.print("Pot: ");
  // Serial.print(potValue);
  // Serial.print(" Red: ");
  // Serial.print(redLevel);
  // Serial.print(" Blue: ");
  // Serial.println(blueLevel);
  // setColour(redLevel, 0, blueLevel);



  // delay(FADE_DELAY);
}

// Sets the LED colour using brightness values from 0 (off) to 255 (full)
void setColour(int red, int green, int blue) {

  // A common anode LED works "upside down": LOW means on.
  // So we flip the values before sending them to the pins.
  if (COMMON_ANODE) {
    red   = 255 - red;
    green = 255 - green;
    blue  = 255 - blue;
  }

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

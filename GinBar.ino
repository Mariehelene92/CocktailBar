/******* Recipe definition *******/
#include "recipes.h"

/******* Pin and variable definition *******/
#define PUMP_1 2
#define PUMP_2 3
#define PUMP_3 4
int pumpNUmber = 3;
int PUMPS[3] {PUMP_1, PUMP_2, PUMP_3};
#define AGITATEUR 5
#define BUTTON_UP A1
#define BUTTON_DOWN A3
#define BUTTON_VAL A2
#define DATA 8
#define CLK 7
bool busy = true;
int tempChoice = 1;
int choice = 0;
float percentage = 0.8;

/******* Scale *******/
#include "HX711.h"
HX711 scale(DATA, CLK);

/******* OLED screen *******/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/******* Buttons *******/
bool buttonUp;
bool previousButtonUp;
bool buttonDown;
bool previousButtonDown;
bool buttonVal;
bool previousButtonVal;

void setup() {
  initBar();
}

void loop() {
  if (Serial.available()) {
    choice = Serial.parseInt();
    Serial.print(choice);         // print the character
  }
  HMIProcessing();

  if (choice > 0 && choice <= rows) {
    prepareBeverage(choice, cocktailName[choice - 1]);
    choice = 0;
  }
}

/******* initBar -> initializes all the components of the bar *******/
void initBar() {
  busy = true;
  Serial.begin(9600);
  Serial.println(F("Initializing Bar"));
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  //Clear the buffer
  display.clearDisplay();


  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);
  pinMode(PUMP_3, OUTPUT);
  digitalWrite(PUMP_1, HIGH);
  digitalWrite(PUMP_2, HIGH);
  digitalWrite(PUMP_3, HIGH);
  pinMode(AGITATEUR, OUTPUT);
  digitalWrite(AGITATEUR, HIGH);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_VAL, INPUT_PULLUP);

  scale.set_scale(-1896.37f);
  scale.tare();

  busy = false;
  Serial.println(F("Type 1, 2 or 3 to choose a cocktail."));
}

/******* prepareBeverage -> goes through all the steps to prepare a beverage *******/
void prepareBeverage(int requestedBeverage, String cocktail) {
  busy = true;
  oledDisplay(F("Commande en cours"));

  for (int jj = 0; jj < 3; jj++) {
    int bottle = cocktails[requestedBeverage - 1][jj];
    if (bottle != 0) {
      int quantity = cocktails[requestedBeverage - 1][3 + jj];
      pourLiquid(bottle, quantity);
      delay(500);
    }
  }
  mixBeverage(500);
  oledDisplay(F("Boisson   prete"));
  delay(1000);

  busy = false;
  Serial.println(F("Fancy another drink?"));
}

/******* pourLiquid -> pours the quantity of liquid defined in recipes.h *******/
void pourLiquid(int bottle, int dispensingQuantity) {
  scale.power_up();
  delay(100);
  scale.tare();
  float testScale = scale.get_units();
  float limitQuantity = (float) dispensingQuantity * percentage;
  openPump(bottle);
  while (testScale < limitQuantity) {     //Si on a pas la bonne quantité, soit on continues de monter la paddle por faire couler le liquide, soit on attends qu'il y en ait suffisamment.
    testScale = scale.get_units();
    Serial.println(testScale);
    delay(10);
  }
  closePump(bottle);
  delay(2000);
  scale.power_down();             // put the ADC in sleep mode
}

/******* openPump -> starts the pump number bottle *******/
void openPump(int bottle) {
  digitalWrite(PUMPS[bottle - 1], LOW);
}

/******* closePump -> stops the pump number bottle *******/
void closePump(int bottle) {
  digitalWrite(PUMPS[bottle - 1], HIGH);
}

/******* mixBeverage -> starts the magnetic stirrer for a defined time *******/
void mixBeverage(int stirTime) {
  digitalWrite(AGITATEUR, LOW);
  delay(stirTime);
  digitalWrite(AGITATEUR, HIGH);
}

/******* HMIProcessing -> processes the pushbuttons and displays info on the OLED *******/
void HMIProcessing() {
  buttonUpPushed();
  buttonDownPushed();
  buttonValPushed();
  if (!busy) {
    if (buttonUp) {
      tempChoice += 1;
      if (tempChoice > rows) {
        tempChoice = 1;
      }
    } else if (buttonDown) {
      tempChoice -= 1;
      if (tempChoice < 1) {
        tempChoice = rows;
      }
    } else if (buttonVal) {
      choice = tempChoice;
      tempChoice = 1;
    }
  }

  if (!buttonVal) {
    oledDisplay(cocktailName[tempChoice - 1]);
  } else {
    oledDisplay(cocktailName[choice - 1]);
  }
  delay(100);
}

/******* buttonUpPushed -> checks if the Up button has been pushed *******/
void buttonUpPushed() {
  bool button = digitalRead(BUTTON_UP);
  if (!button && previousButtonUp)//si bouton appuyé
  {
    buttonUp = true;
  } else {
    buttonUp = false;
  }
  previousButtonUp = button;
}

/******* buttonDownPushed -> checks if the Down button has been pushed *******/
void buttonDownPushed() {
  bool button = digitalRead(BUTTON_DOWN);
  if (!button && previousButtonDown)//si bouton appuyé
  {
    buttonDown = true;
  } else {
    buttonDown = false;
  }
  previousButtonDown = button;
}

/******* buttonValPushed -> checks if the Val button has been pushed *******/
void buttonValPushed() {
  bool button = digitalRead(BUTTON_VAL);
  if (!button && previousButtonVal)//si bouton appuyé
  {
    buttonVal = true;
  } else {
    buttonVal = false;
  }
  previousButtonVal = button;
}

/******* oledDisplay -> displays a string on the oled *******/
void oledDisplay(String string) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(string);
  display.display();      // Show initial text
  delay(50);
}

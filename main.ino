#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16x2 display

int xPin = A0;  // select the input pin for the X axis of the joystick
int yPin = A1;  // select the input pin for the Y axis of the joystick
int xValue = 0;  // variable to store the X axis value
int yValue = 0;  // variable to store the Y axis value

int dotX = 7;  // initial X position of the dot
int dotY = 1;  // initial Y position of the dot

int targetX = 0;  // X position of the target
int targetY = 0;  // Y position of the target

bool gameStarted = false;  // flag to indicate if the game has started
bool gameFinished = false;  // flag to indicate if the game has finished

void setup() {
  lcd.init();  // initialize the LCD
  lcd.backlight();  // turn on the backlight
  lcd.setCursor(dotX, dotY);
  lcd.print("*");
  Serial.begin(9600);  // initialize serial communication at 9600 bits per second
}

void loop() {
  if (!gameStarted) {
    // game has not started yet, display the instructions and wait for the player to press the joystick button
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESS THE BUTTON");
    lcd.setCursor(0, 1);
    lcd.print("TO START");
    while (digitalRead(2) == HIGH) {
      // wait for the player to press the joystick button
    }
    delay(500);  // debounce the button
    gameStarted = true;
    generateTarget();
  }
  else if (gameFinished) {
    // game has finished, display the result and wait for the player to press the joystick button to restart
    lcd.clear();
    lcd.setCursor(0, 0);
    if (didWin()) {
      lcd.print("YOU WIN!");
    }
    else {
      lcd.print("GAME OVER");
    }
    lcd.setCursor(0, 1);
    lcd.print("PRESS BUTTON TO");
    lcd.setCursor(0, 2);
    lcd.print("PLAY AGAIN");
    while (digitalRead(2) == HIGH) {
      // wait for the player to press the joystick button
    }
    delay(500);  // debounce the button
    gameStarted = false;
    gameFinished = false;
    dotX = 7;
    dotY = 1;
    targetX = 0;
    targetY = 0;
    lcd.clear();
  }
  else {
    // game is in progress, move the dot based on the joystick input
    xValue = analogRead(xPin);  // read the X axis value
    yValue = analogRead(yPin);  // read the Y axis value

    // update the dot position based on the joystick values
    if (xValue < 450 && dotX > 0) {
      dotX--;
    }
    if (xValue > 550 && dotX < 15) {
      dotX++;
    }
    if (yValue < 450 && dotY > 0) {
      dotY--;
    }
    if (yValue > 550 && dotY < 1) {
      dotY++;
    }

    // check if the dot has reached the target
    if (dotX == targetX && dotY == targetY) {
      gameFinished = true;
    }

    // update the LCD display with the new dot position
    lcd.clear();
    lcd.setCursor(dotX, dotY);
    lcd.print("*");

    // display the target location
    lcd.setCursor(targetX, targetY);
    lcd.write(byte(1));

    // delay to slow down the dot movement
    delay(100);
  }
}

void generateTarget() {
  // randomly generate a target location
  randomSeed(analogRead(0));
  targetX = random(0, 16);
  targetY = random(0, 2);

  // display the target location
  lcd.setCursor(targetX, targetY);
  lcd.write(byte(1));
}

bool didWin() {
  // check if the player won the game
  if (dotX == targetX && dotY == targetY) {
    return true;
  }
  else {
    return false;
  }
}


#include <LedControl.h>  // Include the LedControl library to control the LED matrix

// LED Matrix setup: define pins for communication with the LED matrix
#define CLK_PIN 10    // Clock pin for the LED matrix
#define CS_PIN 11     // Chip select pin for the LED matrix
#define DIN_PIN 12    // Data input pin for the LED matrix

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // Create an instance of the LedControl class

// Joystick pins: define pins for the joystick
#define JOY_X A3  // X-axis pin for the joystick
#define JOY_Y A2  // Y-axis pin for the joystick
#define JOY_SW 2  // Button pin on the joystick

// Game variables
const int matrixSize = 8;  // The size of the LED matrix (8x8)
int snakeX[64];            // Array to store the X positions of the snake's body
int snakeY[64];            // Array to store the Y positions of the snake's body
int snakeLength = 1;       // The length of the snake, starting with 1
int appleX, appleY;        // Position of the apple
int dirX = 0, dirY = 0;    // Direction of movement (X and Y axis)
bool gameOver = false;     // Boolean flag to check if the game is over

void setup() {
  pinMode(JOY_SW, INPUT_PULLUP); // Set joystick button pin as input with internal pull-up resistor
  lc.shutdown(0, false);         // Wake up the LED matrix
  lc.setIntensity(0, 8);         // Set the brightness of the LED matrix
  lc.clearDisplay(0);            // Clear the LED matrix

  // Initialize the snake position at the center of the matrix
  snakeX[0] = 4;
  snakeY[0] = 4;

  // Place the first apple randomly on the matrix
  placeApple();
}

void loop() {
  if (gameOver) {   // If the game is over, show the Game Over screen
    displayGameOver();
    if (digitalRead(JOY_SW) == LOW) { // If the joystick button is pressed, reset the game
      resetGame();
    }
    return;  // Stop the game loop and wait for the button press
  }

  readJoystick();  // Read the joystick input to control the snake
  moveSnake();     // Move the snake based on the joystick input
  checkCollision(); // Check for collisions with the snake's body or apple
  drawMatrix();     // Draw the snake and apple on the LED matrix

  delay(200); // Control the speed of the game (lower value = faster)
}

void readJoystick() {
  int xVal = analogRead(JOY_X); // Read the X axis value from the joystick
  int yVal = analogRead(JOY_Y); // Read the Y axis value from the joystick

  // Check joystick direction and set movement direction accordingly
  if (xVal < 400 && dirX == 0) {   // If joystick is moved to the left
    dirX = -1;
    dirY = 0;
  } else if (xVal > 600 && dirX == 0) { // If joystick is moved to the right
    dirX = 1;
    dirY = 0;
  } else if (yVal < 400 && dirY == 0) { // If joystick is moved up
    dirX = 0;
    dirY = -1;
  } else if (yVal > 600 && dirY == 0) { // If joystick is moved down
    dirX = 0;
    dirY = 1;
  }
}

void moveSnake() {
  // Move the body of the snake by shifting each segment to the position of the previous segment
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move the head of the snake in the direction specified by dirX and dirY
  snakeX[0] += dirX;
  snakeY[0] += dirY;

  // Handle wrapping the snake around the edges of the matrix
  if (snakeX[0] < 0) snakeX[0] = matrixSize - 1;   // Wrap around to the right if snake goes off the left
  if (snakeX[0] >= matrixSize) snakeX[0] = 0;       // Wrap around to the left if snake goes off the right
  if (snakeY[0] < 0) snakeY[0] = matrixSize - 1;    // Wrap around to the bottom if snake goes off the top
  if (snakeY[0] >= matrixSize) snakeY[0] = 0;       // Wrap around to the top if snake goes off the bottom
}

void checkCollision() {
  // Check if the snake has collided with itself
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true; // Set game over flag if collision with body
      return;
    }
  }

  // Check if the snake has eaten an apple
  if (snakeX[0] == appleX && snakeY[0] == appleY) {
    snakeLength++;    // Increase snake length
    placeApple();     // Place a new apple on the matrix
  }
}

void placeApple() {
  while (true) {
    appleX = random(0, matrixSize); // Random X position for apple
    appleY = random(0, matrixSize); // Random Y position for apple

    // Ensure apple does not overlap the snake's body
    bool valid = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == appleX && snakeY[i] == appleY) {
        valid = false; // If apple overlaps with snake, try again
        break;
      }
    }

    if (valid) break; // Break out of the loop if the apple position is valid
  }
}

void drawMatrix() {
  lc.clearDisplay(0);  // Clear the LED matrix display

  // Draw the snake on the matrix
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true); // Set LED for each segment of the snake's body
  }

  // Draw the apple on the matrix
  lc.setLed(0, appleY, appleX, true); // Set LED for the apple's position
}

void displayGameOver() {
  lc.clearDisplay(0); // Clear the display to show the game over screen
  for (int i = 0; i < matrixSize; i++) {
    lc.setRow(0, i, 0b10101010); // Display a pattern for the game over screen
  }
}

void resetGame() {
  snakeLength = 1;  // Reset snake length to 1
  snakeX[0] = 4;    // Reset snake position to the center
  snakeY[0] = 4;
  dirX = 0;         // Reset snake movement direction to 0
  dirY = 0;
  gameOver = false; // Set game over flag to false
  placeApple();     // Place a new apple
}

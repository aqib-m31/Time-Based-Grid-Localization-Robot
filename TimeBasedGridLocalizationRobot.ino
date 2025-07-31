/*
* TimeBasedGridLocalizationRobot.ino
* An autonomous robot that navigates to a destination on a 5x5 grid using time-based movement control.
*/

// Grid Configuration
#define ROWS 5
#define COLS 5

int GRID[ROWS][COLS] = {
  { 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0 },  // Destination marked as 1
  { 0, 0, 0, 0, 0 },
};

//Motor Driver Pins (PWM for motor speed control)
const int EN_A = 10;  // Right motor enable pin (Grey)
const int EN_B = 11;  // Left motor enable pin (Orange)

// Motors from each side are connected to the same output pins of Motor Driver
// Motor control pins
const int MOTOR_RIGHT_1 = 8;  // INPUT 1 - Blue
const int MOTOR_RIGHT_2 = 9;  // INPUT 2 - Purple
const int MOTOR_LEFT_1 = 6;   // INPUT 3 - Green
const int MOTOR_LEFT_2 = 7;   // INPUT 4 - Yellow

const int CELL_TRAVERSE_TIME = 880;  // Time to traverse one grid cell (ms)

// Time constants (in ms) for x-degree turn
enum RotationTime {
  TURN_TIME_180 = 1250,  // Time for 180-degree turn
  TURN_TIME_90 = 890,    // Time for 90-degree turn
};

// Robot position and destination tracking
int originX = 0;
int originY = 0;
int destX = 0;
int destY = 0;
int curX = 0;
int curY = 0;

int destination = 0;  // Flag to indicate destination is reached

void setup() {
  // Set motor driver front pins as OUTPUT pins
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(MOTOR_RIGHT_1, OUTPUT);
  pinMode(MOTOR_RIGHT_2, OUTPUT);
  pinMode(MOTOR_LEFT_1, OUTPUT);
  pinMode(MOTOR_LEFT_2, OUTPUT);

  // Find destination coordinates from the grid
  get_destination_coordinates();
}

void loop() {
  if (destination == 0) {
    gotoDestination(destX, destY);
    destination = 1;  // Mark destination reached
  } else {
    stop(2000);  // Stop once destination is reached
  }
}

// Get destination coordinates from the grid
void get_destination_coordinates() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < ROWS; j++) {
      if (GRID[i][j] == 1) {  // Find destination cell marked with 1
        destX = i;
        destY = j;
        return;
      }
    }
  }
}

// Set speed for motors
void setSpeed(unsigned int speed) {
  analogWrite(EN_A, speed);
  analogWrite(EN_B, speed);
}

// Move forward all 4 motors for a given time and speed
void forward(unsigned int time, unsigned int speed) {
  setSpeed(speed);
  digitalWrite(MOTOR_RIGHT_1, HIGH);
  digitalWrite(MOTOR_RIGHT_2, LOW);
  digitalWrite(MOTOR_LEFT_1, HIGH);
  digitalWrite(MOTOR_LEFT_2, LOW);
  delay(time);
}

// Move backward all 4 motors for a given time and speed
void reverse(unsigned int time, unsigned int speed) {
  setSpeed(speed);
  digitalWrite(MOTOR_RIGHT_1, LOW);
  digitalWrite(MOTOR_RIGHT_2, HIGH);
  digitalWrite(MOTOR_LEFT_1, LOW);
  digitalWrite(MOTOR_LEFT_2, HIGH);
  delay(time);
}

// Stop all motors
void stop(unsigned int time) {
  digitalWrite(MOTOR_RIGHT_1, LOW);
  digitalWrite(MOTOR_RIGHT_2, LOW);
  digitalWrite(MOTOR_LEFT_1, LOW);
  digitalWrite(MOTOR_LEFT_2, LOW);
  delay(time);
}

// Turn robot left
void turnLeft(unsigned int time, unsigned int speed) {
  setSpeed(speed);
  digitalWrite(MOTOR_RIGHT_1, HIGH);
  digitalWrite(MOTOR_RIGHT_2, LOW);
  digitalWrite(MOTOR_LEFT_1, LOW);
  digitalWrite(MOTOR_LEFT_2, HIGH);
  delay(time);
}

// Turn robot right
void turnRight(unsigned int time, unsigned int speed) {
  setSpeed(speed);
  digitalWrite(MOTOR_RIGHT_1, LOW);
  digitalWrite(MOTOR_RIGHT_2, HIGH);
  digitalWrite(MOTOR_LEFT_1, HIGH);
  digitalWrite(MOTOR_LEFT_2, LOW);
  delay(time);
}

// Navigate to destination coordinates
void gotoDestination(int destX, int destY) {
  int deltaX = destX - curX;
  int deltaY = destY - curY;

  // Move along Y-axis (forward or backward)
  if (deltaY > 0) {
    forward(deltaY * CELL_TRAVERSE_TIME, 70);
    curY += deltaY;
  } else if (deltaY < 0) {
    reverse(deltaY * CELL_TRAVERSE_TIME, 70);
    curY -= deltaY;
  }

  stop(2000);

  // Adjust orientation and move along X-axis
  if (deltaX > 0) {
    turnRight(TURN_TIME_90, 60);
  } else if (deltaX < 0) {
    turnLeft(TURN_TIME_90, 60);
  }

  stop(500);

  forward(abs(deltaX) * CELL_TRAVERSE_TIME, 70);
  curX += deltaX;

  stop(2000);

  if (deltaX > 0) {
    turnLeft(TURN_TIME_90, 60);
  } else if (deltaX < 0) {
    turnRight(TURN_TIME_90, 60);
  }

  stop(2000);
}

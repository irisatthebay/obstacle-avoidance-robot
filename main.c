#include <NewPing.h>        // Ultrasonic sensor function library. Install this library before use.
#include <Servo.h>          // Servo motor library. This is a standard Arduino library.

const int LeftMotorForward = 2;    // Left motor forward control pin
const int LeftMotorBackward = 3;   // Left motor backward control pin
const int RightMotorForward = 4;   // Right motor forward control pin
const int RightMotorBackward = 5;  // Right motor backward control pin

// Sensor pins
#define trig_pin A1   // Ultrasonic sensor trigger pin connected to analog input A1
#define echo_pin A2   // Ultrasonic sensor echo pin connected to analog input A2

#define maximum_distance 200  // Maximum distance for ultrasonic sensor readings in cm
boolean goesForward = false;  // State to track forward movement
int distance = 100;           // Initial distance to an obstacle

NewPing sonar(trig_pin, echo_pin, maximum_distance); // Initialize ultrasonic sensor
Servo servo_motor;           // Initialize servo motor

void setup() {
  // Configure motor pins as outputs
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  servo_motor.attach(8); // Attach servo motor to pin 8

  servo_motor.write(115); // Set servo to initial forward-facing position
  delay(2000);            // Allow servo to stabilize

  // Take multiple initial distance readings to stabilize sensor input
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  int distanceRight = 0;  // Variable to store distance on the right
  int distanceLeft = 0;   // Variable to store distance on the left
  delay(50);              // Small delay for stable readings

  if (distance <= 20) {   // If an obstacle is detected within 20 cm
    moveStop();           // Stop the robot
    delay(300);
    moveBackward();       // Move backward slightly
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight(); // Check distance to the right
    delay(300);
    distanceLeft = lookLeft();   // Check distance to the left
    delay(300);

    // Decide to turn based on which side has more space
    if (distance >= distanceLeft) {
      turnRight();        // Turn right if the right side is clearer
      moveStop();
    } else {
      turnLeft();         // Otherwise, turn left
      moveStop();
    }
  } else {
    moveForward();        // Move forward if no obstacle is close
  }
  distance = readPing();  // Update the distance reading
}

int lookRight() {  
  servo_motor.write(50);       // Rotate servo to the right
  delay(500);                  // Allow time for the sensor to stabilize
  int distance = readPing();   // Get distance reading
  delay(100);
  servo_motor.write(115);      // Reset servo to forward position
  return distance;             // Return the measured distance
}

int lookLeft() {
  servo_motor.write(170);      // Rotate servo to the left
  delay(500);                  // Allow time for the sensor to stabilize
  int distance = readPing();   // Get distance reading
  delay(100);
  servo_motor.write(115);      // Reset servo to forward position
  return distance;             // Return the measured distance
}

int readPing() {
  delay(70);                   // Small delay for stable readings
  int cm = sonar.ping_cm();    // Get distance in cm from the ultrasonic sensor
  if (cm == 0) {               // If no object is detected
    cm = 250;                  // Set distance to a default maximum value
  }
  return cm;                   // Return the distance
}

void moveStop() {
  // Stop all motor movements
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward() {
  if (!goesForward) {         // If not already moving forward
    goesForward = true;       // Update state
    digitalWrite(LeftMotorForward, HIGH);   // Turn on forward movement
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);   // Turn off backward movement
    digitalWrite(RightMotorBackward, LOW);
  }
}

void moveBackward() {
  goesForward = false;        // Update state
  digitalWrite(LeftMotorBackward, HIGH);    // Turn on backward movement
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);      // Turn off forward movement
  digitalWrite(RightMotorForward, LOW);
}

void turnRight() {
  // Turn the robot to the right
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  delay(500);                 // Allow time for turning
  // Resume forward movement
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void turnLeft() {
  // Turn the robot to the left
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  delay(500);                 // Allow time for turning
  // Resume forward movement
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

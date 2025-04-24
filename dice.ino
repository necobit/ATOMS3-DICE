#include "M5AtomS3.h"
#include "M5_IMU.h"

// IMU sensor
IMU imu;

// Variables for dice
int diceResult = 0;
bool rolling = false;
unsigned long rollStartTime = 0;
const unsigned long rollDuration = 3000; // 3 seconds of animation

// Threshold for detecting shake
const float shakeThreshold = 2.0; // Adjust based on sensitivity needed

void setup() {
  M5.begin(true, true, false, false); // Initialize M5AtomS3
  M5.dis.clear();
  
  // Initialize IMU
  if (!imu.begin()) {
    M5.dis.fillpix(0xff0000); // Red if IMU initialization fails
    delay(1000);
  }
  
  // Show ready state
  M5.dis.fillpix(0x00ff00); // Green when ready
  delay(500);
  M5.dis.clear();
  
  // Display initial dice face
  displayNumber(1);
  
  randomSeed(analogRead(0)); // Initialize random number generator
}

void loop() {
  M5.update();
  
  float accX, accY, accZ;
  
  // Read accelerometer data
  imu.getAccel(&accX, &accY, &accZ);
  
  // Calculate total acceleration magnitude
  float totalAcc = sqrt(accX * accX + accY * accY + accZ * accZ);
  
  // Check if shaking (acceleration exceeds threshold)
  if (!rolling && totalAcc > shakeThreshold) {
    // Start rolling animation
    rolling = true;
    rollStartTime = millis();
    M5.dis.fillpix(0x0000ff); // Blue during rolling
    delay(100);
  }
  
  // Handle dice rolling animation
  if (rolling) {
    unsigned long currentTime = millis();
    
    // During the rolling animation
    if (currentTime - rollStartTime < rollDuration) {
      // Display random numbers during animation
      if ((currentTime - rollStartTime) % 200 == 0) { // Change number every 200ms
        int randomFace = random(1, 7); // Random number between 1-6
        displayNumber(randomFace);
      }
    } else {
      // Animation finished, show final result
      diceResult = random(1, 7); // Final random number between 1-6
      displayNumber(diceResult);
      
      // Reset rolling state
      rolling = false;
      
      // Wait a moment before allowing another shake
      delay(1000);
    }
  }
  
  // Small delay to prevent too frequent readings
  delay(50);
}

// Function to display dice number on the LED matrix
void displayNumber(int number) {
  M5.dis.clear();
  
  switch (number) {
    case 1:
      // Center dot
      M5.dis.drawpix(2, 2, 0xffffff);
      break;
      
    case 2:
      // Top-left and bottom-right
      M5.dis.drawpix(1, 1, 0xffffff);
      M5.dis.drawpix(3, 3, 0xffffff);
      break;
      
    case 3:
      // Top-left, center, and bottom-right
      M5.dis.drawpix(1, 1, 0xffffff);
      M5.dis.drawpix(2, 2, 0xffffff);
      M5.dis.drawpix(3, 3, 0xffffff);
      break;
      
    case 4:
      // Four corners
      M5.dis.drawpix(1, 1, 0xffffff);
      M5.dis.drawpix(1, 3, 0xffffff);
      M5.dis.drawpix(3, 1, 0xffffff);
      M5.dis.drawpix(3, 3, 0xffffff);
      break;
      
    case 5:
      // Four corners and center
      M5.dis.drawpix(1, 1, 0xffffff);
      M5.dis.drawpix(1, 3, 0xffffff);
      M5.dis.drawpix(2, 2, 0xffffff);
      M5.dis.drawpix(3, 1, 0xffffff);
      M5.dis.drawpix(3, 3, 0xffffff);
      break;
      
    case 6:
      // Six dots in 2 columns of 3
      M5.dis.drawpix(1, 1, 0xffffff);
      M5.dis.drawpix(1, 2, 0xffffff);
      M5.dis.drawpix(1, 3, 0xffffff);
      M5.dis.drawpix(3, 1, 0xffffff);
      M5.dis.drawpix(3, 2, 0xffffff);
      M5.dis.drawpix(3, 3, 0xffffff);
      break;
  }
}

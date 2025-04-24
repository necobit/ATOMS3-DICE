#include "M5Unified.h"

// Variables for dice
int diceResult = 0;
bool rolling = false;
unsigned long rollStartTime = 0;
const unsigned long rollDuration = 3000; // 3 seconds of animation

// Threshold for detecting shake
const float shakeThreshold = 2.0; // Adjust based on sensitivity needed

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // Show ready state
  M5.Display.fillScreen(GREEN);
  delay(500);
  M5.Display.fillScreen(BLACK);
  
  // Display initial dice face
  displayNumber(1);
  
  randomSeed(analogRead(0)); // Initialize random number generator
}

void loop() {
  M5.update();
  
  float accX, accY, accZ;
  
  // Read accelerometer data
  M5.Imu.getAccelData(&accX, &accY, &accZ);
  
  // Calculate total acceleration magnitude
  float totalAcc = sqrt(accX * accX + accY * accY + accZ * accZ);
  
  // Check if shaking (acceleration exceeds threshold)
  if (!rolling && totalAcc > shakeThreshold) {
    // Start rolling animation
    rolling = true;
    rollStartTime = millis();
    // Start immediately with a random dice face instead of blue screen
    displayNumber(random(1, 7));
  }
  
  // Handle dice rolling animation
  if (rolling) {
    static unsigned long lastChangeTime = 0;
    unsigned long currentTime = millis();
    
    // During the rolling animation
    if (currentTime - rollStartTime < rollDuration) {
      // Display random numbers during animation - ensure we change every 100ms
      if (currentTime - lastChangeTime >= 100) { // Change number every 100ms (0.1 seconds)
        int randomFace = random(1, 7); // Random number between 1-6
        displayNumber(randomFace);
        lastChangeTime = currentTime;
      }
    } else {
      // Animation finished, show final result
      diceResult = random(1, 7); // Final random number between 1-6
      displayNumber(diceResult);
      
      // Add white corners to indicate final result
      int cornerSize = 5;
      M5.Display.fillRect(0, 0, cornerSize, cornerSize, WHITE);                                   // Top-left
      M5.Display.fillRect(M5.Display.width() - cornerSize, 0, cornerSize, cornerSize, WHITE);     // Top-right
      M5.Display.fillRect(0, M5.Display.height() - cornerSize, cornerSize, cornerSize, WHITE);    // Bottom-left
      M5.Display.fillRect(M5.Display.width() - cornerSize, M5.Display.height() - cornerSize, cornerSize, cornerSize, WHITE); // Bottom-right
      
      // Reset rolling state
      rolling = false;
      lastChangeTime = 0;
      
      // Wait a moment before allowing another shake
      delay(1000);
    }
  }
  
  // Small delay to prevent too frequent readings
  delay(50);
}

// Function to display dice number on the LED matrix
void displayNumber(int number) {
  M5.Display.fillScreen(BLACK);
  
  // For ATOMS3, we need to adjust the display method
  // The LED matrix is 5x5 but we'll use a 3x3 grid for dice patterns
  int centerX = M5.Display.width() / 2;
  int centerY = M5.Display.height() / 2;
  int dotSize = 8;
  int spacing = 20;
  
  switch (number) {
    case 1:
      // Center dot
      M5.Display.fillCircle(centerX, centerY, dotSize, WHITE);
      break;
      
    case 2:
      // Top-left and bottom-right
      M5.Display.fillCircle(centerX - spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY + spacing, dotSize, WHITE);
      break;
      
    case 3:
      // Top-left, center, and bottom-right
      M5.Display.fillCircle(centerX - spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX, centerY, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY + spacing, dotSize, WHITE);
      break;
      
    case 4:
      // Four corners
      M5.Display.fillCircle(centerX - spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX - spacing, centerY + spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY + spacing, dotSize, WHITE);
      break;
      
    case 5:
      // Four corners and center
      M5.Display.fillCircle(centerX - spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX - spacing, centerY + spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX, centerY, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY + spacing, dotSize, WHITE);
      break;
      
    case 6:
      // Six dots in 2 columns of 3
      M5.Display.fillCircle(centerX - spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX - spacing, centerY, dotSize, WHITE);
      M5.Display.fillCircle(centerX - spacing, centerY + spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY - spacing, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY, dotSize, WHITE);
      M5.Display.fillCircle(centerX + spacing, centerY + spacing, dotSize, WHITE);
      break;
  }
}

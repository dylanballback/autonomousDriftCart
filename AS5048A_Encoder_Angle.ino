
/*
Wiring between AS5048A and the Arduino Uno:

5V of the AS5048A to 5V on the Arduino Uno.
Or 
3.3V of the AS5048A to 3.3V on the Arduino Uno.
Whichever you prefer to use.
GND to GND.
MOSI (Master Out Slave In) of AS5048A to D11 on Arduino (MOSI).
MISO (Master In Slave Out) to D12 on Arduino (MISO).
SCK (Serial Clock) to D13 on Arduino (SCK).
CS (Chip Select) to  D10 on Arduino.

This code will read out the angle in degrees from the AS5048A.
This is being sent through serial to my computer running ROS1 Noetitc 
where I am able to read the encoder data. 
*/

#include <SPI.h>

// Chip Select Pin for AS5048A
const int CSPin = 10;

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  
  // Set up the chip select pin as an output
  pinMode(CSPin, OUTPUT);
  
  // Begin SPI
  SPI.begin();
  
  // Configure SPI for the AS5048A
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1)); // Adjust the speed if needed
  
  // Pull CS pin high to deselect the encoder
  digitalWrite(CSPin, HIGH);
}

void loop() {
  // Variable to hold the raw angle data
  unsigned int angleRaw = readAS5048A();
  
  // Convert the raw data to degrees (0 - 360)
  float angleDeg = (float)angleRaw * 360.0 / 16384.0;
  
  // Print the angle in degrees to the serial monitor
  //Serial.print("Angle: ");
  Serial.print(angleDeg);
  Serial.println();
  //Serial.println(" degrees");
  
  delay(10);
  // Small delay before reading again
  //delay(100);
}

unsigned int readAS5048A() {
  // Variable to store the received data
  unsigned int data = 0;
  
  // Pull CS pin low to select the encoder
  digitalWrite(CSPin, LOW);
  
  // Wait for the data to be ready
  delayMicroseconds(1);
  
  // Send read angle command (0xFFFF) and receive 16 bits of data
  data = SPI.transfer16(0xFFFF);
  
  // Pull CS pin high to deselect the encoder
  digitalWrite(CSPin, HIGH);
  
  // Data processing: clear the error flag (bit 14) and the parity bit (bit 15)
  data = data & 0x3FFF;
  
  return data;
}

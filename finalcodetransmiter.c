#include <SoftwareSerial.h>

SoftwareSerial loraSerial(7, 8); // RX, TX

const int trigPin = 6;
const int echoPin = 5;

void setup() {
  Serial.begin(9600);
  loraSerial.begin(115200); // Default baud rate for RYLR896

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("LoRa Sender with Ultrasonic Sensor");

  // Set device address
  loraSerial.print("AT+ADDRESS=1\r\n");
  delay(100);
  while (loraSerial.available()) {
    Serial.print((char)loraSerial.read());
  }

  // Set network ID
  loraSerial.print("AT+NETWORKID=5\r\n");
  delay(100);
  while (loraSerial.available()) {
    Serial.print((char)loraSerial.read());
  }

  // Flush any remaining data
  while (loraSerial.available()) {
    loraSerial.read();
  }
}

void loop() {
  // Send a trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  float distance = duration * 0.034 / 2;

  // Construct the message
  String message = "DIST," + String(distance, 2); // Add a prefix for better parsing
  Serial.println("Sending packet: " + message);

  // Send the distance via LoRa
  String command = "AT+SEND=2," + String(message.length()) + "," + message + "\r\n";
  Serial.println("Command: " + command); // Debug print to verify the command
  loraSerial.print(command);

  delay(2000); // Delay to prevent flooding the receiver
}

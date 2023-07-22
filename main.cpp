const int queryBytes = 8; // Number of bytes in the query
uint8_t receivedData[queryBytes]; // Array to store received data
int receivedCount = 0; // Counter for received bytes
uint8_t storedData[queryBytes]; // Array to store data for communication with the sensor
uint8_t* sensorData = nullptr; // Pointer to dynamically allocated array for sensor's returned data
int responseBytes = 0; // Number of bytes in the response
bool allBytesReceived = false; // Flag to indicate when all bytes are received
bool transmissionComplete = false; // Flag to indicate completion of transmission

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial1.begin(9600); // Initialize Serial1 for communication with the sensor
}

void loop() {
  if (Serial2.available()) {
    receivedData[receivedCount] = Serial2.read();
    receivedCount++;

    if (receivedCount == queryBytes) {
      allBytesReceived = true; // Set the flag to indicate all query bytes are received
      receivedCount = 0; // Reset the received count for the next set of data
    }
  }

  if (allBytesReceived && !transmissionComplete) {
    // All query bytes received
    Serial.println("Received query data:");
    for (int i = 0; i < queryBytes; i++) {
      Serial.print(receivedData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Store query data in storedData array
    for (int i = 0; i < queryBytes; i++) {
      storedData[i] = receivedData[i];
    }

    // Print storedData array
    Serial.println("Stored query data:");
    for (int i = 0; i < queryBytes; i++) {
      Serial.print(storedData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Write storedData to Serial1 for communication with the sensor
    Serial.println("Sending query data to the sensor:");
    for (int i = 0; i < queryBytes; i++) {
      Serial1.write(storedData[i]);
    }

    // Wait for the sensor's response
    delay(100);

    // Determine the number of response bytes based on Serial1.available()
    responseBytes = Serial1.available();
    Serial.print("Number of response bytes: ");
    Serial.println(responseBytes);

    // Dynamically allocate memory for sensorData array based on responseBytes
    sensorData = new uint8_t[responseBytes];

    // Receive and store the sensor's returned data
    Serial.println("Sensor's returned data:");
    for (int i = 0; i < responseBytes; i++) {
      if (Serial1.available()) {
        sensorData[i] = Serial1.read();
        Serial.print(sensorData[i], HEX);
        Serial.print(" ");
      }
    }
    Serial.println();

    transmissionComplete = true; // Set the flag to indicate transmission is complete
  }

  if (transmissionComplete) {
    // Write sensorData to Serial2
    Serial.println("Writing sensor data to Serial2:");
    for (int i = 0; i < responseBytes; i++) {
      Serial2.write(sensorData[i]);
    }
    Serial.println();

    // Clear storedData and receivedData arrays for the next transmission
    memset(storedData, 0, sizeof(storedData));
    memset(receivedData, 0, sizeof(receivedData));

    // Clear dynamically allocated sensorData array and reset responseBytes
    delete[] sensorData;
    sensorData = nullptr;
    responseBytes = 0;

    transmissionComplete = false; // Reset the flag for the next transmission
    allBytesReceived = false; // Reset the flag to indicate new query data is expected
  }
}

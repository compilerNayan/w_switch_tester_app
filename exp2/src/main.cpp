#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiServer.h>
#include <ESPmDNS.h>

// ====== Config ======
const char* STA_SSID     = "Garfield";
const char* STA_PASSWORD = "123Madhu$$";

WiFiServer server(8080);
String serialNumber;
bool switchedToWiFi = false;

// Generate random base36 string for serial number
String randomBase36(int len) {
  String s = "";
  const char* chars = "0123456789abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < len; i++) {
    s += chars[random(36)];
  }
  return s;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Generate serial number
  serialNumber = randomBase36(6);
  Serial.println("Device Serial Number: " + serialNumber);

  // Step 1: Start AP with SSID IoT_<serialNumber>
  String apSSID = "IoT_" + serialNumber;
  Serial.println("Starting AP: " + apSSID);
  WiFi.softAP(apSSID.c_str());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Step 2: Start TCP server in AP mode
  server.begin();
  Serial.println("TCP server started on port 8080 (AP mode)");

  // Step 3: Start mDNS responder with serialNumber.local
  if (MDNS.begin(serialNumber.c_str())) {
    Serial.println("mDNS responder started: " + serialNumber + ".local");
    MDNS.addService("http", "tcp", 8080);
  } else {
    Serial.println("Error setting up mDNS responder!");
  }
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    // Read request (optional)
    while (client.connected() && client.available()) {
      client.read(); // discard
    }
    // Send 200 OK response
    client.println("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK");
    client.stop();

    if (!switchedToWiFi) {
      // Step 4: Stop AP + server
      Serial.println("Stopping AP and TCP server...");
      server.stop();
      WiFi.softAPdisconnect(true);

      // Step 5: Connect to Wi-Fi
      Serial.println("Connecting to Wi-Fi...");
      WiFi.begin(STA_SSID, STA_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("\nConnected to Wi-Fi!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // Step 6: Restart TCP server in STA mode
      server.begin();
      Serial.println("TCP server started on port 8080 (Wi-Fi mode)");

      // Step 7: Restart mDNS responder in STA mode
      if (MDNS.begin(serialNumber.c_str())) {
        Serial.println("mDNS responder restarted: " + serialNumber + ".local");
        MDNS.addService("http", "tcp", 8080);
      } else {
        Serial.println("Error restarting mDNS responder!");
      }

      switchedToWiFi = true;
    }
  }
}

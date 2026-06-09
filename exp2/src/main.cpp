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

  // Start AP with SSID IoT_<serialNumber>
  String apSSID = "IoT_" + serialNumber;
  Serial.println("Starting AP: " + apSSID);
  WiFi.softAP(apSSID.c_str());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start TCP server in AP mode
  server.begin();
  Serial.println("TCP server started on port 8080 (AP mode)");

  // Start mDNS responder
  if (MDNS.begin(serialNumber.c_str())) {
    Serial.println("mDNS responder started: " + serialNumber + ".local");
    MDNS.addService("http", "tcp", 8080);
  } else {
    Serial.println("Error setting up mDNS responder!");
  }
}

void sendHttpResponse(WiFiClient &client, const String &body, const char *contentType) {
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: ";
  response += contentType;
  response += "\r\n";
  response += "Content-Length: " + String(body.length()) + "\r\n";
  response += "Connection: close\r\n\r\n";
  response += body;
  client.print(response);
}

void sendHotspotJsonResponse(WiFiClient &client) {
  String body =
      "{ \"ssid\": \"" + String(STA_SSID) + "\", \"password\": \"" + String(STA_PASSWORD) + "\" }";
  sendHttpResponse(client, body, "application/json");
}

void sendInProgressResponse(WiFiClient &client) {
  sendHttpResponse(client, "InProgress", "text/plain");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected() && client.available()) {
      client.read(); // discard request data
    }

    if (switchedToWiFi) {
      sendInProgressResponse(client);
    } else {
      sendHotspotJsonResponse(client);
    }
    client.stop();

    if (!switchedToWiFi) {
      // Stop AP + server
      Serial.println("Stopping AP and TCP server...");
      server.stop();
      WiFi.softAPdisconnect(true);

      // Connect to Wi-Fi
      Serial.println("Connecting to Wi-Fi...");
      WiFi.begin(STA_SSID, STA_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("\nConnected to Wi-Fi!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // Restart TCP server in STA mode
      server.begin();
      Serial.println("TCP server started on port 8080 (Wi-Fi mode)");

      // Restart mDNS responder in STA mode
      MDNS.end();
      delay(500);
      if (MDNS.begin(serialNumber.c_str())) {
        Serial.println("mDNS responder restarted: " + serialNumber + ".local");
        if (!MDNS.addService("http", "tcp", 8080)) {
          Serial.println("Failed to add http.tcp service!");
        }
      } else {
        Serial.println("Error restarting mDNS responder!");
      }

      switchedToWiFi = true;
    }
  }
}

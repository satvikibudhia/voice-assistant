#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

const char* ssid = "OPPOF17";
const char* password = "satviki@123";
const char* apiKey = "3GX548-6H6Q382UYW";

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
   Serial.println(WiFi.localIP());
  // Handle Web Server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/query", HTTP_GET, handleQuery);

  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><head><style>body {text-align: center; padding: 40px; font-family: Arial, sans-serif;}" \
                "form {display: inline-block; text-align: left;}" \
                "h1 {color: #333;}" \
                "input[type=text], input[type=submit] {padding: 10px; margin: 5px 0;}" \
                "input[type=submit] {background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer;}" \
                "</style></head>" \
                "<body><h1 style='color: #FF5733;'>TONI - Totally Overrated Noise Initiator</h1>" \
                "<form action=\"/query\">Query:<br><input type='text' name='query'><br>" \
                "<input type='submit' value='Submit'></form></body></html>";

  server.send(200, "text/html", html);
}

void handleQuery() {
  String message;
  if (server.hasArg("query")) {
    message = server.arg("query");
    message.replace(" ", "+");
    sendRequestToWolframAlpha(message);
  } else {
    message = "No message sent";
  }
  server.send(200, "text/plain", "Query received");
}

void sendRequestToWolframAlpha(String inputText) {
  HTTPClient http;

  // Your Wolfram Alpha API endpoint
  // Construct the request URL
  String url = "https://api.wolframalpha.com/v1/result?appid=" + String(apiKey) + "&i=" + inputText;
  
  // Set up HTTP request headers
  http.begin(url);

  // Make the HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Ans: " + response);
  } else {
    Serial.print("Error on Wolfram Alpha request. HTTP Code: ");
    Serial.println(httpResponseCode);
  }
  
  // Close the connection
  http.end();
}
#include <ESP8266WiFi.h>

// Add wifi access point credentiaals
const char* ssid     = "embedded";
const char* password = "123456";

WiFiServer server(80);// Set port to 80

String header; // This storees the HTTP request
String datetime;

String greenstate = "off";// state of green LED

void setup() {
  Serial.begin(115200);
  //connect to access point
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());// this will display the Ip address of the Pi which should be entered into your browser
  server.begin();
}

void transmit(String s){
  for(int i=0;i<s.length();i++){
    Serial.write(s[i]);
    delay(10);
    Serial.flush();
  }
}

String formatDate(String d){
  String result;
  if(d.length()>0){
    switch(d.charAt(1)){
      case '1':
      result="Sunday" + d.substring(2);
      break;

      case '2':
      result="Monday" + d.substring(2);
      break;

      case '3':
      result="Tuesday" + d.substring(2);
      break;

      case '4':
      result="Wednesday" + d.substring(2);
      break;

      case '5':
      result="Thursday" + d.substring(2);
      break;

      case '6':
      result="Friday" + d.substring(2);
      break;

      case '7':
      result="Saturday" + d.substring(2);
      break;
    }
  }
  return result;
}

void loop(){
  
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                   // print it out the serial monitor
        delay(10);
        Serial.flush();
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /LED/on") >= 0) {
              transmit("LED ON");
              greenstate = "on";
              transmit("Q");
            } else if (header.indexOf("GET /LED/off") >= 0) {
              transmit("LED OFF");
              greenstate = "off";
              transmit("Z");
            } else if (header.indexOf("GET /Date/on") >= 0) {
              transmit("Data/time on");
              transmit("Y");
            } 
            
       
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Simple IOT Application</h1>");
            client.println("<body><h3>ESP8266 Web Server</h3>");
            
            // Display current state, and ON/OFF buttons for LED
            client.println("<p>LED flashing - State " + greenstate + "</p>");
            
            // If the green LED is off, it displays the ON button       
            if (greenstate == "off") {
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println("<p>Data/time</p>");
            client.println("<p><a href=\"/Date/on\"><button class=\"button\">Get Date&Time</button></a></p>");

            client.println("</body></html>");

            if(Serial.available()>0)
              datetime=Serial.readString();
              String outp=formatDate(datetime);
            client.println("<p>"+outp+"</p>");  
 
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    transmit("Client disconnected.");
    transmit("");
  }
}

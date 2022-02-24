/**
 * @file BoundingBox.ino
 * @author Derek Jamison (derek@jamisoncreations.com)
 * @brief Draws a bounding box and a plus symbol on the screen.
 * @version 0.1
 * @date 2022-02-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <DNSServer.h>
#include <WiFi.h>
#include "escreen.h" // https://github.com/jamisonderek/qr-wifi.git
#include "qrcode.h" // https://github.com/ricmoo/QRCode

const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4);
DNSServer dnsServer;
WiFiServer server(80);

const String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Welcome to Free-Wifi!</h1><h2>Click <a href=\"/watch\">here</a> to accept the terms that we "
  "can sell all of your browsing history to pay for this high speed connection.</h2></body></html>";

const String response2HTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Enjoy!</h1><h2>Enjoy your free day of high-speed internet.<p/>&#128169;&#128169; This feature has not been implemented <a=ref=\"/ble\">yet</a>. &#128169;&#128169; </h2></body></html>";

const String response3aHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>BLE SCAN:</h1><h2><ul>";
const String response3bHTML = "</ul></h2></body></html>";

#define PIN_BUILTIN_LED 2

EScreen* screen;

String& escape(String& input) {
  // NOTE: Escape any \ characters in your data by prefixing with a \ character.
  // NOTE: Escape any semicolons in your data by prefixing with a \ character.
  input.replace("\\", "\\\\");
  input.replace(";", "\\;");
  return input;
}

void fillImage(String& ssid, String& password)
{
  screen->clear();

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(6)];
  String es = escape(ssid);
  String ep = escape(password);
  String msg = "WIFI:T:WPA;S:"+es+";P:"+ep+";;";
  qrcode_initText(&qrcode, qrcodeData, 6, 0, msg.c_str());

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
          screen->setPixel(x*3+i+3, y*3+j+5, qrcode_getModule(&qrcode, x, y));
        }
      }
    }
  }
}

String genPassword() {
  static char* chars = "0123456789bcdfghjklmnpqrstvwxzbcdfghjklmnpqrstvwxz!#$%*-=_+.";
  static int range = strlen(chars);
  String password = String();
  for (int i=0;i<16;i++) {
    password = password + chars[random(0,range)];
  }
  return password;
}

void setup()
{
  Serial.begin(115200);
  screen = new EScreen(14, 13, 15, 27, 26, 25);

  String ssid = String("Linksys-5G");
  String password = genPassword();

  pinMode(PIN_BUILTIN_LED, OUTPUT);
  Serial.println(ssid.c_str());
  Serial.println(password.c_str());
  fillImage(ssid, password);
  screen->drawDisplay();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid.c_str(), password.c_str());
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(DNS_PORT, "*", apIP);
  server.begin();
}

void loop()
{
  digitalWrite(PIN_BUILTIN_LED, HIGH);
  delay(100);  
  digitalWrite(PIN_BUILTIN_LED, LOW);
  delay(100);

  // ----------------------------------------------------------
  // Below code is copied from ESP example.  This should be removed in future.
  //   Created for arduino-esp32 on 04 July, 2018
  //   by Elochukwu Ifediora (fedy0)

  dnsServer.processNextRequest();
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    String currentLine = "";
    bool anon = true;
    bool ble = false;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            if (ble) {
              client.print(response3aHTML);
              client.print("<li>item1</li><li>item2</li>");
              client.print(response3bHTML);
            } else {
              client.print(anon?responseHTML:response2HTML);
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.indexOf("/watch")>0) {
          anon = false;
        } else if (currentLine.indexOf("/ble")>0) {
          ble = true;
        }
      }
    }
    client.stop();
  }
}

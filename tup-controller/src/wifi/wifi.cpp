#include <WiFi.h>

bool wifiSetup(const char* ssid, const char* password)
{
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    for(int i=0; i<10; i++){ 
        Serial.print("*");
        if (WiFi.status() != WL_CONNECTED) {
            delay(1000);
        }else{
            Serial.println("");
            Serial.print("WiFi connected - >");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            return true;            
        }
    }
    return false;
}

String wifiGet(const char* host, int port ,String url){
    WiFiClient client;     
    
    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        return "";
    }

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + *host + "\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return "";
        }
    }

    String line;
    while(client.available()) {
        line += client.readStringUntil('\r');
    }

    return line;
}

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLlF0G9yvB"
#define BLYNK_DEVICE_NAME "wifi control"

#include <Wire.h>
char auth[] = "cas8De3d65PBbkbRjScXrQgcHnjiiCxB"; // You should get Auth Token in the Blynk App.
#include <MAX30100_PulseOximeter.h>
#include <BlynkSimpleEsp8266.h>

#include <Blynk.h>
#include <ESP8266WiFi.h>

 

#define REPORTING_PERIOD_MS 3000

 
             
char ssid[] = "Hi";                                     // Your WiFi credentials.
char pass[] = "kingmedoa7a***";
 
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
 
void setup()
{
    Serial.begin(115200);
    
    pinMode(16, OUTPUT);
    Blynk.begin(auth, ssid, pass,"blynk-cloud.com",8080);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
        pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}
 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
 
        Blynk.virtualWrite(V0, BPM);
        Blynk.virtualWrite(V1, SpO2);
 
        tsLastReport = millis();
    }
}

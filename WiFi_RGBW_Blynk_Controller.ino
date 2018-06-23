
#include <Wire.h>
#include <Adafruit_RGBWDriver.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

#define PIN_LED_ENABLE 5

Adafruit_RGBWDriver led0 = Adafruit_RGBWDriver(0x61, 4, 5, 6, 7);
Adafruit_RGBWDriver led1 = Adafruit_RGBWDriver(0x61, 8, 9, 10, 11);
Adafruit_RGBWDriver led2 = Adafruit_RGBWDriver(0x61, 12, 13, 14, 15);

char auth[] = "asdfasdf";
char ssid[] = "Neuland Express";
char pass[] = "MySuperSecurePasswordThatImNotUploadingToGithub:)";

void setup() {
  Serial.begin(74880);
  Serial.println("16 channel PWM test!");
  Wire.begin(0, 2);
//  Wire.setClock(400000);
  scan();
  led0.begin();
  led0.setPWMFreq(1600);  // This is the maximum PWM frequency
  pinMode(PIN_LED_ENABLE, OUTPUT);
  digitalWrite(PIN_LED_ENABLE, LOW);  // LED output enable

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V0) //Button Widget is writing to pin V1
{
  uint8_t r = param[0].asInt();
  uint8_t g = param[1].asInt();
  uint8_t b = param[2].asInt();
  Serial.print(r); Serial.print("\t"); Serial.print(g); Serial.print("\t"); Serial.print(b); Serial.print("\n");
  led0.setColor(r, g, b, 0);
  led1.setColor(r, g, b, 0);
  led2.setColor(r, g, b, 0);
}

BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  uint8_t bright = param.asInt();
  Serial.println(bright);
}

BLYNK_WRITE(V2) //Button Widget is writing to pin V1
{
  uint8_t btn = param.asInt();
  Serial.println(btn);
}

void scan(){
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}


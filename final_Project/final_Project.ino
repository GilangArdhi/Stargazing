/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
//mendeklarasikan/mendefisinikan port virtual yang ada di blynk
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//identitas dari projek yang dibuat pada blynk
#define BLYNK_TEMPLATE_ID "TMPLD-x2K3nA"
#define BLYNK_DEVICE_NAME "Smarth Infus"
#define BLYNK_AUTH_TOKEN "1CDN6IV4LgL3F3YuD5kWe2df0ogkvUU0"

//library yang digunakan untuk menyambung ke wifi
#include <WiFi.h>
#include <WiFiClient.h>

//library yang digunakan agar esp32 dapat tersambung dengan blynk
#include <BlynkSimpleEsp32.h>

//library sensor berat hx711
#include "HX711.h"

//deklarasi untuk pin DOUT dan CLK pada sensor berat
const int DOUT = 18;
const int CLK = 4;

//deklarasi HX711/sensor berat
HX711 scale;
float calibration_factor = 1100.0;

//deklarasi gram yang akan digunakan sebagai
//variable yang menampung nilai berat
float GRAM;
//variable untuk menampung nilai maximal berat
float maxGRAM = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// memasukkan authentication project yang dibuat 
//diaplikasi blynk
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//memasukkan nama dan password wifi yang digunakan
char ssid[] = "MSI 3906";
char pass[] = "12345678";

//fitur pada blynk untuk mengirim data
BlynkTimer timer;


//fungsi untuk mengirim data ke blynk
void myTimerEvent(){
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 10;
  Blynk.virtualWrite(V0, GRAM);
  delay(100);
  if (maxGRAM < GRAM){
    maxGRAM = GRAM;
  } else if (GRAM < 1.0){
    maxGRAM = 0;
  }
  if (GRAM < (maxGRAM/6)){
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, LOW);
  } else if (GRAM > (maxGRAM/6) && GRAM < (maxGRAM/3)){
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V1, LOW);
  } else {
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V1, LOW);
  }
}

//fungsi yang digunakan untuk mengatur virtual pin
//atau komponen virtual yang ada pada blynk
//berdasarkan data yang diterima oleh blynk (GRAM)
BLYNK_WRITE(V0){
  if (param.asFloat() < (1000/6)){
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, LOW);
  } else if (param.asFloat() > (1000/6) && param.asFloat() < (1000/3)){
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V1, LOW);
  } else {
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V1, LOW);
  }
}

//fungsi yang digunakan untuk mengkofigurasi komponen
//dijalankan sekali
void setup()
{
  Serial.begin(115200);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);

  //sensor berat mulai menghitung berat
  scale.begin(DOUT, CLK);
  
  //fungsi yang digunakan untuk mengkalibrasi nilai
  //berat yang baca oleh sensor berat agar lebih akurat
  scale.set_scale();
  scale.tare();

  //memulai blynk
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  //memulai mengirim serta mengolah data dari fungsi yang dibuat tadi
  Blynk.run();
  timer.run();
  if (GRAM < (maxGRAM/6)){
    digitalWrite(23, HIGH);
    digitalWrite(22, LOW);
    digitalWrite(21, LOW);
  } else if (GRAM > (maxGRAM/6) && GRAM < (maxGRAM/3)){
    digitalWrite(22, HIGH);
    digitalWrite(21, LOW);
    digitalWrite(23, LOW);
  } else {
    digitalWrite(23, LOW);
    digitalWrite(22, LOW);
    digitalWrite(21, HIGH);
  }

  Serial.print ("Berat = ");
  Serial.println (GRAM);
  Serial.print ("Berat max = ");
  Serial.println (maxGRAM);
}

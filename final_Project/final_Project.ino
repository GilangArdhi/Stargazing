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

//dekalarasi button dan led
const int ledHijau = 21;
const int ledKuning = 22;
const int ledMerah = 23;
const int ledBiru = 2;
#define btn 19
int nBtn = 0;
unsigned long prevMillis = 0;

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

  
  //Mengambil berat maximal infus
  if (nBtn == 0 ){
    maxGRAM = GRAM;
    digitalWrite(ledBiru, HIGH);
    delay(250);
    digitalWrite(ledBiru, LOW);
    delay(250);
    digitalWrite(ledBiru, HIGH);
    delay(250);
    digitalWrite(ledBiru, LOW);
    delay(250);
  }

  //kondisi yang digunakan untuk mengatur virtual pin
  //atau komponen virtual yang ada pada blynk
  //berdasarkan data yang diterima oleh blynk (GRAM)
  if (GRAM < (maxGRAM/6)){
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V3, LOW);
  } else if (GRAM > (maxGRAM/6) && GRAM < (maxGRAM/3)){
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V3, LOW);
  } else if (GRAM > (maxGRAM/3)){
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V3, HIGH);
  }
}

//fungsi yang digunakan untuk mengkofigurasi komponen
//dijalankan sekali
void setup()
{
  Serial.begin(115200);
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(ledBiru, OUTPUT);
  pinMode(btn, INPUT_PULLUP);

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
  //Membaca/mengambil nilai btn
  nBtn = digitalRead(btn);
  //memulai mengirim serta mengolah data dari fungsi yang dibuat tadi
  Blynk.run();
  timer.run();
  
  if (GRAM < (maxGRAM/6) || GRAM < 1.0){
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, LOW);
  } else if (GRAM > (maxGRAM/6) && GRAM < (maxGRAM/3)){
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledMerah, LOW);
  } else if (GRAM > (maxGRAM/3)){
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, HIGH);
  }


  Serial.print ("Nilai Button = ");
  Serial.println(nBtn);
  Serial.print ("Berat = ");
  Serial.println(GRAM);
  Serial.print ("Berat max = ");
  Serial.println (maxGRAM);
}

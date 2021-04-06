
  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27, 16, 2);

//SENSOR BERAT
  #include "HX711.h"
  #define DOUT  A2 
  #define CLK  A3

    #define DOUT2  11 
  #define CLK2  10

  
  HX711 scale(DOUT, CLK);
    HX711 scale2(DOUT2, CLK2);
  
  float calibration_factor = 1500;
    float calibration_factor2 = 6000;
  int GRAM = 0;
  int GRAM2 = 0;


//MOTOR
  int R_IS = 6;
  int R_EN = 2;
  int R_PWM = 3;
  int L_IS = 7;
  int L_EN = 4;
  int L_PWM = 5;
  int ma = 8;
  int mu= 12; 

  int lamp = 10;
  int lamp2 = 11;


  int potensio2 = 0;
  int potensio3 = 0;
  int sensornya = 0;


void setup() {
  pinMode(DOUT2, INPUT);
  pinMode(CLK2, INPUT);
  
//SENSOR BERAT SETUP
  Serial.begin(9600);
  Serial.println("tekan a,s,d,f untuk menaikan calibration_factor ke 10,100,1000,10000");
  Serial.println("tekan z,x,c,v untuk menurunkan calibration_factor ke 10,100,1000,10000");
  Serial.println("Tekan T untuk Tare");
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average();
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);


//LCD SETUP
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  
//MOTOR SETUP
  Serial.begin(9600);
  pinMode(R_IS, OUTPUT);
  pinMode(R_EN, OUTPUT); 
  pinMode(R_PWM, OUTPUT);
  pinMode(L_IS, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(L_PWM, OUTPUT); 
  pinMode(ma, INPUT);
  pinMode(mu, INPUT);

  analogWrite(ma, 0);
  analogWrite(mu, 0);
  digitalWrite(R_IS, LOW);
  digitalWrite(L_IS, LOW);
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);
  
//LAMPU
  pinMode(9,OUTPUT);



}

void loop() {
  //SENSOR KALIBRASI
  int W1 = GRAM+1;
  int W2 = GRAM2;
  int b = W1;
  int b2 = W2;
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;
  
  Serial.print("Reading: ");
  Serial.print(W1);
  Serial.print(" Gram");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  scale2.set_scale(calibration_factor2);
  GRAM2 = scale2.get_units(), 4;
  
  Serial.print("Reading2: ");
  Serial.print(W2);
  Serial.print(" Gram");
  Serial.print(" calibration_factor2: ");
  Serial.print(calibration_factor2);
  Serial.println();

  int w = ((b+b2)*10);
  
  lcd.setCursor(8, 0);
  lcd.print("W=");
  lcd.setCursor(11, 0);
  lcd.print(w);


  //SPEED MOTOR  
  int  v = analogRead(A0);
  int  potensio = map(v, 0, 1023, 0, 256);
  int kec = map (potensio, 44, 255, 10, 50);
  
  potensio3 = potensio + 5;
  //Serial.println(potensio);
  //Serial.println(potensio2); 
  lcd.setCursor(0, 0);
  lcd.print("v= ");
  lcd.setCursor(3, 0);
  lcd.print(kec);
 
  

//SENSOR DISTANCE
  int  a = analogRead(A1);
  int sensor = 10650.08*pow(a, -0.935)-10;
  
  lcd.setCursor(0, 1);
  lcd.print("s= ");
  lcd.setCursor(3, 1);
  Serial.println(sensor);



//MOTOR  
  int maju = digitalRead(ma);
  int mundur = digitalRead(mu);
  


int t = (450/kec)*250;
int t2 = (300/kec)*250;
int t3 = (800/kec)*250;


if (sensor < 30){
  lcd.print("Scan Process");
}
else if(w>20 && w<500 && kec > 10 && maju == HIGH){
  lcd.print("Process");
}
 else{
  lcd.print("Done");
 }
  

if(sensor<30){
    analogWrite(L_PWM, potensio);
    analogWrite(R_PWM, 0);
    delay(t);
    digitalWrite(9, HIGH);
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
    delay(1000);
    digitalWrite(9, LOW);
    analogWrite(L_PWM, potensio);
    analogWrite(R_PWM, 0);
    delay(t3);
    //Serial.println("sensor = ");
    //Serial.println(sensor);
     
  }

delay(500); 

if(w>20 && w<1000 && kec > 10 && maju == HIGH){
  delay(2000);
     analogWrite(L_PWM, potensio);
  analogWrite(R_PWM, 0);
  delay(t2);
   analogWrite(lamp, potensio);
   analogWrite(lamp2, 0);
  
  }
else if(w>20 && w<1000 && kec >10 && mundur == HIGH){

   analogWrite(R_PWM, potensio);
   analogWrite(L_PWM, 0);
    delay(t2);
  analogWrite(lamp, 0);
  analogWrite(lamp2, potensio);
 // Serial.println("mundur");


 
}
//Sensor Distance


else{
  lcd.print("Berhenti");
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, 0);
  analogWrite(lamp, LOW);
 // Serial.println("berhenti");
}



 if(potensio2 >= potensio)
{
  lcd.clear(); 
}

 if(potensio3 >= potensio)
{
  lcd.clear(); 
}
  
}
 

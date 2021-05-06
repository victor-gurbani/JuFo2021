/*

  IDEAS:

  + GPS ◊
  + .apk ◊

*/
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial GPRS(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8

float cm = 30.00;
int Trig = 12;
int Echo = 11;
const int buzzer = 9;
int buttonPin = 4;
int motorPin = 6;
int on = 1;
float duration;
float distancia;
float laston = 0;
float interactive = 0;
char debug;
String debugdis;
bool sound = true;

int SOS = 0;
void setup()
{
  Serial.begin(9600);
  GPRS.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop()
{
  if (digitalRead(buttonPin) == HIGH)//se ha pulsado el boton?
  {
    on = on + 1;
    on = on % 2;
    SOS = 0;

    if (on == 0)
    {
      Serial.println("turning off...");
       digitalWrite(motorPin, LOW);
      if(sound)
      {
        tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
        delay(500); // Wait for 500 millisecond(s)
        noTone(buzzer);
      }

    }else{
      Serial.println("turning on...");
        digitalWrite(motorPin, LOW);
      if(sound)
      {
        tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
        delay(500); // Wait for 500 millisecond(s)
        noTone(buzzer);
      }

      laston = millis();
      interactive = millis();
    }


    while (digitalRead(buttonPin) == HIGH)
    {
      delay(100);
      if(sound)
      {
      tone(buzzer, 494, 50); 
      }
      SOS += 1;
        if(SOS > 29){
      
          Serial.println("Sending SOS message");
          tone(buzzer, 784, 500);
          delay(500);
          GPRS.println("AT"); //Handshaking with SIM900
          delay(500);
          GPRS.println("AT+CMGF=1"); // Configuring TEXT mode
          delay(500);
          GPRS.println("AT+CMGS=\"+34658166246\"");//change with phone number to sms
          delay(500);
          delay(500);
          GPRS.print("SOS message: Help!"); //text content
          delay(500);
          GPRS.write( 0x1a );   
          Serial.println("SOS message has been send");
      
          SOS = 0;
            
          while(digitalRead(buttonPin) == HIGH){};
        }
      } 
    }
  



  if (on == 0)
  {
    //Serial.println(" OFF");
    while(on == 0 and digitalRead(buttonPin)==LOW){
      if(Serial.available())
      {
        debug = Serial.read();
         // Serial.print(debug);
          if (debug == '+')
          {
            on=1;
       Serial.println("turning on...");
        digitalWrite(motorPin, LOW);
      if(sound)
      {
        tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
        delay(500); // Wait for 500 millisecond(s)
        noTone(buzzer);
      }

      laston = millis();
      interactive = millis();
    }
      }
    }
  }else{
    
    //Serial.print(" ON ––– Last on: ");
    //Serial.print((millis() - laston) / 1000);
    //Serial.print(" ––– Last interactive: ");
    //Serial.print((millis() - interactive) / 1000);
    if ((millis() - interactive) / 1000 > 600) // 600= 10 minutos
    {
      on = 0;
      Serial.println("turning off...");
      if(sound)
      {
        tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
        delay(500); // Wait for 500 millisecond(s)
        noTone(buzzer);
      }
      digitalWrite(motorPin, LOW);
    }

    digitalWrite(Trig, LOW);
    delay(4);
    digitalWrite(Trig, HIGH);
    delay(10);
    digitalWrite(Trig, LOW);

    duration = pulseIn(Echo, HIGH);
    duration = duration / 2;
    distancia = duration / 29;
    //Serial.print(" ––– Distance: ");
    //Serial.print(distancia);
    //Serial.println(" cm;");
    if (!(distancia == 0.00))
    {
      if (distancia < cm * 1 / 3) // menor q un tercio
      {
        
        if(sound)
        {
          tone(buzzer, 523, 100);
        }
        digitalWrite(motorPin, HIGH);
        delay(5);
        interactive = millis();
        
      }
      else if (distancia < cm * 2 / 3) // mayor q un tercio(else if) menor q 2 tercios
      {
        if(sound)
        {
          tone(buzzer, 523, 100);
        }
        digitalWrite(motorPin, HIGH);
        delay(100);
        if(sound)
        {
          tone(buzzer, 493, 100);
        }

        delay(70);
        digitalWrite(motorPin, LOW);
        delay(30);
        interactive = millis();

      }
      else if (distancia < cm /*&& distancia > cm * 2 / 3*/)// menor q cm (mayor q dos tercios)
      {
        if(sound)
        {
          tone(buzzer, 523, 100);
        }
        digitalWrite(motorPin, HIGH);

        delay(150);

        if(sound)
        {
          tone(buzzer, 493, 100);
        }

        digitalWrite(motorPin, LOW);
        delay(150);
        interactive = millis();

      }else{
        if(!(sound))
        {
          noTone(buzzer);
        }
        digitalWrite(motorPin, LOW);
      }
    }
  }
    if(Serial.available())
      {
        debug = Serial.read();
          //Serial.print(debug);
          if (debug == 's')
          {
            //Serial.print(" Enter distance:\n");
            while(Serial.available() == 0);
            debugdis = Serial.readString();
            int t = debugdis.toInt();
            Serial.print("New virtual size is: ");
            Serial.println(t);
            cm = t;
          }
          if (debug == 'c')
          {
            Serial.print(" Current distance is ");
            Serial.println(distancia);
            Serial.println("cm");

          }
          if (debug == 'v')
          {
            Serial.print(" Virtual size is ");
            Serial.print(cm);
            Serial.println(" cm");

          }
          
          if (debug == '-')
          {
            on = 0;
                  Serial.println("turning off...");
       digitalWrite(motorPin, LOW);
      if(sound)
      {
        tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
        delay(250); // Wait for 500 millisecond(s)
        tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
        delay(500); // Wait for 500 millisecond(s)
        noTone(buzzer);
      }


          }
          if (debug == '+')
          {
            on = 1;
            Serial.println("turning on...");
            digitalWrite(motorPin, LOW);
            if(sound)
            {
              tone(buzzer, 523, 500); // play tone 60 (C5 = 523 Hz)
              delay(250); // Wait for 500 millisecond(s)
              tone(buzzer, 659, 500); // play tone 64 (E5 = 659 Hz)
              delay(250); // Wait for 500 millisecond(s)
              tone(buzzer, 784, 500); // play tone 67 (G5 = 784 Hz)
              delay(250); // Wait for 500 millisecond(s)
              tone(buzzer, 1047, 500); // play tone 72 (C6 = 1047 Hz)
              delay(500); // Wait for 500 millisecond(s)
              noTone(buzzer);
            }
            //Serial.print(" Now ON\n ");

          }
          if (debug == 'b')
          {
            Serial.print(" Time since first boot is ");
            Serial.print(millis()/3600000);
            Serial.print(" h ");
            Serial.print((millis()%3600000)/60000);
            Serial.print(" min ");
            Serial.print(((millis()%3600000)%60000)/1000);
            Serial.println(" sec ");


          }
          if (debug == 'm')
          {
            if (sound){
              sound = false;
              Serial.print(" Sound is OFF\n ");

            }else{
              sound = true;
            Serial.print(" Sound is ON\n ");
            }
          }
          if (debug == 'h')
          {
            Serial.print(" \n\tHelp:\n\t'h' - help (view command list)\n\t'c' - view current cm\n\t'v' - view virtual sizem\n\t's' - set cm (may take a while)\n\t'-' - turn off\n\t'+' - turn on\n\t'm' - mute/unmute\n\t'b' - time since first boot\n\t");
          }
        }
}

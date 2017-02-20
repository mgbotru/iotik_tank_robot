#define BLYNK_PRINT Serial

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "559cefe2a0ce4c3f9db2322aef44c5db";
char ssid[] = "IOTIK";
char pass[] = "Terminator812";

#define PWMA 5
#define DIRA 4
#define PWMB 12
#define DIRB 13

#define SRV1 0
#define SRV2 2
Servo servo_1;
Servo servo_2;

float srv1 = 90;
float srv2 = 90;

void setup()
{
  // Инициализация последовательного порта
  Serial.begin(115200);
  // Инициализация выходов для управления моторами
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  // Инициализация выходов для управления сервомоторами
  pinMode(SRV1, OUTPUT);
  pinMode(SRV2, OUTPUT);
  servo_1.attach(SRV1);
  servo_2.attach(SRV2);
  servo_1.write(srv1);
  servo_2.write(srv2);
  // Инициализация Blynk
  Blynk.begin(auth, ssid, pass);

  //motorA_setpower(50, true);
  //delay(5000);
  //motorA_setpower(-50, true);
  //delay(5000);
  //motorA_setpower(0, true);
  //delay(5000);
  //motorB_setpower(75, false);
  //delay(5000);
  //motorB_setpower(-75, false);
  //delay(5000);
  //motorB_setpower(0, false);
  //delay(5000);


}

void loop()
{
  Blynk.run();
}

// Мощность мотора "A" от -100% до +100% (от знака зависит направление вращения)
void motorA_setpower(float pwr, bool invert)
{
  // Проверка, инвертирован ли мотор
  if (invert)
  {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100)
  {
    pwr = -100;
  }
  if (pwr > 100)
  {
    pwr = 100;
  }
  // Установка направления
  if (pwr < 0)
  {
    digitalWrite(DIRA, LOW);
  }
  else
  {
    digitalWrite(DIRA, HIGH);
  }
  // Установка мощности
  int pwmvalue = fabs(pwr) * 10.23;
  analogWrite(PWMA, pwmvalue);
}

// Мощность мотора "B" от -100% до +100% (от знака зависит направление вращения)
void motorB_setpower(float pwr, bool invert)
{
  // Проверка, инвертирован ли мотор
  if (invert)
  {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100)
  {
    pwr = -100;
  }
  if (pwr > 100)
  {
    pwr = 100;
  }
  // Установка направления
  if (pwr < 0)
  {
    digitalWrite(DIRB, LOW);
  }
  else
  {
    digitalWrite(DIRB, HIGH);
  }
  // Установка мощности
  int pwmvalue = fabs(pwr) * 10.23;
  analogWrite(PWMB, pwmvalue);
}

BLYNK_WRITE(V0)
{
  float x = param[0].asInt() - 512;
  float y = param[1].asInt() - 512;
  float u = (y - x) / 3;
  float v = (y + x) / 3;
  motorA_setpower(v / 2, true);
  motorB_setpower(u / 2, false);
  delay(10);
  motorA_setpower(v, true);
  motorB_setpower(u, false);
  Serial.println("Power motors:");
  Serial.print("L = ");
  Serial.print(u);
  Serial.print("; R = ");
  Serial.println(v);
}

BLYNK_WRITE(V1)
{
  float z = param.asInt();
  float u = z / 1023 * 180;
  servo_1.write(u);
  Serial.println("Servo motor #1:");
  Serial.print("Angle = ");
  Serial.println(u);
}

BLYNK_WRITE(V2)
{
  float z = param.asInt();
  float u = z / 1023 * 180;
  servo_2.write(u);
  Serial.println("Servo motor #2:");
  Serial.print("Angle = ");
  Serial.println(u);
}


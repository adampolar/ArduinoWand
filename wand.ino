#include <Wire.h>

#define MAGNETOMETER
#define GYROSCOPE
#define ACCELEROMETER

void setup() {
  
  //setup megnetometer
  pinMode(13, OUTPUT);
  Serial.print("here");
  Serial.begin(115200);
  Wire.begin();
  digitalWrite(13,LOW);
  Wire.beginTransmission(30);
  Wire.write(0x00);
  Wire.write(0x70);
  Wire.endTransmission();
  digitalWrite(13,HIGH);
  Wire.beginTransmission(30);
  Wire.write(0x01);
  Wire.write(0xA0);
  Wire.endTransmission();
  Wire.beginTransmission(30);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);;
  Serial.print("here");
  
  //setup gyroscope
  Wire.beginTransmission(104);
  Wire.write(117);
  Wire.endTransmission();
  Wire.requestFrom(104,1);
  while (Wire.available() < 1);
  byte i = Wire.read();
  Serial.println(i);
  Wire.beginTransmission(104);
  Wire.write(107);
  Wire.write(0|0x01/*clock select*/);
  Wire.endTransmission();
  
  //turn off temp
  Wire.beginTransmission(104);
  Wire.write(107);
  Wire.endTransmission();
  Wire.requestFrom(104, 1);
  while (Wire.available() < 1);
  byte pwrmng = Wire.read();
  Wire.beginTransmission(104);
  Wire.write(107);
  Wire.write(pwrmng|8);
  Wire.endTransmission();
  
  //config accel
  Wire.beginTransmission(104);
  Wire.write(28);
  Wire.endTransmission();
  Wire.requestFrom(104, 1);
  while (Wire.available() < 1);
  byte accel = Wire.read();
  Wire.beginTransmission(104);
  Wire.write(28);
  Wire.write(accel&0b11100111);
  Wire.endTransmission();
  
}

void loop() {
  #ifdef MAGNETOMETER
  Wire.requestFrom(30, 6);
  while (Wire.available() < 6);
  byte a = Wire.read();
  byte b = Wire.read();
  byte c = Wire.read();
  byte d = Wire.read();
  byte e = Wire.read();
  byte f = Wire.read();
  int x = (a<<8)|b;
  int y = (c<<8)|d;
  int z = (e<<8)|f;
  float yaw = atan2(z, x);
  float pitch = atan2(y, z);
  float roll = atan2(x, y);
  
  //Serial.print("YAW "); Serial.println(yaw * 180/PI);
  //Serial.print("PITCH "); Serial.println(pitch * 180/PI);
  //Serial.print("ROLL "); Serial.println(roll * 180/PI);
  
  Wire.beginTransmission(30);
  Wire.write(0x03);
  Wire.endTransmission();
  #endif
  
  
  //gyroscope
  Wire.beginTransmission(104);
  Wire.write(59);
  Wire.endTransmission();
  Wire.requestFrom(104,14);
  while (Wire.available() < 14);
  
  int xa = Wire.read()<<8|Wire.read();
  int ya = Wire.read()<<8|Wire.read();
  int za = Wire.read()<<8|Wire.read();
  
  int temp = Wire.read()<<8|Wire.read();
  
  int xg = Wire.read()<<8|Wire.read();
  int yg = Wire.read()<<8|Wire.read();
  int zg = Wire.read()<<8|Wire.read();
  #ifdef ACCELEROMETER
  Serial.print("xa"); Serial.print(xa);
  Serial.print("\tya"); Serial.print(ya);
  Serial.print("\tza"); Serial.print(za);
  #endif
  /*ACCEL_XANGLE = 57.295*atan((float)ACCEL_YOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_XOUT,2)));
  ACCEL_YANGLE = 57.295*atan((float)-ACCEL_XOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_YOUT,2)));*/
  #ifdef TEMPERATURE
  Serial.print("temp"); Serial.println((temp+0.01)/340 + 36.53);
  #endif
  #ifdef GYROSCOPE
  Serial.print("\txg"); Serial.print(xg);
  Serial.print("\tyg"); Serial.print(yg);
  Serial.print("\tzg"); Serial.print(zg);
  #endif
  
  #ifdef MAGNETOMETER
  Serial.print("\txm"); Serial.print(x);
  Serial.print("\tym"); Serial.print(y);
  Serial.print("\tzm"); Serial.print(z);
  Serial.print("\tya"); Serial.print(yaw);
  Serial.print("\tpi"); Serial.print(pitch);
  Serial.print("\tro"); Serial.print(roll);
  #endif
  
  
  Serial.println();
  
  
  
  
  
}

#include <SoftwareSerial.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
SoftwareSerial MySerial(7, 8); // pin 7 connects to TX of HC-05 | pin 8 connects to RX of HC-05

long timer = 0;
long sleep_timer_start, sleep_timer_end,sleep_timer_end2;
float x,y,z;
int activate, interrupt,stage_sleep_time, interrupt_sleep_timer,interrupt_for_deep_sleep, total_sleep, total_deep_sleep, total_light_sleep, deep_sleep, light_sleep, interrupt_timer=0;
void setup() {
MySerial.begin(9600);
Wire.begin();
mpu6050.begin();
//mpu6050.calcGyroOffsets(true);

}

void loop() {
mpu6050.update();

if(millis() - timer > 1000){ 
x = mpu6050.getGyroX();
y = mpu6050.getGyroY();
z = mpu6050.getGyroZ();

if (activate == 0){ // first sleep confirmation
if ((x<=20 || x>=-20) && (y<=20 || y>=-20) && (z<=20 || z>=-20)) {
sleep_timer_start = millis()/1000-sleep_timer_end;
if (sleep_timer_start == 30){
activate = 1;}
}
if ((x>=20 || x<=-20) || (y>=20 || y<=-20) || (z>=20 || z<=-20)){
sleep_timer_end =(millis()/1000); }
}

if (activate == 1){ // sleeping mode
light_sleep = (millis()/1000)-sleep_timer_end;

if (interrupt == 0){
if (light_sleep >= 40){
if (interrupt_for_deep_sleep > 40){
if (light_sleep - interrupt_sleep_timer >= 10){
deep_sleep = light_sleep - interrupt_for_deep_sleep;
}
}
}
}
light_sleep = light_sleep - deep_sleep;

if ((x>=20 || x<=-20) || (y>=20 || y<=-20) || (z>=20 || z<=-20)){
interrupt_sleep_timer = (millis()/1000)-sleep_timer_end; 
interrupt_for_deep_sleep = light_sleep;
interrupt =interrupt+1;
delay(2000);}



if ((millis()/1000)- sleep_timer_end -interrupt_sleep_timer > 30) {
interrupt =0; 
}

if ((millis()/1000)- sleep_timer_end - interrupt_sleep_timer <= 30){
if (interrupt >=5){
sleep_timer_end =(millis()/1000);
if (light_sleep >= 60){ // second sleep confirmation
total_light_sleep = total_light_sleep + light_sleep;
total_deep_sleep = total_deep_sleep + deep_sleep;
total_sleep = total_light_sleep + total_deep_sleep; }
activate =0;
interrupt =0;
deep_sleep= 0;
light_sleep= 0;
interrupt_sleep_timer=0;
interrupt_for_deep_sleep=0;
}
}

}
stage_sleep_time = light_sleep + deep_sleep; 
if (stage_sleep_time >= 120){
sleep_timer_end =(millis()/1000);
total_light_sleep = total_light_sleep + light_sleep;
total_deep_sleep = total_deep_sleep + deep_sleep;
total_sleep = total_light_sleep + total_deep_sleep; 
activate =0;
interrupt =0;
deep_sleep= 0;
light_sleep= 0;
interrupt_sleep_timer=0;
interrupt_for_deep_sleep=0; 
}

MySerial.print(sleep_timer_start); // just to know code and sensor working fine
MySerial.print(",");
if (light_sleep >= 60){ 
MySerial.print(light_sleep);
MySerial.print(",");
MySerial.print(deep_sleep);
MySerial.print(","); 
MySerial.print(total_light_sleep);
MySerial.print(",");
MySerial.print(total_deep_sleep);
MySerial.print(",");
MySerial.print(total_sleep);
MySerial.print(";"); }
else {
MySerial.print(0);
MySerial.print(",");
MySerial.print(0);
MySerial.print(","); 
MySerial.print(total_light_sleep);
MySerial.print(",");
MySerial.print(total_deep_sleep);
MySerial.print(",");
MySerial.print(total_sleep);
MySerial.print(";");
} 
timer = millis();
}

}

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
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
//mpu6050.calcGyroOffsets(true);

}

void loop() {
mpu6050.update();

if(millis() - timer > 5000){
  Serial.print("============================\n");
x = mpu6050.getGyroX();
y = mpu6050.getGyroY();
z = mpu6050.getGyroZ();

//check condition, If our hand is not moving or in rest.
if (activate == 0){ // first sleep confirmation
  if ((x<=20 || x>=-20) && (y<=20 || y>=-20) && (z<=20 || z>=-20)) {
     sleep_timer_start = millis()/1000-sleep_timer_end;
      if (sleep_timer_start == 30){
        activate = 1;}
}
//Check if within 30 seconds our hand move then it will gain start the timer.
  if ((x>=20 || x<=-20) || (y>=20 || y<=-20) || (z>=20 || z<=-20)){
    sleep_timer_end =(millis()/1000); }
}

if (activate == 1){ // sleeping mode
light_sleep = (millis()/1000)-sleep_timer_end;
//When sleep cycle starts after 30 seconds, we go to deep sleep.
  if (interrupt == 0){
    if (light_sleep >= 60){
      if (light_sleep - interrupt_sleep_timer >= 10){
        deep_sleep = light_sleep - interrupt_for_deep_sleep;
      }
    }
  }
light_sleep = light_sleep - deep_sleep;
Serial.print("light_sleep: ");
Serial.println(light_sleep);
Serial.print("deep_sleep: ");
Serial.println(deep_sleep);

//If you are in sleeping mode then your hand movement doesn’t make light sleep equal to zero. 
//Whenever your hand move it will count interrupt.
           if ((x>=20 || x<=-20) || (y>=20 || y<=-20) || (z>=20 || z<=-20)){
            interrupt_sleep_timer = (millis()/1000)-sleep_timer_end; 
            interrupt_for_deep_sleep = light_sleep;
            Serial.print("interrupt_sleep_timer: ");
            Serial.println(interrupt_sleep_timer);
            interrupt = interrupt + 1;
            Serial.print("interrupt counter: ");
            Serial.println(interrupt);
            delay(2500);}


//if your last interruption was 5 minutes ago then it will make interrupt is equal to zero.
if ((millis()/1000)- sleep_timer_end -interrupt_sleep_timer > 300) {
interrupt =0; 
}

if ((millis()/1000)- sleep_timer_end - interrupt_sleep_timer <= 10){
if (interrupt >=5){
sleep_timer_end =(millis()/1000);
if (light_sleep >= 120){ // second sleep confirmation
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

Serial.print("total_light_sleep: ");
Serial.println(total_light_sleep);
Serial.print("total_deep_sleep: ");
Serial.println(total_deep_sleep);
Serial.print("total_sleep: ");
Serial.println(total_sleep);
Serial.println("============================\n");

} 
timer = millis();
}

}

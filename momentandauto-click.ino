/* Code to control the mouse pointer with the movement of the head
 * and assisted click.
 * Change the values at vx and vy (+300 and -100 in my case) 
 * setting them with the TEST code.
 * 
 * Gabry295
*/

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy, vx_prec, vy_prec;
int count=0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
    }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx+300)/200;  // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
  vy = -(gz-100)/200; // same here about "-100"
  
  Mouse.move(vx, vy);
  
  
  if ( (vx_prec-5)<=vx && vx<=vx_prec+5 && (vy_prec-5)<=vy && vy<=vy_prec+5) { // checking the pointer doesn't move too much from its actual position: (in this case a 10 pixel square)
    count++;                                                                  
    if(count == 100){ // the click will happen after 2 seconds the pointer has stopped in the 10px square: 20ms of delay 100 times it's 2000ms = 2s
      if (!Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.press(MOUSE_LEFT);
        count = 0;
      }
    }
    else {
      if (Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.release(MOUSE_LEFT);
      }
    }
  }
  else {
    vx_prec = vx; // updating values to check the position of the pointer and allow the click
    vy_prec = vy;
    count = 0;
    }
  
  delay(20);
}
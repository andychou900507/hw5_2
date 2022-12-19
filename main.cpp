#include "mbed.h"
#include "bbcar.h"
  #include "PwmIn.h"
  #include <iostream>
  #include "uLCD_4DGL.h"

  #define CENTER_BASE 1500
  #define unitsFC 360                          // Units in a full circle
  #define dutyScale 1000                       // Scale duty cycle to 1/000ths
  #define dcMin 29                             // Minimum duty cycle
  #define dcMax 971                            // Maximum duty cycle
  #define q2min unitsFC/4                      // For checking if in 1st uadrant
  #define q3max q2min * 3                      // For checking if in 4th uadrant

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmOut pin11(D11), pin13(D13);
PwmIn pin10(D10), pin12(D12);
BBCar car(pin11, pin10, pin13, pin12, servo_ticker, servo_feedback_ticker);
BusInOut qti_pin(D4,D5,D6,D7);
uLCD_4DGL uLCD(D1, D0, D2);

  volatile int angle1, targetAngle1 = 3;              // Global shared ariables
  volatile int Kp1 = 1;                          // Proportional constant
  volatile float tCycle1;
  volatile int theta1;
  volatile int thetaP1;
  volatile int turns1 = 0;
  volatile int angle2, targetAngle2 = 3;              // Global shared ariables
  volatile int Kp2 = 1;                          // Proportional constant
  volatile float tCycle2;
  volatile int theta2;
  volatile int thetaP2;
  volatile int turns2 = 0;

  void feedback360(){                           // Position monitoring
     tCycle1 = pin10.period();
     int dc1 = dutyScale * pin10.dutycycle();
     theta1 = (unitsFC - 1) -                   // Calculate angle
              ((dc1 - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta1 < 0)                             // Keep theta valid
        theta1 = 0;
     else if(theta1 > (unitsFC - 1))
        theta1 = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta1 < q2min) && (thetaP1 > q3max))
        turns1++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP1 < q2min) && (theta1 > q3max))
        turns1 --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns1 >= 0)
        angle1 = (turns1 * unitsFC) + theta1;
     else if(turns1 <  0)
        angle1 = ((turns1 + 1) * unitsFC) - (unitsFC - theta1);

     thetaP1 = theta1;                           // Theta previous for next rep


     tCycle2 = pin12.period();
     int dc2 = dutyScale * pin12.dutycycle();
     theta2 = (unitsFC - 1) -                   // Calculate angle
              ((dc2 - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta2 < 0)                             // Keep theta valid
        theta2 = 0;
     else if(theta2 > (unitsFC - 1))
        theta2 = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta2 < q2min) && (thetaP2 > q3max))
        turns2++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP2 < q2min) && (theta2 > q3max))
        turns2 --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns2 >= 0)
        angle2 = (turns2 * unitsFC) + theta2;
     else if(turns2 <  0)
        angle2 = ((turns2 + 1) * unitsFC) - (unitsFC - theta2);

     thetaP2 = theta2;                           // Theta previous for next rep
  }


int main() {

   parallax_qti qti1(qti_pin);
   int pattern;
   int count = 0;
   int count2 = 0;
   float distance1 = 0;
   float distance2 = 0;
   float distance_store[5] = {0, 0, 0, 0, 0};
   float final_distance = 0;
   servo_feedback_ticker.attach(&feedback360, 5ms);

   car.goStraight(-30);
   while(1) {
      pattern = (int)qti1;
     // printf("%d\n",pattern);


      if (pattern == 0b1000) car.turn(-40, 0.3);
      else if (pattern == 0b1100) {
          car.turn(-40, 0.3);
          count = 0;
      }
      else if (pattern == 0b0100) {
          car.turn(-40, 0.3);
          count = 0;
      }
      else if (pattern == 0b0110) {
          car.goStraight(-30);
          count = 0;
      }
      else if (pattern == 0b0010) {
          car.turn(-40, -0.3);
          count = 0;
      }
      else if (pattern == 0b0011) {
          car.turn(-40, -0.3);
          count = 0;
      }
      else if (pattern == 0b0001) {
          car.turn(-40, -0.3);
          count = 0;
      }
      else if (pattern == 0b1111) {
          if (count < 400) {
            if (count == 390){
                //printf("turns = %d, angle = %d\n", turns, angle);
                distance1 = -3.1416 * 6.6 * angle1 / 360;
                distance2 = 3.1416 * 6.6 * angle2 / 360;
                if (count2 < 4) {
                    distance_store[count2] = (distance1 + distance2) / 2;
                    printf("count2 = %d\n", count2);
                    //printf("angle1 = %d\n", angle1);
                    //printf("angle2 = %d\n", angle2);
                    printf("distance = %f\n", (distance1 + distance2) / 2);
                    uLCD.text_width(1); // 4X size text
                    uLCD.text_height(1);
                    uLCD.color(RED);
                    uLCD.locate(1, count2+1);
                    int distance_uLCD =  (distance1 + distance2) / 2;
                    uLCD.printf("distance = %d\n", distance_uLCD);
                    count2++;
                } else {
                    distance_store[count2] = (distance1 + distance2) / 2;
                    printf("count2 = %d\n", count2);
                    printf("distance = %f\n", (distance1 + distance2) / 2);
                    uLCD.text_width(1); // 4X size text
                    uLCD.text_height(1);
                    uLCD.color(RED);
                    uLCD.locate(1, count2+1);
                    int distance_uLCD =  (distance1 + distance2) / 2;
                    uLCD.printf("distance = %d\n", distance_uLCD);
                    final_distance = distance_store[4] - distance_store[0];
                    printf("circumference = %f\n", final_distance);
                    int total = final_distance;
                    uLCD.text_width(1); // 4X size text
                    uLCD.text_height(1);
                    uLCD.color(RED);
                    uLCD.locate(1, 6);
                    uLCD.printf("total = %d\n", total);
                    distance_store[0] = 0;
                    distance_store[1] = 0;
                    distance_store[2] = 0;
                    distance_store[3] = 0;
                    distance_store[4] = 0;
                    count2 = 0;
                }
            }
            car.stop();
            count++;
          } else {
            car.goStraight(-30);
          }
      }
//      else if (pattern == 0b0111) {
//          car.turn(-40, -0.3);
//          count = 0;
//      }
//      else if (pattern == 0b1110) {
//          car.turn(-40, 0.3);
//          count = 0;
//      }
      else {
          car.goStraight(-30);
          count = 0;
      }
      ThisThread::sleep_for(10ms);
   }
}
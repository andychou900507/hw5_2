# hw5_2

How to set up this program?
1. Import bbcar.h and PwmIn.h to control the car, and and uLCD_4DGL to utilize the uLCD.
2. Connect the PwmOut and PwmIn of the two servos, four QTI sensor pin, and Tx and Rx of uLCD.
3. Connect the board and then compile and run the program.
4. Press the reset button.
5. Connect the mbed board and the two servos to the power supply.
6. Put the car on the first special marking.

What are the results?
1. After setting up the car and pressing the reset button, the car will stop at the first special marking for about four seconds and then start to go ahead.
2. The car will utilize the feedback signal transmitted by QTI sensors to determine whether it is going to turn left or right.
3. When the car arrives the following special markings, it will display the place of the four special markings. Besides, at these four special markings, the car will always stop for about four seconds.
4. After the car go back to its starting point, it will calculate and display the total distance of the circle track.

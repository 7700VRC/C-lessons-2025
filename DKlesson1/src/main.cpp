/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       georgekirkman                                             */
/*    Created:      3/26/2025, 4:17:27 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
controller  Controller1;
brain Brain;

int AutonSelected=0;
int AutonMin=0;
int AutonMax=2;

void drawGUI(){
Brain.Screen.clearScreen();
Brain.Screen.setFillColor(red);
Brain.Screen.drawRectangle(50, 50, 100, 100);
Brain.Screen.setFillColor(transparent);
Brain.Screen.printAt (50,175,"Auton Selected");
}

void selectAuton(){
  int x= Brain.Screen.xPosition();
  int y= Brain.Screen.yPosition();

  if(x>=50){
    if(x<=150){
      if(y>=50){
        if(y<=150){
          AutonSelected=AutonSelected+1;
          if(AutonSelected>AutonMax){
            AutonSelected=AutonMin;
          }
          Brain.Screen.printAt(1,200, "Auton Selected =  %d  ",AutonSelected);
      }
    }
  }

  }
  return;
}
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
Brain.Screen.printAt(1,20,"Pre Auto is running my friend");
drawGUI();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  Brain.Screen.printAt(1,40,"My Auto is running ");
  switch (AutonSelected) {
    case 0:
      //code 0
      Brain.Screen.drawCircle(200,200,25);
      break;
      case 1:
      //code 1
      Brain.Screen.clearScreen();
      Brain.Screen.drawLine(1,20,200,200);
      break;
      case 2:
      //code 2
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor(blue);
      Brain.Screen.drawRectangle(1,20,200,200);
      break;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  Brain.Screen.printAt(1,60,"User is running ");
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
Brain.Screen.pressed(selectAuton);
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

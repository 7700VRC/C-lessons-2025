/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       skibidy                                         */
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

/// MOTORS and Devices Info for 7899C Robot
//Scoring/Intake Motors
motor Intake = motor(PORT11, ratio6_1, false);
motor WallStake = motor(PORT12, ratio18_1, false);
//Drive Motors
motor RightTop = motor(PORT7, ratio6_1, true);
motor RightMiddle = motor(PORT5, ratio6_1, false);
motor RightBack = motor(PORT6, ratio6_1, false);
motor LeftTop = motor(PORT10, ratio6_1, false);
motor LeftMiddle = motor(PORT9, ratio6_1, true);
motor LeftBack = motor(PORT8, ratio6_1, true);
//Pneumatics
digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
pneumatics Doinker = pneumatics(Brain.ThreeWirePort.H);
//Gyro
inertial Gyro = inertial(PORT2);
//Potentiometer
analog_in LBpot = analog_in(Brain.ThreeWirePort.B);

//END-- MOTORS and Devices Info for 7899C Robot
//hi
/*
// MOTORS and Devices Info for 7899A Robot
//drivebase motors
motor FrontLeft = motor (PORT14, ratio6_1, true);
motor FrontRight = motor (PORT8, ratio6_1, false);
motor MiddleLeft = motor (PORT17, ratio6_1, true);
motor MiddleRight = motor (PORT7, ratio6_1, false);
motor BackLeft = motor (PORT20, ratio6_1, true);
motor BackRight = motor (PORT1, ratio6_1, false);

//subsystems
motor skibiditoilet = motor (PORT9, ratio6_1, false);
motor ladyblack = motor (PORT4, ratio36_1, true);

//postons
pneumatics mogoClamp = Brain.ThreeWirePort.A;
*/

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



void drive(int lspeed, int rspeed, int wt){
  LeftBack.spin(forward,lspeed,pct);
  LeftMiddle.spin(forward,lspeed,pct);
  LeftTop.spin(forward,lspeed,pct);

  RightBack.spin(forward,rspeed,pct);
  RightMiddle.spin(forward,rspeed,pct);
  RightTop.spin(forward,rspeed,pct);

  wait(wt,msec);
}

void driveBrake(){
  LeftBack.stop(brake);
  LeftMiddle.stop(brake);
  LeftTop.stop(brake);
  RightBack.stop(brake);
  RightMiddle.stop(brake);
  RightTop.stop(brake);
}
float Pi=3.14;  
float D=2.75; //wheel diameter
float G=36.0/48.0;
void inchDrive(float target){
 LeftBack.setPosition(0,rev);
 float x = 0.0;
 float error=target;
 float accuracy=0.5;
 float kp=7.0;
 float speed=kp*error;
 while(fabs(error)>accuracy){
  drive(speed,speed,10);
  x=LeftBack.position(rev)*Pi*D*G;
  error=target-x;
 }
driveBrake();
}

float r=12.0;
void arcDrive(float R, float angle){
  float TargetL= 2*Pi*(R+r)*angle/360;
  float errorL=TargetL;
  float kp=2.0;
  float accuracy = 0.5;
  float s=0.0;
  float rspeed;
  float lspeed;
  while(fabs(errorL)>accuracy){
    lspeed=kp*errorL;
    if(lspeed>100) lspeed=100;
    if(lspeed<100) lspeed=-100;
    rspeed=lspeed*R/(R+r);
    drive(lspeed, rspeed, 10);
    s=LeftMiddle.position(rev)*Pi*D*G;
    errorL=TargetL-s;
  }
  driveBrake();
}
void gyroTurn(float target){
 float heading=0.0;
 float error= target-heading;
 float kp=0.7;
 float speed=kp*error;
 float accuracy=0.5;
 Gyro.setRotation(0.0,degrees);
 while(fabs(error)>accuracy){
  drive(speed,-speed,10);
  heading=Gyro.rotation(degrees);
  error=target-heading;
  speed=kp*error;
 }
 driveBrake();
}

void closeClamp(){
  Clamp.set(true);
}
void openClamp(){
  Clamp.set(false);
}

void toggleClamp(){
  Clamp.set(!Clamp.value());
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
  wait(2000, msec);
  toggleClamp();
  wait(200, msec);
  openClamp();
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
      arcDrive(24,90);
      wait(500, msec);
      inchDrive(12);
      break;
      case 1:
      //code 1
      Brain.Screen.clearScreen();
      Brain.Screen.drawLine(1,20,200,200);
      case 2:
      //code 2
      Brain.Screen.clearScreen();
  }
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

// Main will set up the competition functions and callbacks.
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
//iicbtbhowytomicdaaicelhtlwistwylwhliticdaaicelhtllylyiatimbbbistitcitysgbinlylaygijayiwtikwmytiss
//dynhigqwtneamyaasdydlamtwidnrohydftsotbpltyhasnssspbbbohiwywuodrtmcylaljlmstwittyocwrtasaatmhaislalbtyfitcilyfts - wagtdlimbsittbutlidmabihtgtomcityttwittyocwrtasaatmhaislalbdyfitcilyfts - cilyjtoyikilyfts
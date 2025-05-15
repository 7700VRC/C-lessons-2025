/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       jonathan xu                                               */
/*    Created:      4/2/2025, 4:??:?? PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <iostream>
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
digital_out Doinker = digital_out(Brain.ThreeWirePort.H);
//Gyro
inertial Gyro = inertial(PORT20);
//Potentiometer
analog_in LBpot = analog_in(Brain.ThreeWirePort.B);

//END-- MOTORS and Devices Info for 7899C Robot

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
//random numbers

float pi=3.1415926535897932384626433832795028841971693993751058;
float D=2.75;
float G=36.0/48.0;
float r=12.0;


//6 7 :)



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
void closeClamp(){
  Clamp.set(true);
}

void openClamp(){
  Clamp.set(false);
}

void toggleClamp(){
  Clamp.set(!Clamp.value());
}

void doinker(){
  Doinker.set(!Doinker.value());
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
Brain.Screen.printAt(1,20,"Pre Auto is running  >:)");
drawGUI();
wait(2000,msec); 
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}


void drive(int lspeed, int rspeed, int wt){
 //left motors
  LeftBack.spin(fwd,lspeed,pct);
  LeftMiddle.spin(fwd,lspeed,pct);
  LeftTop.spin(fwd,lspeed,pct);
 //right motors
  RightBack.spin(fwd,rspeed,pct);
  RightMiddle.spin(fwd,rspeed,pct);
  RightTop.spin(fwd,rspeed,pct);
  //wait
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
//inchdrive with heading correction cuz im goated that way
void inchDrive(float target){
  LeftBack.setPosition(0,rev);
  float x=LeftBack.position(rev)*pi*D*G;
  float error=target;
  float accuracy=0.5;
  float kp=4.0;
  float speed=kp*error;
  float heading=0.0;
  Gyro.resetHeading();
  Gyro.setRotation(0.0,degrees);
  float gyro=Gyro.rotation(degrees);
  float correction=(heading-gyro)*0.75;
  while(fabs(error)>accuracy){
    float x=LeftBack.position(rev)*pi*D*G;
    error=target-x;
    speed=kp*error;
    if(speed>100){
      speed=100;
    }
    if(speed<-100){
      speed=-100;
    }
    float gyro=Gyro.rotation(degrees);
    float correction=heading-gyro;
    drive(speed+correction,speed-correction,10);
  }
  driveBrake();
  wait(250,msec);
}
//PIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPIDPID
void gyroTurn(float target){
  float heading = 0.0;
  float error = target-heading;
  float kp = 0.7;
  float kd = 0.7;
  float de = 0;
  float dt = 10;
  float perror=0;
  float speed = kp*error;
  float accuracy = 0.5;
  float mpower = 0.05;
  while(fabs(error)>accuracy){
    drive(speed,-speed,10);
    heading = Gyro.rotation(degrees);
    error = target-heading;
    de=error-perror;
    speed=kp*error+kd*(de/dt);
    if (fabs(speed)<mpower){
      speed=mpower *speed/fabs(speed);
    }
    perror=error;

  }
  driveBrake();
  wait(250,msec);
}

void arcTurn(float R, float angle){
  float targetL=pi*D*angle*(r+R)/360;
  float s=LeftMiddle.position(rev)*pi*D*G;
  float errorL=targetL-s;
  float kp=4.0;
  float accuracy=0.5;
  float lspeed=kp*errorL;
  float rspeed=lspeed/(r+R)/R;
  while(fabs(errorL)>accuracy){
    if(lspeed>100) lspeed=100;
    if (lspeed<-100) lspeed=-100;
    drive(lspeed,rspeed,10);
    float s=LeftMiddle.position(rev)*pi*D*G;
    errorL=targetL-s;
    float rspeed=kp*errorL;
    float lspeed=rspeed*(r+R)/R;
  }
  driveBrake();
}
//   {}    ___        {}
//---[]---/ []     ---[]---  
//   /\               /\                                                                                                                                                                                                                                                                                                                                                        \
//  /  \             /  \                                                                                                                                                                                                                                                                                                                                                       \
//    
//      pew pew noises
//

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
      std::cout<<"GyroTurn Running"<<std::endl;
      for(int count=0; count<4; count=count+1){
        gyroTurn(90);
      }
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
  while (true) {
    float lstick=Controller1.Axis3.position();
    float sstick=Controller1.Axis1.position();

    drive(lstick+sstick,lstick-sstick,10);

  }
}

//
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
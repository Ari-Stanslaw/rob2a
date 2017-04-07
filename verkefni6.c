#pragma config(Sensor, in3,    lineFollowerRIGHT, sensorLineFollower)
#pragma config(Sensor, in4,    lineFollowerCENTER, sensorLineFollower)
#pragma config(Sensor, in5,    lineFollowerLEFT, sensorLineFollower)
#pragma config(Sensor, in8,    lightSensor,    sensorReflection)
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  sonarSensor,    sensorSONAR_cm)
#pragma config(Sensor, dgtl8,  armSensor,      sensorTouch)
#pragma config(Motor,  port2,           leftMotor,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port3,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           clawMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           armMotor,      tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*----------------------------------------------------------------------------------------------------*\
|*                             	    - Triple Sensor Line Tracking -                                   *|
|*                                      ROBOTC on VEX 2.0 CORTEX                                      *|
|*                                                                                                    *|
|*  This program uses 3 VEX Line Follower Sensors to track a black line on a light(er) surface.       *|
|*  There is a two second pause at the beginning of the program.                                      *|
|*                                                                                                    *|
|*                                        ROBOT CONFIGURATION                                         *|
|*    NOTES:                                                                                          *|
|*    1)  Reversing 'rightMotor' (port 2) in the "Motors and Sensors Setup" is needed with the        *|
|*        "Squarebot" mode, but may not be needed for all robot configurations.                       *|
|*    2)  Lighting conditions, line darkness, and surface lightness change from place to place,       *|
|*        so the value of 'threshold' may need to be changed to better suit your environment.         *|
|*                                                                                                    *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]          [Name]              [Type]                [Description]                     *|
|*    Motor  - Port 2     rightMotor          VEX 3-wire module     Right side motor                  *|
|*    Motor  - Port 3     leftMotor           VEX 3-wire module     Left side motor                   *|
|*    Analog - Port 1     lineFollowerRIGHT   VEX Light Sensor      Front-right, facing down          *|
|*    Analog - Port 2     lineFollowerCENTER  VEX Light Sensor      Front-center, facing down         *|
|*    Analog - Port 3     lineFollowerLEFT    VEX Light Sensor      Front-left, facing down           *|
\*-----------------------------------------------------------------------------------------------4246-*/


//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++

const float WHEELBASE = 2.4;

void reset()
{
	motor[rightMotor] = 0;
	motor[leftMotor]  = 0;
	wait1Msec(1000);
}
void resetcoder()
{
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;
}
void forward()
{
	while(abs(SensorValue[leftEncoder]) < 564) // While the right encoder is less than distance:
	{
		motor[rightMotor] = 60;		  // Motor on port2 is run at full (60) power reverse
		motor[leftMotor]  = 60;		  // Motor on port3 is run at full (60) power
  }
}
void backward()
{
		motor[rightMotor] = -30;
		motor[leftMotor]  = -30;
		wait1Msec(1000);
}
void armDown()
{
    motor[armMotor] = 50;
}
void armUp()
{
    motor[armMotor] = 30;
}
void clawopen()
{
	motor[clawMotor] = 50;
  wait1Msec(1000);
}

void stopGrab()
{
	motor[clawMotor] = 0;
	wait1Msec(1100);
}
void grab()
{

	motor[clawMotor] = -60;
	wait1Msec(500);
	stopGrab();
	motor[armMotor] = -50;
  wait1Msec(2000);
  motor[armMotor] = 0;
  wait1Msec(1000);
}
void drop()
{
	motor[clawMotor] = 50;
  wait1Msec(1000);
	stopGrab();
	motor[armMotor] = -50;
  wait1Msec(1000);
  motor[armMotor] = 0;
  wait1Msec(1000);
}

void get()
{
	motor[leftMotor]  = 30;
  motor[rightMotor] = 30;
  wait1Msec(1000);
  reset();
}

void turnAround(int deg)
{
	while(abs(SensorValue[leftEncoder]) < deg * WHEELBASE) // While the right encoder is less than distance:
	{
		motor[rightMotor] = -60;		  // Motor on port2 is run at full (-60) power reverse
		motor[leftMotor]  = 60;		  // Motor on port3 is run at full (60) power
	}
}



task main()
{
 	wait1Msec(2000);// The program waits for 2000 milliseconds before continuing.

 	bool starts = true;

 	bool holding = false;

  int threshold = 2300;

  while(starts && vexRT[Btn8D] != 1)
  {

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
    displayLCDCenteredString(0, "LEFT  CNTR  RGHT");        //  Display   |
    displayLCDPos(1,0);                                     //  Sensor    |
    displayNextLCDNumber(SensorValue(lineFollowerLEFT));    //  Readings  |
    displayLCDPos(1,6);                                     //  to LCD.   |
    displayNextLCDNumber(SensorValue(lineFollowerCENTER));  //            |
    displayLCDPos(1,12);                                    //  L  C  R   |
    displayNextLCDNumber(SensorValue(lineFollowerRIGHT));   //  x  x  x   |
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
		while(SensorValue(sonarSensor) > 20 && vexRT[Btn8D] != 1)
  	{
    	// RIGHT sensor sees dark:
    	if(SensorValue(lineFollowerRIGHT) > threshold && vexRT[Btn8D] != 1)
    	{
     	 	// counter-steer right:
      	motor[leftMotor]  = 60;
     	 	motor[rightMotor] = -30;
    	}
    	// CENTER sensor sees dark:
    	if(SensorValue(lineFollowerCENTER) > 2500 && vexRT[Btn8D] != 1)
    	{
      	// go straight
      	motor[leftMotor]  = 60;
      	motor[rightMotor] = 60;
    	}
    	// LEFT sensor sees dark:
    	if(SensorValue(lineFollowerLEFT) > threshold && vexRT[Btn8D] != 1)
    	{
      	// counter-steer left:
      	motor[leftMotor]  = -30;
      	motor[rightMotor] = 60;
    	}
    }
    while(SensorValue(sonarSensor) <= 38 && vexRT[Btn8D] != 1 && holding == false)
		{
			motor[leftMotor]  = 0;
      motor[rightMotor] = 0;
      wait1Msec(1000);

      clawopen();
      armDown();
      stopGrab();
      get();
			grab();
			resetcoder();
			turnAround(180);
			holding = true;

		}

		while(SensorValue(sonarSensor) <= 20 && vexRT[Btn8D] != 1 && holding == true)
		{
			motor[leftMotor]  = 0;
      motor[rightMotor] = 0;
      wait1Msec(1000);

      get();
      armDown();
      stopGrab();
			drop();
			resetcoder();
			backward();
			turnAround(180);
			holding = false;
			starts = false;
		}

	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "UW_sensorMux.c"
void sensorConfig();
void driveBoth(int motor_PC, int motor_PD);
void driveDist(float dist, int speed);
void driveAngle(int angle, int speed);
void displayShipping(int colour, int zone);
void selectShipping(int &colour, int &zone);
void driveTillShelf(int colour);
void driveUltra(int dist, int direction, int speed);
void startup();
void shutdown();

task main()
{
	sensorConfig();
	int colour = 0;
	int zone = 0;

	displayShipping(colour, zone);
	time1[T1] = 0;
	while(time1[T1] < 300000)
	{
		displayBigTextLine(9,"COLOUR: %d", readMuxSensor(msensor_S3_1));
		if(getButtonPress(buttonEnter))
		{
			selectShipping(colour, zone);
		}
		/*
		if(readMuxSensor(msensor_S3_2)==(int)colorRed
			|| readMuxSensor(msensor_S3_2)==(int)colorGreen
			|| readMuxSensor(msensor_S3_2)==(int)colorBlue)
		{
			colour = readMuxSensor(msensor_S3_2);
			//shelfspace function
			//shipblock
		}
		*/
		wait1Msec(100);
	}

}

void sensorConfig()
{
	SensorType[S3] = sensorEV3_GenericI2C;
	wait1Msec(100);
	// configure each channel on the sensor mux
	if (!initSensorMux(msensor_S3_1, colorMeasureColor))
		return;
	//Ground Color
	if (!initSensorMux(msensor_S3_2, colorMeasureColor))
		return;
	//Forklift Color
	if (!initSensorMux(msensor_S3_3, sonarCM))
		return;
	//Forklift Ultra

	SensorType[S2] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S2] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Ultrasonic;
	//Front Facing Ultra
	wait1Msec(100);
}

void driveBoth(int motor_PC, int motor_PD)
{
	motor[motorC] = -motor_PC;
	motor[motorD] = -motor_PD;
}

void driveDist(float dist, int speed)
{
	const float C_CM = (4*PI)/180;
	int c_dist = nMotorEncoder[motorC];
	int direction = 1;
	if (dist < 0)
	{
		direction = -1;
	}
	driveBoth(speed*direction, speed*direction);
	while(direction*nMotorEncoder[motorC] > direction*((-dist/C_CM)+(c_dist)))
	{
	}
	driveBoth(0,0);
}

void driveAngle(int angle, int speed)
{
	int gyro_angle = getGyroDegrees(S2);
	int direction = 1;
	if(angle < 0)
	{
		direction = -1;
	}
	driveBoth(direction*speed,direction*-speed);
	while (direction*(getGyroDegrees(S2))<direction*(gyro_angle+angle+(direction*-45)))
	{
	}
	driveBoth((direction*speed)/2,(direction*-speed)/2);
	while (direction*(getGyroDegrees(S2))<direction*(gyro_angle+angle))
	{
	}
	driveBoth(0,0);
}

void displayShipping(int colour, int zone)
{
	displayBigTextLine(0,"PRESS ENTER");
	displayBigTextLine(3,"TO SHIP");
	/*
	displayBigTextLine(6,"--------");
	displayBigTextLine(9,"COLOUR: %d", colour);
	displayBigTextLine(12,"ZONE: %d", zone);
	*/
}

void selectShipping(int &colour, int &zone)
{
	while(getButtonPress(buttonAny))
	{
	}
	displayBigTextLine(0,"SELECT A");
	displayBigTextLine(3,"COLOURED BLOCK.");
	displayBigTextLine(6,"LEFT: RED");
	displayBigTextLine(9,"ENTER: GREEN");
	displayBigTextLine(12,"RIGHT: BLUE");
	while(!getButtonPress(buttonLeft) && !getButtonPress(buttonEnter)
		&& !getButtonPress(buttonRight))
	{
	}
	if(getButtonPress(buttonLeft))
	{
		colour = (int)colorRed;
	}
	if(getButtonPress(buttonEnter))
	{
		colour = (int)colorGreen;
	}
	if(getButtonPress(buttonRight))
	{
		colour = (int)colorBlue;
	}
	while(getButtonPress(buttonAny))
	{
	}
	displayBigTextLine(0,"SELECT A");
	displayBigTextLine(3,"SHIPPING ZONE.");
	displayBigTextLine(6,"LEFT: ZONE 1");
	displayBigTextLine(9,"ENTER:ZONE 2");
	displayBigTextLine(12,"RIGHT: ZONE 3");
	while(!getButtonPress(buttonLeft) && !getButtonPress(buttonEnter)
		&& !getButtonPress(buttonRight))
	{
	}
	if(getButtonPress(buttonLeft))
	{
		zone = (int)colorRed;
	}
	if(getButtonPress(buttonEnter))
	{
		zone = (int)colorGreen;
	}
	if(getButtonPress(buttonRight))
	{
		zone = (int)colorBlue;
	}
	eraseDisplay();
	displayShipping(colour, zone);
	while(getButtonPress(buttonAny))
	{
	}
}

void driveTillShelf(int colour)
{
	if (colour == (int)colorRed)
	{
		driveBoth(20,20);
		while(readMuxSensor(msensor_S3_1)!=(int)colorRed)
		{
			wait1Msec(25);
		}
		driveBoth(0, 0);
	}
	else if (colour == (int)colorGreen)
	{
		driveBoth(20,20);
		while(readMuxSensor(msensor_S3_1)!=(int)colorGreen)
		{
			wait1Msec(25);
		}
		driveBoth(0, 0);
	}
	else
	{
		driveBoth(20,20);
		while(readMuxSensor(msensor_S3_1)!=(int)colorBlue)
		{
			wait1Msec(25);
		}
		driveBoth(0, 0);
	}
}

void driveUltra(int dist, int direction, int speed)
{
	driveBoth(speed*direction, speed*direction);
	while(sensorValue[S4]*direction > dist*direction)
	{
	}
	driveBoth(0,0);
}

void startup()
{
	//move to default position
}

void shutdown()
{
	//Go from default to garage
}

//timer code
/*
time1[T1] = 0;
	while(time1[T1] < 10000)
	{
		displayBigTextLine(8,"C2 %d",readMuxSensor(msensor_S3_2));
		if(readMuxSensor(msensor_S3_2) == 5)
		{
			time1[T1] = 0;
		}
		displayBigTextLine(12,"TIMER %d",time1[T1]);
		wait1Msec(100)
	}
*/

/*
Shipping sensor code:
	time1[T1] = 0;
	int colour = 0;
	int zone = 0;
	while(time1[T1] < 1000000)
	{
		displayBigTextLine(0,"PRESS ENTER");
		displayBigTextLine(3,"TO SHIP");
		displayBigTextLine(6,"--------");
		displayBigTextLine(9,"COLOUR: %d", colour);
		displayBigTextLine(12,"ZONE: %d", zone);
		if(getButtonPress(buttonEnter))
		{
			selectShipping(colour, zone);
		}
		wait1Msec(100);
	}
*/

/*
	int exit=1;
	while (exit==1)
	{
		displayBigTextLine(4,"C1 %d",readMuxSensor(msensor_S3_1));
		displayBigTextLine(8,"C2 %d",readMuxSensor(msensor_S3_2));
		displayBigTextLine(12,"C3 %d",readMuxSensor(msensor_S3_3));
		wait1Msec(250);
		if(getButtonPress(buttonUp))
		{
			motor[motorA] = -50;
		}
		else if(getButtonPress(buttonDown))
		{
			motor[motorA] = 50;
		}
		else
		{
			motor[motorA] = 0;
		}
	}
	*/

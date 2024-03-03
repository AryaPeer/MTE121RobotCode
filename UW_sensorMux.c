#include "mindsensors-ev3smux.h"
/*
* This is a wrapper for the mindsensors-ev3smux.h function calls and is based on
* mindsensors-ev3smux-test1.c
* (xander_at_botbench.com)
*
*/

// The SMUX can have up to 3 sensors attached to it.
tMSEV3 muxedSensor[3];

// Configure your sensor type here.  The following are available:
// colorReflectedLight
// colorAmbientLight
// colorMeasureColor
// gyroAngle
// gyroRate
// sonarCM
// sonarInches
// sonarPresence
// touchStateBump

tEV3SensorTypeMode typeMode[3];

bool initSensorMux(tMUXSensor muxPort, tEV3SensorTypeMode cType)
{
	bool okay = true;
	int index = muxPort % 4;
	typeMode[index] = cType;

	if (!initSensor(&muxedSensor[index], muxPort, typeMode[index]))
	{
		writeDebugStreamLine("initSensor() failed!");
		okay = false;
	}
	return okay;
}

int readMuxSensor(tMUXSensor c_muxPort)
{
	int index = c_muxPort % 4;
	if (!readSensor(&muxedSensor[index]))
		writeDebugStreamLine("readSensor() failed! for %d", index);

	switch(muxedSensor[index].typeMode)
	{
	case touchStateBump:
		return (int)muxedSensor[index].touch;

	case colorReflectedLight:
	case colorAmbientLight:
		return (int)muxedSensor[index].light;

	case colorMeasureColor:
		return (int)muxedSensor[index].color;

	case gyroAngle:
		return (int)muxedSensor[index].angle;

	case gyroRate:
		return (int)muxedSensor[index].rate;

	case sonarCM:
	case sonarInches:
		return (int)(muxedSensor[index].distance/10.0);

	case sonarPresence:
		return (int)muxedSensor[index].presence;
	}
	return 0;
}

#pragma config(Sensor, S1,     color2,         sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2,     indicatorOfZeroPos, sensorColorNxtRED)
#pragma config(Sensor, S3,     widthT,         sensorEV3_Touch)
#pragma config(Sensor, S4,     color1,         sensorI2CCustom)
#pragma config(Motor,  motorA,          colorTrueM,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          hand,          tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorC,          width,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          height,        tmotorEV3_Medium, PIDControl, encoder)
void returnToZero()
{
	motor[height] = -100;
	sleep(1500);
	motor[height] = 0;
	motor[width] = 20;
	while(SensorValue[widthT] != 0);
	motor[width] = -20;
	while(SensorValue[widthT] != 1);
	motor[width] = 0;
	resetMotorEncoder(height);
	resetMotorEncoder(width);
	resetMotorEncoder(colorTrueM);
}
task main()
{
returnToZero();
setMotorTarget(width, 121, 100);
setMotorTarget(height, 656, 100);
waitUntilMotorStop(colorTrueM);
waitUntilMotorStop(width);
waitUntilMotorStop(height);
}

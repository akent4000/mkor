#pragma config(Sensor, S1,     color2,         sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2,     indicatorOfZeroPos, sensorColorNxtRED)
#pragma config(Sensor, S3,     widthT,         sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     color1,         sensorI2CCustom)
#pragma config(Motor,  motorA,          colorTrueM,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          hand,          tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorC,          width,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          height,        tmotorEV3_Medium, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "hitechnic-colour-v2.h"
tHTCS2 colorSensor;
int currentX;
int currentY = 0;
int      color[6] = {5, 4, 2, 1, 3, 6};
int true_color[6] = {2, 6, 5, 3, 4, 1};
int true_color_adv[6] = {0, 0, 0, 0, 0, 0};
int types[6] = {1, 0, 1, 2, 0, 2};
int graphs[3][6];
int graph_size[3];
int graph_amount_ball[3];
int graph_amount_cubes[3];
int graph_amount_cube_simple[3];
int graph_amount_cube_holl[3];
int cargo[6][7];
int getColor();
int whiteCube;
int refl = 48;//52;
int PosEncoder = 450;
int PosEncoderMin = 160;
/*
0 - color
1 - black
2 - blue
3 - green
4 - yellow ball
5 - red
6 - white ball

1 - type
0 - cube
1 - cube with hole
2 - ball

2 - destination

3 - x position

4 - z position
0 - below
1 - on top

5 - cargo above me (number of mas)
0 - 5 - cargo
-1 - null
6 - true position
*/
int findPlaceByColor(int col)//Advanced2
{
	for(int i = 0; i < 6; i++)
	{
		if(col == true_color[i])
		{
			return i;
		}
	}
	return -1;
}
void fillTrueColorAdvanced() //Advanced2
{
	for(int i = 0; i < 6; i++)
	{
		true_color_adv[i] = color[findPlaceByColor(color[i])];
	}
}
void fillTypes() //Advanced1
{
	for(int i = 0; i < 6; i++)
	{
		if(color[i] == 4 || color[i] == 6)
		{
			types[true_color[i] - 1] = 0;
		}
		else
		{
			if(true_color[i] != 6 && true_color[i] != 4)
			{
				types[true_color[i] - 1] = 1;
			}
		}
	}
}
void setPositionX(int x)
{
	int FirstPosition = 0;
	int SecondPosition = 0;
	int speed = 50;
	//int SlowSpeed = 20;
	//int pos;
	//int distances[6] = {1278,1020,706,409,110,-145};
	//pos = distances[x];
	//setMotorTarget(width, pos, 100);
	//waitUntilMotorStop(width);
	if(x > currentX)
	{
		motor[width] = -1 * speed;
		int i = currentX;
		for(; i < x; i++)
		{
			FirstPosition = getMotorEncoder(width);
			while(SensorValue[widthT] <= refl)
			{
				SecondPosition = getMotorEncoder(width);
				if(abs(SecondPosition - FirstPosition) >= PosEncoder)
				{
					setMotorTarget(width, FirstPosition, speed);
					waitUntilMotorStop(width);
					i--;
				}
			}
			while(SensorValue[widthT] > refl)
			{
				{
					SecondPosition = getMotorEncoder(width);
					if(abs(SecondPosition - FirstPosition) >= PosEncoder)
					{
						setMotorTarget(width, FirstPosition, speed);
						waitUntilMotorStop(width);
						i--;
					}
				}
			}
			if(abs(SecondPosition - FirstPosition) <= PosEncoderMin)
			{
				i--;
			}
		}
		motor[width] = 0;
	}
	else if(x < currentX)
	{
		motor[width] = speed;
		int i = currentX;
		for(; i > x; i--)
		{
			FirstPosition = getMotorEncoder(width);
			while(SensorValue[widthT] <= refl)
			{
				SecondPosition = getMotorEncoder(width);
				if(abs(SecondPosition - FirstPosition) >= PosEncoder)
				{
					setMotorTarget(width, FirstPosition, -1 * speed);
					waitUntilMotorStop(width);
					i++;
				}
			}
			while(SensorValue[widthT] > refl)
			{
				{
					SecondPosition = getMotorEncoder(width);
					if(abs(SecondPosition - FirstPosition) >= PosEncoder)
					{
						setMotorTarget(width, FirstPosition, -1 * speed);
						waitUntilMotorStop(width);
						i++;
					}
				}
			}
			if(abs(SecondPosition - FirstPosition) <= PosEncoderMin)
			{
				i++;
			}
		}
		motor[width] = 0;
	}
	else
	{

	}
	currentX = x;
}
void setPositionY(int y)
{
	int pos;
	int distances[5] = {785, 576, 130, 665, 410};
	pos = distances[y];
	setMotorTarget(height, pos, 100);
	waitUntilMotorStop(height);
}
void upHand()
{
	moveMotorTarget(height, 180, -100);
	waitUntilMotorStop(height);
}
void getC()
{
	motor[hand] = -75;
	sleep(75);
	motor[hand] = -20;
}
void setC()
{
	setMotorTarget(hand,0,55);
	waitUntilMotorStop(hand);
}
int check_stay(int first_cargo, int second_cargo)
{
	if(cargo[first_cargo][1] == 2 && cargo[second_cargo][1] != 1)
	{
		return 0;
	}
	else if(cargo[second_cargo][1] == 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void read_color()
{
	for(int i = 0; i < 6; i++)
	{
		cargo[i][0] = color[i];
		switch(cargo[i][0])
		{
		case 1:
			cargo[i][1] = types[0];
			break;
		case 2:
			cargo[i][1] = types[1];
			break;
		case 3:
			cargo[i][1] = types[2];
			break;
		case 4:
			cargo[i][1] = types[3];
			break;
		case 5:
			cargo[i][1] = types[4];
			break;
		case 6:
			cargo[i][1] = types[5];
			break;
		}

		cargo[i][3] = i;
		cargo[i][4] = 0;
		cargo[i][5] = -1;
	}
}

void read_true_color()
{
	for(int i = 0; i < 6; i++)
	{
		for(int i1 = 0; i1 < 6; i1++)
		{
			if(true_color[i] == cargo[i1][0])
			{
				cargo[i1][2] = i;
				if(i == i1)
				{
					cargo[i1][6] = 1;
				}
				else
				{
					cargo[i1][6] = 0;
				}
			}
		}
	}
}

void true_position()
{
	for(int i = 0; i < 6; i++)
	{
		if(cargo[i][2] == cargo[i][3] && cargo[i][4] == 0)
		{
			cargo[i][6] = 1;
		}
		else
		{
			cargo[i][6] = 0;
		}
	}
}

int find_cargo(int destination, int destinationZ)
{
	int i = 0;
	while(i < 6 && (cargo[i][3] != destination || cargo[i][4] != destinationZ)) i++;
	return i < 6 ? i : -1;
}
int check_number_of_cargo_on_pos(int destination)
{
	int i = 0;
	int n = 0;
	while(i < 6)
	{
		n += (cargo[i][3] == destination) ? 1 : 0;
		i++;
	}
	return n;
}

void graphing()
{
	for(int i = 0; i < 3; i++)
	{
		graph_size[i] = 0;
		graph_amount_ball[i] = 0;
		graph_amount_cubes[i] = 0;
		graph_amount_cube_holl[i] = 0;
		graph_amount_cube_simple[i] = 0;
	}
	true_position();
	int unused_cargo[6] = {1, 1, 1, 1, 1, 1};
	int amount_of_unused_cargo = 6;
	int first_cargo = -1;
	int last_cargo = -1;

	for(int i = 0; i < 3; i++)
	{
		for(int i1 = 0; i1 < 6; i1++)
		{
			graphs[i][i1] = -1;
		}
	}

	for(int i = 0; i < 6; i++)
	{
		if(cargo[i][6] == 1)
		{
			unused_cargo[i] = 0;
			amount_of_unused_cargo--;
		}
	}
	for(int i = 0; i < 3 && amount_of_unused_cargo > 0; i++)
	{
		for(first_cargo = 0; first_cargo <  6 && unused_cargo[first_cargo] == 0; first_cargo++);
		graphs[i][0] = first_cargo;
		unused_cargo[first_cargo] = 0;
		amount_of_unused_cargo --;
		graph_size[i]++;
		for(int i1 = 1; cargo[first_cargo][3] != cargo[graphs[i][i1 - 1]][2]; i1++)
		{
			last_cargo = find_cargo(cargo[graphs[i][i1 - 1]][2],0);
			graphs[i][i1] = last_cargo;
			unused_cargo[last_cargo] = 0;
			amount_of_unused_cargo --;
			graph_size[i]++;
		}
	}

	for(int i = 0; i < 3; i++)
	{
		for(int i1 = 0; i1 < 6; i1++)
		{
			if(graphs[i][i1] != -1)
			{
				switch(cargo[graphs[i][i1]][1])
				{
				case 0:
					graph_amount_cubes[i]++;
					graph_amount_cube_simple[i]++;
					break;
				case 1:
					graph_amount_cubes[i]++;
					graph_amount_cube_holl[i]++;
					break;
				case 2:
					graph_amount_ball[i]++;
					break;
				}
			}
		}
	}
}
/*
void write()
{
printf("Cargo mas:\n");
for(int i = 0; i < 6; i++)
{
printf("%d: | ", i);
for(int i1 = 0; i1 < 7; i1++)
{
printf("%d | ", cargo[i][i1]);
}
printf("\n");
}

printf("\nGraphs mas:\n");
for(int i = 0; i < 3; i++)
{
printf("%d: | ", i);
for(int i1 = 0; i1 < 6; i1++)
{
printf("%d | ", graphs[i][i1]);
}
printf("Size = %d, Amount cubes = %d, Amount simple cubes = %d, Amount cubes with hole = %d, Amount balls = %d\n\n", graph_size[i], graph_amount_cubes[i], graph_amount_cube_simple[i], graph_amount_cube_holl[i], graph_amount_ball[i]);
}


}
*/
/*
int true_position_for_cycle()
{
true_position();
int ret = 1;
for(int i = 0; i < 6; i++)
{
if(cargo[i][6] == 0)
{
ret = 0;
return ret;
}
}
return ret;
}
*/
int min(int n1, int n2)
{
	return n1 < n2 ? n1 :	n2;
}
int max(int n1, int n2)
{
	return n1 > n2 ? n1 :	n2;
}
int findCargoByNotTypeEx(int type, int excludeX1, int excludeX2, int excludeX3)
{
	int checkedVars[6] = {-1,-1,-1,-1,-1,-1};
	int c = 0;
	int needVar = -1;
	int minAbs = 100;
	for(int i = 0; i<6; i++)
	{
		if(cargo[i][1] != type && cargo[i][3] != excludeX1 && cargo[i][3] != excludeX2 && cargo[i][3] != excludeX3 && cargo[i][5] == -1 && cargo[i][4] == 0)
		{
			checkedVars[c++] = i;
		}
	}
	//while(getButtonPress(buttonEnter) == 0);
	for(int i = 0; i < 6; i++)
	{
		if(checkedVars[i] == -1)
			break;
		if(abs(currentX - cargo[checkedVars[i]][3]) < minAbs)
		{
			minAbs = abs(currentX - cargo[checkedVars[i]][3]);
			needVar = checkedVars[i];
		}
	}
	return needVar;
}

void movement(int cargo_for_movement, int x)
{
	setPositionY(2);
	if(cargo[cargo_for_movement][5] == -1 && cargo[cargo_for_movement][3] != x)
	{
		int oldX = cargo[cargo_for_movement][3];
		int cargosOnOldX = check_number_of_cargo_on_pos(oldX);
		int cargosOnX = check_number_of_cargo_on_pos(x);
		if(cargosOnX == 0)
		{
			setPositionX(oldX);
			if(cargosOnOldX == 1)
			{
				if(cargo[cargo_for_movement][0] == 3)
				{
					setPositionY(3);
				}
				else
				{
					setPositionY(0);
				}
			}
			else if(cargosOnOldX == 2)
			{
				if(cargo[cargo_for_movement][0] == 3 || cargo[find_cargo(oldX, 0)][0] == 3)
				{
					setPositionY(4);
				}
				else
				{
					setPositionY(1);
				}
			}
			getC();
			setPositionY(2);
			//int maxZ = max(check_number_of_cargo_on_pos(x), cargo[cargo_for_movement][4]);
			//for(int i = 0; i < 6; i++)
			//{
			//	if(cargo[i][3] > min(oldX, x) && cargo[i][3] < max(oldX,x))
			//		maxZ = max(maxZ, cargo[i][4] + 1);
			//}
			//if(whiteCube > min(oldX, x) && whiteCube < max(oldX, x))
			//	maxZ = 2;
			//setPositionY(maxZ);
			//if(maxZ != 2)
			//{
			//	upHand();
			//}
			setPositionX(x);
			if(cargo[cargo_for_movement][0] == 3 || cargo[find_cargo(x, 0)][0] == 3)
			{
				setPositionY(3);
			}
			else
			{
				setPositionY(0);
			}
			setC();
			setPositionY(2);
			cargo[cargo_for_movement][3] = x;
			cargo[cargo_for_movement][4] = 0;

		}
		else
		{
			if(check_stay(cargo_for_movement, find_cargo(x,0)) == 1)
			{
				setPositionX(oldX);
				if(cargosOnOldX == 1)
				{
					if(cargo[cargo_for_movement][0] == 3)
					{
						setPositionY(3);
					}
					else
					{
						setPositionY(0);
					}
				}
				else if(cargosOnOldX == 2)
				{
					if(cargo[cargo_for_movement][0] == 3 || cargo[find_cargo(oldX, 0)][0] == 3)
					{
						setPositionY(4);
					}
					else
					{
						setPositionY(1);
					}
				}
				getC();
				setPositionY(2);
				//int maxZ = max(check_number_of_cargo_on_pos(x), cargo[cargo_for_movement][4]);
				//for(int i = 0; i < 6; i++)
				//{
				//	if(cargo[i][3] > min(oldX, x) && cargo[i][3] < max(oldX,x))
				//		maxZ = max(maxZ, cargo[i][4] + 1);
				//}
				//if(whiteCube > min(oldX, x) && whiteCube < max(oldX, x))
				//	maxZ = 2;
				//setPositionY(maxZ);
				//if(maxZ != 2)
				//{
				//	upHand();
				//}
				setPositionX(x);
				if(cargo[cargo_for_movement][0] == 3 || cargo[find_cargo(x, 0)][0] == 3)
				{
					setPositionY(4);
				}
				else
				{
					setPositionY(1);
				}
				setC();
				cargo[find_cargo(x,0)][5] = cargo_for_movement;
				cargo[cargo_for_movement][3] = x;
				cargo[cargo_for_movement][4] = 1;
			}
		}
		if(check_number_of_cargo_on_pos(oldX) == 1)
		{
			cargo[find_cargo(oldX,0)][5] = -1;
		}
		true_position();
	}
	setPositionY(2);
}
int findGraphByCargo(int cargoForSearch)
{
	for(int i = 0; i < 3; i++)
	{
		for(int k = 0; k < 6; k++)
		{
			if(graphs[i][k] == cargoForSearch) return i;
		}
	}
	return -1;
}
int findCargoByTypeInGraph(int type, int graph, int excludeDes, int excludeX)
{
	int checkedVars[6] = {-1,-1,-1,-1,-1,-1};
	int c = 0;
	int needVar = -1;
	int minAbs = 100;
	for(int i = 0; i<6;i++)
	{
		if(cargo[i][1] == type && findGraphByCargo(i) == graph && cargo[i][3] != excludeDes && cargo[i][3] != excludeX)
		{
			checkedVars[c++] = i;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if(checkedVars[i] == -1)
			break;
		if(abs(currentX - cargo[checkedVars[i]][3]) < minAbs)
		{
			minAbs = abs(currentX - cargo[checkedVars[i]][3]);
			needVar = checkedVars[i];
		}
	}
	return needVar;
}

int findCargoByNotTypeInGraph(int type, int graph, int excludeDes, int excludeX)
{
	int checkedVars[6] = {-1,-1,-1,-1,-1,-1};
	int c = 0;
	int needVar = -1;
	int minAbs = 100;
	for(int i = 0; i < 6;i++)
	{
		if(cargo[i][1] != type && findGraphByCargo(i) == graph && cargo[i][3] != excludeDes && cargo[i][3] != excludeX)
		{
			checkedVars[c++] = i;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if(checkedVars[i] == -1)
			break;
		if(abs(currentX - cargo[checkedVars[i]][3]) < minAbs)
		{
			minAbs = abs(currentX - cargo[checkedVars[i]][3]);
			needVar = checkedVars[i];
		}
	}
	return needVar;

}

int findCargoByTypeNotInGraph(int type, int graph)
{
	int checkedVars[6] = {-1,-1,-1,-1,-1,-1};
	int c = 0;
	int needVar = -1;
	int minAbs = 100;
	for(int i = 0; i<6;i++)
	{
		if(cargo[i][1] == type && findGraphByCargo(i) != graph)
		{
			checkedVars[c++] = i;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if(checkedVars[i] == -1)
			break;
		if(abs(currentX - cargo[checkedVars[i]][3]) < minAbs)
		{
			minAbs = abs(currentX - cargo[checkedVars[i]][3]);
			needVar = checkedVars[i];
		}
	}
	return needVar;
}
int findCargoByNotTypeNotInGraph(int type, int graph)
{
	int checkedVars[6] = {-1,-1,-1,-1,-1,-1};
	int c = 0;
	int needVar = -1;
	int minAbs = 100;
	for(int i = 0; i<6;i++)
	{
		if(cargo[i][1] != type && findGraphByCargo(i) != graph)
		{
			checkedVars[c++] = i;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if(checkedVars[i] == -1)
			break;
		if(abs(currentX - cargo[checkedVars[i]][3]) < minAbs)
		{
			minAbs = abs(currentX - cargo[checkedVars[i]][3]);
			needVar = checkedVars[i];
		}
	}
	return needVar;
}
int findCargoByDestination(int destination)
{
	for(int i = 0; i < 6; i++)
	{
		if(cargo[i][2] == destination)
		{
			return i;
		}
	}
	return -1;
}
int checkCargo()
{
	int n = 0;
	for(int i = 0; i < 6; i++)
	{
		n += cargo[i][3] == cargo[i][2] && cargo[i][4] == 0 ? 1 : 0;
	}
	return n == 6 ? 1 : 0;
}

void returnToZero()
{
	motor[height] = -100;
	sleep(1500);
	motor[height] = 0;
	motor[width] = -20;
	while(SensorValue[widthT] != 7 && SensorValue[widthT] != 5);
	motor[width] = 0;
	resetMotorEncoder(height);
	resetMotorEncoder(width);
	resetMotorEncoder(colorTrueM);
	currentX = 4;
	SensorMode[widthT] = modeEV3Color_Reflected;
	motor[width] = -50;
	int FirstPosition = getMotorEncoder(width);
	while(SensorValue[widthT] <= refl);
	while(SensorValue[widthT] > refl);
	motor[width] = 0;
	currentX = 5;
}


void filling_color_mas()
{
	for(int i = 5; i >= 0; i--)
	{
		setPositionX(i);
		color[i] = getColor();
		if(color[i] == 1)
		{
			setPositionY(0);
			getC();
			sleep(500);
			if(getMotorEncoder(hand) < -60)
			{
				color[i] = 9;
			}
			setC();
			setPositionY(2);
		}
	}
}

void checkColorMas()
{
	int varVoid = 10;
	int sum = 21;
	for(int i = 0; i < 6; i++)
	{
		if(color[i] == 9)
		{
			varVoid = i;
		}
		else
		{
			sum -= color[i];
		}
	}
	if(varVoid != 10)
	{
		color[varVoid] = sum;
		types[color[varVoid] - 1] = 2;
	}
}

void filling_true_color_mas()
{
	int distances[6] = {-857,-747,-636,-512,-399,-284};
	for(int i = 5; i >= 0; i--)
	{
		setMotorTarget(colorTrueM, distances[i], 100);
		waitUntilMotorStop(colorTrueM);
		true_color[i] = SensorValue(color2);
	}
	setMotorTarget(colorTrueM, 0, 100);
}
int getColor()
{
	readSensor(&colorSensor);
	if(colorSensor.color == 0 || colorSensor.color == 12)
	{
		return 1;
	} else if(colorSensor.color == 2)
	{
		return 2;
	} else if(colorSensor.color == 4)
	{
		return 3;

	} else if(colorSensor.color == 7 || colorSensor.color == 6 || colorSensor.color == 5)
	{
		return 4;
	} else if(colorSensor.color == 8 || colorSensor.color == 9)
	{
		return 5;
	} else if(colorSensor.color == 14 || colorSensor.color == 17)
	{
		return 6;
	}
	return -1;
}
bool checkColorMassiveToCorrect()
{
	for(int i = 1; i <= 6; i++)
	{
		int number = 0;
		for(int k = 0; k < 6; k++)
		{
			if(i == color[k])
			{
				number++;
			}
		}
		if(number != 1) return(false);
	}
	return(true);
}
bool checkTrueColorMassiveToCorrect()
{
	for(int i = 1; i <= 6; i++)
	{
		int number = 0;
		for(int k = 0; k < 6; k++)
		{
			if(i == true_color[k])
			{
				number++;
			}
		}
		if(number != 1) return(false);
	}
	return(true);
}
void refillIncorrectColors()
{
	for(int i = 0; i < 6; i++)
	{
		if(color[i] == -1 || color[i] == 7)
		{
			setPositionX(i);
			color[i] = getColor();
		}
		for(int k = 0; k < 6; k++)
		{
			if(i != k && color[i] == color[k])
			{
				setPositionX(i);
				color[i] = getColor();
				setPositionX(k);
				color[k] = getColor();
			}
		}
	}
}
void refillIncorrectTrueColors()
{
	int distances[6] = {-857,-747,-636,-512,-399,-284};
	for(int i = 0; i < 6; i++)
	{
		if(true_color[i] == 0 || true_color[i] == 7)
		{
			setMotorTarget(colorTrueM, distances[i], 100);
			waitUntilMotorStop(colorTrueM);
			true_color[i] = SensorValue(color2);
		}
		for(int k = 0; k < 6; k++)
		{
			if(i != k && true_color[i] == true_color[k])
			{
				setMotorTarget(colorTrueM, distances[i], 100);
				waitUntilMotorStop(colorTrueM);
				true_color[i] = SensorValue(color2);
				setMotorTarget(colorTrueM, distances[k], 100);
				waitUntilMotorStop(colorTrueM);
				true_color[k] = SensorValue(color2);
			}
		}
	}
}
int find_cargo_by_color(int color_for_search)
{
	int i = 0;
	for(; cargo[i][0] != color_for_search; i++);
	return i;
}

int find_place_cargo_by_color(int color_for_search)
{
	int i = 0;
	for(; color[i] != color_for_search; i++);
	return i;
}

task main()
{
	clearTimer(T1);
	initSensor(&colorSensor, color1);
	SensorType[indicatorOfZeroPos] = sensorColorNxtBLUE;
	moveMotorTarget(width, 80, -100);
	waitUntilMotorStop(width);
	moveMotorTarget(width, 150, 100);
	waitUntilMotorStop(width);
	returnToZero();
	SensorMode[widthT] = modeEV3Color_Color;
	setPositionY(2);

	SensorMode[widthT] = modeEV3Color_Reflected;

	filling_color_mas();
	checkColorMas();
	do
	{
		refillIncorrectColors();
		checkColorMas();
		displayCenteredTextLine(1,"%d %d %d %d %d %d",color[0], color[1], color[2], color[3], color[4], color[5]);
	}
	while(checkColorMassiveToCorrect() == false);
	filling_true_color_mas();
	do
	{
		refillIncorrectTrueColors();
		displayCenteredTextLine(2,"%d %d %d %d %d %d",true_color[0], true_color[1], true_color[2], true_color[3], true_color[4], true_color[5]);
	}
	while(checkTrueColorMassiveToCorrect() == false);
	setMotorTarget(colorTrueM, 0 ,100);
	displayCenteredTextLine(4,"%d %d %d %d %d %d", types[0], types[1], types[2], types[3], types[4], types[5]);
	displayCenteredTextLine(6,"%d %d %d %d %d %d",true_color[0], true_color[1], true_color[2], true_color[3], true_color[4], true_color[5]);
	read_color();
	read_true_color();
	true_position();
	graphing();
	while(checkCargo() == 0)
	{
		for(int i = 0; i < 3; i++)
		{
			if(graph_amount_cubes[i] == 4)
			{

				int cargoForMove = findCargoByNotTypeInGraph(2,i, -1, -1);
				int cargoForStay = findCargoByNotTypeInGraph(2,i, cargo[cargoForMove][2], cargo[cargoForMove][3]);
				int cargoForRemove = find_cargo(cargo[cargoForMove][2],0);
				int oldX = cargo[cargoForMove][3];
				int newX = cargo[cargoForRemove][3];
				int middleX = cargo[cargoForStay][3];
				movement(cargoForMove, middleX);
				movement(cargoForRemove, oldX);
				movement(cargoForMove, newX);
			}
			else if(graph_amount_ball[i] == 2 && graph_size[i] == 2)
			{
				int cargoForMove1 = findCargoByTypeInGraph(2,i,-1,-1);
				int cargoForMove2 = findCargoByTypeInGraph(2,i,cargo[cargoForMove1][3],-1);
				int cargoForStay = findCargoByTypeNotInGraph(1,i);
				int X1 = cargo[cargoForMove1][3];
				int X2 = cargo[cargoForMove2][3];
				int middleX = cargo[cargoForStay][3];
				movement(cargoForMove1, middleX);
				movement(cargoForMove2, X1);
				movement(cargoForMove1, X2);
			}
			else if(graph_size[i] != 0)
			{
				int cargoForStay = findCargoByNotTypeNotInGraph(2,i);
				int cargoForMoveOut = findCargoByNotTypeInGraph(2,i,-1,-1);
				int currentDestinstion = cargo[cargoForMoveOut][3];
				movement(cargoForMoveOut, cargo[cargoForStay][3]);
				while(findCargoByDestination(currentDestinstion) != cargoForMoveOut)
				{
					int cargoForMove = findCargoByDestination(currentDestinstion);
					int temp = cargo[cargoForMove][3];
					movement(cargoForMove, cargo[cargoForMove][2]);
					currentDestinstion = temp;
				}
				movement(cargoForMoveOut, cargo[cargoForMoveOut][2]);
			}
			graphing();
		}
	}
	int temp = 21;
	for(int i = 0; i < 6; i++)
	{
		if(color[i] != 9)
		{
			temp -= color[i];
		}
	}
	if(temp != 1)
	{
		if(temp != 4 && temp != 6)
		{
			movement(find_cargo_by_color(4), cargo[find_cargo_by_color(3)][3]);
			movement(find_cargo_by_color(6), cargo[find_cargo_by_color(1)][3]);
		}
		else if(temp == 6)
		{
			movement(find_cargo_by_color(4), cargo[find_cargo_by_color(3)][3]);
		}
		else if(temp == 4)
		{
			movement(find_cargo_by_color(6), cargo[find_cargo_by_color(3)][3]);
		}
		else if(temp == 3)
		{
			int xWhite = 0;
			for(; color[xWhite] != 6; xWhite++);
			int xYellow = 0;
			for(; color[xYellow] != 4; xYellow++);
			int xBlack = 0;
			for(; color[xBlack] != 6; xBlack++);

			if(abs(xBlack - xWhite) > abs(xBlack - xYellow))
			{
				movement(find_cargo_by_color(4), cargo[find_cargo_by_color(1)][3]);
			}
			else if(abs(xBlack - xWhite) < abs(xBlack - xYellow))
			{
				movement(find_cargo_by_color(6), cargo[find_cargo_by_color(1)][3]);
			}
			else if(abs(xBlack - xWhite) == abs(xBlack - xYellow))
			{
				if(abs(findPlaceByColor(1)) > abs(findPlaceByColor(4)))
				{
					movement(find_cargo_by_color(4), cargo[find_cargo_by_color(1)][3]);
				}
				else if(abs(findPlaceByColor(1)) < abs(findPlaceByColor(4)))
				{
					movement(find_cargo_by_color(6), cargo[find_cargo_by_color(1)][3]);
				}
				else if(abs(findPlaceByColor(1)) == abs(findPlaceByColor(4)))
				{

				}
			}
		}
	}
	else if(temp == 1)
	{
		int xWhite = 0;
		for(; color[xWhite] != 6; xWhite++);
		int xYellow = 0;
		for(; color[xYellow] != 4; xYellow++);
		int xGreen = 0;
		for(; color[xGreen] != 6; xGreen++);

		if(abs(xGreen - xWhite) > abs(xGreen - xYellow))
		{
			movement(find_cargo_by_color(4), cargo[find_cargo_by_color(3)][3]);
		}
		else if(abs(xGreen - xWhite) < abs(xGreen - xYellow))
		{
			movement(find_cargo_by_color(6), cargo[find_cargo_by_color(3)][3]);
		}
		else if(abs(xGreen - xWhite) == abs(xGreen - xYellow))
		{
			if(abs(findPlaceByColor(3)) > abs(findPlaceByColor(4)))
			{
				movement(find_cargo_by_color(4), cargo[find_cargo_by_color(3)][3]);
			}
			else if(abs(findPlaceByColor(3)) < abs(findPlaceByColor(4)))
			{
				movement(find_cargo_by_color(6), cargo[find_cargo_by_color(3)][3]);
			}
			else if(abs(findPlaceByColor(3)) == abs(findPlaceByColor(4)))
			{

			}
		}
	}
	setMotorTarget(width, 0, 100);
	setMotorTarget(height, 300, 100);
	waitUntilMotorStop(colorTrueM);
	waitUntilMotorStop(width);
	waitUntilMotorStop(height);
	SensorType[indicatorOfZeroPos] = sensorColorNxtRED;
	displayCenteredTextLine(3,"%d", time1[T1]/1000);
	sleep(2000);
}

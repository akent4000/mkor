

int      color[6] = {5, 4, 2, 1, 3, 6};
int true_color[6] = {2, 6, 5, 3, 4, 1};

int graphs[3][6];
int graph_size[3];
int graph_amount_ball[3];
int graph_amount_cubes[3];
int graph_amount_cube_simple[3];
int graph_amount_cube_holl[3];
int cargo[6][7];
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
            cargo[i][1] = 1;
            break;
        case 2:
            cargo[i][1] = 0;
            break;
        case 3:
            cargo[i][1] = 1;
            break;
        case 4:
            cargo[i][1] = 2;
            break;
        case 5:
            cargo[i][1] = 0;
            break;
        case 6:
            cargo[i][1] = 2;
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
    while((cargo[i][3] != destination || cargo[i][4] != destinationZ) && i < 6) i++;
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

void movement(int cargo_for_movement, int x)
{
    if(cargo[cargo_for_movement][5] == -1 && cargo[cargo_for_movement][3] != x)
    {
        int oldX = cargo[cargo_for_movement][3];
        if(check_number_of_cargo_on_pos(x) == 0)
        {
            cargo[cargo_for_movement][3] = x;
            cargo[cargo_for_movement][4] = 0;
        }
        else if(check_number_of_cargo_on_pos(x) == 1)
        {
            if(check_stay(cargo_for_movement, find_cargo(x,0)) == 1)
            {
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
}
int findCargoForStaying(int cargoForMovement)
{
    for(int i = 0; i < 6; i++)
    {
        check_stay(cargoForMovement, i);
        return i;
    }
    return -1;
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
    for(int i = 0; i<6;i++)
    {
        if(cargo[i][1] == type && findGraphByCargo(i) == graph && cargo[i][3] != excludeDes && cargo[i][3] != excludeX)
        {
            return i;
        }
    }
    return -1;
}

int findCargoByNotTypeInGraph(int type, int graph, int excludeDes, int excludeX)
{
    for(int i = 0; i < 6;i++)
    {
        if(cargo[i][1] != type && findGraphByCargo(i) == graph && cargo[i][3] != excludeDes && cargo[i][3] != excludeX)
        {
            return i;
        }
    }
    return -1;
}

int findCargoByTypeNotInGraph(int type, int graph)
{
    for(int i = 0; i<6;i++)
    {
        if(cargo[i][1] == type && findGraphByCargo(i) != graph)
        {
            return i;
        }
    }
    return -1;
}
int findCargoByNotTypeNotInGraph(int type, int graph)
{
    for(int i = 0; i<6;i++)
    {
        if(cargo[i][1] != type && findGraphByCargo(i) != graph)
        {
            return i;
        }
    }
    return -1;
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
task main()
{
    read_color();
    read_true_color();
    true_position();
    graphing();
    write();
    while(checkCargo() == 0)
    {
    for(int i = 0; i < 3; i++)
    {
        if(graph_amount_cubes[i] == 4)
        {
            int cargoForMove = findCargoByTypeInGraph(1,i, -1, -1);
            int cargoForStay = findCargoByTypeInGraph(1,i, cargo[cargoForMove][2], cargo[cargoForMove][3]);
            int cargoForRemove = find_cargo(cargo[cargoForMove][2],0);
            int oldX = cargo[cargoForMove][3];
            int newX = cargo[cargoForRemove][3];
            int middleX = cargo[cargoForStay][3];
            movement(cargoForMove, middleX);
            write();
            movement(cargoForRemove, oldX);
            write();
            movement(cargoForMove, newX);
            write();
        }
        else if(graph_amount_ball[i] == 2 && graph_size[i] == 2)
        {
            int cargoForMove1 = findCargoByTypeInGraph(2,i,-1,-1);
            int cargoForMove2 = findCargoByTypeInGraph(2,i,cargo[cargoForMove1][3],-1);
            int cargoForStay = findCargoByTypeNotInGraph(1,i);
            int X1 = cargo[cargoForMove1][3];
            int X2 = cargo[cargoForMove2][3];
            int middleX = cargo[cargoForStay][3];
            printf("1: %d 2: %d 3: %d",cargoForMove1,cargoForMove2,cargoForStay);
            movement(cargoForMove1, middleX);
            write();
            movement(cargoForMove2, X1);
            write();
            movement(cargoForMove1, X2);
            write();
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
                write();
            }
            movement(cargoForMoveOut, cargo[cargoForMoveOut][2]);
        }
        graphing();
    }
    }
    write();
}

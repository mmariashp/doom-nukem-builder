
#include "builder.h"

int                     wall_door(int id, t_wall *walls, int n_walls) // finding the index of a corresponding door to a wall of index id, -1 if none
{
	int                 i;
	t_wall              wall;

	i = 0;
	if (id >= 0 && id < n_walls)
		wall = walls[id];
	else
		return (-1);
	while (i < n_walls)
	{
		if (id != i && ((wall.v1 == walls[i].v1 && wall.v2 == walls[i].v2) ||
						(wall.v1 == walls[i].v2 && wall.v2 == walls[i].v1)))
			return (i);
		i++;
	}
	return (-1);
}

void                    delete_door(t_world *world, int id) //deletes door that is duplicate of wall n id
{
	int                 door;

	if (id < 0 || id >= world->n_walls)
		return ;
	door = wall_door(id, world->walls, world->n_walls);
	if (door == -1)
		return ;
	delete_wall(door, world);
	world->walls[id].door = -1;
}

void                    add_door(t_world *world, int id)
{
	if (id < 0 || id >= world->n_walls)
		return ;
	if (add_wall(&world->walls, world->n_walls, world->walls[id].v1, world->walls[id].v2) == FAIL)
		return ;
	world->walls[world->n_walls].type = WALL_DOOR;
	world->walls[world->n_walls].txtr = world->walls[id].txtr;
	world->walls[world->n_walls].v1 = world->walls[id].v1;
	world->walls[world->n_walls].v2 = world->walls[id].v2;
	world->walls[id].door = world->n_walls;
	printf("adding a door at index %d to wall at index %d\n", world->walls[id].door, id);
	world->n_walls++;
	add_wall_in_secs(world, world->walls[id].door, id);
}
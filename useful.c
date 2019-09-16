
#include "builder.h"

unsigned short			within(int value, int min, int max);
unsigned short			within_incl(int value, int min, int max);
int 					selected_item(char set_get_unset, unsigned short id, int value);
int 					lit_item(char set_get_unset, unsigned short id, int value);

unsigned short			within(int value, int min, int max)
{
	if (value <= min)
		return (FALSE);
	if (value >= max)
		return (FALSE);
	return (TRUE);
}

int 					selected_item(char set_get_unset, unsigned short id, int value)
{
	static int 			select[7] = { -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 7)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}

int 					lit_item(char set_get_unset, unsigned short id, int value)
{
	static int 			select[7] = { -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 7)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}


#include "builder.h"

t_vec2d					add(t_vec2d one, t_vec2d two);
t_vec2d					mult(t_vec2d one, t_vec2d two);
t_vec2d					scale(t_vec2d vec, int scale);

int						clamp(int n, int min, int max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

float					clamp_f(float n, float min, float max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

t_vec2d					add(t_vec2d one, t_vec2d two)
{
	return ((t_vec2d){ one.x + two.x, one.y + two.y });
}

//t_vec2d					subtr(t_vec2d one, t_vec2d two)
//{
//	return ((t_vec2d){ one.x + two.x, one.y + two.y });
//}

t_vec2d					mult(t_vec2d one, t_vec2d two)
{
	return ((t_vec2d){ one.x * two.x, one.y * two.y });
}

t_vec2d					scale(t_vec2d vec, int scale)
{
	return ((t_vec2d){ vec.x * scale, vec.y * scale });
}
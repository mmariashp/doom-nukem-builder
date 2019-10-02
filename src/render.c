/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:10:44 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:10:45 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

void					render_frame(t_rec rec, int color, SDL_Renderer *rend)
{
	if (!rend)
		return ;
	draw_line((t_line){ (t_vec){ rec.x, rec.y }, (t_vec){ rec.x + rec.w, rec.y } }, color, rend);
	draw_line((t_line){ (t_vec){ rec.x, rec.y }, (t_vec){ rec.x, rec.y + rec.h } }, color, rend);
	draw_line((t_line){ (t_vec){ rec.x + rec.w, rec.y }, (t_vec){ rec.x + rec.w, rec.y + rec.h } }, color, rend);
	draw_line((t_line){ (t_vec){ rec.x + rec.w, rec.y + rec.h }, (t_vec){ rec.x, rec.y + rec.h } }, color, rend);
}

void					render_txtr_box(t_vec size, t_vec pos, SDL_Texture *t, SDL_Renderer *rend)
{
	SDL_Rect			rect = { pos.x, pos.y, size.x, size.y };
	if (!t || !rend)
		return ;
	SDL_RenderCopy(rend, t, 0, &rect);
}

void				    get_rgb(unsigned char *r, unsigned char *g, unsigned char *b, int color)
{
	if (!r || !g || !b)
		return ;
	*r = (unsigned char)((color >> 16) & 0xFF);
	*g = (unsigned char)((color >> 8) & 0xFF);
	*b = (unsigned char)(color & 0xFF);
}

//void					draw_dot_surf(int x, int y, int color, SDL_Surface *surf)
//{
////	Uint32				*pixel;
////
////	if (!surf || x < 0 || x > W_W || y < 0 || y > W_H)
////		return ;
////	pixel = (Uint32*)((Uint8 *)surf->pixels + y * surf->pitch + x * surf->format->BytesPerPixel);
////	*pixel = color;
//
//
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//	get_rgb(&r, &g, &b, color);
//	Uint32				pixel_color;
//	pixel_color = SDL_MapRGB(surf->format, r, g, b);
//	int bpp = surf->format->BytesPerPixel;
//	/* Here p is the address to the pixel we want to set */
//	Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
//
//	switch(bpp) {
//		case 1:
//			*p = pixel_color;
//			break;
//
//		case 2:
//			*(Uint16 *)p = pixel_color;
//			break;
//
//		case 3:
//			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
//				p[0] = (pixel_color >> 16) & 0xff;
//				p[1] = (pixel_color >> 8) & 0xff;
//				p[2] = pixel_color & 0xff;
//			} else {
//				p[0] = pixel_color & 0xff;
//				p[1] = (pixel_color >> 8) & 0xff;
//				p[2] = (pixel_color >> 16) & 0xff;
//			}
//			break;
//
//		case 4:
//			*(Uint32 *)p = pixel_color;
//			break;
//	}
//}

void					draw_dot2(int x, int y, t_screen s, t_screen **screen)
{

	if (x >= 0 && x < W_W && y >= 0 && y < W_H)
	{
		if (s.color == EMPTY_COLOR && screen[x][y].color != 0)
			return ;
		if (s.color == EMPTY_WALL_CLR && screen[x][y].color != 0)
			return ;
		screen[x][y] = s;
	}
}

void					draw_dot(int x, int y, int color, SDL_Renderer *rend)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	if (x >= 0 && x < W_W && y >= 0 && y < W_H)
	{
		get_rgb(&r, &g, &b, color);
		SDL_SetRenderDrawColor(rend, r, g, b, 255);
		SDL_RenderDrawPoint(rend, x, y);
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	}
}

void				    swap_ints(int *one, int *two)
{
	int				swap;

	if (!one || !two)
		return ;
	swap = *one;
	*one = *two;
	*two = swap;
}

void				    get_len(double *diff, int *long_l, int *i, t_line line)
{
	int				short_l;

	if (!diff || !long_l || !i)
		return ;
	*i = 0;
	short_l = line.p1.y - line.p0.y;
	*long_l = line.p1.x - line.p0.x;
	if (abs(short_l) > abs(*long_l))
	{
		swap_ints(&short_l, long_l);
		*i = 1;
	}
	*diff = short_l == 0 ? *long_l : (double)*long_l / (double)short_l;
}

int                     computeCode(double x, double y)
{
	// initialized as being inside
	int             code = REGION_INSIDE;

	if (x < 0)       // to the left of rectangle
		code = (code | REGION_LEFT);
	else if (x > W_W)  // to the right of rectangle
		code = (code | REGION_RIGHT);
	if (y < 0)       // below the rectangle
		code = (code | REGION_BOTTOM);
	else if (y > W_H)  // above the rectangle
		code = (code | REGION_TOP);
	return (code);
}

int                     clip_line(t_line *l) //Cohen-Sutherland algorithm
{
	int code1 = computeCode(l->p0.x, l->p0.y);
	int code2 = computeCode(l->p1.x, l->p1.y);
	int accept = FALSE;
	while (1)
	{
		if ((code1 == REGION_INSIDE && code2 == REGION_INSIDE) || code1 & code2)
		{
			if (code1 == REGION_INSIDE)
				accept = TRUE;
			return (accept);
		}
		else
		{
			int     code_out;
			double  x = 0;
			double y = 0;
			if (code1 != 0)
				code_out = code1;
			else
				code_out = code2;
			if (code_out & REGION_TOP)
			{
				x = l->p0.x + (l->p1.x - l->p0.x) * (W_H - l->p0.y) / (l->p1.y - l->p0.y);
				y = W_H;
			}
			else if (code_out & REGION_BOTTOM)
			{
				x = l->p0.x + (l->p1.x - l->p0.x) * (0 - l->p0.y) / (l->p1.y - l->p0.y);
				y = 0;
			}
			else if (code_out & REGION_RIGHT)
			{
				y = l->p0.y + (l->p1.y - l->p0.y) * (W_W - l->p0.x) / (l->p1.x - l->p0.x);
				x = W_W;
			}
			else if (code_out & REGION_LEFT)
			{
				y = l->p0.y + (l->p1.y - l->p0.y) * (0 - l->p0.x) / (l->p1.x - l->p0.x);
				x = 0;
			}
			if (code_out == code1)
			{
				l->p0.x = x;
				l->p0.y = y;
				code1 = computeCode(l->p0.x, l->p0.y);
			}
			else
			{
				l->p1.x = x;
				l->p1.y = y;
				code2 = computeCode(l->p1.x, l->p1.y);
			}
		}
	}
	return (accept);
}

void					draw_circle_fill2(t_vec c, int radius,t_screen s, t_screen **screen)
{
	t_vec				p;
	t_vec				d;
	int					radiusError = 0;

	p = (t_vec){radius, 0};
	d = (t_vec){1 - (radius << 1), 0};
	while (p.x >= p.y)
	{
		for (int i = c.x - p.x; i <= c.x + p.x; i++)
		{
			draw_dot2(i, c.y + p.y, s, screen);
			draw_dot2(i, c.y - p.y, s, screen);
		}
		for (int i = c.x - p.y; i <= c.x + p.y; i++)
		{
			draw_dot2(i, c.y + p.x, s, screen);
			draw_dot2(i, c.y - p.x, s, screen);
		}
		p.y++;
		radiusError += d.y;
		d.y += 2;
		if (((radiusError << 1) + d.x) > 0)
		{
			p.x--;
			radiusError += d.x;
			d.x += 2;
		}
	}
}

void				    draw_line2(t_line l, t_screen s, t_screen **screen)
{
	int				len;
	int				i_value;
	double			d;
	int				i;

	if (!clip_line(&l))
		return ;
	get_len(&d, &len, &i, l);
	i_value = len < 0 ? -1 : 1;
	if (!screen || d == 0)
		return ;
	if (i)
	{
		i = -i_value;
		while ((i += i_value) != len)
			draw_dot2(l.p0.x + (int)(i / d), l.p0.y + i, s, screen);
		return ;
	}
	i = -i_value;
	while ((i += i_value) != len)
		draw_dot2(l.p0.x + i, l.p0.y + (int)(i / d), s, screen);
}

void				    draw_line(t_line l, int color, SDL_Renderer *rend)
{
	int				len;
	int				i_value;
	double			d;
	int				i;

	if (!clip_line(&l))
		return ;
	get_len(&d, &len, &i, l);
	i_value = len < 0 ? -1 : 1;
	if (!rend || d == 0)
		return ;
	if (i)
	{
		i = -i_value;
		while ((i += i_value) != len)
			draw_dot(l.p0.x + (int)(i / d), l.p0.y + i, color, rend);
		return ;
	}
	i = -i_value;
	while ((i += i_value) != len)
		draw_dot(l.p0.x + i, l.p0.y + (int)(i / d), color, rend);
}


//void					draw_dot_grid(int x, int y, int color, char **nod)
//{
//    if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE)
//        nod[x][y] = color;
//}

void				    draw_line_grid(t_line l, char c, signed char nod[GRID_SIZE][GRID_SIZE])
{
    int				len;
    int				i_value;
    double			d;
    int				i;

    if (!clip_line(&l))
        return ;
    get_len(&d, &len, &i, l);
    i_value = len < 0 ? -1 : 1;
    if (!nod || d == 0)
        return ;
    if (i)
    {
        i = -i_value;
        while ((i += i_value) != len)
        {
            int x = l.p0.x + (int)(i / d);
            int y = l.p0.y + i;
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE)
                nod[x][y] = c;
        }
        return ;
    }
    i = -i_value;
    while ((i += i_value) != len)
    {
        int x = l.p0.x + i;
        int y = l.p0.y + (int)(i / d);
        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE)
            nod[x][y] = c;
    }
}

void					draw_circle_fill(t_vec c, int radius, int color, SDL_Renderer *rend)
{
	t_vec				p;
	t_vec				d;
	int					radiusError = 0;

	p = (t_vec){radius, 0};
	d = (t_vec){1 - (radius << 1), 0};
	while (p.x >= p.y)
	{
		for (int i = c.x - p.x; i <= c.x + p.x; i++)
		{
			draw_dot(i, c.y + p.y, color, rend);
			draw_dot(i, c.y - p.y, color, rend);
		}
		for (int i = c.x - p.y; i <= c.x + p.y; i++)
		{
			draw_dot(i, c.y + p.x, color, rend);
			draw_dot(i, c.y - p.x, color, rend);
		}
		p.y++;
		radiusError += d.y;
		d.y += 2;
		if (((radiusError << 1) + d.x) > 0)
		{
			p.x--;
			radiusError += d.x;
			d.x += 2;
		}
	}
}

void				    draw_thick_line(t_line l, t_screen s, int r, t_screen **screen)
{
	int				len;
	int				i_value;
	double			d;
	int				i;

	if (!clip_line(&l))
		return ;
	get_len(&d, &len, &i, l);
	i_value = len < 0 ? -1 : 1;
	if (!screen || d == 0)
		return ;
	if (i)
	{
		i = -i_value;
		while ((i += i_value) != len)
			draw_circle_fill2((t_vec){ l.p0.x + (int)(i / d), l.p0.y + i }, r, s, screen);
		return ;
	}
	i = -i_value;
	while ((i += i_value) != len)
		draw_circle_fill2((t_vec){ l.p0.x + i, l.p0.y + (int)(i / d) }, r, s, screen);
}

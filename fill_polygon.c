

# include "builder.h"

void				fillpoly(t_vec2d *p, int polyCorners, int **screen, int color)
{
	int  nodes;
	int nodeX[100];
	int pixelX;
	int  pixelY;
	int i;
	int j;
	int swap ;

	int IMAGE_TOP = 0;
	int IMAGE_BOT = WIN_H;
	int IMAGE_RIGHT = WIN_W;
	int IMAGE_LEFT = 0;
//  Loop through the rows of the image.
	for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOT; pixelY++)
	{
		//  Build a list of nodes.
		nodes = 0;
		j = polyCorners - 1;
		for (i = 0; i < polyCorners; i++)
		{
			if ((p[i].y < (double) pixelY && p[j].y >= (double) pixelY)
				||  (p[j].y < (double) pixelY && p[i].y >= (double) pixelY))
			{
				nodeX[nodes++] = (int) (p[i].x + (double)(pixelY - p[i].y) / (double)(p[j].y - p[i].y)
											   * (double)(p[j].x - p[i].x));
			}
			j = i;
		}

		//  Sort the nodes, via a simple “Bubble” sort.
		i=0;
		while (i < nodes - 1)
		{
			if (nodeX[i] > nodeX[i + 1])
			{
				swap = nodeX[i];
				nodeX[i] = nodeX[i + 1];
				nodeX[i + 1] = swap;
				if (i)
					i--;
			}
			else
				i++;

		}
		//  Fill the pixels between node pairs.
		for (i = 0; i < nodes; i += 2)
		{
			if   (nodeX[i  ]>=IMAGE_RIGHT)
				break;
			if   (nodeX[i+1]> IMAGE_LEFT )
			{
				if (nodeX[i  ]< IMAGE_LEFT )
					nodeX[i  ]=IMAGE_LEFT ;
				if (nodeX[i + 1]> IMAGE_RIGHT)
					nodeX[i + 1]=IMAGE_RIGHT;
				for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++)
				{
					if (screen[pixelX][pixelY] == 0)
						screen[pixelX][pixelY] = color;
				}
			}
		}
	}
}
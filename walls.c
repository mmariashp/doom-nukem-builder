
#include "builder.h"

unsigned short          vec_same(t_vec2d one, t_vec2d two)
{
    if (one.x == two.x && one.y == two.y)
        return (TRUE);
    return (FALSE);
}

int                     wall_door(int id, t_wall *walls, int n_walls)
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

void					delete_wall(int id, t_world *world)
{
    t_wall				*new;
    int 				i;
    int 				j;

    if (!world || id < 0 || id >= world->n_walls)
        return ;
    new = (t_wall *)ft_memalloc(sizeof(t_wall) * (world->n_walls - 1));
    if (!new)
        return ;
    i = 0;
    j = 0;
    while (j < world->n_walls)
    {
        if (j == id)
            j++;
        new[i] = world->walls[j];
        i++;
        j++;
    }
    world->n_walls--;
    free(world->walls);
    world->walls = new;
    i = 0;
    while (i < world->n_sec)
    {
        j = 0;
        while (j < world->sec[i].n_walls)
        {
//            if (world->sec[i].sec_walls[j] == id) // need to delete extra secwall
//                world->sec[i].sec_walls[j]--;
            if (world->sec[i].sec_walls[j] > id)
                world->sec[i].sec_walls[j]--;
            j++;
        }
        i++;
    }
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

void                    add_door(t_world *world, int id) // need  to add in secwalls
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
    world->n_walls++;
}

void					render_wall_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_wall *wall)
{
    SDL_Texture         *back;
    SDL_Texture         *title;
    SDL_Rect			rect;
    t_rec				box;
    t_rec				title_box;
    int 				text_color = DARK_GRAY;
    char                *str;
    int                 i = 0;
    int                 n = 3;
    static char         line[3][20] = { "Texture ", "Portal ", "Door " };
    int                 value[3] = { wall->txtr, wall->type, 1 };
    SDL_Texture *yes = load_texture("yes.png", sdl->rend, 0);
    SDL_Texture *no = load_texture("no.png", sdl->rend, 0);

    if (!sdl || !grid || !media)
        return ;
    back = button_back(2, 1, sdl->rend);
    title = button_back(0, 1, sdl->rend);
    box = layout_menu(0, 0);
    title_box = layout_menu(1, 0);

    rect = (SDL_Rect){ box.x, box.y, box.w, box.h };
    SDL_RenderCopy(sdl->rend, back, NULL, &rect);
    rect = (SDL_Rect){ title_box.x, title_box.y, title_box.w, title_box.h };
    SDL_RenderCopy(sdl->rend, title, NULL, &rect);

    str = ft_strjoin("WALL ", ft_itoa(grid->active[1].y));
    write_text(str, sdl->rend, title_box, text_color, FALSE);
    if (str)
        free(str);
    title = button_back(1, 1, sdl->rend);
    title_box = layout_menu(2, i);
    rect = (SDL_Rect){ title_box.x, title_box.y, title_box.w, title_box.h };
    int k;
    k = media->worlds[media->world_id].textures[value[0]];

    while (i < n)
    {
        title_box = layout_menu(2, i);

        if (i != 2 || value[1] == WALL_EMPTY)
        {
            rect.y = title_box.y;
            SDL_RenderCopy(sdl->rend, title, NULL, &rect);
            write_text(line[i], sdl->rend, title_box, text_color, FALSE);
        }
        title_box = layout_menu(5, i);
        if (i == 0 && k >= 0 && k < media->n_txtrs && media->txtrs[k].sdl_t)
        {
           SDL_Rect rect2 = (SDL_Rect){ title_box.x, title_box.y, title_box.h, title_box.h };
           SDL_Rect rect3 = (SDL_Rect){ media->txtrs[k].size.x / 4, media->txtrs[k].size.y / 4,
                                        media->txtrs[k].size.x / 3, media->txtrs[k].size.y / 3 };
           SDL_RenderCopy(sdl->rend, media->txtrs[k].sdl_t, &rect3, &rect2);
        }
        else if (i == 1 || (i == 2 && value[1] == WALL_EMPTY))
        {
            SDL_Rect rect2 = (SDL_Rect){ title_box.x, title_box.y, title_box.h, title_box.h };
            if (value[i] == 1 && no)
                SDL_RenderCopy(sdl->rend, no, NULL, &rect2);
            else if (yes)
                SDL_RenderCopy(sdl->rend, yes, NULL, &rect2);
        }
        i++;
    }
}

//void					render_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
//{
//	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
//		return ;
//	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
//	SDL_RenderClear(sdl->rend);
//	render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
//	render_screen(sdl->rend, prog->screen);
//    if (prog->button_on == -1)
//	{
//		if (grid->active[1].y >= 0 && grid->active[1].y < media->worlds[media->world_id].n_walls)
//    		render_wall_menu(sdl, grid, media, &media->worlds[media->world_id].walls[grid->active[1].y]);
//	}
//	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons);
//	SDL_RenderPresent(sdl->rend);
//	prog->features[F_REDRAW] = 0;
//
//}
//
//unsigned short			update_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
//{
//    static int          door = -1;
//	if (!sdl || !media || !grid)
//		return (FAIL);
//	if (prog->last_mode_id == MODE_EDITOR)
//    {
//        fill_grid_walls(media->worlds[media->world_id].n_walls, media->worlds[media->world_id].walls,
//                media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
//        prog->last_mode_id = prog->mode_id;
//        grid->active[1].y = -1;
//    }
//    if (prog->last_mode_id == MODE_TEXTURES)
//    {
//        if (grid->active[1].x >= 0 && grid->active[1].x < media->n_txtrs)
//        {
//            int texture;
//            if ((texture = texture_in_world(grid->active[1].x, media->worlds[media->world_id])) == -1)
//            {
//                if (add_texture(&media->worlds[media->world_id].textures, media->worlds[media->world_id].n_txtrs, grid->active[1].x) == FAIL)
//                    return (FAIL);
//                texture = media->worlds[media->world_id].n_txtrs;
//                media->worlds[media->world_id].n_txtrs++;
//            }
//            if (grid->active[0].y >= 0 && grid->active[0].y < media->worlds[media->world_id].n_sec)
//            {
//                media->worlds[media->world_id].walls[grid->active[1].y].txtr = texture ;
//            }
//        }
//        prog->last_mode_id = prog->mode_id;
//    }
//	if (prog->button_lit != -1 && (prog->click.x || prog->click.y)) // when pressing an on screen button
//	{
//		if (prog->button_on != -1)
//			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
//		prog->button_on = prog->button_lit;
//		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
//		prog->features[F_REDRAW] = 1;
//		return (SUCCESS);
//	}
//	if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
//	{
//		prog->features[F_REDRAW] = 1;
//	}
//	if (prog->zoom != 0)
//	{
//		zoom_grid(prog, sdl->mouse, grid);
//	}
//    if (prog->move.x || prog->move.y)
//    {
//        move_grid_keys(prog, grid);
//        return (SUCCESS);
//    }
//    if (mouse_over(grid->box, sdl->mouse) && prog->button_on == -1)
//    {
//         grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
//         if (grid->active[0].x >= 0 && grid->active[0].x < GRID_SIZE && grid->active[0].y >= 0 && grid->active[0].y < GRID_SIZE &&
//             grid->nodes[grid->active[0].x][grid->active[0].y] >= 0)
//         {
//             grid->active[1].x = grid->nodes[grid->active[0].x][grid->active[0].y];
//         }
//         else
//             grid->active[1].x = -1;
//        if (prog->click.x || prog->click.y)
//        {
//            if (grid->active[1].x != -1 && grid->active[1].y == -1)
//            {
//                grid->active[1].y = grid->active[1].x;
//                door = wall_door(grid->active[1].y, media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls);
//                int i = W_DESELECT_BUTTON;
//                while ( i < prog->modes[prog->mode_id].n_buttons)
//                    prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = TRUE;
//                i = 0;
//                while ( i < W_DESELECT_BUTTON)
//                    prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = FALSE;
//                if (media->worlds[media->world_id].walls[grid->active[1].y].type == WALL_EMPTY)
//                {
//                    prog->modes[prog->mode_id].buttons[W_DOOR_BUTTON].vis_lit_on[0] = TRUE;
//                    printf("DOOR %d\n", door);
//                }
//                else
//                {
//                    printf("NO DOOR %d\n", door);
//                    prog->modes[prog->mode_id].buttons[W_DOOR_BUTTON].vis_lit_on[0] = FALSE;
//                }
//            }
//        }
//        prog->features[F_REDRAW] = 1;
//        prog->click = (t_vec2d){ 0, 0 };
//    }
//    else if (prog->button_on == W_PORTAL_BUTTON || prog->button_on == W_DOOR_BUTTON)
//    {
//        if (grid->active[1].y >= 0 && grid->active[1].y < media->worlds[media->world_id].n_walls)
//        {
//            if (prog->button_on == W_PORTAL_BUTTON)
//            {
//                printf("changing wall type of wall n %d\n", grid->active[1].y);
//                media->worlds[media->world_id].walls[grid->active[1].y].type = media->worlds[media->world_id].walls[grid->active[1].y].type == WALL_EMPTY ? WALL_FILLED : WALL_EMPTY;
//            }
//            if (prog->button_on == W_DOOR_BUTTON)
//            {
//                if (media->worlds[media->world_id].walls[grid->active[1].y].door == -1)
//                    add_door(&media->worlds[media->world_id], grid->active[1].y);
////                else
////                    delete_door(&media->worlds[media->world_id], grid->active[1].y);
//            }
//        }
//        prog->features[F_REDRAW] = 1;
//        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
//        prog->button_on = -1;
//    }
//    else if (prog->button_on == W_DESELECT_BUTTON && grid->active[1].y != -1) // sector mode
//    {
//        int i = W_DESELECT_BUTTON;
//        while ( i < prog->modes[prog->mode_id].n_buttons)
//            prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = FALSE;
//        i = 0;
//        while ( i < W_DESELECT_BUTTON)
//            prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = TRUE;
//        grid->active[1] = (t_vec2d){ -1, -1 };
//        prog->features[F_REDRAW] = 1;
//
//        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
//        prog->button_on = -1;
//
//        clean_grid(grid);
//        zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
//        fill_grid(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
//        fill_grid_walls(media->worlds[media->world_id].n_walls, media->worlds[media->world_id].walls,
//                        media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
//    }
//	update_sector_status(media->worlds[media->world_id].sec, media->worlds[media->world_id].walls, media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_sec);
//	return (SUCCESS);
//}
//
//int						input_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
//{
//	int					quit;
//	SDL_Event			event;
//
//	quit = FALSE;
//	if (!sdl || !media || !grid)
//		return (TRUE);
//	while(SDL_PollEvent(&event))
//	{
//		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
//		if (event.type == SDL_QUIT)
//		{
//			quit = TRUE;
//			break ;
//		}
//		else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)
//		{
//			if (event.key.keysym.sym == SDLK_ESCAPE)
//			{
//				quit = TRUE;
//				break ;
//			}
//            else if (event.key.keysym.sym == SDLK_UP)
//                prog->move.y -= 7;
//            else if (event.key.keysym.sym == SDLK_DOWN)
//                prog->move.y += 7;
//            else if (event.key.keysym.sym == SDLK_LEFT)
//                prog->move.x -= 7;
//            else if (event.key.keysym.sym == SDLK_RIGHT)
//                prog->move.x += 7;
//		}
//		if(event.type == SDL_MOUSEWHEEL)
//		{
//			if(event.wheel.y > 0) // scroll up
//				prog->zoom++;
//			else if(event.wheel.y < 0) // scroll down
//				prog->zoom--;
//		}
//		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
//		{
//			if(event.type == SDL_MOUSEBUTTONDOWN)
//				prog->click = sdl->mouse;
//			if(event.type == SDL_MOUSEBUTTONUP)
//			{
//				if (prog->button_lit == W_BACK_BUTTON)
//				{
//					prog->last_mode_id = prog->mode_id;
//					prog->mode_id = MODE_EDITOR;
//					prog->modes[prog->mode_id].buttons[prog->button_lit].vis_lit_on[2] = FALSE;
//					prog->button_lit = -1;
//					prog->button_on = -1;
//                    clean_grid(grid);
//					return (quit);
//				}
//                if (prog->button_lit == WT_BUTTON)
//                {
//                    prog->features[F_REDRAW] = 1;
//                    if (prog->button_on >= 0)
//                        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
//                    prog->last_mode_id = prog->mode_id;
//                    prog->mode_id = MODE_TEXTURES;
//                    prog->button_lit = -1;
//                    prog->button_on = -1;
//                    if (grid->active[1].y >= 0 && grid->active[1].y < media->worlds[media->world_id].n_walls)
//                    {
//                        int k;
//                        k = media->worlds[media->world_id].textures[media->worlds[media->world_id].walls[grid->active[1].y].txtr];
//                        if (k >= 0 && k < media->n_txtrs)
//                        {
//                            prog->button_on = k;
//                            prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
//                        }
//                    }
//                    return (quit);
//                }
//                prog->click = (t_vec2d){ 0, 0 };
//			}
//		}
//	}
//	return (quit);
//}
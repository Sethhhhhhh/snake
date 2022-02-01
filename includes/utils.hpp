#ifndef __UTILS_HPP__
#define __UTILS_HPP__

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <iostream>

enum e_move {
	up,
	down,
	left,
	right
};

typedef struct	s_size {
	int	w;
	int	h;
}				t_size;

const int g_fps				= 30;
const int g_frame_rate		= 1000 / g_fps;
const int g_screen_width	= 1000;
const int g_screen_height	= 1000;
const int g_grid_width		= 20;
const int g_grid_height		= 20;
const int g_block			= g_screen_width / g_grid_width;

/* Colors */

const SDL_Color g_white	= {255, 255, 255};
const SDL_Color g_black	= {10, 10, 10, 200};
const SDL_Color g_blue	= {36, 113, 163, 200};

#endif
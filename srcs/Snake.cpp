#include "Snake.hpp"

Snake::Snake(SDL_Renderer* renderer, unsigned int x, unsigned int y) : _renderer(renderer) {
	SDL_Point	head;
	
	// int	size = 25;
	// for (int i = 0; i < size; i++) {
	// 	head.x = x + size - i;
	// 	head.y = y;

	// 	_snake.push_back(head);
	// }

	head.x = x;
	head.y = y;
	_snake.push_back(head);
	
	_move = e_move::right;


	_food = new Food(_renderer);
	set_random_food_pos();
	

	return;
}

Snake::~Snake(void) {
	// delete _food;
	return;
}

float	Snake::get_speed(void) const {
	return _speed;
}

void	Snake::set_random_food_pos(void) {
	unsigned int 	x;
	unsigned int 	y;
	bool			find = false;

	while (!find) {
		find = true;
		x = std::rand() % g_grid_width;
		y = std::rand() % g_grid_height;

		for (std::vector<SDL_Point>::iterator body = _snake.begin() + 1; body != _snake.end(); body++)
			if (body->x == x && body->y == y)
				find = false;
	}

	_food->set_pos(x, y);
}

unsigned int	Snake::get_size(void) const {
	return _size;
}

void    Snake::update(void) {
	static float	x = floorf(_snake.begin()->x);
	static float	y = floorf(_snake.begin()->y);
	SDL_Point		pos;

    switch (_move) {
		case e_move::up:
			y -= _speed;
			break;

		case e_move::down:
			y += _speed;
			break;

		case e_move::left:
			x -= _speed;
			break;

		case e_move::right:
			x = x + _speed;
			break;
	}

	if (x < 0)
		x = g_grid_width - 1;
	else if (x > g_grid_width - 1)
		x = 0;

	if (y < 0)
		y = g_grid_height - 1;
	else if (y > g_grid_height - 1)
		y = 0;


	pos.x = static_cast<int>(x);
	pos.y = static_cast<int>(y);

	if (pos.x != _snake.begin()->x || pos.y != _snake.begin()->y) {
		_last_move = _move;
		_snake.insert(_snake.begin(), pos);
		
		if (pos.x != _food->get_pos().x || pos.y != _food->get_pos().y) {
			_snake.pop_back();
		}
		else {
			_size++;
			set_random_food_pos();
		}
	}

	
}

e_move	Snake::get_move(void) const {
	return _move;
}

void    Snake::move(e_move move) {
    switch (move) {
		case e_move::up:
			if (_last_move != e_move::down) _move = e_move::up;
			break;

		case e_move::down:
			if (_last_move != e_move::up) _move = e_move::down;
			break;

		case e_move::left:
			if (_last_move != e_move::right) _move = e_move::left;
			break;

		case e_move::right:
			if (_last_move != e_move::left) _move = e_move::right;
			break;
		
		default:
			break;
	}
}

void    Snake::render(void) {
	SDL_Rect	block;

	_food->render();

	block.w = g_block - 4;
	block.h = g_block - 4;
	block.x = _snake.begin()->x * g_block + 2;
	block.y = _snake.begin()->y * g_block + 2;

	SDL_SetRenderDrawColor(_renderer, 21, 67, 96, 255);
	SDL_RenderFillRect(_renderer, &block);

	SDL_SetRenderDrawColor(_renderer, 74, 35, 90, 255);
	SDL_RenderDrawRect(_renderer, &block);
		
	unsigned int i = 0;
	for (std::vector<SDL_Point>::iterator body = _snake.begin() + 1; body != _snake.end(); body++, i++) {
		block.x = body->x * g_block + 2;
		block.y = body->y * g_block + 2;

		if (i % 2) SDL_SetRenderDrawColor(_renderer, 41, 128, 185, 255);
		else SDL_SetRenderDrawColor(_renderer, 52, 152, 219, 255);
		SDL_RenderFillRect(_renderer, &block);
	}	
}

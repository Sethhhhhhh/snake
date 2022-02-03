#include "Game.hpp"

Game::Game(void) {	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Unable to initialize SDL." << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	if (TTF_Init() < 0) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	_window = SDL_CreateWindow("Snake Game - FPS: 0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_screen_width, g_screen_height, SDL_WINDOW_SHOWN);;
	if (!_window) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	std::srand(time(NULL));

	add_font("JetBrainsMono-Medium", 16);
	add_font("JetBrainsMono-Medium", 20);
	add_font("JetBrainsMono-Medium", 24);
	add_font("JetBrainsMono-Medium", 40);

	_snake = new Snake(_renderer);
	
	this->update();


	return;
}

Game::~Game(void) {

	for (std::map<std::string, TTF_Font*>::iterator font = _fonts.begin(); font != _fonts.end(); font++)
		TTF_CloseFont(font->second);
	TTF_Quit();

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);

	SDL_Quit();

	delete _snake;

	return;
}


void	Game::add_font(std::string name, unsigned int size) {
	std::string	index = name + std::to_string(size);

	_fonts[index] = TTF_OpenFont(std::string("resources/" + name + ".ttf").c_str(), size);
	if (!_fonts[index]) {
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	return;
}

TTF_Font*	Game::get_font(std::string name, unsigned int size) {
	std::string	index = name + std::to_string(size);

	if (!_fonts[index]) {
		std::cout << "Unknown font: " << name << std::endl;
		exit(EXIT_FAILURE);
	}

	return _fonts[index];
}


void    Game::update(void) {
	Uint32	after;
	Uint32 	before = SDL_GetTicks();
	Uint32	second = after;
    int 	frame_time = 0;
	int		frames = 0;

    while (_is_running) {
        before = SDL_GetTicks();

		handle_events();

		if (_pause) {
			draw_pause();
			SDL_RenderPresent(_renderer);
			continue;
		}

        render();
		_snake->update();

        frames++;
        after = SDL_GetTicks();

        if (after - second >= 1000) {
            _fps = frames;
            frames = 0;
            second = after;
            
			std::string title = "Snake Game - FPS: " + std::to_string(_fps);
			SDL_SetWindowTitle(_window, title.c_str());
        }

        frame_time = after - before;
        if (g_frame_rate > frame_time)
            SDL_Delay(g_frame_rate - frame_time);
    }

	return;
}

void    Game::handle_events(void) {
	SDL_Event   event;
	
	while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            _is_running = false;
		else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
					_is_running = false;
					break;

				case SDLK_w:
					_snake->move(e_move::up);
					break;

				case SDLK_s:
					_snake->move(e_move::down);
					break;

				case SDLK_a:
					_snake->move(e_move::left);
					break;

				case SDLK_d:
					_snake->move(e_move::right);
					break;
				
				case SDLK_p:
					_pause = !_pause;
					break;

				case SDLK_r:
					if (_snake->is_dead())
						_snake->init();
					break;
            }
        }
    }

	return;
}

void	Game::draw_input(void) {
	e_move snake_move = _snake->get_move();

	Text left = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 20),
		"A",
		g_white,
		snake_move == e_move::left ? g_blue : g_black,
		5
	);
	left.set_pos(5, g_screen_height - left.get_height() - 5);
	left.draw();

	Text down = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 20),
		"S",
		g_white,
		snake_move == e_move::down ? g_blue : g_black,
		5
	);
	down.set_pos(left.get_x() + left.get_width() + 5, g_screen_height - down.get_height() - 5);
	down.draw();

	Text right = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 20),
		"D",
		g_white,
		snake_move == e_move::right ? g_blue : g_black,
		5
	);
	right.set_pos(down.get_x() + down.get_width() + 5, g_screen_height - down.get_height() - 5);
	right.draw();

	Text up = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 20),
		"W",
		g_white,
		snake_move == e_move::up ? g_blue : g_black,
		5
	);
	up.set_pos(left.get_x() + left.get_width() + 5, down.get_y() - up.get_height() - 5);
	up.draw();
}

void	Game::draw_pause(void) {
	Text pause = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 40),
		"PAUSE",
		g_white,
		g_black,
		5
	);
	pause.set_pos(g_screen_width / 2 - pause.get_width() / 2, g_screen_height / 2 - pause.get_height() / 2);
	pause.draw();
}

void    Game::render(void) {
	SDL_Rect    rect;

	/* Background color */
	SDL_SetRenderDrawColor(_renderer, 25, 25, 25, 255);
	SDL_RenderClear(_renderer);

	rect.w = g_screen_width / g_grid_width;
	rect.h = g_screen_height / g_grid_height;
	
	SDL_SetRenderDrawColor(_renderer, 30, 30, 30, 255);
	for (unsigned int x = 0; x < g_grid_width; x++) {
		for (unsigned int y = 0; y < g_grid_height; y++) {
			if ((y + x % 2) % 2 == 0) continue;
			rect.x = x * rect.w;
			rect.y = y * rect.h;
			
			SDL_RenderFillRect(_renderer, &rect);
		}
	}

	_snake->render();

	Text fps = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 16),
		std::string("FPS: " + std::to_string(_fps)).c_str(),
		g_white,
		g_black,
		5
	);
	fps.set_pos(g_screen_width - fps.get_width(), 0);
	fps.draw();

	Text score = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 24),
		std::string("SCORE: " + std::to_string((_snake->get_size() - 1) * 50)).c_str(),
		g_white,
		g_black,
		5
	);
	score.set_pos(g_screen_width / 2 - score.get_width() / 2, 0);
	score.draw();

	Text speed = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 16),
		std::string("SPEED: " + std::to_string(_snake->get_speed())).c_str(),
		g_white,
		g_black,
		5
	);
	speed.set_pos(0, 0);
	speed.draw();

	Text size = Text(
		_renderer,
		get_font("JetBrainsMono-Medium", 16),
		std::string("SIZE: " + std::to_string(_snake->get_size())).c_str(),
		g_white,
		g_black,
		5
	);
	size.set_pos(0, speed.get_height());
	size.draw();
	
	draw_input();

	if (_snake->is_dead()) {
		Text dead = Text(
			_renderer,
			get_font("JetBrainsMono-Medium", 40),
			"DEFEAT",
			g_white,
			g_black,
			5
		);
		dead.set_pos(g_screen_width / 2 - dead.get_width() / 2, g_screen_height / 2 - dead.get_height() / 2);
		dead.draw();


		Text restart = Text(
			_renderer,
			get_font("JetBrainsMono-Medium", 16),
			"Press 'R' to restart the game.",
			g_white,
			g_black,
			5
		);
		restart.set_pos(g_screen_width / 2 - restart.get_width() / 2, g_screen_height / 2 + dead.get_height());
		restart.draw();
	}

	SDL_RenderPresent(_renderer);

	return;
}
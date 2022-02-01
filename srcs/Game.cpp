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
	add_font("JetBrainsMono-Medium", 24);

	_snake = new Snake(_renderer, 0, 0);
	
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
            }
        }
    }

	return;
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

	SDL_RenderPresent(_renderer);

	return;
}
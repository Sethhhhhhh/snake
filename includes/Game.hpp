#ifndef __GAME_HPP__
#define __GAME_HPP__

# include "utils.hpp"
# include "Snake.hpp"
# include "Text.hpp"

class Game {

    private:
        Snake*                              _snake;
        SDL_Window*                         _window;
        SDL_Renderer*                       _renderer;
        std::map<std::string, TTF_Font*>    _fonts;
        
        bool                                _pause = false;
        bool                                _is_running = true;
        unsigned int                        _fps = 0;
        unsigned int                        _score = 3000;

        void    draw_input(void);
        void    draw_pause(void);
    
    public:
        Game(void);
        ~Game(void);

        void        handle_events(void);
        void        update(void);
        void        render(void);
        void        add_font(std::string name, unsigned int size);
        TTF_Font*	get_font(std::string name, unsigned int size);
};

#endif
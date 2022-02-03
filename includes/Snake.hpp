#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

# include "utils.hpp"
# include <vector>
# include <map>
# include <cstdlib>
# include "Food.hpp"

class Snake {

    public:
        Snake(SDL_Renderer* renderer);
        ~Snake(void);

        void            init(void);
        void            render(void);
        void            move(e_move move);
        void            update(void);
        float           get_speed(void) const;
        void            set_random_food_pos(void);
        unsigned int    get_size(void) const;
        e_move          get_move(void) const;
        bool            is_dead(void) const;


    private:
        Food*                       _food;
        std::vector<SDL_Point>      _snake;
        SDL_Renderer*               _renderer;
        e_move                      _move, _last_move;
        float                       _speed = 0.25f;
        unsigned int                _size = 1;

        bool                        _is_dead = false;
};

#endif
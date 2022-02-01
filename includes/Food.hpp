#ifndef __FOOD_HPP__
#define __FOOD_HPP__

# include "utils.hpp"

class Food {

    private:
        SDL_Renderer*   _renderer;
        SDL_Point       _pos;
        SDL_Color       _color;

    public:
        Food(SDL_Renderer *renderer) : _renderer(renderer) {
            _pos.x = -1;
            _pos.y = -1;

            
        };
        ~Food(void) {};

        void    set_pos(unsigned int x, unsigned int y) {
            _pos.x = x;
            _pos.y = y;

            _color = {
                static_cast<Uint8>(std::rand() % 255), 
                static_cast<Uint8>(std::rand() % 255), 
                static_cast<Uint8>(std::rand() % 255),
                255
            };
        };

        SDL_Point    get_pos(void) const {
            return (_pos);
        };

        void    render(void) {
            SDL_Rect    food;

            food.x = _pos.x * g_block + 4;
            food.y = _pos.y * g_block + 4;
            food.w = g_block - 8;
            food.h = g_block - 8;

            SDL_SetRenderDrawColor(_renderer, 39, 174, 96, 255);
            SDL_RenderFillRect(_renderer, &food);

            SDL_SetRenderDrawColor(_renderer, 20, 90, 50, 255);
            SDL_RenderDrawRect(_renderer, &food);
        };
};

#endif
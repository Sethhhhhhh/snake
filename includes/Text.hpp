#ifndef __TEXT_HPP__
#define __TEXT_HPP__

# include "utils.hpp"

class Text {

    private:
        SDL_Renderer*   _renderer;
        SDL_Rect        _text;
        SDL_Rect        _bg;
        SDL_Surface*    _surface;
        SDL_Texture*    _texture;
        unsigned int    _border_size;
    
    public:
        Text(SDL_Renderer* renderer, TTF_Font* font, const char *text, SDL_Color color, SDL_Color bg, unsigned int border_size) : _renderer(renderer), _border_size(border_size) {
            _surface = TTF_RenderText_Blended(font, text, color);
	        _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
        
            _text.w = _surface->w;
            _text.h = _surface->h;
            _text.x = _border_size * 2;
            _text.y = _border_size;

            _bg.w = _text.w + _border_size * 4;
            _bg.h = _text.h + _border_size * 2;
            _bg.x = 0;
            _bg.y = 0;

            SDL_SetRenderDrawColor(_renderer, bg.b, bg.g, bg.r, bg.a);
            SDL_FreeSurface(_surface);
        };

        void    set_pos(int x, int y) {
            _text.x = x + _border_size * 2;
            _text.y = y + _border_size;
            _bg.x = x;
            _bg.y = y;
        };

        int get_width(void) const {
            return _bg.w;
        }

        int get_height(void) const {
            return _bg.h;
        }

        int get_x(void) const {
            return _bg.x;
        }

        int get_y(void) const {
            return _bg.y;
        }

        void    draw(void) {
            
            SDL_RenderFillRect(_renderer, &_bg);
            SDL_RenderCopy(_renderer, _texture, NULL, &_text);

        };

        ~Text(void) {
	        SDL_DestroyTexture(_texture);
        };


};

#endif
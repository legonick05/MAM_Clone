#ifndef SCREEN_H_
#define SCREEN_H_

#include <iostream>
#include <SDL.h>

/* A basic "Screen" class, which initializes SDL, a window, a
 * renderer, a texture, and methods which shorten the code needed
 * in a main() to handle events, coloring, and quitting.
 */

namespace scr{

class Toolbar
{
private:
    const char *title;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;
    Uint32 *_pixels;
    
public:
    const static int TOOL_WIDTH = 200;
    const static int TOOL_HEIGHT = 540;

    Toolbar();
    bool init(int x_pos, int y_pos);
    unsigned char processEvents(SDL_Event &ev);
    void update();
    void close();
};

class Screen
{
private:
    const char *title;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;
    Uint32 *_pixels;
    Uint32 *_pixels2; // Second buffer for blurring effects
    
public:
    const static int SCR_WIDTH = 1280;//960;
    const static int SCR_HEIGHT = 720;//540;
    Toolbar toolbar;

    Screen(const char *title);
    bool init();
    unsigned char processEvents(SDL_Event &ev, Sint32 &xShift);
    void setPixel(int xpx, int ypx, Uint8 red, Uint8 green, Uint8 blue, bool avg);
    void boxBlur(int adjuster);
    void swap();
    void update();
    void clear();
    void close();
};
}

#endif /* SCREEN_H_ */
#include "Screen.h"
using namespace std;

namespace scr{
Toolbar::Toolbar() :
    _window(NULL), _renderer(NULL), _texture(NULL), _pixels(NULL), title(" ")
{
}

bool Toolbar::init(int x_pos, int y_pos)
{
    _window = SDL_CreateWindow(title, x_pos, y_pos, TOOL_WIDTH, 
                TOOL_HEIGHT, SDL_WINDOW_SHOWN);

    if(_window == NULL)
    {
        SDL_Quit(); // Will quit if the window doesn't pop up.
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC, TOOL_WIDTH, TOOL_HEIGHT);

    if(_renderer == NULL)
    {
        cout << "Renderer creation failed." << endl;
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return false;    
    }
    if(_texture == NULL)
    {
        cout << "Texture creation failed." << endl;
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return false;    
    }

    _pixels = new Uint32[TOOL_WIDTH * TOOL_HEIGHT];

    return true;
}

unsigned char Toolbar::processEvents(SDL_Event &ev)
{
    SDL_Event event = ev;

    return 0;
}

void Toolbar::update()
{
    SDL_UpdateTexture(_texture, NULL, _pixels, TOOL_WIDTH*sizeof(Uint32));
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
    SDL_RenderPresent(_renderer);
}

void Toolbar::close()
{
    delete [] _pixels;
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyTexture(_texture);
    SDL_DestroyWindow(_window);
}

Screen::Screen(const char *title) :
    _window(NULL), _renderer(NULL), _texture(NULL), _pixels(NULL), _pixels2(NULL)
{
    this->title = title;   
}

bool Screen::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL initialization failed." << endl;
        return false; // Will quit if SDL fails to load properly.
    }

    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, 
                SDL_WINDOW_SHOWN);

    if(_window == NULL)
    {
        SDL_Quit(); // Will quit if the window doesn't pop up.
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC, SCR_WIDTH, SCR_HEIGHT);

    if(_renderer == NULL)
    {
        cout << "Renderer creation failed." << endl;
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return false;    
    }
    if(_texture == NULL)
    {
        cout << "Texture creation failed." << endl;
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return false;    
    }

    _pixels = new Uint32[SCR_WIDTH * SCR_HEIGHT];
    _pixels2 = new Uint32[SCR_WIDTH * SCR_HEIGHT];

    int x = 0;
    int y = 0;

    SDL_GetWindowPosition(_window, &x, &y);

    toolbar.init(x - 250, y);

    return true;
}

unsigned char Screen::processEvents(SDL_Event &ev, Sint32 &xShift)
{
    SDL_Event event = ev;

    while(SDL_PollEvent(&event))
        {   if(event.type == SDL_QUIT)
            {
                return 1;
            } else if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_SPACE)
                {
                    return 2;
                }
            } else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                return 3;
            } else if(event.type == SDL_MOUSEBUTTONUP)
            {
                return 4;
            } else if(event.type == SDL_MOUSEMOTION)
            {    
                xShift = event.motion.xrel;
                return 5;
            }
        }

    return 0;
}

void Screen::setPixel(int xpx, int ypx, Uint8 red, Uint8 green, Uint8 blue, bool avg)
{
    Uint32 color = 0x00000000;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    if(xpx < 0 || xpx >= SCR_WIDTH || ypx < 0 || ypx >= SCR_HEIGHT)
        return;

    if(avg)
    {
        int color2 = _pixels[(SCR_WIDTH * ypx) + xpx];
    
        if(color2 != 0)
            color = ((int)color + color2)/2;
        else
            _pixels[(SCR_WIDTH * ypx) + xpx] = color;
    }
    _pixels[(SCR_WIDTH * ypx) + xpx] = color;
}

void Screen::boxBlur(int adjuster)
{
    swap();

    Uint8 AvRed;
    Uint8 AvGreen;
    Uint8 AvBlue;

    for(int x = 0; x < SCR_WIDTH; x++)
    {   for(int y = 0; y < SCR_WIDTH; y++)
        {   
            int red = 0;
            int green = 0;
            int blue = 0;

            for(int col = -1; col <= 1; col++)
            {   for(int row = -1; row <= 1; row++)
                {
                    int currentX = x + col;
                    int currentY = y + row;
                    if(currentX >= 0 && currentX < SCR_WIDTH && currentY >= 0 && currentY < SCR_HEIGHT)
                    {
                        Uint32 color = _pixels2[currentY*SCR_WIDTH + currentX];
                        
                        red += ((color & 0xFF000000) >> 24) + adjuster;
                        green += ((color & 0x00FF0000) >> 16) + adjuster;
                        blue += ((color & 0x0000FF00) >> 8) + adjuster;
                    }
                }
            }

            AvRed = red/9;
            AvGreen = green/9;
            AvBlue = blue/9;

            setPixel(x, y, AvRed, AvGreen, AvBlue, false);
        }
    }
}

void Screen::swap()
{
    Uint32 *temp = _pixels;
    _pixels = _pixels2;
    _pixels2 = temp;
}

void Screen::update()
{
    SDL_UpdateTexture(_texture, NULL, _pixels, SCR_WIDTH*sizeof(Uint32));
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
    SDL_RenderPresent(_renderer);
}

void Screen::clear()
{
    memset(_pixels, 0, SCR_WIDTH*SCR_HEIGHT*sizeof(Uint32));
    memset(_pixels2, 0, SCR_WIDTH*SCR_HEIGHT*sizeof(Uint32));
}

void Screen::close()
{
    toolbar.close();
    delete [] _pixels;
    delete [] _pixels2;
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyTexture(_texture);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
}
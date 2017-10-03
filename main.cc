#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Screen.h"
#include "Notes.h"
 using namespace std;
 using namespace scr;
 using namespace pcl;

/* A clone of the Music Animation Machine software in SDL, which
 * uses the classes in "Notes.h". It reads a midi file using
 * the Midifile library, then animates the notes by color and
 * tempo.
 *
 * TODO: 
 * - Fix tempo change issue - check
 * - Implement file chooser
 * - Add shape textures/codes - check
 * - Add more color codes
 */

Uint8 *getColorFromCode(int colorCode, bool colorScheme);
bool getShape(int shapeCode, int x_start, int y_start, int width, int height, int x_pos, int y_pos);
bool getShapeOutline(int shapeCode, int x_start, int y_start, int width, int height, int x_pos, int y_pos);

int main()
{
    srand(time(NULL));

    Screen screen = Screen("MAM Clone");

    if(!screen.init())
    {
        cout << "Error initializing." << endl;
        return 1;
    }

    Piece piece;

    piece.load("/Users/nick/Documents/Nick/C/C++/Basics/SDL/MAM_Clone/test4.mid");

    screen.clear();

    for(int yy = 0; yy < Screen::SCR_HEIGHT; yy++)
    {
        screen.setPixel(Screen::SCR_WIDTH/2, yy, 255, 255, 255, false);
    }

    int x;
    int y;
    int width;
    int globalTempo;
    Uint8 *pColors = getColorFromCode(0, true);
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Sint32 xShift;

    bool quit = false;
    bool playFile = false;
    bool mouseDown = false;
    bool iterateTempo = true;
    bool nowLine = true;
    bool colorScheme = false; // "true" for color by track, "false" for color by pitch.
    char tonic = 3; // C = 0, C#/Db = 1, etc.

    Note* pNotes = piece.getNotes();
    globalTempo = pNotes[0]._tempo;

    while(!quit)
    {
        // TODO:
        // Fix note length problems
        // Implement tempo improvement
        // Finish "toolbar"

        Note _note;

        if(playFile) // Update and draw particles
        {
            screen.clear();
            
            if(nowLine)
            {            
                for(int yy = 0; yy < Screen::SCR_HEIGHT; yy++)
                {
                    screen.setPixel(Screen::SCR_WIDTH/2, yy, 255, 255, 255, false);
                }
            }

            for(int ii = 0; ii < piece.getNumNotes(); ii++) //Draw the note interior
            {
                _note = pNotes[ii];

                pNotes[ii].render = false;

                x = Screen::SCR_WIDTH + round(_note.x_pos/9.0);//12;
                y = -(_note.y_pos - 63) * 14 + Screen::SCR_HEIGHT/2;
                width = Screen::SCR_WIDTH + round((_note.x_pos + _note._duration)/9.0) - x - 1;                

                if(colorScheme)
                {
                    pColors = getColorFromCode(_note._color, true);
        
                    red = pColors[0];
                    green = pColors[1];
                    blue = pColors[2];
                } else
                {
                    pColors = getColorFromCode(((_note.y_pos - 60 - tonic)%12 + 12)%12, false);
        
                    red = pColors[0];
                    green = pColors[1];
                    blue = pColors[2];
                }

//                for(int jj = 0; (double)jj < _note._duration/10.0; jj++) //duration/(20.0/1.5)
//                {
//                    width++;
//                }
                
                if((x <= Screen::SCR_WIDTH/2) && (x >= Screen::SCR_WIDTH/2 - width))//(x >= Screen::SCR_WIDTH/2 - _note._duration/10.0))
                {
//                    red = ((int)red + 0xFF)/2;
//                    green = ((int)green + 0xFF)/2;
//                    blue = ((int)blue + 0xFF)/2;
                    red = 0xFF;
                    green = 0xFF;
                    blue = 0xFF;
                    globalTempo = _note._tempo;
                }
                if((x < Screen::SCR_WIDTH) && (x > -width))//(x > -_note._duration/10.0))
                {
                    pNotes[ii].render = true;
                    for(int jj = 0; jj < width; jj++)//(double)jj < _note._duration/10.0; jj++)
                    {   for(int kk = 0; kk < Note::HEIGHT; kk++)
                        {
                            if(getShape(_note._color, x, y, width, Note::HEIGHT, x + jj, y + kk))
                                screen.setPixel(x + jj, y + kk, red, green, blue, false);
                        }
                    }
//                    for(int jj = -1; (double)jj < _note._duration/10.0; jj++)
//                    {   for(int kk = -1; kk < Note::HEIGHT; kk++)
//                        {
////                            if(getShape(_note._color, x, y, width, Note::HEIGHT, x + jj, y + kk))
////                                screen.setPixel(x + jj, y + kk, red, green, blue, false);
//                            if(getShapeOutline(_note._color, x-1, y-1, width+1, Note::HEIGHT+1, x + jj, y + kk))
//                                screen.setPixel(x + jj, y + kk, 180, 180, 180, false);
//                        }
//                    }
                }
            }
            for(int ii = 0; ii < piece.getNumNotes(); ii++) // Draw the note border
            {
                if(pNotes[ii].render)
                {
                    _note = pNotes[ii];

                    x = Screen::SCR_WIDTH + round(_note.x_pos/9.0);//12;
                    y = -(_note.y_pos - 63) * 14 + Screen::SCR_HEIGHT/2;
                    width = Screen::SCR_WIDTH + round((_note.x_pos + _note._duration)/9.0) - x - 1;

//                    for(int jj = 0; (double)jj < _note._duration/10.0; jj++) //duration/(20.0/1.5)
//                    {
//                        width++;
//                    }
                    
                    for(int jj = 0; jj < width; jj++)//(double)jj < _note._duration/10.0; jj++)
                    {   for(int kk = 0; kk < Note::HEIGHT; kk++)
                        {
//                            if(getShape(_note._color, x, y, width, Note::HEIGHT, x + jj, y + kk))
//                                screen.setPixel(x + jj, y + kk, red, green, blue, false);
                            if(getShapeOutline(_note._color, x, y, width, Note::HEIGHT, x + jj, y + kk))
                                screen.setPixel(x + jj, y + kk, 100, 100, 100, false);
                        }
                    }
                }
            }
            if(iterateTempo)
                piece.update(globalTempo);
        }

        SDL_Event ev;

        switch(screen.processEvents(ev, xShift)) // Check for messages/events
        {
        case 1: // Close
            quit = true;
            break;
        case 2: // Spacebar
            playFile = !playFile;
            iterateTempo = true;
            break;
        case 3: // Mouse down
            mouseDown = true;
            switch(screen.toolbar.processEvents(ev))
            {
            default:;
            }
            playFile = false;
            break;
        case 4: // Mouse up
            mouseDown = false;
            playFile = false;
            iterateTempo = true;
            break;
        case 5: // Mouse motion
            if(mouseDown)
            {
                piece.shift((int)xShift);
                playFile = true;
                iterateTempo = false;
            }
            break;        
        default:;
        }

        screen.update();
    }

    screen.close();
    return 0;
}

Uint8 *getColorFromCode(int colorCode, bool colorScheme)
{
    Uint8 Colors[3];
    Uint8 *pColors = Colors;

    if(colorScheme)
    {    
        switch(colorCode)
        {
        case 0:
            pColors[0] = 0xFF;
            pColors[1] = 0x00;
            pColors[2] = 0x00;
            break;
        case 1:
            pColors[0] = 0xFF;
            pColors[1] = 0xCC;
            pColors[2] = 0x00;
            break;
        case 2:
            pColors[0] = 0x00;
            pColors[1] = 0xAA;
            pColors[2] = 0x00;
            break;
        case 3:
            pColors[0] = 0x00;
            pColors[1] = 0x00;
            pColors[2] = 0xFF;
            break;
        default:
            pColors[0] = 0xFF;
            pColors[1] = 0xFF;
            pColors[2] = 0xFF;
        }
    } else
    {
        switch(colorCode)
        {
        case 0:
            pColors[0] = 0x00;
            pColors[1] = 0x00;
            pColors[2] = 0xDC;
//            pColors[0] = 0x00;
//            pColors[1] = 0x00;
//            pColors[2] = 0xDC;
            break;
        case 1:
            pColors[0] = 0x6E;
            pColors[1] = 0xDC;
            pColors[2] = 0x00;
//            pColors[0] = 0x6E;
//            pColors[1] = 0x00;
//            pColors[2] = 0xDC;
            break;
        case 2:
            pColors[0] = 0xDC;
            pColors[1] = 0x00;
            pColors[2] = 0xDC;
//            pColors[0] = 0xDC;
//            pColors[1] = 0x00;
//            pColors[2] = 0xDC;
            break;
        case 3:
            pColors[0] = 0x00;
            pColors[1] = 0xDC;
            pColors[2] = 0x6E;
//            pColors[0] = 0xDC;
//            pColors[1] = 0x00;
//            pColors[2] = 0x6E;
            break;
        case 4:
            pColors[0] = 0xDC;
            pColors[1] = 0x00;
            pColors[2] = 0x00;
//            pColors[0] = 0xDC;
//            pColors[1] = 0x00;
//            pColors[2] = 0x00;
            break;
        case 5:
            pColors[0] = 0x00;
            pColors[1] = 0x6E;
            pColors[2] = 0xDC;
//            pColors[0] = 0xDC;
//            pColors[1] = 0x6E;
//            pColors[2] = 0x00;
            break;
        case 6:
            pColors[0] = 0xDC;
            pColors[1] = 0xDC;
            pColors[2] = 0x00;
//            pColors[0] = 0xDC;
//            pColors[1] = 0xDC;
//            pColors[2] = 0x00;
            break;
        case 7:
            pColors[0] = 0x6E;
            pColors[1] = 0x00;
            pColors[2] = 0xDC;
//            pColors[0] = 0x6E;
//            pColors[1] = 0xDC;
//            pColors[2] = 0x00;
            break;
        case 8:
            pColors[0] = 0x00;
            pColors[1] = 0xDC;
            pColors[2] = 0x00;
//            pColors[0] = 0x00;
//            pColors[1] = 0xDC;
//            pColors[2] = 0x00;
            break;
        case 9:
            pColors[0] = 0xDC;
            pColors[1] = 0x00;
            pColors[2] = 0x6E;
//            pColors[0] = 0x00;
//            pColors[1] = 0xDC;
//            pColors[2] = 0x6E;
            break;
        case 10:
            pColors[0] = 0x00;
            pColors[1] = 0xDC;
            pColors[2] = 0xDC;
//            pColors[0] = 0x00;
//            pColors[1] = 0xDC;
//            pColors[2] = 0xDC;
            break;
        case 11:
            pColors[0] = 0xDC;
            pColors[1] = 0x6E;
            pColors[2] = 0x00;
//            pColors[0] = 0x00;
//            pColors[1] = 0x6E;
//            pColors[2] = 0xDC;
        }
    }

    return pColors;
}

bool getShape(int shapeCode, int x_start, int y_start, int width, int height, int x_pos, int y_pos)
{
    int xprime = x_pos - x_start;
    int yprime = y_pos - y_start;

    switch(shapeCode)
    {
    case 0: // Basic rectangle
        return true;
    case 1: // Rhombus
        if(xprime <= width/2.0)
        {
            if((((double)height/width)* xprime + yprime >= height/2.0) && (((double)-height/width)* xprime + yprime <= height/2.0))
                return true;       
            else
                return false;
        } else if(xprime > width/2.0)
        {
            if((((double)height/width)* xprime + yprime <= 3.0 * height/2.0) && (((double)-height/width)* xprime + yprime >= -height/2.0))
                return true;       
            else
                return false;
        }
    case 2: // Inverted ellipse
        if((xprime <= width/2.0) && (yprime <= height/2.0))
        {
            if(((double)xprime * xprime)/((double)width * width/4.0) + ((double)yprime * yprime)/((double)height * height/4.0) >= 1.0)
                return true;
            else
                return false;
        } else if((xprime <= width/2.0) && (yprime >= height/2.0))
        {
            int yprime2 = yprime - height;
            if(((double)xprime * xprime)/((double)width * width/4.0) + ((double)yprime2 * yprime2)/((double)height * height/4.0) >= 1.0)
                return true;
            else
                return false;
        } else if((xprime >= width/2.0) && (yprime <= height/2.0))
        {
            int xprime2 = xprime - width;
            if(((double)xprime2 * xprime2)/((double)width * width/4.0) + ((double)yprime * yprime)/((double)height * height/4.0) >= 1.0)
                return true;
            else
                return false;
        } else if((xprime >= width/2.0) && (yprime >= height/2.0))
        {
            int xprime2 = xprime - width;
            int yprime2 = yprime - height;
            if(((double)xprime2 * xprime2)/((double)width * width/4.0) + ((double)yprime2 * yprime2)/((double)height * height/4.0) >= 1.0)
                return true;
            else
                return false;
        }
    case 3:; // Regular Ellipse
        double xprime2 = xprime - (width/2.0);
        double yprime2 = yprime - (height/2.0);
        if((xprime2 * xprime2)/(width * width/4.0) + (yprime2 * yprime2)/(height * height/4.0) <= 1.0)
            return true;
        else
            return false;
    }

    return true;
}

bool getShapeOutline(int shapeCode, int x_start, int y_start, int width, int height, int x_pos, int y_pos)
{
    int xprime = x_pos - x_start;
    int yprime = y_pos - y_start;

    switch(shapeCode)
    {
    case 0: // Rectangle
        if(((xprime == 0) || (xprime == 1) || (xprime == width - 2) || (xprime == width - 1))
             || ((yprime == 0) || (yprime == 1) || (yprime == height - 2) || (yprime == height - 1)))
            return true;
        else
            return false;
    case 1: // Rhombus
        if(xprime <= width/2.0)
        {
            if((fabs(((double)height/width)* xprime + yprime - height/2.0) <= 1) || (fabs(((double)-height/width)* xprime + yprime - height/2.0) <= 1))
                return true;       
            else
                return false;
        } else if(xprime > width/2.0)
        {
            if((fabs(((double)height/width)* xprime + yprime - (3.0 * height/2.0)) <= 1) || (fabs(((double)-height/width)* xprime + yprime + height/2.0) <= 1))
                return true;       
            else
                return false;
        }
    case 2: // Inverted ellipse
        if((xprime <= width/2.0) && (yprime <= height/2.0))
        {
            if(fabs(((double)xprime * xprime)/((double)width * width/4.0) + ((double)yprime * yprime)/((double)height * height/4.0) - 1.0) <= 0.2)
                return true;
            else
                return false;
        } else if((xprime <= width/2.0) && (yprime >= height/2.0))
        {
            int yprime2 = yprime - height;
            if(fabs(((double)xprime * xprime)/((double)width * width/4.0) + ((double)yprime2 * yprime2)/((double)height * height/4.0) - 1.0) <= 0.2)
                return true;
            else
                return false;
        } else if((xprime >= width/2.0) && (yprime <= height/2.0))
        {
            int xprime2 = xprime - width;
            if(fabs(((double)xprime2 * xprime2)/((double)width * width/4.0) + ((double)yprime * yprime)/((double)height * height/4.0) - 1.0) <= 0.2)
                return true;
            else
                return false;
        } else if((xprime >= width/2.0) && (yprime >= height/2.0))
        {
            int xprime2 = xprime - width;
            int yprime2 = yprime - height;
            if(fabs(((double)xprime2 * xprime2)/((double)width * width/4.0) + ((double)yprime2 * yprime2)/((double)height * height/4.0) - 1.0) <= 0.2)
                return true;
            else
                return false;
        }
    case 3:; // Regular Ellipse
        double xprime2 = xprime - (width/2.0);
        double yprime2 = yprime - (height/2.0);
        if(fabs((xprime2 * xprime2)/(width * width/4.0) + (yprime2 * yprime2)/(height * height/4.0) - 1.0) <= 0.25)
            return true;
        else
            return false;
    }


    return true;
}

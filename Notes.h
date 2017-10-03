#ifndef NOTES_H_
#define NOTES_H_

#include <math.h>
#include <MidiFile.h>
#include "Screen.h"
 using namespace scr;

namespace pcl {

struct Note
{
public:
    const static int HEIGHT = 14; // Note height in pixels.
    int _tempo; // Speed in ticks per quarter note
    unsigned char _color;
    int _duration; // Note width in ticks
    int x_pos; // Time in ticks.
    int y_pos; // Pitch code.
    bool render;

    Note();
    void init(double tempo, double time, double pitch, double duration, unsigned char color);
    void shift(int xShift);
    void update(int tempo);
};

class Piece
{
private:
    Note *pNotes;
    int N_NOTES;
public:
    Piece();
    ~Piece();
    Note* getNotes();
    int getNumNotes();
    void shift(int xShift);
    void update(int globalTempo);
    void load(const char *filename);
};
}

#endif /* NOTES_H_ */
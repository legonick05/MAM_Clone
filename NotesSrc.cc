#include "Notes.h"
#include <stdlib.h>
#include <math.h>

namespace pcl {

Note::Note() :
    _tempo(0), _color(0), _duration(0), x_pos(0), y_pos(0), render(false)
{
}

void Note::init(double tempo, double time, double pitch, double duration, unsigned char color)
{
    _tempo = tempo;
    x_pos = time;
    y_pos = pitch;
    _duration = duration;
    _color = color;
}

void Note::shift(int xShift)
{
    x_pos += xShift * 12;
}

void Note::update(int tempo)
{
    x_pos -= round((double)tempo/7.5);//7.5);
}


Piece::Piece() : pNotes(NULL), N_NOTES(0)
{
    //pNotes = new Note[N_NOTES];
}

Piece::~Piece()
{
    delete [] pNotes;
}

Note* Piece::getNotes()
{
    return pNotes;
}

int Piece::getNumNotes()
{
    return N_NOTES;
}

void Piece::shift(int xShift)
{
    for(int ii = 0; ii < N_NOTES; ii++)
    {
        pNotes[ii].shift(xShift);
    }
}

void Piece::update(int globalTempo)
{
    for(int ii = 0; ii < N_NOTES; ii++)
    {
        pNotes[ii].update(globalTempo);
    }
}

void Piece::load(const char *filename)
{
    if(pNotes != NULL)
    {
        delete [] pNotes;
        N_NOTES = 0;
    }

    MidiFile midifile;
    midifile.read(filename);
//    cout << midifile.getFilename() << endl;
    //midifile.absoluteTicks();
    midifile.linkNotePairs();

    int tpq = midifile.getTicksPerQuarterNote();

//    cout << tpq << endl;

    int tracks = midifile.getTrackCount();
//    cout << tracks << endl;
    for(int ii = 0; ii < tracks; ii++)
    {   for(int jj = 0; jj < midifile.getEventCount(ii); jj++ )
        {
            if(midifile[ii][jj].isNoteOn())
            {
                N_NOTES += 1;
            }
        }        
    }
//    int count = 0;
//    for(int ii = 0; ii < tracks; ii++)
//    {   count += midifile.getEventCount(ii);    }
//    cout << N_NOTES << endl;
//    cout << count << endl;

    pNotes = new Note[N_NOTES];

    int bpm;
    int kk = 0;
    for(int ii = 0; ii < tracks; ii++)
    {   for(int jj = 0; jj < midifile.getEventCount(ii); jj++ )
        {
//            if(midifile[ii][jj].isTempo())
//            {
//                bpm = midifile[ii][jj].getTempoBPM();
//                midifile.setTicksPerQuarterNote(480 * (120.0/bpm));
//                /* tempo information is only found in track 0 */
//                //double tempo = midifile[ii][jj].getTempoSPT();
//                cout << "tempo loc: " << ii << " " << jj << endl;
//                cout << "tempo: " << bpm << endl;
//            }
            if(midifile[ii][jj].isNoteOn())
            {
                //pNotes[kk]._tempo = bpm;
                pNotes[kk]._color = ii;
                pNotes[kk]._duration = midifile[ii][jj].getTickDuration();
                pNotes[kk].x_pos = midifile[ii][jj].tick;
                pNotes[kk].y_pos = midifile[ii][jj].getKeyNumber();

                //cout << midifile[ii][jj].isLinked() << endl;
//                cout << pNotes[kk]._tempo << " " << (int)pNotes[kk]._color << " " << pNotes[kk]._duration;
//                cout << " " << pNotes[kk].x_pos << " " << pNotes[kk].y_pos << endl;
                kk++;
            }
        }        
    }

    for(int ii = 0; ii < N_NOTES; ii++)
    {   for(int jj = ii; jj < N_NOTES; jj++)
        {
            if(pNotes[jj].x_pos < pNotes[ii].x_pos)
            {
                Note _note = pNotes[jj];
                pNotes[jj] = pNotes[ii];
                pNotes[ii] = _note;
            }
        }
    }

    kk = 0;
    int first_bpm;

    for(int jj = 0; jj < midifile.getEventCount(0); jj++ )
    {
        if(midifile[0][jj].isTempo())
        {
            first_bpm = midifile[0][jj].getTempoBPM();
            break;
        }
    }
//    cout << midifile.getEventCount(0) << endl;
    midifile.joinTracks();
    midifile.sortTracks();
//    cout << midifile.getEventCount(0) << endl;

    for(int jj = 0; jj < midifile.getEventCount(0); jj++ )
    {
        if(midifile[0][jj].isTempo())
        {
            bpm = midifile[0][jj].getTempoBPM();
        }
        if(midifile[0][jj].isNoteOn())
        {
            pNotes[kk]._tempo = bpm;
            //pNotes[kk]._duration *= (double)first_bpm/bpm;             

//            cout << pNotes[kk]._tempo << " " << (int)pNotes[kk]._color << " " << pNotes[kk]._duration;
//            cout << " " << pNotes[kk].x_pos << " " << pNotes[kk].y_pos << endl;
            kk++;
        }
    }        

//    tpq = midifile.getTicksPerQuarterNote();
//
//    cout << tpq << endl;
}
}
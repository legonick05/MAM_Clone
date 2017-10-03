# MAM_Clone
A C++ program that animates MIDIs in the fashion of the Music Animation Machine

To compile via g++, go to the directory containing the source files, and input:

g++ -std=c++11 main.cc ScreenSrc.cc NotesSrc.cc Binasc.cpp MidiEvent.cpp MidiEventList.cpp MidiFile.cpp MidiMessage.cpp -I(wherever the MIDIFile headers are located) -I/usr/local/include/SDL2 -lSDL2

main.cc               - the main function handling the SDL window, including rendering, scrolling, and event processing. Also includes 
                        some source files for certain flag functions for rendering.

Screen.h/ScreenSrc.cc - The class information for the Screen and Toolbar classes. These contain vital SDL methods needed to perform the
                        operations in main().

Notes.h/NotesSrc.cc   - The class information for the Note and Piece classes. These handle and create note objects from a MIDI's bytes.
                        The Piece class encompasses the Note class, containing a pointer of a Note array.
                        
Make sure, before compiling, that you have a copy of the MIDIFile library's source files, and SDL2 installed in the default location. The program will not run if either things are missing.

Furthermore, I do not own the rights to MIDIFile or SDL2: they belong to their respective owners. Special thanks to them for making their libraries available to the public.

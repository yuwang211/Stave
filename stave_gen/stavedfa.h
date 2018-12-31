#ifndef _STAVEDFA_H__
#define _STAVEDFA_H__

#include "tplib.h"

using namespace tplib;
using std::string;


class StaveDFA{
    
    public:
    
        void init();
        void process(string s);
        void stop();
        int getNoteID(string s);
    
        void loadBaseline(string fn);
        void saveSong(string fn);
    
    private:
    
        static const int STACK_LEN = 100;
        static constexpr double DEFAULT_SPEED = 60.0 / 96.0;
        static const int DEFAULT_VOL = 100;
    
        int stack_size;
        double begin_time[STACK_LEN];
        double last_time[STACK_LEN];
        double speed[STACK_LEN];
        int vol[STACK_LEN];
    
        Block<double> baseline;
        Block<double> song;
    
        void processToken(string s);
        void putNote(int id, double start, double len);
};

#include "stavedfa.cpp"

#endif

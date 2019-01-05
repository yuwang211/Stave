#ifndef _SAMPLE_H__
#define _SAMPLE_H__

#include "tplib.h"

using namespace tplib;

class Sample{
    
    public:
    
    static const int NoteLen = 84;
    double NoteHz[NoteLen];

    
    int SamplePerSec = 44100;
    int WindowLen = SamplePerSec / 44;
    int WindowGap = WindowLen / 2;
    int FFTSize = 4096;
    
    Block<double> wf;
    Block<double> w;
    Block<Complex> fft_f;
    Block<double> fft_e;
    Block<double> note_weight;
    Block<bool> note;
    
    Sample();
    void getWindow(const Block<double> &s, int start);
    void analyse();
    
    private:
    
    int HztoFFT(double hz);
    
};

#include "sample.cpp"

#endif

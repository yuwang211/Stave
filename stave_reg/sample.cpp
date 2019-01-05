
Sample::Sample()
{
    //Set note hz
    NoteHz[0] = 27.5;
    for (int i = 1; i < NoteLen; ++i)
        NoteHz[i] = NoteHz[i - 1] * 1.05946309435929526456182;
    
    //init block
    w.resize(WindowLen);
    wf.resize(WindowLen);
    fft_f.resize(FFTSize * 2);
    fft_e.resize(FFTSize * 2);
    note_weight.resize(NoteLen);
    note.rezize(NoteLen);
    
    
    //generate window function
    SignalProc::genHammingWindow(wf, WindowLen);
    
}

void Sample::getWindow(const Block<double> &s, int start)
{
    SignalProc::getWindow(s, wf, w, start, WindowLen);
}

void Sample::analyse()
{
    SignalProc::getFrequencyDomain(w, fft_f, FFTSize * 2);
    SignalProc::getEnergy(fft_f, fft_e, FFTSize * 2);
    
    for (int i = 0; i < NoteLen; ++i)
    {
        int l = (int)(HztoFFT(NoteHz[i]) / 1.02930223664349202);
        int r = (int)(HztoFFT(NoteHz[i]) * 1.02930223664349202) + 1;
        note_weight[i] = 0;
        for (int j = l; j <= r; ++j)
            note_weight[i] += e[j];
    }
}

double HztoFFT(double hz)
{
    return 2 * (double)FFTSize * hz / (double) SamplePerSec;
}

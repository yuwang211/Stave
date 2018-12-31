#include "tplib.h"
#include "stavedfa.h"

using namespace tplib;
using std::string;

const string BASELINE_FILE = "wav/baseline.wav";
const string NOTE_FILE = "note/hhxdt.txt";

char cache[10000];

StaveDFA dfa;

void solve()
{
    printf("Loading %s...\n", BASELINE_FILE.c_str());
    dfa.loadBaseline(BASELINE_FILE);
    printf("Load baseline success\n");
    FILE *fin = fopen(NOTE_FILE.c_str(), "r");
    dfa.init();
    while (fgets(cache, 10000, fin))
    {
        printf("Process: %s\n", cache);
        dfa.process(string(cache));
    }
    dfa.stop();
    fclose(fin);
    printf("Generation complete\n");
    fflush(stdout);
    dfa.saveSong("output.wav");
}




int main()
{
    printf("Start runing\n");
    solve();
    return 0;
}

void StaveDFA::init()
{
    stack_size = 0;
    begin_time[0] = 0;
    last_time[0] = 0;
    speed[0] = DEFAULT_SPEED;
    vol[0] = DEFAULT_VOL;
    song.resize(44100);
}

void StaveDFA::process(string s)
{
    s = s + " $";
    int i = 0;
    while (i < s.size())
    {
        while (s[i] == ' ') ++i;
        string t = "";
        if (s[i] == '$') break;
        else if (s[i] == '{' || s[i] == '}' || s[i] == ',')
        {
            t = t + s[i];
            ++i;
            processToken(t);
        }
        else{
            while (s[i] != ' ' && s[i] != ',' && s[i] != '{' && s[i] != '}')
            {
                t += s[i];
                ++i;
            }
            processToken(t);
        }
    }
}

void StaveDFA::stop()
{
    song.resize((int)((begin_time[stack_size] + last_time[stack_size]) * 44100));
}

int StaveDFA::getNoteID(string s)
{
    if (s == "0") return 0;
    else if (s == "A") return 1;
    else if (s == "A#") return 2;
    else if (s == "Bb") return 2;
    else if (s == "B") return 3;
    else
    {
        int id;
        if (s[0] == 'C') id = 4;
        else if (s[0] == 'D') id = 6;
        else if (s[0] == 'E') id = 8;
        else if (s[0] == 'F') id = 9;
        else if (s[0] == 'G') id = 11;
        else if (s[0] == 'A') id = 13;
        else if (s[0] == 'B') id = 15;
        if (s[1] == '#') id += (s[2] - '1') * 12 + 1;
        else if (s[1] == 'b') id += (s[2] - '1') * 12 - 1;
        else id += (s[1] - '1') * 12;
        printf("[%s:%d]", s.c_str(), id);
        return id;
    }
}

void StaveDFA::processToken(string s)
{
    printf("[%s]", s.c_str());
    fflush(stdout);
    if (s.size() == 1 && s[0] <= 32) return;
    if (s == "{")
    {
        begin_time[stack_size + 1] = begin_time[stack_size] + last_time[stack_size];
        last_time[++stack_size] = 0;
        speed[stack_size] = speed[stack_size - 1];
        vol[stack_size] = vol[stack_size - 1];
    }
    else if (s == "}")
    {
        last_time[stack_size - 1] += last_time[stack_size];
        --stack_size;
    }
    else if (s == ",")
    {
        last_time[stack_size] = 0;
    }
    else if (s[0] == '#')
    {
        string t = "";
        int i = 1;
        while (i < s.size() && s[i] != '=')
        {
            t = t + s[i];
            ++i;
        }
        string t2 = s.substr(i + 1, s.size() - i - 1);
        if (t == "vol")
        {
            vol[stack_size] = atoi(t2.c_str());
        }
    }
    else
    {
        string t = "";
        int i = 0;
        while (i < s.size() && s[i] != '-')
        {
            t = t + s[i];
            ++i;
        }
        int id = getNoteID(t);
        double l = 0;
        if (i == s.size())
        {
            l = speed[stack_size];
        }
        else
        {
            ++i;
            while (i < s.size())
            {
                int k = 0;
                while (i < s.size() && s[i] != '-')
                {
                    k = k * 10 + (s[i] - '0');
                    ++i;
                }
                l += speed[stack_size] * 4.0 / k;
                ++i;
            }
        }
        putNote(id, begin_time[stack_size] + last_time[stack_size], l);
        last_time[stack_size] += l;
    }
    
}

void StaveDFA::loadBaseline(string fn)
{
    int n;
    tplib::Signal<Block<double> > s;
    n = s.getWavSize(fn);
    baseline.resize(n);
    s.loadWav(baseline, fn, n);
}

void StaveDFA::saveSong(string fn)
{
    tplib::Signal<Block<double> > s;
    s.saveWav(song, fn, (int)((begin_time[stack_size] + last_time[stack_size]) * 44100));
}

void StaveDFA::putNote(int id, double s, double t)
{
    printf("%d %.2lf %.2lf\n", id, s, t);
    fflush(stdout);
    if (id == 0) return;
    int sf = (int)(s * 44100);
    int tf = (int)(t * 44100);
    int bf = (id - 1) * 4 * 44100;
    while (sf + tf >= song.size())
    {
        int os = song.size();
        int ns = song.size() + (song.size() >> 1) + 1;
        song.resize(ns);
        for (int i = os; i < ns; ++i)
            song[i] = 0;
    }
    if (tf < 44100 * 4)
    {
        for (int i = 0; i < tf; ++i)
            song[sf + i] += baseline[bf + i] * vol[stack_size] / 100.0;
    }
    else
    {
        for (int i = 0; i < 44100 * 4; ++i)
            song[sf + i] += baseline[bf + i] * vol[stack_size] / 100.0;
        for (int i = 44100 * 4; i < tf; ++i)
            song[sf + i] += baseline[bf + 44100 * 4 - 1000 + i % 1000] * vol[stack_size] / 100.0;
    }
}

#include <iostream>
#include <sstream>
#include <string>
#include "controller/game.h"
#include "public/global.h"
using namespace std;

int main(int nargs, const char** arglist) {
    try {
        
        /* parse commandline argument */
        string argstr;
        for (int i = 0; i < nargs; i++) {
            argstr += arglist[i];
        }
        
        unsigned long p = string::npos;
        
        if (argstr.find("-text") != string::npos) {
            Global:: TEXTMODE = true;
            cerr << ">> text-only mode on" << endl;
            p = string::npos;
        } else {
            Global:: TEXTMODE = false;
        }

        if (argstr.find("-no-op") != string::npos) {
            Global:: OPTIMIZED = false;
            cerr << ">> optimized mode off" << endl;
            p = string::npos;
        } else {
            Global:: OPTIMIZED = true;
        }        
        
        if ((p = argstr.find("-seed")) != string::npos) {
            string tmp = argstr.substr(p+5) + " "; // to ensure after taking the seed number, ss is nonempty
            stringstream ss(tmp.c_str());
            int i = 0;
            ss >> i;
            if (! ss.good()) {throw string("\nERROR: missing -seed number");} 
            Global:: SEED = i;
            cerr << ">> set seed to " << i << endl;
            p = string::npos;
        } else {
            Global:: SEED = 0;
        }
        
        if ((p = argstr.find("-scriptfile")) != string::npos) {
            string tmp = argstr.substr(p+11) + " ";
            stringstream ss(tmp.c_str());
            string filename;
            ss >> filename;
            if (! ss.good()) {throw string("\nERROR: missing scriptfile name");} 
            Global:: SCRIPTFILE = filename;
            cerr << ">> using scriptfile: "<< filename << endl;
            p = string::npos;
        }
        
        if ((p = argstr.find("-startlevel")) != string::npos) {
            string tmp = argstr.substr(p+11) + " ";
            stringstream ss(tmp.c_str());
            int i;
            ss >> i;
            if (! ss.good()) {throw string("\nERROR: missing -startlevel number");} 
            Global:: STARTLEVEL = i;
            cerr << ">> set startlevel to " << i << endl;
            p = string::npos;
        } else {
            Global:: STARTLEVEL = 0;
        }
    
        #if DEBUG
            int i;
            cerr << "Size: ";
            cin >> i;
            Game game(i);
        #else
            Game game(10); // a 10 x 10 game begins
        #endif
            
    } catch (const string& e) {
        cerr << e << endl;
    }
}


#include <iostream>
#include <sstream>
#include <string>
#include "controller/game.h"
#include "public/global.h"
using namespace std;

int main(int nargs, const char** arglist) {
    try {
        
        // parse arglist
        string argstr;
        for (int i = 0; i < nargs; i++) {
            argstr += arglist[i];
        }
        
        cerr << argstr;
        
        unsigned long p = string::npos;
        
        if (argstr.find("-text") != string::npos) {
            Global:: TEXTMODE = true;
            cerr << "graphic mode on" << endl;
            p = string::npos;
        } else {
            Global:: TEXTMODE = false;
        }
        
        
        if ((p = argstr.find("-seed")) != string::npos) {
            stringstream ss(argstr.substr(p));
            int i;
            ss >> i;
            if (! ss.good()) {throw string("\nERROR: missing -seed number");} 
            Global:: SEED = i;
            cerr << "seed set to: " << i << endl;
            p = string::npos;
        } else {
            Global:: SEED = 0;
        }
        
        if ((p = argstr.find("-scriptfile")) != string::npos) {
            stringstream ss(argstr.substr(p));
            string filename;
            ss >> filename;
            if (! ss.good()) {throw string("\nERROR: missing scriptfile name");} 
            Global:: SCRIPTFILE = filename;
            cerr << "using scriptfile: "<< filename << endl;
            p = string::npos;
        }
        
        if ((p = argstr.find("-startlevel")) != string::npos) {
            stringstream ss(argstr.substr(p));
            int i;
            ss >> i;
            if (! ss.good()) {throw string("\nERROR: missing -startlevel number");} 
            Global:: STARTLEVEL = i;
            cerr << "seed startlevel to: " << i << endl;
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
            Game game(10);
        #endif
    } catch (const string& e) {
        cerr << e << endl;
    }
}

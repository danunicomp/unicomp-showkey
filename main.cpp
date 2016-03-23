/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dshaffer
 *
 * Created on March 11, 2016, 2:10 PM
 */

#include <cstdlib>
#include "clsGetCodes.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>  // for sleep

using namespace std;

const string VERSION = "0.9.160323.A";

int main(int argc, char** argv) {

    sleep(1);
    int codes_to_show = 4;  // set defolt number of codes to show
    int codes_command_line; // command line number of keys
   
   // check for command line options  
     if (argc == 2 ) {
        stringstream(argv[1]) >> codes_command_line;
        if ( codes_command_line != 0) {
            if (codes_command_line > 18) {
                codes_command_line = 18;
            }
            codes_to_show = codes_command_line;
            cout << "Codes to show: " << codes_command_line << endl;
        }
     }
     
    cout  << "=====================================================" << endl << 
            "UNICOMP SHOWKEY - VERSION " << 
            VERSION << endl
             <<   "To exit, hold X key until it repeats" << endl << endl;
    
    
    clsGetCodes GetUnicode;
    vector<int> wholebuffer;   // get codes from show key class
    int exits = 0, x;
    while(1 && exits < 3) {             // loop until hold X key
          system("stty -echo");
        wholebuffer = GetUnicode.GetUnicodeBuffer();
        if (wholebuffer[0] == 45) ++exits;
        else exits = 0;
        for (x=0; x<codes_to_show; ++x)  {
            cout << wholebuffer[x] << "\t";
        }
        if (wholebuffer[18] == 1999) {
            cout << "MAKE" << endl;
        }
        else if (wholebuffer[18] == 999) {
            cout << "BREAK" << endl << endl;
        }
        else {
            cout << endl;
        }

        wholebuffer.clear();
        system("stty echo");
    }
}
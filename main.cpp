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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

 
    cout << endl << "=====================================================" << endl << "UNICOMP SHOWKEY...  to exit, hold X key until it repeats" << endl << endl;
    
    
    clsGetCodes GetUnicode;
    std::vector<int> wholebuffer;
          int exits = 0;
          int x;
        while(1 && exits < 3) {
              system("stty -echo");
            wholebuffer = GetUnicode.GetUnicodeBuffer();
            if (wholebuffer[0] == 45) ++exits;
            else exits = 0;
            for (x=0; x<6; ++x)  {
                cout << wholebuffer[x] << "\t";
            }
            if (wholebuffer[18] == 1999) {
                cout << "MAKE." << endl;
            }
            else if (wholebuffer[18] == 999) {
                cout << "BREAK" << endl << endl;
            }
            else {
                cout << endl;
            }
            
            wholebuffer.clear();
            system("stty echo");
 //          free(wholebuffer)  ;
        }
   

}


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   clsGetCodes.h
 * Author: dshaffer
 *
 * Created on March 10, 2016, 7:40 AM
 */

#ifndef CLSGETCODES_H
#define CLSGETCODES_H

#include <vector>
#include <termios.h>

class clsGetCodes {
public:
    clsGetCodes();
    clsGetCodes(const clsGetCodes& orig);
    virtual ~clsGetCodes();
    std::vector<int> GetUnicodeBuffer (void);
   void Diagnostic (void);
        void clean_up(int) ;
         int getfd(const char *);
private:

     void get_mode(int);
     int is_a_console(int);
     int open_a_console(const char *);
    

     int oldkbmode;
     
    struct termios newkb;
    struct termios old;
};

#endif /* CLSGETCODES_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   clsGetCodes.cpp
 * Author: dshaffer
 * 
 * Created on March 10, 2016, 7:41 AM
 */

#include <vector>
#include <termios.h>
#include <linux/keyboard.h>
#include <stdlib.h>
#include <sys/ioctl.h> /* ioctl */
#include <fcntl.h> /* open */
#include <stdio.h>
#include <linux/kd.h>
#include <string>
#include <iostream>
#include <signal.h>
 #include <unistd.h>

#include "clsGetCodes.h"

clsGetCodes::clsGetCodes() {
}

clsGetCodes::clsGetCodes(const clsGetCodes& orig) {
}

clsGetCodes::~clsGetCodes() {
}

using namespace std;

/*******************   MAIN    ****************************/
 std::vector<int> clsGetCodes::GetUnicodeBuffer (void) {
         int show_keycodes = 1;
    int fd;


    int  buf[19];
 //   int *newbuf = malloc(sizeof(int) * 19); 
    //int newbuf[19];
   // *newbuf = malloc(sizeof(int) * 19); 
    std::vector<int> newbuf(19);
    int i, n;

    fd = clsGetCodes::getfd(NULL);
    /* the program terminates when there is no input for 10 secs */
   // signal(SIGALRM, watch_dog(fd));

    clsGetCodes::get_mode(fd);
    
  
    // THIS IS THE HOOK
    if (tcgetattr(fd, &old) == -1)
            perror("tcgetattr");
    if (tcgetattr(fd, &newkb) == -1)
            perror("tcgetattr");
    ///////////////////////////
    

    newkb.c_lflag &= ~ (ICANON | ECHO | ISIG);
    newkb.c_iflag = 0;
    
    // new.c_cc[VMIN] = sizeof(buf);
    newkb.c_cc[VMIN] = 1;
    newkb.c_cc[VTIME] = 1;	/* 0.1 sec intercharacter timeout */

    
    
    if (tcsetattr(fd, TCSAFLUSH, &newkb) == -1)
            perror("tcsetattr");


    if (ioctl(fd, KDSKBMODE, show_keycodes ? K_MEDIUMRAW : K_RAW)) {
        cout << "Fail?? " << endl;
        perror("KDSKBMODE");
                    clean_up(fd);
            close(fd);
        return newbuf;
    }
     
    usleep(10);
    
    /* show keycodes - 2.6 allows 3-byte reports */
    int t;
 //     
    for (t=0; t<19; ++t) {
        buf[t] = 0;
    }
 //    usleep(1000000);


     //   alarm(100);
        int fullcode;
      
         n = read(fd, buf, sizeof(buf));
     
            if (n == -1) { cout << "ERROR"; clean_up(fd); return newbuf; }
        i = 0;
        int y = buf[0];
        fullcode = y;
        
        for (i=0;i<n; ++i) {
          buf[i] = ((fullcode & (255 << (8*i))) >> (8*i) );
        }

        if (buf[0] < 128) {
            buf[18] = 1999;
        }
        else
        {
            buf[18] = 999;
        }

            newbuf.clear();
            for (t=0;t<19;++t) {
                newbuf.push_back(buf[t]);
//               if ( newbuf[0] == 1) {
//                   clean_up(fd);
//                   close(fd);
//                   return newbuf;
//               }
                buf[t] = 0;  //flush buffer
            }
 
            clean_up(fd);
            close(fd);
            return newbuf;
     

}



 ////////////////////////////////////////////////////

 // ******* CLEAN UP ************************
void clsGetCodes::clean_up(int fd ) {

    if (ioctl(fd, KDSKBMODE, oldkbmode)) {

        return;
    }
    
    if (tcsetattr(fd, 0, &old) == -1) {
        perror("tcsetattr");

    }
    //if (tcsetattr(fd, 0, &clsGetCodes.old.) == -1)
    //    perror("tcsetattr");
    close(fd);
    return;
}

// ************ GET MODE ***********************

void clsGetCodes::get_mode(int fd) {
    string m;

    if (ioctl(fd, KDGKBMODE, &oldkbmode)) {
            perror("KDGKBMODE");
            cout <<  "fail get_mode" << endl;
            clean_up(fd);
                close(fd);
            return;
    }
    switch(oldkbmode) {
        case K_RAW:
            m = "RAW"; break;
        case K_XLATE:
            m = "XLATE"; break;
        case K_MEDIUMRAW:
            m = "MEDIUMRAW"; break;
        case K_UNICODE:
            m = "UNICODE"; break;
    }
}
////////////////////////////////////////////////////////

// ***************** IS A CONSOLE *************************
int clsGetCodes::is_a_console(int fd) {
    char arg;

    arg = 0;
    return (isatty (fd)
        && ioctl(fd, KDGKBTYPE, &arg) == 0
        && ((arg == KB_101) || (arg == KB_84)));
}

//////////////////////////////////////////////////////////

// ***************** OPEN A CONSOLE *********************
int clsGetCodes::open_a_console(const char *fnam) {
	int fd;

	/*
	 * For ioctl purposes we only need some fd and permissions
	 * do not matter. But setfont:activatemap() does a write.
	 */
	fd = open(fnam, O_RDWR);
	if (fd < 0)
		fd = open(fnam, O_WRONLY);
	if (fd < 0)
		fd = open(fnam, O_RDONLY);
	if (fd < 0)
		return -1;
	if (!is_a_console(fd)) {
		close(fd);
		return -1;
	}
	return fd;
}

///////////////////////////////////////////////////////////


// ***************  GET FD *****************************
//  GET FILE DISCRIPTER. INPUT
int clsGetCodes::getfd(const char *fnam) {
	int fd;
   //     printf ("FINDING FD\n");
	if (fnam) {
		fd = clsGetCodes::open_a_console(fnam);
		if (fd >= 0) {
                   // printf ("FD FOUND   NULL");  
			return fd;
                }
		fprintf(stderr,"Couldn't open %s\n", fnam);
		exit(1);
	}

	fd = clsGetCodes::open_a_console("/proc/self/fd/0");
	if (fd >= 0) {
   //        printf ("FD FOUND   /proc/self/fd/0");     
            return fd;  
        }

	fd = clsGetCodes::open_a_console("/dev/tty");
	if (fd >= 0) {
 //           printf ("FD FOUND   /dev/tty");
            return fd;
        }

	fd = clsGetCodes::open_a_console("/dev/tty0");
	if (fd >= 0) {
 //         printf ("FD FOUND   /dev/tty0");
            return fd;
        }
	fd = clsGetCodes::open_a_console("/dev/vc/0");
	if (fd >= 0)  {
 //         printf ("FD FOUND   /dev/vc/0");
            return fd;
        }
	fd = clsGetCodes::open_a_console("/dev/console");
	if (fd >= 0)  {
 //         printf ("FD FOUND   /dev/console");
            return fd;
        }
//           printf ("FD NOT FOUND\n");
	for (fd = 0; fd < 3; fd++)
		if (is_a_console(fd))
			return fd;

	fprintf(stderr, "Couldn't get a file descriptor referring to the console\n");
	exit(1);		/* total failure */
}


/////////////////          DIAGNOTIC        //////////////////////////////////////)

 void Diagnostic (void) {
     
 }
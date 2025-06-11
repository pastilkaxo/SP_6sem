/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 41 */

/* prog.c */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int
main(int argc, char *argv[])
{
    void *hLib = dlopen("./libLibProject.so", RTLD_NOW);

    void (*x1)(void); 
    void (*x2)(void); 

    *(void **) (&x1) = (void (*)(void)) dlsym(hLib, "x1");
    *(void **) (&x2) = (void (*)(void)) dlsym(hLib, "x2");

    x1();
    x2();

    dlclose(hLib);
    exit(EXIT_SUCCESS);
}

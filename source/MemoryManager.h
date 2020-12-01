/* Author: Mark Tasker
 * Date: 05/09/2020
 * 
 * Purpose: Overall program runs demand paging algorithms. 
*/


#ifndef MEMORY_MANAGER_H
#define MEMORY_MAGANER_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


/* Starting menu and passing number of physical frames. */

void startMenu(int physicalFramesNum);

#endif // !MEMORY_MANAGER_H


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
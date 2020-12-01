/* Author: Mark Tasker
 * Date: 05/09/2020
 * 
 * Purpose: Overall program runs demand paging algorithms. Main initiates program
*/

#include "MemoryManager.h"

int main(int argv, char* argc[])
{
	if (argv < 2)
	{
		printf("Number of physical pages required! Please check command line arguments!\n");

		return -1;
	}


	/* Checking physical frames number is between 1 and 8 and correcting it if it is not. */

	int physicalFramesNum = atoi(argc[1]);

	if (physicalFramesNum > 8)
	{
		physicalFramesNum = 8;
	}

	if (physicalFramesNum < 1)
	{
		physicalFramesNum = 1;
	}
	startMenu(physicalFramesNum);

	return 0;
}
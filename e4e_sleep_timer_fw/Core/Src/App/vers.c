/*
 * vers.c
 *
 *  Created on: Feb 14, 2022
 *      Author: ntlhui
 */
#include <Debug/conio.h>
#include <string.h>


#define MAJOR_VER	0
#define MINOR_VER	0
#define PATCH_VER	0
#define BUILD_NUM	1

#define BRANCH_NAME	""

void E4E_PrintBanner(void)
{
	E4E_Println("E4E Sleep Timer FW v%2d.%02d.%02d.%02d%s", MAJOR_VER, MINOR_VER, PATCH_VER, BUILD_NUM, BRANCH_NAME);
	if(strlen(BRANCH_NAME) != 0)
	{
		E4E_Println("Prototype FW.  Build at %s %s", __DATE__, __TIME__);
	}

}

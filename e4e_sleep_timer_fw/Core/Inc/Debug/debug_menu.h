/*
 * debug_menu.h
 *
 *  Created on: Feb 14, 2022
 *      Author: ntlhui
 */

#ifndef INC_DEBUG_DEBUG_MENU_H_
#define INC_DEBUG_DEBUG_MENU_H_

#include <stddef.h>

#define ESC_CHAR	'\x1b'

typedef enum E4E_DebugMenuType_
{
	E4E_DebugMenuType_Null,
	E4E_DebugMenuType_Menu,
	E4E_DebugMenuType_Cmd,
	E4E_DebugMenuType__NELEMS
}E4E_DebugMenuType_e;

typedef struct E4E_DebugMenu_
{
	char command;
	E4E_DebugMenuType_e menuType;
	struct E4E_DebugMenu_ *pSubMenu;
	int (*pCommandFunc)(void);
	const char* description;
}E4E_DebugMenu_t;

void E4E_DebugApp(void);

int E4E_DebugMenuProcess(const E4E_DebugMenu_t* const pMenu);
#endif /* INC_DEBUG_DEBUG_MENU_H_ */

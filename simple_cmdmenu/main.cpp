// simple_cmdmenu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include "cmenu.h"

int main()
{

	/* pMenu::setup(
		"menu name",
		{"main", "aimbot", "visuals"},
	)
	*/
	

	std::shared_ptr<cmenu> pMenu = std::make_shared<cmenu>("brasilhook");
	
	pMenu->setup();
	
	
}


// simple_cmdmenu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include "cmenu.h"

int main()
{
	

	std::shared_ptr<cmenu> pMenu = std::make_shared<cmenu>("brasilhook");
	
	pMenu->setup();
	
	
}


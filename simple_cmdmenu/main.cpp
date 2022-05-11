// simple_cmdmenu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include "cmenu.h"

int main()
{
	std::shared_ptr<cmenu> pMenu = std::make_shared<cmenu>();
	
	cbutton cb = cbutton("button", []() {std::cout << "button1"; });
	ccheckbox ccb = ccheckbox("checkbox", []() {std::cout << "checkbox1"; });
	cslider cs = cslider("slider", []() {std::cout << "slider1"; });
	
	cbutton cb2 = cbutton("button2", []() {; });

	cdropbox cd = cdropbox("dropbox", { &ccb, &cb2 });
	

	cd.call();
	
	pMenu->addmul({ &cb, &ccb, &cs, &cd });
	
	pMenu->draw();
	
}


#include "cmenu.h"
#include <Windows.h>


void cmenu::setup()
{
	ccheckbox cb = ccheckbox("checkbox1", []() {; });
	ccheckbox cdcb = ccheckbox("cd_checkbox1", []() {; });
	ccheckbox cdcb2 = ccheckbox("cd_checkbox2", []() {; });
	
	int x = 25;
	
	cslider cs = cslider("slider1", &x);

	std::string title = this->title;
	cbutton cb1 = cbutton("change title", [title]() {SetConsoleTitleA((LPCSTR)title.c_str()); });
	
	cdropbox cd = cdropbox("dropbox1", { &cdcb, &cdcb2});

	this->add_tabs({ "main","misc","other" });

	this->addmul("main", { &cb1, &cb, &cd, &cs });
	this->addmul("misc", { &cdcb });	
	
	while (1) {
		this->draw();
		this->input();
		system("cls");
	}

	
}

#pragma once
#ifndef __CMENU
#include <vector>
#include <string>
#include <functional>
#include <iostream>

// base item class
class citem
{
public:
	citem(std::string name, std::function<void()> func) {
		this->name = name;
		this->func = func;
	}
	~citem() {}

	virtual std::string draw() {
		return name + " is a base citem\n";
	}

	virtual std::function<void()> call() {
		return func;
	}

private:
	std::string name;
	std::function<void()> func;
};

// button
class cbutton : public citem
{
public:
	cbutton(std::string name, std::function<void()> func)
		: citem(name, func) {
		this->name = name;
		this->func = func;
	}
	~cbutton() {}
	
	inline std::string draw() override {
		std::string output = "[b] " + name + "\n";
		return output;
	}

	inline std::function<void()> call() override {
		return func;
	}
	
private:
	std::string name;
	std::function<void()> func;
};

// checkbox
class ccheckbox : public citem
{
public:
	ccheckbox(std::string name, std::function<void()> func)
		: citem(name, func) {
		this->name = name;
		this->func = func;
	}
	~ccheckbox() {}
	
	inline std::string draw() override {
		std::string output = "[ ] " + name + "\n";
		if(state)
			output[1] = 'X';
		else
			output[1] = ' ';
		return output;
	}

	inline std::function<void()> call() override {
		state = !state;
		return func;
	}

private:
	bool state = false;
	std::string name;
	std::function<void()> func;
};

// slider
class cslider : public citem
{
public:
	cslider(std::string name, std::function<void()> func)
		: citem(name, func) {
		this->name = name;
		this->func = func;
	}
	~cslider() {}

	virtual std::string draw() override {
		return "[" + std::to_string(value) + "] " + name + "\n";
	}

	virtual std::function<void()> call() override {
		// ask user for input n change value
		return func;
	}

private:
	int value = 0;
	std::string name;
	std::function<void()> func;
};

// dropbox
class cdropbox : public ccheckbox
{
public:
	cdropbox(std::string name, std::vector<citem*> items)
		: ccheckbox(name, []() {; }) {
		this->name = name;
		this->items = items;
	}
	~cdropbox() {}
	
	virtual std::string draw() override {
		std::string output = "[ ] " + name + "\n";
		if (state) {
			output[1] = 'X';
			for (auto& i : items) {
				output += " - " + i->draw();
			}
		}
		else {
			output[1] = ' ';
		}
		return output;
	}

	virtual std::function<void()> call() override {
		state = !state;
		// get input
		// if not opened, open
		// if already opened, ask user which item they want to select, if they input nothing ('') close menu
		return []() {; };
	}

private:
	bool state = false;
	std::string name;
	std::vector<citem*> items;
};

/*
* 1. tabs
*	use directory type system
* 
* 2. custom text ?
*	use existing button
* 
* 3. way of actually differentiating items from one another
* 
*/

// stores all items
// handles input, output, and other stuff
class cmenu
{
public:
	cmenu() { }
	~cmenu() {}

	// add a single item
	inline void add(citem* item) {
		item_list.push_back(item);
	}

	// add multiple items at a time
	inline void addmul(std::vector<citem*> data) {
		for (const auto& d : data) {
			item_list.push_back(d);
		}
	}

	// yes
	inline void call(citem* item) {
		item->call()();
	}
	
	// draw the menu
	inline void draw() {
		std::cout << tab << std::endl << "-------\n";
		for (auto& i : item_list) {
			std::cout << i->draw();
		}
	}

	// input handler
	inline void input() {
		//todo
	}
	
	// run
	inline void run() {
		// draw
		// error handling
		// input
		// call
	}
	
	void setup();
	
private:
	int count = 0;
	std::string tab = "main";
	std::vector<citem*> item_list;
};


#endif // !__CMENU




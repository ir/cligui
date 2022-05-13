#pragma once
#ifndef __CMENU
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class citem;

struct item {
	int id;
	citem* item;
};

struct tab
{
	std::string name;
	std::vector<item> items;
	int count = 0;
};

namespace helpers
{
	/*void out_unicode(const wchar_t* data) {
		_setmode(_fileno(stdout), _O_WTEXT);
		wprintf(text);
		_setmode(_fileno(stdout), _O_TEXT);
	}*/
}

// base item class
class citem
{
public:
	citem(std::string name, std::function<void()> func) {
		this->name = name;
		this->func = func;
	}
	~citem() {}

	inline virtual std::string draw() {
		return name + " is a base citem\n";
	}

	inline virtual std::function<void()> call() {
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
	
	// get state
	inline bool get_state() {
		return state;
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
	cslider(std::string name, int* value)
		: citem(name, []() {; }) {
		this->name = name;
		this->value = value;
	}
	~cslider() {}

	inline virtual std::string draw() override {
		return "[" + std::to_string(*value) + "] " + name + "\n";
	}

	inline virtual std::function<void()> call() override {
		// ask user for input n change value
		std::string input;
		std::cout << this->name << ": ";
		std::getline(std::cin, input);
		int len = input.length();
		if (len == 0) {
			return []() {; };
		}

		if (len >= 0 && len <= 100) {
			if (!isdigit(input[0]))
				return []() {; };
			int id = std::stoi(input); // converted input
			
			*value = id;
		}
		return []() {; };
	}

private:
	int* value;
	std::string name;
};

/*
* 1. add better way of visualizing dropboxes within dropboxes
*	- store current level of indentation
*		during a draw, the dropbox must know the current level of indentation, 
*		the dropbox itself could store the indentation level 
*		but if you move the dropbox around you'll have to make sure to properly update that information
*/
// dropbox
class cdropbox : public ccheckbox 
{
public:
	cdropbox(std::string name, std::vector<citem*> items)
		: ccheckbox(name, []() {; }) {
		this->name = name;
		for (auto& i : items) {
			this->item_list.push_back({ this->count, i });
			this->count++;
		}
	}
	~cdropbox() {}
	
	inline virtual std::string draw() override {
		std::string output = "[ ] " + name + "\n";
		if (state) {
			output[1] = 'D';
			for (auto& i : item_list) {
				output += " - " + std::to_string(i.id) + ": " + i.item->draw();
			}
		}
		else {
			output[1] = ' ';
		}
		return output;
	}

	inline virtual std::function<void()> call() override {
		//state = !state;
		// get input
		if (state == false) {
			state = true;
			return []() {; };
		}
		
		std::string input;
		std::cout << this->name << ": ";
		std::getline(std::cin, input);
		int len = input.length();
		if (len == 0) {
			return []() {; };
		}
		
		if (len == 1) {
			if (!isdigit(input[0]))
				return []() {; };
			int id = std::stoi(input); // converted input
			if (id < 1 || id > (int)item_list.size()) {
				if (id == 0)
					state = false;
				return []() {; };
			}
			return item_list[id - 1].item->call();
		}

		return []() {; };
	}

private:
	bool state = false;
	int count = 1;
	std::string name;
	std::vector<item> item_list;
};

/*
* is this really necessary? i feel like cdropbox already basically does this
* 
class child : public cdropbox
{
public:
	child(std::string name, ccheckbox* parent, std::vector<citem*> items)
		: cdropbox(name, items) {
		this->name = name;
		this->items = items;
		this->enabled = parent->get_state();
	}
	child()
	

private:
	bool enabled;
	int count;
	std::string name;
	std::vector<citem*> items;
};
*/

/*
* 1. tabs
*	use directory type system
* 
* 2. custom text ?
*	use existing button
* 
* 3. way of actually differentiating items from one another
* 
* 4. way of only displaying items if they are enabled
*	parent item(?)
* 
*/

// stores all items
// handles input, output, and other stuff
class cmenu
{
public:
	cmenu(std::string title) {
		this->title = title;
	}
	~cmenu() {}

	// add multiple items at a time
	inline void addmul(std::string t, std::vector<citem*> data) {
		int ind = -1;
		for (size_t i = 0; i < tab_list.size(); i++) {
			if (tab_list[i].name == t){
				ind = i;
			}
		}
		if (ind < 0) {
			std::cout << "poop\n";
			return;
		}
		for (const auto& d : data) {
			tab_list[ind].count++;
			tab_list[ind].items.push_back({ tab_list[ind].count, d});
		}
	}

	// yes
	inline void call(citem* item) {
		item->call()();
	}

	// draw the menu
	inline void draw() {
		std::cout << tab_display() << std::endl << "--------\n";
		
		for (auto& t : tab_list) {
			if (t.name == cur_tab) {
				for (auto& i : t.items) {
					std::cout << i.id << ": " << i.item->draw();
				}
			}
		}
	}

	// input handler
	inline void input() {
		//todo
		// change tab
		std::string input;
		std::getline(std::cin, input);
		int len = input.length();
		if (len == 0)
			return;

		// if input is a number
		if (len == 1) {
			if(!isdigit(input[0]))
				return;
			int id = std::stoi(input);
			for (const auto& t : tab_list) {
				if (t.name == cur_tab) {
					if (id < 1 || id > t.count)
						return;
					call(t.items[id - 1].item);
					return;
				}
			}
			return;
		}
		// if input is a string, check if empty, otherwise change tab
		for (auto& t : tab_list) {
			if (input == t.name) {
				cur_tab = input;
			}
		}
	}

	// run
	inline void run() {
		// draw
		// error handling
		// input
		// call
	}

	/*
	* 1. custom menu name, will display in the top left corner and as console text (animation[?])
	* 2. adds tabs
	* 3. adds all items into menu
	*/
	void setup();
	
	inline void add_tab(std::string name) {
		tab_list.push_back({ name });
		if (tab_list.size() == 1) {
			cur_tab = tab_list[0].name;
		}
	}

	inline void add_tabs(std::vector<std::string> names) {
		for (const auto& n : names) {
			add_tab(n);
		}
	}

private:
	std::string title;
	std::string cur_tab;
	std::vector<tab> tab_list;
	
	inline tab& get_tab(std::string title) {
		for (auto& t : tab_list) {
			if (t.name == title)
				return t;
		}
		std::cout << "not found\n";
	}
	
	inline std::string tab_display() {
		std::string tabs = "| ";
		for(auto& t : tab_list) {
			tabs += t.name + " [";
			if(t.name == cur_tab) {
				tabs += "X] | ";
			}
			else {
				tabs += " ] | ";
			}
		}
		return tabs;
	}

	//std::vector<citem*> item_list;
};


#endif // !__CMENU




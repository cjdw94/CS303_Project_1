#include "Library.h"

Library::Library()
{
	
}

void Library::add_book(string name)
{
	book_list.push_back(Book(name));
}

void Library::add_employee(string name)
{
	Employee empl(name);

	for (size_t i = 0; i < book_list.size(); i++) {
		book_list[i].addToQueue(empl);
	}
}

void Library::circulate_book(string name, Date circ_date)
{
	for (size_t i = 0; i < book_list.size(); i++) {
		if (book_list[i].getName() == name)
			book_list[i].circulateBook(circ_date);
	}
}

void Library::pass_on(string name, Date pass_date)
{
	for (size_t i = 0; i < book_list.size(); i++) {
		if (book_list[i].getName() == name)
			book_list[i].passOn(pass_date);
	}
}

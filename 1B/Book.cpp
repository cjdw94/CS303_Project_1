#include "Book.h"

void Book::circulateBook(Date start_date)
{
	start = start_date;
	last_pass = start;
}

void Book::passOn(Date pass_date)
{
	wait_list.front().update_retain(last_pass, pass_date);
	wait_list.pop();
	wait_list.update_priority(last_pass, pass_date);
	
	
	if(wait_list.isEmpty()){
		archived = true;
	}
	last_pass = pass_date;
}

void Book::addToQueue(Employee& empl)
{
	wait_list.push(empl);
}

string Book::getName() {
	return name;
}

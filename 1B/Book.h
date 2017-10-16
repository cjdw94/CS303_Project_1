#pragma once
#include <string>
#include "PriQueue.h"
#include "Date.h"
#include "Employee.h"
using namespace std;

class Book {
public:
	Book(string book_name) { name = book_name;  archived = false; };

	void circulateBook(Date start_date);

	void passOn(Date pass_date);

	void addToQueue(Employee& empl);

	string getName();

private:
	Date start, end, last_pass;
	string name;
	bool archived;
	PriQueue wait_list;
};
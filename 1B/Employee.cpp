#include "Employee.h"

void Employee::update_retain(Date start, Date pass)
{
	this->retain_time += pass - start;
	cout << name << ": " << retain_time << endl;
}

void Employee::update_wait(Date start, Date pass)
{
	this->wait_time += pass - start;
	cout << name << ": " << wait_time << endl;
}

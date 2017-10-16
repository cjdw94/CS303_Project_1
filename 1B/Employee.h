#pragma once
#include <string>
#include "Date.h"
#include <iostream>

using namespace std;

class Employee {
public:
	Employee(string empl_name) { name = empl_name; wait_time = 0; retain_time = 0; };

	Employee() { wait_time = 0; retain_time = 0; };

	void update_retain(Date start, Date pass);

	void update_wait(Date start, Date pass);

private:
	string name;
	int wait_time, retain_time;
};

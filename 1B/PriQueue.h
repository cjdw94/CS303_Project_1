#pragma once
#include <queue>
#include "Employee.h"
#include "Date.h"

using namespace std;

class PriQueue {
public:
	PriQueue();

	void push(Employee emp);

	Employee front();

	void update_priority(Date start, Date today);

	bool isEmpty();

	int size();

	void pop();

private:
	queue<Employee> priqueue;
};
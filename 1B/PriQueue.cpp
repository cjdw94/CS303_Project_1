#include "PriQueue.h"

PriQueue::PriQueue()
{
}

void PriQueue::push(Employee emp)
{
	priqueue.emplace(emp);
}

Employee PriQueue::front()
{
	return priqueue.front();
}

void PriQueue::update_priority(Date start, Date today)
{

	size_t idx = 0;
	while (idx < priqueue.size()) {
		priqueue.front().update_wait(start, today);
		priqueue.push(priqueue.front());
		priqueue.pop();
		idx++;
	}


}

bool PriQueue::isEmpty()
{
	return priqueue.empty();
}

int PriQueue::size()
{
	return priqueue.size();
}

void PriQueue::pop()
{
	priqueue.pop();
}

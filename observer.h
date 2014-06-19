#ifndef __OBSERVER_PATTERN_H__
#define __OBSERVER_PATTERN_H__

#include <vector>

namespace gnilk 
{


class IObserver
{
public:
	virtual void OnEvent(void *pObject, int event) = 0;
};

class IObservable
{
public:
	virtual void AddObserver(IObserver *pObserver) = 0;
	virtual void NotifyObservers(void *pObject, int event) = 0;
};

class Observable 
{
public:
	void AddObserver(IObserver *pObserver);
	void NotifyObservers(void *pObject, int event);
private:
	std::vector<IObserver *> observers;
};

} // namespace

#endif
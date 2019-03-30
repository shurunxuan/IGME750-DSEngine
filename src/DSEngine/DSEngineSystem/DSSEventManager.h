#pragma once
#include <list>
#include <map>
#include <typeindex>
#include "HandlerFunctionBase.h"
typedef std::list<HandlerFunctionBase*> HandlerList;
class DSSEventManager {
public:
	template<typename EventType>
	void publish(EventType * evnt);

	template<class T, class EventType>
	void subscribe(T * instance, void (T::*memberFunction)(EventType *));
private:
	std::map<std::type_index, HandlerList*> subscribers;
};
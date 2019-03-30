#include "DSSEventManager.h"
#include "MemberFunctionHandler.h"
template<typename EventType>
void DSSEventManager::publish(EventType * evnt) {
	HandlerList * handlers = subscribers[typeid(EventType)];

	if (handlers == nullptr) {
		return;
	}

	for (auto & handler : *handlers) {
		if (handler != nullptr) {
			handler->exec(evnt);
		}
	}
}

template<class T, class EventType>
void DSSEventManager::subscribe(T * instance, void (T::*memberFunction)(EventType *)) {
	HandlerList * handlers = subscribers[typeid(EventType)];

	//First time initialization
	if (handlers == nullptr) {
		handlers = new HandlerList();
		subscribers[typeid(EventType)] = handlers;
	}

	handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
}
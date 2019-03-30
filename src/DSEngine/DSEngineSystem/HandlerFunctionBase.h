#pragma once
#include "Event.h"
class HandlerFunctionBase {
	public:

		inline void exec(Event *evnt) {
			call(evnt);
		}
private:
	// Implemented by MemberFunctionHandler
	virtual void call(Event * evnt) = 0;
};
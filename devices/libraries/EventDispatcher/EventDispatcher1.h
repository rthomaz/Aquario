#ifndef EventDispatcher1_h
#define EventDispatcher1_h

#include "vector"

#include "Listener1.h"

namespace ART
{
	template<class T>
	class EventDispatcher1
	{

	public:

		EventDispatcher1();
		~EventDispatcher1();
				
		void							addListener(Listener1<T>* listener);
		bool							removeListener(Listener1<T>* listener);
		bool							throwEvent(void* params);		

	private:

		std::vector<Listener1<T>*>		_listeners;

	};

	template class EventDispatcher1<std::function<void(char*, unsigned char*, unsigned int)>>;
	template class EventDispatcher1<Listener1<std::function<void(char*, unsigned char*, unsigned int)>>>;
}

#endif
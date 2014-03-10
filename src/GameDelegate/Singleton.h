//
//  Singleton.h
//  MagicTT
//
//  Created by tao.wu on 13-4-10.
//
//

#ifndef MagicTT_Singleton_h
#define MagicTT_Singleton_h

#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class Singleton
{
public:
	static inline T* Instance()
	{
		if (0 == _instance.get())
		{
			_instance.reset(new T);
		}
		return _instance.get();
	}
	
	static inline void Destroy()
	{
		
	}
protected:
	Singleton()
	{
		
	}
	virtual ~Singleton()
	{
		
	}
	Singleton(const Singleton&){}
	Singleton & operator = (const Singleton&){};
	
    //	friend class auto_ptr<T>;
	static auto_ptr<T> _instance;
};

//Singleton.cpp
template <typename T>
auto_ptr<T> Singleton<T>::_instance;

#define DECLARE_SINGLETON_CLASS(TYPE)	\
friend class auto_ptr<TYPE>;	\
friend class Singleton<TYPE>;


#endif

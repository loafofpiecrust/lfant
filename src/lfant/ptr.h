
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

// Internal

namespace lfant
{

class ptr_base
{
};

template<typename T>
class ptr : public unique_ptr<T>, public ptr_base
{
public:
	explicit ptr(T* v) :
		unique_ptr<T>(v)
	{
		cout << "ptr(T*)\n";
	}

	operator T*()
	{
		return this->get();
	}

};

template<typename T, typename R>
struct is_ptr : public boost::enable_if_c<boost::is_base_of<ptr_base, T>::value, R>
{
};

template<typename T, typename R>
struct is_not_ptr : public boost::disable_if_c<boost::is_base_of<ptr_base, T>::value, R>
{
};

}

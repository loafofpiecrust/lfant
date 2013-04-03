
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

template<typename T, typename _Dp = default_delete<T> >
class ptr : public unique_ptr<T, _Dp>, public ptr_base
{
public:
	ptr() noexcept
	//	: unique_ptr<T, _Dp>::unique_ptr()
	{
	}

	ptr(T* v) noexcept :
		unique_ptr<T, _Dp>::unique_ptr(v)
	{
	}

	/*
	ptr(const ptr<T>& p) :
		unique_ptr<T>::unique_ptr(p.release())
	{
	}
	*/
	/*
	~ptr()
	{
	}
	*/

	operator T*()
	{
		return this->get();
	}
};

template<typename T>
struct is_ptr : public boost::is_base_of<ptr_base, T>
{
};

template<typename T>
struct is_not_ptr : public boost::is_base_of<ptr_base, T>
{
};

}

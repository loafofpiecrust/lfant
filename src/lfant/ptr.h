
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
class ptr : public ptr_base
{
public:
	typedef T* ptr_type;
	typedef T value_type;

	ptr()
	{
	}

	ptr(ptr_type v) :
		value(v)
	{
		cout << "ptr::ctor(T*): new value: " << value << "\n";
	}

	ptr(ptr<T>& p) :
		value(p.release())
	{
		cout << "ptr::ctor(ptr): new value: " << value << "\n";
	}

	~ptr()
	{
		if(value != nullptr)
		{
			cout << "ptr::dtor: delete " << value << "\n";
			delete value;
		}
	}

	ptr_type get()
	{
		return value;
	}

	void reset(ptr_type v)
	{
		if(value != nullptr)
		{
			cout << "ptr::reset: Trying to delete value, " << value << ".\n";
			delete value;
		}
		cout << "ptr::reset: old value: " << value << "\n";
		value = v;
		cout << "ptr::reset: new value: " << value << "\n";
	}

	operator ptr_type()
	{
		return get();
	}

	ptr<T>& operator=(ptr_type v)
	{
		reset(v);
		return *this;
	}

	ptr<T>& operator=(ptr<T>& p)
	{
		reset(p.release());
		return *this;
	}

	operator bool()
	{
		return value != nullptr;
	}

	bool operator==(ptr_type v)
	{
		return value == v;
	}

	bool operator==(const ptr<T>& p)
	{
		return value == p.value;
	}

	ptr_type operator->()
	{
		if(value)
		{
			return value;
		}
		return nullptr;
	}

	value_type operator*()
	{
		return *value;
	}

private:
	ptr_type value = nullptr;

	ptr_type release()
	{
		ptr_type r = value;
		value = nullptr;
		cout << "ptr::release: Releasing value, " << value << "\n";
		return r;
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

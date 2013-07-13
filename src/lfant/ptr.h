
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
	ptr()
	{
	}

	ptr(T* v) :
		value(v)
	{
	}

	ptr(const ptr<T>& x) = delete;

	ptr(ptr<T>&& x) :
		value(x.release())
	{
	}

	~ptr()
	{
		reset();
	}

	T* get() const noexcept
	{
		return value;
	}

	void reset(T* v = nullptr)
	{
		if(value != nullptr)
		{
			delete value;
		}
		value = v;
	}

	void swap(ptr<T>& x)
	{
		T* v = x.get();
		x.value = get();
		value = v;
	}

	T* release() noexcept
	{
		T* v = value;
		value = nullptr;
		return v;
	}

	operator T*() const
	{
		return get();
	}

	T* operator->() const noexcept
	{
		return get();
	}

	T& operator*() const
	{
		return *get();
	}

	operator bool() const
	{
		return value != nullptr;
	}

	T& operator[](uint32_t i)
	{
		return value[i];
	}

	ptr<T>& operator=(T* v)
	{
		reset(v);
		return *this;
	}

	ptr<T>& operator=(const ptr<T>& x) = delete;
/*	{
		reset(x.release());
		return *this;
	}*/

	ptr<T>& operator=(ptr<T>&& x)
	{
		reset(x.release());
		return *this;
	}

	bool operator==(T* v)
	{
		return get() == v;
	}

private:
	T* value = nullptr;
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

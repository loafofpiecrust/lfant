
#pragma once

// internal

// external
#include <vector>
#include <iostream>

namespace lfant
{

template<typename T1, typename T2>
class qumap : public std::vector<std::pair<T1, T2>>
{
	typedef std::pair<T1, T2> value_type;
	typedef std::vector<value_type> parType;

	using parType::push_back;
public:
	using parType::operator[];
	using parType::erase;
	using parType::size;
	using parType::at;
	using parType::begin;

	void push_back(const value_type& x)
	{

	}

	T2& operator[](const T1& key)
	{
		for(uint i = 0; i < size(); ++i)
		{
			auto& f = operator[](i);
			if(f.first == key)
			{
				return f.second;
			}
		}

		push_back({key, T2()});
		return this->back().second;
	}

	void erase(const T1& key)
	{
		for(uint i = 0; i < size(); ++i)
		{
			if(at(i).first == key)
			{
				parType::erase(begin()+i);
			}
		}
	}
};

}

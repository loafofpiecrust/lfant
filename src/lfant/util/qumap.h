
#pragma once

// internal

// external
#include <deque>
#include <iostream>

namespace lfant {

template<typename T1, typename T2>
class qupair
{
public:
	T1 first;
	T2 second;

	qupair() {}

	qupair(T1 f, T2 s) :
		first(f), second(s)
	{
	}
};

template<typename T1, typename T2>
class qumap : public std::deque<qupair<T1, T2>>
{
	typedef qupair<T1, T2> valType;
	typedef std::deque<valType> parType;
public:
	T2& operator[](const T1& key)
	{
		for(int i = 0; i < this->size(); ++i)
		{
			if(this->at(i).first == key)
			{
				return this->at(i).second;
			}
		}
		this->emplace_back(key, T2());
		return this->at(this->size()-1).second;
	}

	void erase(const T1& key)
	{
		for(uint i = 0; i < this->size(); ++i)
		{
			if(this->at(i).first == key)
			{
				parType::erase(this->begin()+i);
			}
		}
	}
};

}
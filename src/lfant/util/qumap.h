
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

	qupair(T1 f) :
		first(f)
	{
	}
};

template<typename T1, typename T2>
class qumap : public std::deque<qupair<T1, T2>>
{
	typedef qupair<T1, T2> valType;
	typedef std::deque<valType> parType;
public:
	using parType::operator[];
	using parType::erase;

	T2& operator[](const T1& key)
	{
		for(auto& i : *this)
		{
			if(i.first == key)
			{
				return i.second;
			}
		}
	//	valType* v = new valType(key);
	//	this->push_back(*v);
	//	return v->second;
		this->emplace_back(key);
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
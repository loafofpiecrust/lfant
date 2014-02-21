/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// External

// Internal

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

/**
 *
 */
template<typename TEnum, typename TType = byte>
class BitField
{
public:
	BitField()
	{
	}
	~BitField()
	{
	}

	void set(const TEnum& flag)
	{
		value = flag;
	}

	void add(const TEnum& flag)
	{
		value |= flag;
	}

	bool check(const TEnum& flag)
	{
		return value & flag;
	}

	void remove(const TEnum& flag)
	{
		value &= ~flag;
	}

	void clear()
	{
		value = 0;
	}

	TType& operator=(const TEnum& flag)
	{
		set(flag);
	}

	void operator+=(const TEnum& flag)
	{
		add(flag);
	}

	void operator-=(const TEnum& flag)
	{
		remove(flag);
	}

	bool operator==(const TEnum& flag)
	{
		return check(flag);
	}

private:
	TType value;
};

/** @} */
/** @} */
}

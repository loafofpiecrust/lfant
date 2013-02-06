/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-09-30 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once
#include "stdafx.hpp"

// External

// Internal

namespace sfs
{
	/** @addtogroup Engine
	 *	@{
	 */
	/** @addtogroup Types
	 *	@{
	 */

	template<typename T = float>
	class Range
	{
	public:
		union
		{
			T min, start, major, primary, first;
		};
		union
		{
			T max, end, minor, secondary, second;
		};

		T operator[](byte idx)
		{
			if(idx == 0)
			{
				return min;
			}
			else
			{
				return max;
			}
		}

		Range(T one, T two) :
			min(one), max(two)
		{
		}

		Range()
		{
		}
	};

/// @}
/// @}

} /* namespace sfs */

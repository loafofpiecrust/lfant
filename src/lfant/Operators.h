/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-11-01 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once

#include <lfant/stdafx.h>

// External
#include <IdOp.h>

// Internal

namespace lfant
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

template<typename T>
struct OpContains
{
	template<typename O>
	bool operator()(T && left, O && right) const
	{
		return string(left).find(string(right)) != string::npos;
	}
};

IDOP_CREATE_LEFT_HANDED_RET(<, _contains_, >, OpContains, bool)
#define contains <_contains_>

/** @} */
/** @} */
}

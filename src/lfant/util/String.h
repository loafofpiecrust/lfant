/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-10-27 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once


#include <lfant/stdafx.h>

// External
#include <deque>
#include <boost/tokenizer.hpp>

// Internal

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

std::deque<string> Split(string str, string dropDelim = " ", string keepDelim = "");
std::deque<string> Split(std::deque<string> str, string dropDelim = " ", string keepDelim = "");

string Extension(string path);
string Replace(string str, string find, string replace);
string ReplaceAll(string str, string find, string replace);

std::deque<string> SplitParens(string str, string dropDelim = " ", string keepDelim = "");

/** @} */
/** @} */
}

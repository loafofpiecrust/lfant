//---------------------------------------------------------------------------//
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_ALGORITHM_MINMAX_ELEMENT_HPP
#define BOOST_COMPUTE_ALGORITHM_MINMAX_ELEMENT_HPP

#include <utility>

#include <boost/compute/system.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/algorithm/max_element.hpp>
#include <boost/compute/algorithm/min_element.hpp>

namespace boost {
namespace compute {

template<class InputIterator>
inline std::pair<InputIterator, InputIterator>
minmax_element(InputIterator first,
               InputIterator last,
               command_queue &queue = system::default_queue())
{
    if(first == last){
        // empty range
        return std::make_pair(first, first);
    }

    return std::make_pair(min_element(first, last, queue),
                          max_element(first, last, queue));
}

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_ALGORITHM_MINMAX_ELEMENT_HPP

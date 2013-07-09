/*
 * Boost.Extension / main header:
 *         main header for extensions
 *
 * (C) Copyright Jeremy Pack 2007
 * Copyrignt 2008 Stjepan Rajko
 * 
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_DECL_HPP
#define BOOST_EXTENSION_DECL_HPP

#if __GNUC__ >= 4
#  define BOOST_EXTENSION_EXPORT_DECL __attribute__((visibility("default")))
#  define BOOST_EXTENSION_IMPORT_DECL __attribute__((visibility("hidden")))
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(MSC_VER)
#  define BOOST_EXTENSION_EXPORT_DECL __declspec(dllexport)
#  define BOOST_EXTENSION_IMPORT_DECL __declspec(dllimport)
#elif
#  define BOOST_EXTENSION_EXPORT_DECL
#  define BOOST_EXTENSION_IMPORT_DECL
#endif

#endif
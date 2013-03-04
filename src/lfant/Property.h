/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-07-17 by Taylor Snead
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

#include <lfant/stdafx.h>

// External
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/move/move.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>
#include <stddef.h>

// Internal
#include <lfant/TypeInfo.h>

#define RETURN_TYPE(func) boost::function_types::result_type<decltype(func)>::type
#define ARG_TYPE(func, arg) boost::mpl::at_c<boost::function_types::parameter_types<decltype(func)>, arg>::type

#define PROP_RO(CLASSTYPE, NAME, GETFUNC)\
struct CLASSTYPE##NAME : public Property\
{\
	typedef CLASSTYPE##NAME MyType;\
	typedef RETURN_TYPE(&CLASSTYPE::GETFUNC) GETTYPE;\
	operator GETTYPE()\
	{\
		return get();\
	}\
	GETTYPE operator ()() const \
	{\
		return get();\
	}\
	GETTYPE get() const \
	{\
		return reinterpret_cast<CLASSTYPE*>((MyType*)this - offsetof(CLASSTYPE, NAME))->GETFUNC();\
	}\
	GETTYPE operator->() {\
		return get();\
	}\
} NAME;\

#define PROP_RW(CLASSTYPE, NAME, GETFUNC, SETFUNC)\
class prop_CLASSTYPE##NAME : public Property\
{\
public:\
	typedef prop_CLASSTYPE##NAME MyType;\
	typedef RETURN_TYPE(&CLASSTYPE::GETFUNC) GETTYPE;\
	typedef typename remove_ref<GETTYPE>::type GETTYPESIMPLE;\
	typedef ARG_TYPE(&CLASSTYPE::SETFUNC, 1) SETTYPE;\
	operator const GETTYPE()\
	{\
		return get();\
	}\
	GETTYPE operator ()() const \
	{\
		return get();\
	}\
	GETTYPE get() const \
	{\
	   return reinterpret_cast<CLASSTYPE*>((MyType*)(this) - offsetof(CLASSTYPE, NAME))->GETFUNC();\
	}\
	GETTYPE set(SETTYPE value)\
	{\
	   reinterpret_cast<CLASSTYPE*>((MyType*)(this) - offsetof(CLASSTYPE, NAME))->SETFUNC(value);\
	   return get();\
	}\
	GETTYPE operator =(SETTYPE value)\
	{\
		set(value);\
		return get();\
	}\
	GETTYPE operator->() {\
		return get();\
	}\
} NAME;

#define PROP_BINARY_OP(OP) \
 template <class TA, class TB> \
auto operator OP (TA a, TB& b) -> typename boost::enable_if_c<!is_property<TA>::value && is_property<TB>::value, typename remove_ref<TA>::type >::type \
{ \
  return a OP b.get();\
}\
template <class TA, class TB> \
auto operator OP (TA& a, TB b) -> typename boost::enable_if_c<is_property<TA>::value && !is_property<TB>::value, typename remove_ref<typename TA::GETTYPE>::type >::type \
{ \
  return a.get() OP b;\
}\
template <class TA, class TB> \
auto operator OP ( TA& a, TB& b) -> typename enable_if_both_prop<TA, TB, typename remove_ref<typename TA::GETTYPE>::type >::type \
{ \
  return a.get() OP b.get();\
}

#define PROP_BOOL_OP(OP) \
template <class TA, class TB> \
auto operator OP (const TA& a, const TB& b) -> typename enable_if_any_prop<TA, TB, bool>::type \
{ \
  return eval(a) OP eval(b);\
}

#define PROP_COMPOUND_OP(OPC, OP) \
template <class TA, class TB> \
auto operator OPC (TA& a, TB& b) -> typename boost::enable_if_c<!is_property<TA>::value && is_property<TB>::value, typename remove_ref<TA>::type >::type \
{ \
  return a = a OP b.get();\
}\
template <class TA, class TB> \
auto operator OPC (TA& a, TB b) -> typename boost::enable_if_c<is_property<TA>::value && !is_property<TB>::value, typename remove_ref<typename TA::GETTYPE>::type >::type \
{ \
  return a.set(a.get() OP b);\
}\
template <class TA, class TB> \
auto operator OPC (TA& a, TB& b) -> typename boost::enable_if_c<is_property<TA>::value && is_property<TB>::value, typename remove_ref<typename TA::GETTYPE>::type >::type \
{ \
  return a.set(a.get() OP b.get());\
}

namespace lfant
{

class Property
{
};

template<typename T>
struct is_property : public boost::is_base_of<Property, typename remove_ref<T>::type>
{
};

template<typename TA, typename TB, typename R>
struct enable_if_any_prop : public boost::enable_if_c < is_property<TA>::value || is_property<TB>::value,
		typename remove_ref<R>::type >
{
};

template<typename TA, typename TB, typename R>
struct enable_if_both_prop : public boost::enable_if_c < is_property<TA>::value && is_property<TB>::value,
		typename remove_ref<R>::type >
{
};

template<typename TA, typename R>
struct enable_if_prop : public boost::enable_if_c<is_property<TA>::value, typename remove_ref<R>::type>
{
};

template<typename TA, typename R>
struct enable_if_not_prop : public boost::enable_if_c < !is_property<TA>::value, typename remove_ref<R>::type >
{
};
template<typename T>
auto eval(T& v) -> typename enable_if_not_prop<T, T>::type
{
	return v;
}

template<typename T>
auto eval(T& v) -> typename enable_if_prop<T, typename T::GETTYPE >::type
{
	return v.get();
}

PROP_BINARY_OP(+)
PROP_BINARY_OP(-)
PROP_BINARY_OP(*)
PROP_BINARY_OP( /)
PROP_BINARY_OP( %)

PROP_BOOL_OP( >)
PROP_BOOL_OP( <)
PROP_BOOL_OP( >=)
PROP_BOOL_OP( <=)
PROP_BOOL_OP( ==)
PROP_BOOL_OP( !=)
PROP_BOOL_OP( &&)
PROP_BOOL_OP( ||)

PROP_COMPOUND_OP( += , +)
PROP_COMPOUND_OP( -= , -)
PROP_COMPOUND_OP( *= , *)
PROP_COMPOUND_OP( /= , /)
PROP_COMPOUND_OP( %= , %)
}

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
/*template<class C, typename G, typename S>
 class Property
 {
 public:
 typedef G (C::*getType)();
 typedef void (C::*setType)( S );

 Property( C* ptr, getType get ) :
 instance( ptr ), get( get ), readOnly( true )
 {
 }

 Property( C* ptr, getType get, setType set ) :
 instance( ptr ), get( get ), set( set )
 {
 }

 ~Property()
 {
 }

 operator G() const
 {
 return (instance->*get)();
 }

 operator S()
 {
 return (instance->*get)();
 }

 G operator=( S value ) const
 {
 if (!readOnly) (instance->*set)( value );
 return operator G();
 }

 template<typename TT>
 bool operator==( TT other ) const
 {
 return operator G() == other;
 }

 G operator()() const
 {
 return operator G();
 }

 G operator()( S arg )
 {
 operator=( arg );
 return operator G();
 }

 G operator->() const
 {
 return operator G();
 }

 Property<C, G, S>& operator()( C* ptr, getType get, setType set )
 {
 return *(new Property( ptr, get, set ));
 }

 private:

 C* instance;
 getType get;
 setType set;
 bool readOnly = false;
 };*/

/**	@}*/
/**	@}*/
}

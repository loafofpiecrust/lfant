
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/util/qumap.h>

// external

namespace lfant {

#define DECLARE_REGISTRY(type) \
	public:\
	static TypeRegistry<type> registry; \
	template<typename C> \
	class RegistryEntry \
	{ \
	public:\
		RegistryEntry(string name) { \
			type::registry.Register<C>(name); \
		} \
	};\
	type* New(string name) {\
		return registry.New(name);\
	}

#define IMPLEMENT_REGISTRY(type) TypeRegistry<type> type::registry __attribute__((init_priority(101)));

#define DECLARE_TYPE(parent, type) \
	static const parent::RegistryEntry<type> _registryEntry;

#define IMPLEMENT_TYPE_NAME(parent, type, name) \
	const parent::RegistryEntry<type> type::_registryEntry {#type};

#define IMPLEMENT_TYPE(parent, type) \
	IMPLEMENT_TYPE_NAME(parent, type, type)

template<typename T>
class TypeRegistry
{
public:

	typedef T*(*funcType)();
	template<typename C>
	static T* Create()
	{
		return new C();
	}

	template<typename C>
	void Register(string name)
	{
		data[name] = &TypeRegistry<T>::Create<C>;
	}

	template<typename C>
	void Add(string name)
	{
		this->Register<C>(name);
	}

	funcType Get(string name)
	{
		return data[name];
	}

	T* New(string name)
	{
		funcType f = data[name];
		if(f)
		{
			return f();
		}
		return nullptr;
	}

protected:
	qumap<string, funcType> data;
};

}
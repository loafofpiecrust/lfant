
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/util/qumap.h>

// external

namespace lfant {

template<typename T>
class RegistryEntryBase
{
public:
	typedef T*(*funcType)();

	bool Inherits(string super, bool recursive = true)
	{
		if(!parent) return false;

		if(!recursive)
		{
			return super == parent->name || super == parent->typeName;
		}
		else
		{
			RegistryEntryBase* superInst = parent;
			while(superInst)
			{
				if(super == superInst->name || super == superInst->typeName)
				{
					return true;
				}
				else
				{
					superInst = superInst->parent;
				}
			}
		}
		return false;
	}

	string typeName = "";
	string name = "";
	RegistryEntryBase* parent = nullptr;
	funcType func;
};

#define DECLARE_REGISTRY(regType) \
	public:\
	static TypeRegistry<regType> registry; \
	template<typename C> \
	class RegistryEntry : public RegistryEntryBase<regType> \
	{ \
	public:\
		RegistryEntry(string name, string parent = "") { \
			if(parent != "") this->parent = regType::registry.Get(parent); \
			this->typeName = type::Name<C>(); \
			this->name = name; \
			regType::registry.Register<C>(this); \
		} \
	};\
	regType* New(string name) {\
		return registry.New(name);\
	}

#define IMPLEMENT_REGISTRY(type) TypeRegistry<type> type::registry __attribute__((init_priority(101)));

#define DECLARE_TYPE(parent, type) \
	static const parent::RegistryEntry<type> _registryEntry;

#define IMPLEMENT_TYPE(parent, type) \
	const parent::RegistryEntry<type> type::_registryEntry {#type};

#define IMPLEMENT_SUBTYPE(parent, type, super) \
	const parent::RegistryEntry<type> type::_registryEntry {#type, #super};

template<typename T>
class TypeRegistry
{
public:
	typedef T*(*funcType)();
	typedef RegistryEntryBase<T> entryType;

	template<typename C>
	static T* Create()
	{
		return new C();
	}

	template<typename C>
	void Register(entryType* inst)
	{
		for(uint i = 0; i < data.size(); ++i)
		{
			if(data[i]->name == inst->name)
			{
				data[i] = inst;
				inst->func = &TypeRegistry<T>::Create<C>;
				return;
			}
		}
		data.push_back(inst);
		inst->func = &TypeRegistry<T>::Create<C>;
	}

	entryType* Get(string name)
	{
		to_lower(name);
		for(auto& entry : data)
		{
			string entryName = entry->name; to_lower(entryName);
			string entryType = entry->typeName; to_lower(entryType);
			if(name == entryName || name == entryType)
			{
				return entry;
			}
		}
		return nullptr;
	}

	T* New(string name)
	{
		funcType f = Get(name)->func;
		if(f)
		{
			return f();
		}
		return nullptr;
	}

protected:
	deque<entryType*> data;
};

}

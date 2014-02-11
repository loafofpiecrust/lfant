
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/util/qumap.h>

// external

namespace lfant {

class RegistryEntryBase
{
public:
	typedef Object*(*funcType)();

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
	
	string typeName;
	RegistryEntryBase* parent = nullptr;
	funcType func;
};

template<typename C>
class RegistryEntry : public RegistryEntryBase 
{ 
public:
	RegistryEntry(string base, string name, string parent = "") { 
		if(parent != "") this->parent = registry.Get(parent); 
		this->typeName = type::Name<C>();
		regType::registry.Register<C>(base, name, this); 
	} 
	regType* New(string name) {
		return registry.New(name);
	}
};

#define DECLARE_REGISTRY(regType) \
	public:\
	static TypeRegistry<regType> registry; \
	template<typename C> \
	

#define IMPLEMENT_REGISTRY(type) TypeRegistry<type> type::registry __attribute__((init_priority(101)));

#define DECLARE_TYPE(parent, type) \
	static const parent::RegistryEntry<type> _registryEntry;

#define IMPLEMENT_TYPE(parent, type) \
	const RegistryEntry<type> type::_registryEntry {#parent, #type};

#define IMPLEMENT_SUBTYPE(parent, type, super) \
	const RegistryEntry<type> type::_registryEntry {#parent, #type, #super};

class TypeRegistry
{
public:
	typedef Object*(*funcType)();
	typedef RegistryEntryBase entryType;

	template<typename C>
	static C* Create()
	{
		return new C();
	}

	template<typename C>
	void Register(string base, string name, entryType* inst)
	{
		if(Get(base, name))
		{
			// do something?
		}
		data[{base, name}] = inst;
		inst->func = &TypeRegistry::Create<C>;
	}

	entryType* Get(string base, string name)
	{
	/*	for(auto& entry : data)
		{
			string entryName = entry->name; to_lower(entryName);
			string entryType = entry->typeName; to_lower(entryType);
			if(name == entryName || name == entryType)
			{
				return entry;
			}
		}*/
		return data[{base, name}];
	}

	Object* New(string base, string name)
	{
		funcType f = Get(base, name)->func;
		if(f)
		{
			return f();
		}
		return nullptr;
	}

protected:
	unordered_map<pair<string, string>, RegistryEntryBase<T>*> data;
};

TypeRegistry typeRegistry;

}

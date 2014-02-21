
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/util/qumap.h>
#include <lfant/TypeInfo.h>

// external
#include <unordered_map>

namespace lfant {


#define DECLARE_REGISTRY(regType) \
public:\
	static lfant::TypeRegistry<regType> typeRegistry;\
public:\
	static regType* NewFromString(string type) {\
		return typeRegistry.New(type);\
	}

#define IMPLEMENT_REGISTRY(type) lfant::TypeRegistry<type> type::typeRegistry __attribute__((init_priority(101)));

#define IMPLEMENT_TYPE(parent, type) \
	static const lfant::TypeRegistry<parent>::Entry<type> type_registryEntry_ {parent::typeRegistry, #type};

#define IMPLEMENT_SUBTYPE(parent, type, super) \
	static const lfant::TypeRegistry<parent>::Entry<type> type_registryEntry_ {parent::typeRegistry, #type, #super};

template<typename BaseT>
class TypeRegistry
{
public:
	typedef BaseT*(*funcType)();

	class EntryBase
	{
	public:
		typedef BaseT*(*funcType)();

		bool Inherits(string super, bool recursive = true)
		{
			/// @todo Redo this?
			if(!parent) return false;

		/*	if(super == parent->name || super == parent->typeName)
			{
				return true;
			}
			else if(recursive)
			{
				return parent->Inherits(super, true);
			}*/
			return false;
		}

		string typeName;
		EntryBase* parent = nullptr;
		funcType func;

	protected:

		void SetFunc(funcType f)
		{
			func = f;
		}
	};

	template<typename C>
	class Entry : public TypeRegistry<BaseT>::EntryBase
	{
	public:
		Entry(TypeRegistry<BaseT>& registry, string name, string parent = "")
		{
			if(parent != "") this->parent = registry.Get(parent);
			this->typeName = type::Name<C>();
			this->SetFunc((funcType)&Entry<C>::New);
			registry.Register<C>(name, this);
		}

		static C* New()
		{
			return new C();
		}
	};

	typedef EntryBase entryType;

	template<typename C>
	void Register(string name, entryType* inst)
	{
		entryType*& ref = data[name];
		if(ref)
		{
			// do something?
		}
		else
		{
			ref = inst;
		}
	}

	entryType* Get(string name)
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
		return data[name];
	}

	BaseT* New(string name)
	{
		funcType f = Get(name)->func;
		if(f)
		{
			return f();
		}
		return nullptr;
	}

protected:
	std::unordered_map<string, entryType*> data;
};




}

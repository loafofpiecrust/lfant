
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/util/qumap.h>
#include <lfant/TypeInfo.h>

// external
#include <unordered_map>
#include <typeindex>

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

/// @todo Please, please find a better way to do this
template<typename BaseT>
class TypeRegistry
{
public:
	typedef BaseT*(*funcType)();

	class EntryBase
	{
	public:
		typedef BaseT*(*funcType)();

		EntryBase(const std::type_info& ti) :
			typeInfo(ti)
		{
		}

		virtual ~EntryBase() {}

		bool Inherits(string super, bool recursive = true)
		{
			/// @todo Redo this?
			if(!parent) return false;

			if(super == parent->name || super == parent->typeName)
			{
				return true;
			}
			else if(recursive)
			{
				return parent->Inherits(super, true);
			}
			return false;
		}

		bool Inherits(EntryBase* super, bool recursive = true)
		{
			if(!parent) return false;

			if(super == parent)
				return true;
			else if(recursive)
				return parent->Inherits(super, true);
			else
				return false;
		}

		bool Inherits(const std::type_info& ti)
		{
			if(parent)
			{
				return parent->typeInfo == ti;
			}
			return false;
		}

		bool Is(const std::type_info& ti)
		{
			return typeInfo == ti;
		}

		string name = "";
		string typeName = "";
		const std::type_info& typeInfo;
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
		Entry(TypeRegistry<BaseT>& registry, string name, string parent = "") :
			EntryBase(typeid(C))
		{
			if(parent != "") this->parent = registry.Get(parent);
			this->name = name;
			this->typeName = type::Name<C>();
			this->SetFunc((funcType)&Entry<C>::New);
			registry.Register<C>(name, this);
		}

		virtual ~Entry() {}

	private:
		static C* New()
		{
			return new C();
		}
	};

	typedef EntryBase entryType;

	template<typename C>
	void Register(string name, entryType* inst)
	{
		auto it = data.find(name);
	//	entryType*& ref = data[name];
		if(it != data.end())
		{
			// do something?
		}
		else
		{
			data.insert(make_pair(name, std::unique_ptr<entryType>(inst)));
			dataReverse[typeid(C)] = inst;
		}
	}

	entryType* Get(string name)
	{
		auto it = data.find(name);
		if(it == data.end())
			return nullptr;
		else
			return it->second.get();
	}

	entryType* Get(const std::type_info& ti)
	{
		return dataReverse.at(ti);
	}

	BaseT* New(string name)
	{
		auto entry = Get(name);
		if(entry)
		{
			return static_cast<BaseT*>(entry->func());
		}
		return nullptr;
	}

protected:
	std::unordered_map<string, std::unique_ptr<entryType>> data;
	std::unordered_map<std::type_index, entryType*> dataReverse;
};




}

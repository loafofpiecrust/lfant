
// Internal
#include <lfant/Subsystem.h>

// External

class asIScriptEngine;
class CScriptBuilder;

namespace lfant
{

class Script
{
public:

	class Class
	{
	public:
		Class() {}

		template<typename T>
		void Create(string type, bool ref)
		{
			Create(type, ref, sizeof(T));
		}

		template<typename T, typename R>
		void Method(string decl, R (T::*func));

		void Variable();
		void Operator();


	private:
		void Create(string type, bool ref, int size);

		string type;
	};

	Script(string module);

	void LoadFile(string path);
	void Compile();

private:
	CScriptBuilder* builder;
};

/**
 *
 */
class ScriptSystem : public Subsystem
{
	friend class Script;
public:

	virtual void Init();

	void OnDestroy();

	template<typename T>
	void RegisterFunction(string decl, T* func);

	template<typename T>
	void RegisterVariable(string name, T* var);


	void CallFunction(string module, string call);

protected:

private:

	asIScriptEngine* scriptEngine;

};

}

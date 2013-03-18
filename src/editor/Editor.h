
#pragma once
#include <lfant/stdafx.h>

// External
#include <forward_list>

// Internal
#include <lfant/Engine.h>


namespace lfant
{
namespace editor
{

/**
 *
 */
class Editor : public Engine
{
public:
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();


protected:

private:

};

extern "C" void Launch();

}
}

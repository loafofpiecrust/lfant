
#pragma once
#include <lfant/stdafx.h>

// External
#include <wx/app.h>

// Internal
#include <editor/gui/Types.h>
#include <lfant/Object.h>
#include <lfant/ptr.h>

namespace lfant
{
namespace editor
{
namespace gui
{

class Frame;

/**
 *
 */
class App : public AppBase
{
public:
	App();
	virtual ~App();

	virtual bool OnInit();

protected:

	ptr<Frame> frame;

private:
};

}
}
}

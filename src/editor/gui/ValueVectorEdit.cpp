#include "ValueVectorEdit.h"

#include "editor/gui/ValueDoubleSpinBox.h"

#include <QHBoxLayout>

namespace lfant {
namespace editor {
namespace gui {

ValueVectorEdit::ValueVectorEdit(QWidget *parent, boost::any& val) :
	QWidget(parent)
{
	setLayout(new QHBoxLayout(this));
	setPointer(val);
}

void ValueVectorEdit::setPointer(boost::any& val)
{
	if(val.type() == typeid(vec2*))
	{
		val_vec2 = boost::any_cast<vec2*>(val);
		for(int i = 0; i < 2; ++i)
		{
			auto vdsb = new ValueDoubleSpinBox(this);
			layout()->addWidget(vdsb);
			vdsb->setPointer(&((*val_vec2)[i]));
		}
	}
	if(val.type() == typeid(vec3*))
	{
		val_vec3 = boost::any_cast<vec3*>(val);
		for(int i = 0; i < 3; ++i)
		{
			auto vdsb = new ValueDoubleSpinBox(this);
			layout()->addWidget(vdsb);
			vdsb->setPointer(&((*val_vec3)[i]));
		}
	}
	if(val.type() == typeid(vec4*))
	{
		val_vec4 = boost::any_cast<vec4*>(val);
		for(int i = 0; i < 4; ++i)
		{
			auto vdsb = new ValueDoubleSpinBox(this);
			layout()->addWidget(vdsb);
			vdsb->setPointer(&((*val_vec4)[i]));
		}
	}
}

} // namespace gui
} // namespace editor
} // namespace lfant

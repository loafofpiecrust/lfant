#include "ValueDoubleSpinBox.h"

namespace lfant {
namespace editor {
namespace gui {

ValueDoubleSpinBox::ValueDoubleSpinBox(QWidget *parent) :
	QDoubleSpinBox(parent)
{
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(setPointerValue(double)));
}

void ValueDoubleSpinBox::setPointer(double* ptr)
{
	value_ptr = ptr;
	float_ptr = nullptr;
}

void ValueDoubleSpinBox::setPointer(float* ptr)
{
	float_ptr = ptr;
	value_ptr = nullptr;
}

void ValueDoubleSpinBox::setPointerValue(double val)
{
	if(value_ptr)
	{
		*value_ptr = val;
	}
	else if(float_ptr)
	{
		*float_ptr = (float)val;
	}
}

void ValueDoubleSpinBox::update()
{
	if(value_ptr)
	{
		setValue(*value_ptr);
	}
	else if(float_ptr)
	{
		setValue((double)*float_ptr);
	}
	QDoubleSpinBox::update();
}

} // namespace gui
} // namespace editor
}

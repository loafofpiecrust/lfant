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
	float_ptr = nullptr;
	value_ptr = ptr;
}

void ValueDoubleSpinBox::setPointer(float* ptr)
{
	value_ptr = nullptr;
	float_ptr = ptr;
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

void ValueDoubleSpinBox::paintEvent(QPaintEvent *event)
{
	if(value_ptr)
	{
		setValue(*value_ptr);
	}
	else if(float_ptr)
	{
		setValue((double)*float_ptr);
	}
	QDoubleSpinBox::paintEvent(event);
}

} // namespace gui
} // namespace editor
}

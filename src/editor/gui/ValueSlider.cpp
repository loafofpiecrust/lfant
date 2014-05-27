#include "ValueSlider.h"

namespace lfant {
namespace editor {
namespace gui {

ValueSlider::ValueSlider(QWidget *parent) :
	QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(setPointerValue(int)));
	setOrientation(Qt::Orientation::Horizontal);
}

void ValueSlider::setPointer(boost::any& val)
{
	if(val.type() == typeid(int16*))
		val_int16 = boost::any_cast<int16*>(val);
	else if(val.type() == typeid(uint16*))
		val_uint16 = boost::any_cast<uint16*>(val);
	else if(val.type() == typeid(int32*))
		val_int32 = boost::any_cast<int32*>(val);
	else if(val.type() == typeid(uint32*))
		val_uint32 = boost::any_cast<uint32*>(val);
}


void ValueSlider::setPointerValue(int val)
{
	if(val_int16)
		*val_int16 = (int16)val;
	else if(val_uint16)
		*val_uint16 = (uint16)val;
	else if(val_int32)
		*val_int32 = (int32)val;
	else if(val_uint32)
		*val_uint32 = (uint32)val;
}

void ValueSlider::paintEvent(QPaintEvent *event)
{
	if(val_int16)
		setValue((int)*val_int16);
	else if(val_uint16)
		setValue((int)*val_uint16);
	else if(val_int32)
		setValue((int)*val_int32);
	else if(val_uint32)
		setValue((int)*val_uint32);
	QSlider::paintEvent(event);
}

}
} // namespace editor
}

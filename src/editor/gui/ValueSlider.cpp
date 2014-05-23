#include "ValueSlider.h"

namespace lfant {
namespace editor {
namespace gui {

ValueSlider::ValueSlider(QWidget *parent) :
	QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(setPointerValue(int)));
}

void ValueSlider::setInternalValue()
{
	setValue(*value_ptr);
}

}
} // namespace editor
}

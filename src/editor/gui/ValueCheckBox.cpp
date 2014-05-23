#include "ValueCheckBox.h"

namespace lfant {
namespace editor {
namespace gui {

ValueCheckBox::ValueCheckBox(QWidget *parent) :
	QCheckBox(parent)
{
	connect(this, SIGNAL(toggled(bool)), this, SLOT(setPointerValue(bool)));
}

void ValueCheckBox::setInternalValue()
{
	setChecked(*value_ptr);
}

} // namespace gui
} // namespace editor
} // namespace lfant

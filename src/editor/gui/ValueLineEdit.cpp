#include "ValueLineEdit.h"

namespace lfant {
namespace editor {
namespace gui {

ValueLineEdit::ValueLineEdit(QWidget *parent) :
	QLineEdit(parent)
{
	connect(this, SIGNAL(textEdited(QString)), this, SLOT(setPointerValue(QString)));
}

void ValueLineEdit::setInternalValue()
{
	setText(QString::fromStdString(*value_ptr));
}


} // namespace gui
} // namespace editor
}

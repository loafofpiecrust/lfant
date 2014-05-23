#ifndef EDITOR_GUI_VALUELINEEDIT_H
#define EDITOR_GUI_VALUELINEEDIT_H

#include <QLineEdit>
#include "ValueWidget.h"

namespace lfant {
namespace editor {
namespace gui {

class ValueLineEdit : public QLineEdit
{
	Q_OBJECT
	ValueWidget(QLineEdit, std::string)
public:
	explicit ValueLineEdit(QWidget *parent = 0);

private:
	void setInternalValue();

};

} // namespace gui
} // namespace editor
}

#endif // EDITOR_GUI_VALUELINEEDIT_H

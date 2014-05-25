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
public:
	explicit ValueLineEdit(QWidget *parent = 0);

	void setPointer(std::string* ptr)
	{
		value_ptr = ptr;
	}

public slots:
	void setPointerValue(std::string value)
	{
		if(value_ptr) *value_ptr = value;
	}

	void paintEvent(QPaintEvent *event)
	{
		setInternalValue();
		QLineEdit::paintEvent(event);
	}

private:
	std::string* value_ptr;

private:
	void setInternalValue();

};

} // namespace gui
} // namespace editor
}

#endif // EDITOR_GUI_VALUELINEEDIT_H

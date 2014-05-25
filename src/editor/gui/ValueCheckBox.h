#ifndef LFANT_EDITOR_GUI_VALUECHECKBOX_H
#define LFANT_EDITOR_GUI_VALUECHECKBOX_H

#include <QCheckBox>
#include "ValueWidget.h"

namespace lfant {
namespace editor {
namespace gui {

class ValueCheckBox : public QCheckBox
{
	Q_OBJECT
public:
	explicit ValueCheckBox(QWidget *parent = 0);

	void setPointer(bool* ptr)
	{
		value_ptr = ptr;
	}

public slots:
	void setPointerValue(bool value)
	{
		if(value_ptr) *value_ptr = value;
	}

	void paintEvent(QPaintEvent *event)
	{
		setInternalValue();
		QCheckBox::paintEvent(event);
	}

private:
	bool* value_ptr;

	void setInternalValue();

};

} // namespace gui
} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_GUI_VALUECHECKBOX_H

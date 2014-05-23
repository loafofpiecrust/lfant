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
	ValueWidget(QCheckBox, bool)
public:
	explicit ValueCheckBox(QWidget *parent = 0);

signals:

public slots:

private:
	void setInternalValue();

};

} // namespace gui
} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_GUI_VALUECHECKBOX_H

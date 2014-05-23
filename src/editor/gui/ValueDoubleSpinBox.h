#ifndef EDITOR_GUI_VALUEDOUBLESPINBOX_H
#define EDITOR_GUI_VALUEDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include "ValueWidget.h"

namespace lfant {
namespace editor {
namespace gui {

class ValueDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
//	ValueWidget(QDoubleSpinBox, double)
public:
	explicit ValueDoubleSpinBox(QWidget *parent = 0);

	void setPointer(float* ptr);
	void setPointer(double* ptr);

public slots:
	void setPointerValue(double val);
	void update();

private:
	float* float_ptr;
	double* value_ptr;
};

} // namespace gui
} // namespace editor
}

#endif // EDITOR_GUI_VALUEDOUBLESPINBOX_H

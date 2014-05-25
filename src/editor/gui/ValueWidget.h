#ifndef LFANT_EDITOR_GUI_VALUEWIDGET_H
#define LFANT_EDITOR_GUI_VALUEWIDGET_H

#include <QWidget>

namespace lfant {
namespace editor {
namespace gui {

#define ValueWidget(_Parent, _ValType) \
public:\
	void setPointer(_ValType* ptr)\
	{\
		value_ptr = ptr;\
	}\
public slots:\
	void setPointerValue(_ValType value)\
	{\
		if(value_ptr) *value_ptr = value;\
	}\
	void paintEvent(QPaintEvent *event)\
	{\
		setInternalValue();\
		_Parent::paintEvent(event);\
	}\
private:\
	_ValType* value_ptr;

} // namespace gui
} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_GUI_VALUEWIDGET_H

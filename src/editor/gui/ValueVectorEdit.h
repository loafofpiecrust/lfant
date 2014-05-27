#ifndef LFANT_EDITOR_GUI_VALUEVECTOREDIT_H
#define LFANT_EDITOR_GUI_VALUEVECTOREDIT_H

#include "lfant/stdafx.h"

#include <QWidget>

#include <boost/any.hpp>

namespace lfant {
namespace editor {
namespace gui {

class ValueVectorEdit : public QWidget
{
	Q_OBJECT
public:
	explicit ValueVectorEdit(QWidget *parent, boost::any& val);

	void setPointer(boost::any& val);

signals:

public slots:

private:
	union
	{
		vec2* val_vec2;
		vec3* val_vec3;
		vec4* val_vec4;
	};

};

} // namespace gui
} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_GUI_VALUEVECTOREDIT_H

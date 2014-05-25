#ifndef EDITOR_VALUESLIDER_H
#define EDITOR_VALUESLIDER_H

#include "lfant/stdafx.h"

#include <QSlider>
#include "ValueWidget.h"

#include <boost/any.hpp>

namespace lfant {
namespace editor {
namespace gui {

class ValueSlider : public QSlider
{
	Q_OBJECT
//	ValueWidget(QSlider, int)
public:
	explicit ValueSlider(QWidget *parent = nullptr);

	void setPointer(boost::any& val);

public slots:
	void setPointerValue(int val);
	void paintEvent(QPaintEvent* event);

private:
	union
	{
		int16* val_int16;
		uint16* val_uint16;
		int32* val_int32;
		uint32* val_uint32;
	};

};

}
} // namespace editor
}

#endif // EDITOR_VALUESLIDER_H

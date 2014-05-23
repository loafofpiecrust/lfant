#ifndef EDITOR_VALUESLIDER_H
#define EDITOR_VALUESLIDER_H

#include <QSlider>
#include "ValueWidget.h"

namespace lfant {
namespace editor {
namespace gui {

class ValueSlider : public QSlider
{
	Q_OBJECT
	ValueWidget(QSlider, int)
public:
	explicit ValueSlider(QWidget *parent = nullptr);

signals:

public slots:

private:
	void setInternalValue();

};

}
} // namespace editor
}

#endif // EDITOR_VALUESLIDER_H

#ifndef LFANT_EDITOR_INSPECTOR_H
#define LFANT_EDITOR_INSPECTOR_H

#include <lfant/Entity.h>

#include <QWidget>
#include <QFormLayout>

namespace lfant {
namespace editor {

class Inspector : public QWidget
{
	Q_OBJECT
public:
	explicit Inspector(QWidget* parent = nullptr);

	QWidget* CreateInput(const Properties::Value& val, QWidget* parent);
	void AddChild(Properties* prop, QWidget* parent);

signals:

public slots:
	void Refresh();
	void SetEntity(QString name);
	void SetEntity(lfant::Entity* ent);

	void SetGame(lfant::Game* game);

private:
	lfant::Game* game = nullptr;
	lfant::Entity* currentEnt = nullptr;
	lfant::Properties* currentProp = nullptr;
};

} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_INSPECTOR_H

#ifndef LFANT_EDITOR_ENTITYTREE_H
#define LFANT_EDITOR_ENTITYTREE_H

#include <lfant/Entity.h>

#include <QTreeWidget>

namespace lfant {
namespace editor {

class EntityTree : public QTreeWidget, public Object
{
	Q_OBJECT
public:
	explicit EntityTree(QWidget *parent = 0);

signals:
	void EntityChanged(lfant::Entity*);

public slots:
	void ItemChanged(QTreeWidgetItem* selected, QTreeWidgetItem* deselected);

	void SetGame(lfant::Game* game);

private:
	void EntityAdded(Entity* ent);

	lfant::Game* game = nullptr;
	void AddEntity(Entity* ent, QTreeWidgetItem* parent);
};

} // namespace editor
} // namespace lfant

#endif // LFANT_EDITOR_ENTITYTREE_H

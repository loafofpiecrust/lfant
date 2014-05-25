#include "EntityTree.h"

#include <lfant/Game.h>
#include <lfant/Scene.h>

#include <editor/MainWindow.h>

#include <QDebug>

namespace lfant {
namespace editor {

EntityTree::EntityTree(QWidget *parent) :
	QTreeWidget(parent),
	Object()
{
	qDebug() << "i got here yay\n";
	connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(ItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
//	game = ((MainWindow*)window())->game;

//	qDebug() << "windu: " << window()->window()->objectName() << "\n";
//	lfant::editor::MainWindow* win = static_cast<lfant::editor::MainWindow*>(window());
//	connect(win, SIGNAL(SetGame(lfant::Game*)), this, SLOT(SetGame(lfant::Game*)));
}

void EntityTree::ItemChanged(QTreeWidgetItem* selected, QTreeWidgetItem* deselected)
{
	if(!game) return;
	qDebug() << "EntityTree::ItemChanged()";
	void* dat = selected->data(1, Qt::UserRole).value<void*>();
	Entity* datEnt = reinterpret_cast<Entity*>(dat);
	qDebug() << dat << " and " << datEnt << "\n";
	EntityChanged(reinterpret_cast<Entity*>(dat));
}

void EntityTree::SetGame(Game* game)
{
	qDebug() << "tree game set";
	this->game = game;
	ConnectEvent(SENDER(game->scene.get(), AddEntity), RECEIVER(this, EntityAdded));

	EntityAdded(game->scene->GetRoot());
}

void EntityTree::AddEntity(Entity *ent, QTreeWidgetItem* parent)
{
	qDebug() << "add ent " << (void*)ent;
	auto item = new QTreeWidgetItem();
	item->setData(0, Qt::DisplayRole, QString::fromStdString(ent->GetName()));
	item->setData(1, Qt::UserRole, qVariantFromValue((void*)ent));
	parent->addChild(item);

	for(auto& child : ent->GetChildren())
	{
		AddEntity(child, item);
	}
}

void EntityTree::EntityAdded(Entity* ent)
{
	if(ent->GetParent())
	{
		auto items = findItems(QString::fromStdString(ent->GetParent()->GetName()), Qt::MatchRecursive|Qt::MatchFixedString);
		for(auto& item : items)
		{
			QVariant dat = item->data(1, Qt::UserRole);
			if(dat.data() == (void*)(ent->GetParent()))
			{
				// found it
				AddEntity(ent, item);
				break;
			}
		}
	}
	else
	{
		qDebug() << "its a rut!\n";
		AddEntity(ent, invisibleRootItem());
	}
}

} // namespace editor
} // namespace lfant

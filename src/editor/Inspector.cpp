#include "Inspector.h"

#include <editor/MainWindow.h>
#include <editor/gui/ValueDoubleSpinBox.h>
#include <editor/gui/ValueSlider.h>
#include <editor/gui/ValueLineEdit.h>

#include <lfant/Scene.h>
#include <lfant/Entity.h>

#include <QFormLayout>
#include <QSlider>
#include <QSpinBox>
#include <QScrollArea>
#include <QCheckBox>
#include <QLineEdit>
#include <QDebug>
#include <QMenuBar>

#include "ui_MainWindow.h"

namespace lfant {
namespace editor {

Inspector::Inspector(QWidget* parent) :
	QWidget(parent),
	currentProp(new Properties)
{
	QFormLayout* form = dynamic_cast<QFormLayout*>(layout());
	if(!form)
	{
		form = new QFormLayout(this);
		setLayout(form);
	}
	qDebug() << "doin a thing\n";

//	game = ((MainWindow*)window())->game;
//	connect(((MainWindow*)window())->ui->actionRefresh, SIGNAL(triggered()), this, SLOT(Refresh()));
//	connect(((MainWindow*)window()), SIGNAL(SetGame(lfant::Game*)), this, SLOT(SetGame(lfant::Game*)));
}

QWidget* Inspector::CreateInput(const Properties::Value& val, QWidget* parent)
{
	QWidget* in = nullptr;
	const std::type_info& type = *(val.type);
	if(type == typeid(int16_t) || type == typeid(uint16_t) || type == typeid(int32_t) || type == typeid(uint32_t))
	{
		in = new QWidget(parent);
		QLayout* lay = new QHBoxLayout(in);

		auto slider = new gui::ValueSlider(in);
	//	slider->setPointer(&val.str);
		slider->setOrientation(Qt::Orientation::Horizontal);
		lay->addWidget(slider);

		auto spin = new QSpinBox(in);
		lay->addWidget(spin);

		connect(slider, SIGNAL(valueChanged(int)), spin, SLOT(setValue(int)));
		connect(spin, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
	}
	else if(type == typeid(bool))
	{
		in = new QCheckBox(parent);
	}
	else if(type == typeid(float) || type == typeid(double))
	{
		in = new gui::ValueDoubleSpinBox(parent);
	}
	else
	{
		in = new gui::ValueLineEdit(parent);
	}
	in->setUserData(0, (QObjectUserData*)&(val));
	return in;
}

void Inspector::AddChild(Properties* prop, QWidget* parent)
{
	QWidget* top = new QWidget(parent);
	QLayout* lay = new QHBoxLayout(top);
	QScrollArea* scroll = new QScrollArea(top);
	scroll->setWidgetResizable(true);
	QWidget* scrollContents = new QWidget();
	QFormLayout* form = new QFormLayout(scrollContents);

	scroll->setWidget(scrollContents);
	lay->addWidget(scroll);

	for(auto& val : prop->values)
	{
		form->addRow(QString::fromStdString(val.first), CreateInput(val.second, scrollContents));
	}

	for(auto& child : prop->children)
	{
		AddChild(child, scrollContents);
	}
}

void Inspector::Refresh()
{
	if(!currentEnt) return;
	qDebug() << "shizzy shiz " << QString::fromStdString(currentEnt->GetName()) << "\n";

	// resets the layout
	QFormLayout* form = (QFormLayout*)layout();
	for(int i = 0; i < children().size(); ++i)
	{
		if(children()[i]) delete children()[i];
	}

	currentProp->Clear();
	currentEnt->Save(currentProp);

	for(auto& val : currentProp->values)
	{
		form->addRow(QString::fromStdString(val.first), CreateInput(val.second, this));
	}

	form->addItem(new QSpacerItem(0, 12));

	for(auto& comp : currentProp->children)
	{
		if(comp->IsType("Component"))
		{
			AddChild(comp, this);
		}
	}

}

void Inspector::SetEntity(QString name)
{
	qDebug() << "Inspector::SetEntity()\n";
	currentEnt = game->scene->GetRoot()->GetChild(name.toStdString(), true);
}

void Inspector::SetEntity(Entity* ent)
{
	currentEnt = ent;
}

void Inspector::SetGame(Game* game)
{
	this->game = game;
}

} // namespace editor
} // namespace lfant

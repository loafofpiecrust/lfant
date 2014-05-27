#include "Inspector.h"

#include <editor/MainWindow.h>
#include <editor/gui/ValueDoubleSpinBox.h>
#include <editor/gui/ValueSlider.h>
#include <editor/gui/ValueLineEdit.h>
#include "editor/gui/ValueCheckBox.h"
#include "editor/gui/ValueVectorEdit.h"

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

#include <functional>

namespace lfant {
namespace editor {

Inspector::Inspector(QWidget* parent) :
	QWidget(parent),
	currentProp(new Properties)
{
	form = new QFormLayout();
	setLayout(form);
	qDebug() << "doin a thing\n";

//	game = ((MainWindow*)window())->game;
//	connect(((MainWindow*)window())->ui->actionRefresh, SIGNAL(triggered()), this, SLOT(Refresh()));
//	connect(((MainWindow*)window()), SIGNAL(SetGame(lfant::Game*)), this, SLOT(SetGame(lfant::Game*)));
}

QWidget* Inspector::CreateInput(boost::any& val, QWidget* parent)
{
	std::cout << "creating input\n";
	QWidget* in = nullptr;
	const std::type_info& type = val.type();
	if(type == typeid(int32*) || type == typeid(uint32*) || type == typeid(uint16*) || type == typeid(int16*))
	{
		in = new QWidget(parent);
		QLayout* lay = new QHBoxLayout(in);

		auto slider = new gui::ValueSlider(in);
	//	auto slider = new QSlider(in);

	//	int* i = boost::any_cast<int*>(val);
		slider->setPointer(val);
		lay->addWidget(slider);
		slider->setOrientation(Qt::Orientation::Horizontal);

		auto spin = new QSpinBox(in);
		lay->addWidget(spin);

		connect(slider, SIGNAL(valueChanged(int)), spin, SLOT(setValue(int)));
		connect(spin, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
	}
	else if(type == typeid(bool*))
	{
		auto vcb = new gui::ValueCheckBox(parent);
		vcb->setPointer(boost::any_cast<bool*>(val));
		in = vcb;
	}
	else if(type == typeid(double*))
	{
		auto vdsb = new gui::ValueDoubleSpinBox(parent);
		vdsb->setPointer(boost::any_cast<double*>(val));
		in = vdsb;
	}
	else if(type == typeid(float*))
	{
		auto vdsb = new gui::ValueDoubleSpinBox(parent);
		vdsb->setPointer(boost::any_cast<float*>(val));
		in = vdsb;
	}
	else if(type == typeid(string*))
	{
		auto vle = new gui::ValueLineEdit(parent);
		vle->setPointer(boost::any_cast<string*>(val));
		in = vle;
	}
	else if(type == typeid(vec2*) || type == typeid(vec3*) || type == typeid(vec4*))
	{
		in = new gui::ValueVectorEdit(parent, val);
	}
	else
	{
		// unhandled type
		in = new QSlider(parent);
	}
//	in->setUserData(0, (QObjectUserData*)&(val));
	std::cout << "suckz dix " << in << "\n";
	return in;
}

void Inspector::AddChild(Properties* prop, QWidget* parent, QFormLayout* form)
{
/*	QWidget* top = new QWidget(parent);
	QLayout* lay = new QHBoxLayout(top);
	QScrollArea* scroll = new QScrollArea(top);
	scroll->setWidgetResizable(true);
	QWidget* scrollContents = new QWidget();
	QFormLayout* form = new QFormLayout(scrollContents);*/

	QWidget* haiWidget = new QWidget(parent);
	haiWidget->setObjectName("haiWidget");
//	haiWidget->setAcceptDrops(false);
	haiWidget->setLayoutDirection(Qt::LeftToRight);
	auto verticalLayout = new QVBoxLayout(haiWidget);
//	verticalLayout->setSpacing(6);
//	verticalLayout->setContentsMargins(11, 11, 11, 11);
//	verticalLayout->setObjectName("verticalLayout");
	auto pushButton = new QPushButton(haiWidget);
//	pushButton->setObjectName(QStringLiteral("pushButton"));
	pushButton->setLayoutDirection(Qt::LeftToRight);
	pushButton->setCheckable(true);
	pushButton->setChecked(false);
//	pushButton->setAutoDefault(false);
//	pushButton->setDefault(false);
//	pushButton->setFlat(false);

	verticalLayout->addWidget(pushButton);

	auto scrollArea = new QScrollArea(haiWidget);
//	scrollArea->setObjectName(QStringLiteral("scrollArea"));
//	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setWidgetResizable(true);
	auto scrollAreaWidgetContents = new QWidget();
//	scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
//	scrollAreaWidgetContents->setGeometry(QRect(0, 0, 144, 68));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
	scrollAreaWidgetContents->setSizePolicy(sizePolicy);
	auto formLayout = new QFormLayout(scrollAreaWidgetContents);
	formLayout->setSpacing(6);
	formLayout->setContentsMargins(11, 11, 11, 11);
//	formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
	formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	scrollArea->setWidget(scrollAreaWidgetContents);

	verticalLayout->addWidget(scrollArea);

	form->addRow(QString::fromStdString(prop->name), haiWidget);

//	scroll->setWidget(scrollContents);
//	lay->addWidget(scroll);

	for(auto& val : prop->values)
	{
		formLayout->addRow(QString::fromStdString(val.first), CreateInput(val.second, scrollAreaWidgetContents));
	}

	for(auto& child : prop->children)
	{
		AddChild(child, scrollAreaWidgetContents, formLayout);
	}
}

void Inspector::Refresh()
{
	if(!currentEnt) return;
	qDebug() << "shizzy shiz " << QString::fromStdString(currentEnt->GetName()) << "\n";

	// resets the layout
//	QFormLayout* form = (QFormLayout*)layout();
	for(int i = 0; i < children().size(); ++i)
	{
		QWidget* const child = dynamic_cast<QWidget* const>(children()[i]);
		if(child)
		{
			form->removeWidget(child);
			delete child;
		}
	}

	currentProp->Clear();
	currentProp->SetMode(Properties::Mode::Output);
	currentProp->SetSaveMode(Properties::SaveMode::Pointer);
	currentEnt->Serialize(currentProp);

	boost::any a = &currentProp->name;
	form->addRow("name", CreateInput(a, this));
	for(auto& val : currentProp->values)
	{
		form->addRow(QString::fromStdString(val.first), CreateInput(val.second, this));
	}

	form->addItem(new QSpacerItem(0, 12));

	for(auto& comp : currentProp->children)
	{
		qDebug() << "trying shit component, type: " << QString::fromStdString(comp->type);
	//	if(comp->IsType("Component"))
		{
			AddChild(comp, this, form);
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

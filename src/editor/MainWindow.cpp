#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <lfant/FileSystem.h>
#include <lfant/Game.h>

#include <QFileDialog>
#include <QDebug>

#include <boost/extension/shared_library.hpp>
#include <boost/filesystem.hpp>

namespace lfant {
namespace editor {

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	qDebug() << this << "\n";
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	CloseGame();
//	delete ui;
}

void MainWindow::OpenProjectDialog()
{
	QFileDialog dialog(this, "Choose project dir");
	connect(&dialog, SIGNAL(fileSelected(QString)), SLOT(LoadProject(QString)));
	dialog.exec();
}

void MainWindow::NewProject(QString path, QString title)
{
}

void MainWindow::LoadProject(QString path)
{
	projectPath = path.toStdString();
}

void MainWindow::OpenGame()
{
	boost::extensions::shared_library lib(projectPath);

	if(!lib.open())
	{
		std::cout << "Failed to load " << projectPath << ": '" << dlerror() << "\n";
		return;
	}

	boost::function<lfant::Game*()> func = lib.get<lfant::Game*>("Create");
	if(func)
	{
		std::cout << "Loading game...\n";
		game = func();
	//	game->standAlone = false;
		boost::filesystem::path p(projectPath);
		game->gameFolder = p.remove_filename().string()+"/../../..";
		game->programFolder = p.remove_filename().string();

	//	Deinit();
	//	Init();
		game->Init();

		SetGame(game);
	}
	else
	{
		std::cout << "Couldn't launch game: " << dlerror() << "\n";
	}
}

void MainWindow::CloseGame()
{
	game->Destroy();
	game.reset();
}

void MainWindow::SaveProject()
{
}

void MainWindow::Exit()
{

}

void MainWindow::AddChildEntity()
{

}

void MainWindow::RefreshEntity()
{

}

void MainWindow::AddComponent(QString typeName)
{

}

void MainWindow::Shutdown()
{
	closed = true;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if(game)
	{
		CloseGame();
	}
	closed = true;
}

}
}

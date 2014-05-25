#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <lfant/Game.h>

namespace Ui {
class MainWindow;
}

namespace lfant {
namespace editor {

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	ptr<Ui::MainWindow> ui;
	ptr<lfant::Game> game = nullptr;
	bool closed = false;

private slots:
	// File
	void OpenProjectDialog();
	void NewProject(QString path, QString title);
	void LoadProject(QString path);
	void OpenGame();
	void CloseGame();
	void SaveProject();
	void Exit();

	// Edit

	// Scene
	void AddChildEntity();

	// Entity
	void RefreshEntity();
	void AddComponent(QString typeName);

	void Shutdown();

signals:
	void SetGame(lfant::Game*);

private:
	void closeEvent(QCloseEvent* event);

	string projectPath = "";
};

}
}

#endif // MAINWINDOW_H

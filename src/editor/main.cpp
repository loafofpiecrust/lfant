#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	lfant::editor::MainWindow w;
	w.show();

	while(!a.closingDown())
	{
		a.processEvents();
		if(w.game)
			w.game->Update();
	}
}

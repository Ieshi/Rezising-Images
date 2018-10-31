
#include <QtWidgets/QApplication>
#include <presentation.h>
#include "travail.h"


int main(int argc, char *argv[])
{
QApplication app(argc, argv);
presentation pres;

pres.setWindowTitle("Interface de redimensionnement");

pres.show();

return app.exec();
}

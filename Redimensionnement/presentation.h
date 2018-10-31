
#ifndef PRESENTATION_H
#define PRESENTATION_H
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
#include <QtWidgets>
#include <QMenu>
#include <QGroupBox>
#include <QImage>
#include <QPixmap>
#include "travail.h"



class presentation: public QMainWindow
{private:
    QString image_avant;
    QString image_apres;
    QImage im_image_avant;
    QMdiArea* pres = new QMdiArea;
    QLineEdit tailleAbb;
    QLineEdit tailleOrd;
    QCheckBox* lissage= new QCheckBox(tr("Lissage prioritisé"));
    QCheckBox* nettete= new QCheckBox(tr("Netteté préservé"));
    QCheckBox* flou= new QCheckBox(tr("Flou toléré"));

    public:
            presentation();
    private:
            slots void choisirImage();
            slots void enregistrerImage();
            void affiche_Av();
            void affiche_Apr(QImage, QString);
            slots void demarrer();
            slots void demarageRapide();
};


#endif // PRESENTATION_H

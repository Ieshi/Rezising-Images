
#ifndef TRAVAIL_H
#define TRAVAIL_H
#include <qmath.h>
#include <QtGui>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
#include <QtWidgets>
#include <QMenu>
#include <QImage>
#include <QPixmap>
#include <QGroupBox>
#include "presentation.h"
#include "windows.h"
#include <math.h>



QImage creer_im_avant(QString);
QString choixAlgorithme(QCheckBox *nettete, QCheckBox *lissage, QCheckBox *flou);

double max(double x,double y);
double min(double x,double y);
int arondi(double x);

double sinc(double x);

double c0(double t);
double c1(double t);
double c2(double t);
double c3(double t);
double Hr(QImage ime,int i,int j,double x);
double Hg(QImage ime,int i,int j,double x);
double Hb(QImage ime,int i,int j,double x);

QImage algorithmeBilineaire(QImage imgAv, int ls, int cs);
QImage algorithmeBicubique(QImage imgAv, int ls, int cs);
QImage algorithmeSplineCubique(QImage imgAv, int ls, int cs);
QImage algorithmeBrut(QImage imgAv, int ls, int cs);
QImage algorithmeSinc(QImage imgAv, int ls, int cs);


#endif // TRAVAIL_H

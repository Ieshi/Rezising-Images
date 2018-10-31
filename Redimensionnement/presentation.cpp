#include "presentation.h"




presentation::presentation():QMainWindow()
{

    setCentralWidget(pres);

//Fonctionalités
QMdiSubWindow* b_outils=new QMdiSubWindow(pres);
QGridLayout* outils_grid = new QGridLayout;
b_outils->setWindowTitle("Boîte à outil");
         delete b_outils->layout();
         b_outils->setLayout(outils_grid);
    QGroupBox* taille= new QGroupBox(tr("Taille"));
       QVBoxLayout* part1 = new QVBoxLayout;
         part1->addWidget(&tailleAbb);
         part1->addWidget(&tailleOrd);
         part1->addStretch(1);
         taille->setLayout(part1);
       outils_grid->addWidget(taille, 0, 0);

    QGroupBox* criteres=new QGroupBox(tr("Critères"));
       QVBoxLayout* part2= new QVBoxLayout;
         part2->addWidget(lissage);
         part2->addWidget(nettete);
         part2->addWidget(flou);
         part2->addStretch(1);
         criteres->setLayout(part2);
       outils_grid->addWidget(criteres, 1, 0);

     QPushButton* dem=new QPushButton(tr("Demarrer"));
        outils_grid->addWidget(dem, 2, 0);
        connect(dem, QPushButton::clicked, this,presentation::demarrer);

     QPushButton* demRapide=new QPushButton(tr("Redimensionnement rapide"));
        outils_grid->addWidget(demRapide, 3, 0);
        connect(demRapide, QPushButton::clicked, this,presentation::demarageRapide);



 //Menu
 QMenu *fichier = menuBar()->addMenu("&Fichier");                 //Raccourcis F
      QAction* nouveau = new QAction("&Nouveau", this);
      fichier->addAction(nouveau);
              nouveau->setShortcut(QKeySequence("Ctrl+N"));
              connect(nouveau,&QAction::triggered,this,presentation::choisirImage);

      QAction* enregistrer = new QAction("&Enregister", this);
      fichier->addAction(enregistrer);
              enregistrer->setShortcut(QKeySequence("Ctrl+S"));
              connect(enregistrer,&QAction::triggered,this,presentation::enregistrerImage);

      QAction* quitter = new QAction("&Quitter", this);
      fichier->addAction(quitter);
             connect(quitter, SIGNAL(triggered()), this, SLOT(quit()));
             quitter->setShortcut(QKeySequence("Alt+F4"));



//Barre d'outil
QToolBar *barreFichier = addToolBar("Fichier");
      barreFichier->addAction(nouveau);
      nouveau->setIcon(QIcon("nouveau.png"));
      barreFichier->addAction(enregistrer);
      enregistrer->setIcon(QIcon("enregistrer.png"));
      barreFichier->addAction(quitter);
      quitter->setIcon(QIcon("quitter.png"));
      barreFichier->addSeparator();

}

void presentation::choisirImage()
{image_avant = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(),"Images (*.png *.gif *.jpg *.jpeg)");
 im_image_avant=creer_im_avant(image_avant);
 affiche_Av();
}

void presentation::enregistrerImage()
{image_apres = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
}

void presentation::affiche_Av()
{

    //Fenêtre pour afficher l'image avant le traitement
                QLabel* imgAv= new QLabel;
                QMdiSubWindow* avant= pres->addSubWindow(imgAv);
                imgAv->setPixmap(QPixmap(image_avant));
                avant->setWindowTitle("Avant");
                avant->show();

}


void presentation::affiche_Apr(QImage imageApres, QString chaine)
{
 QPixmap imgPixmap = QPixmap::fromImage(imageApres);
    //Fenêtre pour afficher l'image après le traitement
    QLabel* imgApr= new QLabel;
    QMdiSubWindow *apres= pres->addSubWindow(imgApr);
    imgApr->setPixmap(imgPixmap);
    apres->setWindowTitle(chaine);
    apres->show();
}


//Démarrer
    void presentation::demarrer()
{QString chaineAbb = tailleAbb.text();
 QString chaineOrd = tailleOrd.text();
 bool ok1, ok2;
 int x= chaineAbb.toInt(&ok1, 10);
 int y= chaineOrd.toInt(&ok2,10);
      if(ok1==true && ok2==true)
        {QString chaine=choixAlgorithme(nettete, lissage, flou);
        if(chaine=="Bilineaire")
             {QImage imgApres=algorithmeBilineaire(im_image_avant, x, y);
              affiche_Apr(imgApres, chaine);
             }
        if(chaine=="Bicubique")
             {QImage imgApres=algorithmeBicubique(im_image_avant, x, y);
              affiche_Apr(imgApres, chaine);
             }
        if(chaine=="Brut")
             {QImage imgApres=algorithmeBrut(im_image_avant, x, y);
              affiche_Apr(imgApres, chaine);
             }
        if(chaine=="Spline Cubique")
             {QImage imgApres=algorithmeSplineCubique(im_image_avant, x, y);
              affiche_Apr(imgApres, chaine);
             }
        if(chaine=="Sinc")
             {QImage imgApres=algorithmeSinc(im_image_avant, x, y);
             affiche_Apr(imgApres, chaine);
             }
          }

}


//Redimensionnement rapide

    void presentation::demarageRapide()
{QString chaineAbb = tailleAbb.text();
 QString chaineOrd = tailleOrd.text();
 bool ok1, ok2;
 int x= chaineAbb.toInt(&ok1, 10);
 int y= chaineOrd.toInt(&ok2,10);
      if(ok1==true && ok2==true)
        {QImage imgApres=algorithmeBrut(im_image_avant, x, y);
          QString chaine="Redimensionnement Rapide";
          affiche_Apr(imgApres, chaine);
        }

}





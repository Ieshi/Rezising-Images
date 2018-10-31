#include "travail.h"




//Lecture de l'image
QImage creer_im_avant(QString chaine)
{   QPixmap *pix_im_avant= new QPixmap(chaine);
    QImage img_Av=pix_im_avant->toImage();
    return img_Av;
}


//Choix de l'algorithme
QString choixAlgorithme(QCheckBox* nettete, QCheckBox* lissage, QCheckBox* flou)
{QString chaine;
  if(nettete->isChecked()==true && lissage->isChecked()==true && flou->isChecked()==true)  chaine="Sinc";
  if(nettete->isChecked()==true && lissage->isChecked()==true && flou->isChecked()==false) chaine="Spline Cubique";
  if(nettete->isChecked()==true && lissage->isChecked()==false) chaine="Bicubique";
  if(nettete->isChecked()==false && lissage->isChecked()==true) chaine="Bilineaire";
  if(nettete->isChecked()==false && lissage->isChecked()==false && flou->isChecked()==true) chaine="Sinc";
  if(nettete->isChecked()==false && lissage->isChecked()==false && flou->isChecked()==false)chaine="Brut";
return chaine;
}



//Traitement !


//Pour les arondis
int arondi(double x)
 {

    int y;
    if(x-floor(x)<0.5)
             y=floor(x);

    else if(x-floor(x)==0)
             y=x;

    else
             y=x+1;

    return y;
}

//Designer le min et le max (réduire le nombre de if)
double max(double x,double y)
{
    if(x>y)
    {
        return x;
    }
     else
    {
         return y;
     }
}

double min(double x,double y)
{
    if(x>y)
    {
        return y;
    }
     else
    {
         return x;
     }
}

//sinus cardinal

double sinc(double x)
{
    if(x==0)
    return 1;

     return (sin(x)/x);

}


//Parametres pour la methode des splines

double c0(double t){
    float a=-0.01;
    return (-a*t*t*t+a*t*t);
}

double c1(double t){
    float a=-0.01;
    return (-(a+2)*t*t*t+(2*a+3)*t*t-a*t);
}

double c2(double t){
    float a=-0.01;
    return ((a+2)*t*t*t-(a+3)*t*t+1);
}

double c3(double t){
    float a=-0.01;
    return (a*t*t*t-2*a*t*t+a*t);
}


double R(QImage ime,int i,int j,double x){

    return (qRed(ime.pixel(j,i-1))*c3(x-i)+qRed(ime.pixel(j,i))*c2(x-i)+qRed(ime.pixel(j,i+1))*c1(x-i)+qRed(ime.pixel(j,i+2))*c0(x-i));
}

double G(QImage ime,int i,int j,double x){

    return (qGreen(ime.pixel(j,i-1))*c3(x-i)+qGreen(ime.pixel(j,i))*c2(x-i)+qGreen(ime.pixel(j,i+1))*c1(x-i)+qGreen(ime.pixel(j,i+2))*c0(x-i));
}

double B(QImage ime,int i,int j,double x){

    return (qBlue(ime.pixel(j,i-1))*c3(x-i)+qBlue(ime.pixel(j,i))*c2(x-i)+qBlue(ime.pixel(j,i+1))*c1(x-i)+qBlue(ime.pixel(j,i+2))*c0(x-i));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Brut
QImage algorithmeBrut(QImage imgAv, int ls, int cs)
{
    double x,y;
    QRgb val;

    int ce=imgAv.width();
    int le=imgAv.height();

    QImage image_sortie(cs,ls,imgAv.format());

    for(int i=0;i<ls;i++)
    {
        for(int j=0;j<cs;j++)
        {
            x=i*double(le)/(ls);
            y=j*double(ce)/(cs);

            x=arondi(x);
            y=arondi(y);

            x=(int)min(x,le-1);
            y=(int)min(y,ce-1);

            val=imgAv.pixel(y,x);

            image_sortie.setPixel(j,i,val);

        }
     }
    return image_sortie;
}




//Bilinéaire

QImage algorithmeBilineaire(QImage imgAv, int ls, int cs) //ls: ligne sortie cs:colonne sortie
{double x,xr,y,yr,dx[4],dy[4],xi[4],yi[4],p[4],s;
    int val2,val3,val1;
    QRgb val;
    int ce=imgAv.width();
    int le=imgAv.height();


    QImage image_sortie(cs,ls,imgAv.format());

    for(int i=0;i<ls;i++)
             {
                 for(int j=0;j<cs;j++)
                 {
                   x=i*double(le)/ls;
                   y=j*double(ce)/cs;

                   x=min(x,le-1);  //Pour ne pas dépasser la limite de la matrice
                   y=min(y,ce-1);

                   xr=x;           // coordonnées réelles
                   yr=y;

                   x=arondi(x);
                   y=arondi(y);    // 1)calcul de l'element le plus proche


                  // on tombe exactement sur un pixel existant
                   if((xr==x)&&(yr==y))
                   {
                    image_sortie.setPixel(j,i,imgAv.pixel(y,x));         //Cas distribution proportionelle
                   }

                   else
                   {
                   dx[0]=xr-x;              xi[0]=x;
                   dy[0]=yr-y;              yi[0]=y;

                   dx[1]=xr-xi[1];          xi[1]=xi[0];
                   dy[1]=yr-yi[1];          yi[1]=yi[0]+1;

                   dx[2]=xr-xi[2];          xi[2]=xi[0]+1;
                   dy[2]=yr-yi[2];          yi[2]=yi[0];

                   dx[3]=xr-xi[3];          xi[3]=xi[0]+1;
                   dy[3]=yr-yi[3];          yi[3]=yi[0]+1;



                   if(dx[0]==0)
                   {
                     p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                     p[1]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                     p[3]=0;
                     p[2]=0;
                     s=p[0]+p[1]+p[2]+p[3];
                     val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3])))*(1/s);
                     val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3])))*(1/s);
                     val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3])))*(1/s);
                     val=qRgb(val1,val3,val2);
                     image_sortie.setPixel(j,i,val);
                   }
                   else if(dy[0]==0)
                   {
                    p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                    p[2]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                    p[1]=0;
                    p[3]=0;
                    s=p[0]+p[1]+p[2]+p[3];

                    val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val=qRgb(val1,val3,val2);
                    image_sortie.setPixel(j,i,val);
                    }

                   else
                   {
                    p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                    p[1]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                    p[2]=1/sqrt(dx[2]*dx[2]+dy[2]*dy[2]);
                    p[3]=1/sqrt(dx[3]*dx[3]+dy[3]*dy[3]);
                    s=p[0]+p[1]+p[2]+p[3];

                    val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3])))*(1/s);
                    val=qRgb(val1,val3,val2);
                    image_sortie.setPixel(j,i,val);
                }
               }

               }
             }
    return image_sortie;
        }




//Spline Bicubique
QImage algorithmeSplineCubique(QImage imgAv, int ls, int cs)
{double x,xr,y,yr;
    QRgb val;
    int ce=imgAv.width();
    int le=imgAv.height();


    QImage image_sortie(cs,ls,imgAv.format());

    double r,g,b;
for(int i=0;i<ls;i++)
 {
     for(int j=0;j<cs;j++)
     {
       x=i*double(le)/ls;
       y=j*double(ce)/cs;

 // Réelles
       xr=x;
       yr=y;

  // L'element le plus proche
       x=floor(x);
       y=floor(y);



       if(x>0 && x<le-1 && y>0 && y<ce-1)
       {
           r=R(imgAv,x,y-1,xr)*c3(yr-y)+R(imgAv,x,y,xr)*c2(yr-y)+R(imgAv,x,y+1,xr)*c1(yr-y)+R(imgAv,x,y+2,xr)*c0(yr-y);
           g=G(imgAv,x,y-1,xr)*c3(yr-y)+G(imgAv,x,y,xr)*c2(yr-y)+G(imgAv,x,y+1,xr)*c1(yr-y)+G(imgAv,x,y+2,xr)*c0(yr-y);
           b=B(imgAv,x,y-1,xr)*c3(yr-y)+B(imgAv,x,y,xr)*c2(yr-y)+B(imgAv,x,y+1,xr)*c1(yr-y)+B(imgAv,x,y+2,xr)*c0(yr-y);

               val=qRgb(r,g,b);
               image_sortie.setPixel(j,i,val);
       }
       else{
            image_sortie.setPixel(j,i,0);
            }
      }
   }


    return image_sortie;

}



//Utilisation de sinus cardinal
QImage algorithmeSinc(QImage imgAv, int ls, int cs)
{double x,xr,y,yr;
    double val2,val3,val1;
    QRgb val;
    int ce=imgAv.width();
    int le=imgAv.height();

    int masque=1/2;


    QImage image_sortie(cs,ls,imgAv.format());

    double s1,s11,s22,s33,vs;

    for(int i=0;i<ls;i++)
     {
         for(int j=0;j<cs;j++)
         {
           x=i*double(le)/ls;
           y=j*double(ce)/cs;

           x=min(x,le-1);
           y=min(y,ce-1);

           xr=x;                             // coordonnées réelles
           yr=y;

           x=arondi(x);
           y=arondi(y);

           s1=0;s11=0;s22=0;s33=0;

       for(int k1=x-masque;k1<=x+masque;k1++)
          {
           for(int k2=y-masque;k2<=y+masque;k2++)
             {
               if(k1>=0 && k1<le && k2>=0 && k2<ce)
               {
               vs=fabs(sinc(M_PI*sqrt((xr-k1)*(xr-k1)+(yr-k2)*(yr-k2))));
               s1=s1+vs;

               if(vs>0)
               {
               val1=qRed(imgAv.pixel(k2,k1))*vs;
               val2=qBlue(imgAv.pixel(k2,k1))*vs;
               val3=qGreen(imgAv.pixel(k2,k1))*vs;

               s11=s11+val1;
               s22=s22+val2;
               s33=s33+val3;
               }

               }
             }
       }

       s11=s11/s1;
       s22=s22/s1;
       s33=s33/s1;

       val=qRgb(s11,s33,s22);
       image_sortie.setPixel(j,i,val);

       }
     }
    return image_sortie;
}


//Bicubique

QImage algorithmeBicubique(QImage imgAv, int ls, int cs) //ls: ligne sortie cs:colonne sortie
{double x,xr,y,yr,dx[9],dy[9],xi[9],yi[9],p[9],s;
    int val2,val3,val1;
    QRgb val;
    int ce=imgAv.width();
    int le=imgAv.height();


    QImage image_sortie(cs,ls,imgAv.format());

    for(int i=0;i<ls;i++)
             {
                 for(int j=0;j<cs;j++)
                 {
                   x=i*double(le)/ls;
                   y=j*double(ce)/cs;

                   x=min(x,le-1);  //Pour ne pas dépasser la limite de la matrice
                   y=min(y,ce-1);

                   xr=x;           // coordonnées réelles
                   yr=y;

                   x=arondi(x);
                   y=arondi(y);    // 1)calcul de l'element le plus proche


                  // on tombe exactement sur un pixel existant
                   if((xr==x)&&(yr==y))
                   {
                    image_sortie.setPixel(j,i,imgAv.pixel(y,x));         //Cas distribution proportionelle
                   }

                   else
                   {
                   dx[0]=xr-x;              xi[0]=x;
                   dy[0]=yr-y;              yi[0]=y;

                   dx[1]=xr-xi[1];          xi[1]=xi[0];
                   dy[1]=yr-yi[1];          yi[1]=yi[0]+1;

                   dx[2]=xr-xi[2];          xi[2]=xi[0]+1;
                   dy[2]=yr-yi[2];          yi[2]=yi[0];

                   dx[3]=xr-xi[3];          xi[3]=xi[0]+1;
                   dy[3]=yr-yi[3];          yi[3]=yi[0]+1;

                   dx[4]=xr-xi[4];          xi[4]=xi[0];
                   dy[4]=yr-yi[4];          yi[4]=yi[0]+2;

                   dx[5]=xr-xi[5];          xi[5]=xi[0]+2;
                   dy[5]=yr-yi[5];          yi[5]=yi[0];

                   dx[6]=xr-xi[6];          xi[6]=xi[0]+1;
                   dy[6]=yr-yi[6];          yi[6]=yi[0]+2;

                   dx[7]=xr-xi[7];          xi[7]=xi[0]+2;
                   dy[7]=yr-yi[7];          yi[7]=yi[0]+1;

                   dx[8]=xr-xi[7];          xi[8]=xi[0]+2;
                   dy[8]=yr-yi[7];          yi[8]=yi[0]+2;



                   if(dx[0]==0)
                   {
                     p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                     p[1]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                     p[2]=0;
                     p[3]=0;
                     p[4]=1/sqrt(dx[4]*dx[4]+dy[4]*dy[4]);
                     p[5]=0;
                     p[6]=0;
                     p[7]=0;
                     p[8]=0;
                     s=p[0]+p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]+p[8];
                     val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3]))+p[4]*qRed(imgAv.pixel(yi[4],xi[4]))+p[5]*qRed(imgAv.pixel(yi[5],xi[5]))+p[6]*qRed(imgAv.pixel(yi[6],xi[6]))+p[7]*qRed(imgAv.pixel(yi[7],xi[7]))+p[8]*qRed(imgAv.pixel(yi[8],xi[8])))*(1/s);
                     val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3]))+p[4]*qBlue(imgAv.pixel(yi[4],xi[4]))+p[5]*qBlue(imgAv.pixel(yi[5],xi[5]))+p[6]*qBlue(imgAv.pixel(yi[6],xi[6]))+p[7]*qBlue(imgAv.pixel(yi[7],xi[7]))+p[8]*qBlue(imgAv.pixel(yi[8],xi[8])))*(1/s);
                     val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3]))+p[4]*qGreen(imgAv.pixel(yi[4],xi[4]))+p[5]*qGreen(imgAv.pixel(yi[5],xi[5]))+p[6]*qGreen(imgAv.pixel(yi[6],xi[6]))+p[7]*qGreen(imgAv.pixel(yi[7],xi[7]))+p[8]*qGreen(imgAv.pixel(yi[8],xi[8])))*(1/s);
                     val=qRgb(val1,val3,val2);
                     image_sortie.setPixel(j,i,val);
                   }


                   else if(dy[0]==0)
                   {
                    p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                    p[2]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                    p[1]=0;
                    p[3]=0;
                    p[4]=0;
                    p[5]=1/sqrt(dx[5]*dx[5]+dy[5]*dy[5]);
                    p[6]=0;
                    p[7]=0;
                    p[8]=0;
                    s=p[0]+p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]+p[8];
                    val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3]))+p[4]*qRed(imgAv.pixel(yi[4],xi[4]))+p[5]*qRed(imgAv.pixel(yi[5],xi[5]))+p[6]*qRed(imgAv.pixel(yi[6],xi[6]))+p[7]*qRed(imgAv.pixel(yi[7],xi[7]))+p[8]*qRed(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3]))+p[4]*qBlue(imgAv.pixel(yi[4],xi[4]))+p[5]*qBlue(imgAv.pixel(yi[5],xi[5]))+p[6]*qBlue(imgAv.pixel(yi[6],xi[6]))+p[7]*qBlue(imgAv.pixel(yi[7],xi[7]))+p[8]*qBlue(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3]))+p[4]*qGreen(imgAv.pixel(yi[4],xi[4]))+p[5]*qGreen(imgAv.pixel(yi[5],xi[5]))+p[6]*qGreen(imgAv.pixel(yi[6],xi[6]))+p[7]*qGreen(imgAv.pixel(yi[7],xi[7]))+p[8]*qGreen(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val=qRgb(val1,val3,val2);
                    image_sortie.setPixel(j,i,val);
                    }

                   else
                   {
                    p[0]=1/sqrt(dx[0]*dx[0]+dy[0]*dy[0]);
                    p[1]=1/sqrt(dx[1]*dx[1]+dy[1]*dy[1]);
                    p[2]=1/sqrt(dx[2]*dx[2]+dy[2]*dy[2]);
                    p[3]=1/sqrt(dx[3]*dx[3]+dy[3]*dy[3]);
                    p[4]=1/sqrt(dx[4]*dx[4]+dy[4]*dy[4]);
                    p[5]=1/sqrt(dx[5]*dx[5]+dy[5]*dy[5]);
                    p[6]=1/sqrt(dx[6]*dx[6]+dy[6]*dy[6]);
                    p[7]=1/sqrt(dx[7]*dx[7]+dy[7]*dy[7]);
                    p[8]=1/sqrt(dx[8]*dx[8]+dy[8]*dy[8]);
                    s=p[0]+p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]+p[8];
                    val1=(p[0]*qRed(imgAv.pixel(yi[0],xi[0]))+p[1]*qRed(imgAv.pixel(yi[1],xi[1]))+p[2]*qRed(imgAv.pixel(yi[2],xi[2]))+p[3]*qRed(imgAv.pixel(yi[3],xi[3]))+p[4]*qRed(imgAv.pixel(yi[4],xi[4]))+p[5]*qRed(imgAv.pixel(yi[5],xi[5]))+p[6]*qRed(imgAv.pixel(yi[6],xi[6]))+p[7]*qRed(imgAv.pixel(yi[7],xi[7]))+p[8]*qRed(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val2=(p[0]*qBlue(imgAv.pixel(yi[0],xi[0]))+p[1]*qBlue(imgAv.pixel(yi[1],xi[1]))+p[2]*qBlue(imgAv.pixel(yi[2],xi[2]))+p[3]*qBlue(imgAv.pixel(yi[3],xi[3]))+p[4]*qBlue(imgAv.pixel(yi[4],xi[4]))+p[5]*qBlue(imgAv.pixel(yi[5],xi[5]))+p[6]*qBlue(imgAv.pixel(yi[6],xi[6]))+p[7]*qBlue(imgAv.pixel(yi[7],xi[7]))+p[8]*qBlue(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val3=(p[0]*qGreen(imgAv.pixel(yi[0],xi[0]))+p[1]*qGreen(imgAv.pixel(yi[1],xi[1]))+p[2]*qGreen(imgAv.pixel(yi[2],xi[2]))+p[3]*qGreen(imgAv.pixel(yi[3],xi[3]))+p[4]*qGreen(imgAv.pixel(yi[4],xi[4]))+p[5]*qGreen(imgAv.pixel(yi[5],xi[5]))+p[6]*qGreen(imgAv.pixel(yi[6],xi[6]))+p[7]*qGreen(imgAv.pixel(yi[7],xi[7]))+p[8]*qGreen(imgAv.pixel(yi[8],xi[8])))*(1/s);
                    val=qRgb(val1,val3,val2);
                    image_sortie.setPixel(j,i,val);
                }
               }

               }
             }
    return image_sortie;
        }

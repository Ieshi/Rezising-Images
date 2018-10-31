/*


//Démarrer
    void presentation::demarrer()
{QString chaineAbb = tailleAbb->selectedText();
 QString chaineOrd = tailleOrd->selectedText();
 int x= chaineAbb.toInt(NULL, 10);
 int y=chaineOrd.toInt(NULL,10);
        QString chaine=choixAlgorithme(temps, qualite);
        /*if(chaine=="Bilineaire")
             {Mat imgApres=algorithmeBilineaire(creer_mat_avant(image_avant), x, y);
              affiche_Apr(imgApres);
             }*/
        /*if(chaine=="Bicubique")
             {Mat imgApres=algorithmeBicubique(creer_mat_avant(image_avant), x, y);
              affiche_Apr(imgApres);
             }*/
         //Mat imgApres=algorithmeLanczos(creer_mat_avant(image_avant), x, y);
           //  affiche_Apr(imgApres);

/*}











//Lecture de l'image
Mat creer_mat_avant(QString chaine)
{Mat mat_img_Av=imread("chaine");
    return mat_img_Av;
}

//Choix de l'algorithme
QString choixAlgorithme(QCheckBox* temps, QCheckBox* qualite)
{QString chaine;
  if(temps->isChecked()==true)
    {if(qualite->isChecked()==true)
          chaine="Bicubique";
      else chaine="Bilineaire";
    }
  else chaine="Lanczos";
return chaine;
}



//A comprendre
QImage Mat_to_QImage(Mat imgMat)
{
     Mat tmp(imgMat); // make the same cv::Mat //Construction par copie
     cvtColor(imgMat, tmp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage imgQIm((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
     imgQIm.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return imgQIm;
}


//Lanczos

double sinc(double x)
{
  double pi = 3.1415926;
  x = (x * pi);
  if (x < 0.01 && x > -0.01)
    return 1.0 + x*x*(-1.0/6.0 + x*x*1.0/120.0);
  return sin(x)/x;
}

double LanczosFilter(double x)
{
  if (std::abs(x) < 3) {
    double pi = 3.1415926;
    return sinc(x)*sinc(x/3);
  } else {
    return 0.0;
  }
}

Mat algorithmeLanczos(Mat imageAv, int x, int y)
{int lanParam=3;
  int saut_y= imageAv.cols/y;
  int saut_x= imageAv.rows/x;
  int ancienneLigne=imageAv.rows;
  int ancienneColonne=imageAv.cols;

  // Apply filter first in width, then in height.
  Mat image_horiz;
  image_horiz.rows=ancienneLigne;
  image_horiz.cols=y;
  for (int l = 0; l < ancienneLigne; l++)
  {
    for (int c = 0; c < y; c++) {
      // a is the new col in terms of the old col coordinates.
      int a = c*saut_y;
      // The old col corresponding to the closest new col.


      image_horiz.at<uint8_t>(l,c) = 0;
      int largeur = 0;
      // Add up terms across the filter.
      for (int i = a - lanParam + 1; i < a + lanParam; i++)
      {
        if (i >= 0 && i < imageAv.cols) {
          double lanc_term = LanczosFilter(a - i);
          image_horiz.at<uint8_t>(l,c) += imageAv.at<uint8_t>(l,i)*lanc_term;
          largeur += lanc_term;
        }
      }
      // Normalize the filter.
      image_horiz.at<uint8_t>(l,c) /= largeur;
      // Strap the pixel values to valid values.
      image_horiz.at<uint8_t>(l,c) = (image_horiz.at<uint8_t>(l,c) > 1.0) ? 1.0 : image_horiz.at<uint8_t>(l,c);
      image_horiz.at<uint8_t>(l,c) = (image_horiz.at<uint8_t>(l,c) < 0.0) ? 0.0 : image_horiz.at<uint8_t>(l,c);
    }
  }

  // Now apply a vertical filter to the horiz image.
  Mat imgApr;
  imgApr.rows=x;
  imgApr.cols=y;
  for (int l = 0; l < x; l++) {
    double a = l*saut_x;
    for (int c = 0; c < y; c++)
    {
      imgApr.at<uint8_t>(l,c) = 0;
      int longueur = 0;
      for (int i = a - lanParam + 1; i < a + lanParam; i++) {
        if (i >= 0 && i < ancienneLigne) {
          double lanc_term = LanczosFilter(a - i);
          imgApr.at<uint8_t>(l,c) += imgApr.at<uint8_t>(i,c)*lanc_term;
          longueur += lanc_term;
        }
      }
      imgApr.at<uint8_t>(l,c) /= longueur;
      imgApr.at<uint8_t>(l,c) = (imgApr.at<uint8_t>(l,c) > 1) ? 1.0 : imgApr.at<uint8_t>(l,c);
      imgApr.at<uint8_t>(l,c) = (imgApr.at<uint8_t>(l,c) < 0) ? 0.0 : imgApr.at<uint8_t>(l,c);
    }
  }
  return imgApr;
}

/*
//Interpolation bilinéaire
Mat algorithmeBilineaire(Mat imgAvant, int x, int y)
{Mat imgApres;
    imgApres.rows=x;
    imgApres.cols=y;
 int saut_x=imgAvant.rows/x;        //Entre chaque ligne
 int saut_y=imgAvant.cols/y;        //Entre chaque colonne
 int q=0;
 int p=0;
 //Si il y'a agrandissement
 if (saut_x!=0 && saut_y!=0)
      {//Distribution proportionelle des pixels de imgAvant
           imgApres.at<uint8_t>(0,0)=imgAvant.at<uint8_t>(0,0);
                for(int i=0; i<imgAvant.rows; i++)
                     {for(int j=0; j<imgAvant.cols; j++)
                          {imgApres.at<uint8_t>(i+saut_x*p,j+saut_y*q) = imgAvant.at<uint8_t>(i,j);
                           q++;
                          }
                       p++;
                     }
 //Pour récupérer la valeur du pixel avec un entier a 8bits
 q=0;
 p=0;
 uint8_t tmp1;
 uint8_t tmp2;
 uint8_t tmp3;
 uint8_t tmp4;
    for (int i=0; i<imgAvant.rows; i++)
         {for(int j=0; j<imgAvant.cols; j++)
                 {if((i+saut_x*p)%saut_x!=0 || (j+saut_y*q)%saut_y!=0)
                       {for(int k=1; k<=saut_x; k++)
                            {for(int l=1; l<=saut_y; l++)
                                 {if(i+saut_x*p<(saut_x+1)/2)
                                       {if(j+saut_y*q<(saut_y+1)/2)   //En haut a gauche
                                           {if(i==0)
                                              {tmp1=imgAvant.at<uint8_t>(i,j-1);
                                               tmp2=imgAvant.at<uint8_t>(i,j);
                                               tmp3=imgAvant.at<uint8_t>(i+1,j-1);
                                               tmp4=imgAvant.at<uint8_t>(i+1,j);
                                               imgApres.at<uint8_t>(i,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                              }
                                            else
                                               {if(j==0)
                                                   {tmp1=imgAvant.at<uint8_t>(i,j);
                                                    tmp2=imgAvant.at<uint8_t>(i,j+1);
                                                    tmp3=imgAvant.at<uint8_t>(i+1,j);
                                                    tmp4=imgAvant.at<uint8_t>(i+1,j-1);
                                                    imgApres.at<uint8_t>(i+k,j)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                                   }
                                                 else
                                                    {tmp1=imgAvant.at<uint8_t>(i-1,j-1);
                                                     tmp2=imgAvant.at<uint8_t>(i-1,j);
                                                     tmp3=imgAvant.at<uint8_t>(i,j);
                                                     tmp4=imgAvant.at<uint8_t>(i,j-1);
                                                     imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                                    }
                                                 }
                                            }

                                    else                                           //En haut a droite
                                         {if(i==0)
                                           {tmp1=imgAvant.at<uint8_t>(i,j-1);
                                            tmp2=imgAvant.at<uint8_t>(i,j);
                                            tmp3=imgAvant.at<uint8_t>(i+1,j-1);
                                            tmp4=imgAvant.at<uint8_t>(i+1,j);
                                            imgApres.at<uint8_t>(i,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                           }

                                            else
                                             {if(j==imgAvant.cols)
                                                       {tmp1=imgAvant.at<uint8_t>(i-1,j);
                                                        tmp2=imgAvant.at<uint8_t>(i,j);
                                                        tmp3=imgAvant.at<uint8_t>(i-1,j-1);
                                                        tmp4=imgAvant.at<uint8_t>(i,j-1);
                                                        imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                                       }
                                              else
                                                       {tmp1=imgAvant.at<uint8_t>(i-1,j);
                                                        tmp2=imgAvant.at<uint8_t>(i,j);
                                                        tmp3=imgAvant.at<uint8_t>(i-1,j+1);
                                                        tmp4=imgAvant.at<uint8_t>(i,j+1);
                                                        imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                                       }
                                           }
                                   }
                        }
                        else
                           {if(j+saut_y*q<(saut_y+1)/2)            //En bas a gauche
                              {if(j==0)
                                    {tmp1=imgAvant.at<uint8_t>(i,j);
                                     tmp2=imgAvant.at<uint8_t>(i,j+1);
                                     tmp3=imgAvant.at<uint8_t>(i+1,j);
                                     tmp4=imgAvant.at<uint8_t>(i+1,j+1);
                                     imgApres.at<uint8_t>(i+k,j)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                    }
                                  else
                                     {if(i==imgAvant.rows)
                                        {tmp1=imgAvant.at<uint8_t>(i-1,j);
                                         tmp2=imgAvant.at<uint8_t>(i,j);
                                         tmp3=imgAvant.at<uint8_t>(i-1,j-1);
                                         tmp4=imgAvant.at<uint8_t>(i,j-1);
                                         imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                        }

                                       else
                                        {tmp1=imgAvant.at<uint8_t>(i,j-1);
                                         tmp2=imgAvant.at<uint8_t>(i,j);
                                         tmp3=imgAvant.at<uint8_t>(i+1,j-1);
                                         tmp4=imgAvant.at<uint8_t>(i+1,j);
                                         imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                        }
                                  }
                            }


                         else                                           //En bas a droite
                          {if(i==imgAvant.rows)
                                    {tmp1=imgAvant.at<uint8_t>(i-1,j);
                                     tmp2=imgAvant.at<uint8_t>(i,j);
                                     tmp3=imgAvant.at<uint8_t>(i-1,j+1);
                                     tmp4=imgAvant.at<uint8_t>(i,j+1);
                                     imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                    }

                             else if(j==imgAvant.cols)
                                    {tmp1=imgAvant.at<uint8_t>(i+1,j);
                                     tmp2=imgAvant.at<uint8_t>(i,j);
                                     tmp3=imgAvant.at<uint8_t>(i+1,j-1);
                                     tmp4=imgAvant.at<uint8_t>(i,j-1);
                                     imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                                    }

                             else
                              {tmp1=imgAvant.at<uint8_t>(i+1,j);
                               tmp2=imgAvant.at<uint8_t>(i,j);
                               tmp3=imgAvant.at<uint8_t>(i+1,j+1);
                               tmp4=imgAvant.at<uint8_t>(i,j+1);
                               imgApres.at<uint8_t>(i+k,j+l)=brutAlgoBillineaire(tmp1,tmp2,tmp3,tmp4);
                              }
                            }
                    }
                }
            }
        }
    }
  }
    return imgApres;
 }
}
}

*/




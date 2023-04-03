#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>



//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  // check arguments
  if(argc != 2){
    std::cout << "usage: " << argv[0] << " image" << std::endl;
    return -1;
  }

  // load the input image
  std::cout << "load image ..." << std::endl;
  cv::Mat image = cv::imread(argv[1]);
  if(image.empty()){
    std::cout << "error loading " << argv[1] << std::endl;
    return -1;
  }
  std::cout << "image size : " << image.cols << " x " << image.rows << std::endl;

  // convert to gray scale
  cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);


  // do something here ...
  const unsigned int size = 15;

  // Exercice 1
  cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
  cv::Mat structuringElement2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5));
  cv::Mat structuringElement3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5,5));

  cv::Mat rect_dilate;
  cv::dilate( image, rect_dilate, structuringElement );
  cv::Mat rect_erode;
  cv::erode(image,rect_erode,structuringElement);

  
  cv::Mat elip_dilate;
  cv::dilate( image, elip_dilate, structuringElement2 );
  cv::Mat elip_erode;
  cv::erode(image,elip_erode,structuringElement2);
 
  cv::Mat c_dilate;
  cv::dilate( image, c_dilate, structuringElement3 );
  cv::Mat c_erode;
  cv::erode(image,c_erode,structuringElement3);

  //Exercice 2
  cv::Mat morpho_ex_r;
  cv::morphologyEx(image,morpho_ex_r,cv::MORPH_OPEN,structuringElement);
  cv::Mat morpho_ex_e;
  cv::morphologyEx(image,morpho_ex_e,cv::MORPH_OPEN,structuringElement2);
  cv::Mat morpho_ex_c;
  cv::morphologyEx(image,morpho_ex_c,cv::MORPH_OPEN,structuringElement3);

  cv::Mat morphc_ex_r;
  cv::morphologyEx(image,morphc_ex_r,cv::MORPH_CLOSE,structuringElement);
  cv::Mat morphc_ex_e;
  cv::morphologyEx(image,morphc_ex_e,cv::MORPH_CLOSE,structuringElement2);
  cv::Mat morphc_ex_c;
  cv::morphologyEx(image,morphc_ex_c,cv::MORPH_CLOSE,structuringElement3);

  //Exercice 3
  cv::Mat ex3;
  cv::Mat structuringElementex3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(18,18));
  cv::morphologyEx(image,ex3,cv::MORPH_CLOSE,structuringElementex3);

  //Exercice 4

  cv::Mat ex4 = ex3-image;
  cv::Mat structuringElementex4 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10,10));
  cv::morphologyEx(ex4,ex4,cv::MORPH_OPEN,structuringElementex4);



  cv::Mat test;
  cv::Mat structuringElementext = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(25,65));
  cv::morphologyEx(image,test,cv::MORPH_CLOSE,structuringElementext);
  cv::Mat ex5 = test-ex3;
  cv::Mat structuringElementext2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15,15));
  cv::morphologyEx(ex5,ex5,cv::MORPH_OPEN,structuringElementext2);


  // display everything
  cv::imshow("image", image);
  //cv::waitKey();  
  // cv::imshow("ouverture R",morpho_ex_r);
  // cv::imshow("ouverture E",morpho_ex_e);
  // cv::imshow("ouverture C",morpho_ex_c);

  // cv::imshow("fermeture R",morphc_ex_r);
  // cv::imshow("fermeture E",morphc_ex_e);
  // cv::imshow("fermeture C",morphc_ex_c);
  // cv::imshow("rect dilate", rect_dilate);
  // cv::imshow("rect erode", rect_erode);  cv::imshow("ex3",ex3);
  
  // cv::imshow("elip dilate", elip_dilate);
  // cv::imshow("elip erode", elip_erode);
  // cv::imshow("cross dilate", c_dilate);
  // cv::imshow("cross erode", c_erode);
  
  // cv::imshow("ouverture R",morpho_ex_r);
  // cv::imshow("ouverture E",morpho_ex_e);
  // cv::imshow("ouverture C",morpho_ex_c);

  // cv::imshow("fermeture R",morphc_ex_r);
  // cv::imshow("fermeture E",morphc_ex_e);
  // cv::imshow("fermeture C",morphc_ex_c);
  cv::imshow("ex3",ex3);
  cv::imshow("ex4",ex4);
  cv::imshow("ex5",ex5);
  cv::waitKey();
 
  return 0;
}



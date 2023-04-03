#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include "fourierTransform.hpp"


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

#if 1
  // convert to gray scale
  cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
#else
  // extract one channel
  std::cout << "extract image channels ..." << std::endl;
  if(image.channels() == 3){
    cv::Mat channel[3];
    cv::split(image, channel);
    image = channel[2]; // green channel
  }
#endif

  // discrete Fourier Transform
  cv::Mat imageFourierMagnitude, imageFourierPhase;
  discreteFourierTransform(image, imageFourierMagnitude, imageFourierPhase);

  // save original magnitude and phase
  cv::imwrite("/tmp/imageFourierMagnitude.png",fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imwrite("/tmp/imageFourierPhase.png",fourierPhaseToDisplay(imageFourierPhase));


  /////////////////////////////////////////////
  // Exercice 1 (enlever les rayures)
#if 0
  imageFourierMagnitude.at<float>(imageFourierMagnitude.rows/2,10+imageFourierMagnitude.cols/2) = 0;//5000000;
  imageFourierMagnitude.at<float>(imageFourierMagnitude.rows/2,-10+imageFourierMagnitude.cols/2) = 0;//5000000;
#endif

  /////////////////////////////////////////////
  // Exercice 2 - Filtres passe bas
#if 0
  // some filtering on 'imageFourierMagnitude' and 'imageFourierPhase'
  float minRadius = 10;
  float maxRadius = 1000000;
  removeRing(imageFourierMagnitude, minRadius, maxRadius);
#endif

  /////////////////////////////////////////////
  // Exercice 2 - Filtres passe haut
#if 0
  // some filtering on 'imageFourierMagnitude' and 'imageFourierPhase'
  float minRadius = 1; // normalement 0 mais pour faire plus joli, on garde la fréquence 0 = couleur moyenne de l'image
  float maxRadius = 15;
  removeRing(imageFourierMagnitude, minRadius, maxRadius);
#endif

  ////////////////////////////////////////////
  // Exercice 2 - Filtres passe bande
#if 1
  // some filtering on 'imageFourierMagnitude' and 'imageFourierPhase'
  float minRadius = 45;
  float maxRadius = 55;
  removeRing(imageFourierMagnitude, 0, minRadius);
  removeRing(imageFourierMagnitude, maxRadius, 100000);
#endif 


/*
  cv::line(imageFourierMagnitude, cv::Point2i(20,50), cv::Point2i(200,20), cv::Scalar(0.0), 3);
  cv::line(imageFourierMagnitude, cv::Point2i(120,20), cv::Point2i(10,200), cv::Scalar(0.0), 3);
  cv::line(imageFourierMagnitude, cv::Point2i(400,350), cv::Point2i(20,250), cv::Scalar(0.0), 3);
  cv::line(imageFourierMagnitude, cv::Point2i(450,400), cv::Point2i(250,10), cv::Scalar(0.0), 3);
  cv::circle(imageFourierMagnitude, cv::Point2i(150,200), 50, cv::Scalar(0.0), -1);
  cv::circle(imageFourierMagnitude, cv::Point2i(300,450), 150, cv::Scalar(0.0), -1);
*/

  // inverse Fourier Transform
  cv::Mat outputImage;
  inverseDiscreteFourierTransform(imageFourierMagnitude, imageFourierPhase, outputImage, CV_8U);

  // display the Fourier magnitude
  cv::Mat magnitudeToDisplay = fourierMagnitudeToDisplay(imageFourierMagnitude);

  // convert magnitude from float to uchar
  // cv::Mat ucharMagnitude;
  // magnitudeToDisplay.convertTo(ucharMagnitude, CV_8UC1, 255, 0);
  
  // convert phase from float to uchar image
  /*
  double minValue, maxValue;
  cv::Mat imageFourierPhaseAbs = cv::abs(imageFourierPhase);
  cv::minMaxLoc(imageFourierPhaseAbs,&minValue,&maxValue);
  cv::Mat ucharPhase;
  imageFourierPhase.convertTo(ucharPhase, CV_8UC1, 0.5*255.0/(maxValue+1), 127);
  */

  // display everything
  cv::namedWindow("Input image");
  cv::namedWindow("Magnitude");
  cv::namedWindow("Output image");

  cv::moveWindow("Input image",100, 50);
  cv::moveWindow("Magnitude",700, 50);
  cv::moveWindow("Output image",100, 400);

  cv::imshow("Input image", image);
  cv::imshow("Magnitude", magnitudeToDisplay);
  cv::imshow("Output image", outputImage);
  cv::waitKey();
 
  // save the images
  cv::imwrite("output/inputImage.jpg",image);
  cv::imwrite("output/magnitude.png", magnitudeToDisplay);
  cv::imwrite("output/filteredImage.png", outputImage);

  return 0;
}



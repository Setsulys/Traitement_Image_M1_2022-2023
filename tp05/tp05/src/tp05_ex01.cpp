#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "fourierTransform.hpp"


cv::Mat SobelXFourierKernel(const int height,const int width)
{
  cv::Mat kernel = cv::Mat (width,height,CV_32F , cv::Scalar(0.0));
  kernel.at<float>(width/2 - 1,height/2 - 1) = -1;
  kernel.at<float>(width/2 ,height/2 - 1) = 0;
  kernel.at<float>(width/2 +1,height/2 - 1) = 1;

  kernel.at<float>(width/2 - 1,height/2) = -2;
  kernel.at<float>(width/2 ,height/2) = 0;
  kernel.at<float>(width/2 + 1,height/2) = 2;

  kernel.at<float>(width/2 - 1,height/2 + 1) = -1;
  kernel.at<float>(width/2,height/2 + 1) = 0;
  kernel.at<float>(width/2 + 1,height/2 + 1) = 1;

  return kernel;
}


cv::Mat SobelYFourierKernel(const int height,const int width)
{
  cv::Mat kernel = cv::Mat (width,height,CV_32F , cv::Scalar(0.0));
  kernel.at<float>(width/2 - 1,height/2 - 1) = -1;
  kernel.at<float>(width/2 ,height/2 - 1) = -2;
  kernel.at<float>(width/2 +1,height/2 - 1) = -1;

  kernel.at<float>(width/2 - 1,height/2 + 1) = 1;
  kernel.at<float>(width/2,height/2 + 1) = 2;
  kernel.at<float>(width/2 + 1,height/2 + 1) = 1;

  return kernel;
}

cv::Mat LaplaceFourierKernel(const int height,const int width)
{
  cv::Mat kernel = cv::Mat (width,height,CV_32F , cv::Scalar(0.0));
  kernel.at<float>(width/2 - 1,height/2 - 1) = 0;
  kernel.at<float>(width/2 ,height/2 - 1) = 1;
  kernel.at<float>(width/2 +1,height/2 - 1) = 0;

  kernel.at<float>(width/2 - 1,height/2) = 1;
  kernel.at<float>(width/2 ,height/2) = -4;
  kernel.at<float>(width/2 + 1,height/2) = 1;

  kernel.at<float>(width/2 - 1,height/2 + 1) = 0;
  kernel.at<float>(width/2,height/2 + 1) = 1;
  kernel.at<float>(width/2 + 1,height/2 + 1) = 0;

  return kernel;
}

cv::Mat RehausserFourierKernel(const int width,const int height)
{
  cv::Mat kernel = cv::Mat (width,height,CV_32F , cv::Scalar(0.0));
  kernel.at<float>(width/2 - 1,height/2 - 1) = 0;
  kernel.at<float>(width/2 ,height/2 - 1) = -1;
  kernel.at<float>(width/2 +1,height/2 - 1) = 0;

  kernel.at<float>(width/2 - 1,height/2) = -1;
  kernel.at<float>(width/2 ,height/2) = 5;
  kernel.at<float>(width/2 + 1,height/2) = -1;

  kernel.at<float>(width/2 - 1,height/2 + 1) = 0;
  kernel.at<float>(width/2,height/2 + 1) = -1;
  kernel.at<float>(width/2 + 1,height/2 + 1) = 0;

  return kernel;
}

cv::Mat gaussianFourierKernel(const int width,const int height,const double sigma)
{
  cv::Mat kernel(width,height,CV_64F,cv::Scalar(1));
  double sum = 0.0;
 
  for(int i =0; i < width;i++)
  {
    for(int j = 0;j < height;j++)
    {
      kernel.at<double>(i,j)= exp(-(pow(i-(width/2),2)+pow(j-(height/2),2))/(2*pow(sigma,2)));
      sum += kernel.at<double>(i,j);
    }
  }
  return kernel / sum;
}
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




  // discrete Fourier Transform
  cv::Mat imageFourierMagnitude, imageFourierPhase;
  discreteFourierTransform(image, imageFourierMagnitude, imageFourierPhase);

  // do someting here ....
  // ...
  //ex1-1/
  //imageFourierMagnitude.at<float>(image.rows/2,image.cols/2 -10)=0;
  //imageFourierMagnitude.at<float>(image.rows/2,image.cols/2 +10)=0;

  //ex1-2
  //removeRing(imageFourierMagnitude,30.,1500.);

  //ex1-3
  //removeRing(imageFourierMagnitude,0.,1.);

  //ex1-4
  // removeRing(imageFourierMagnitude,0.,5.);
  // removeRing(imageFourierMagnitude,55.,1500.);

  //ex2-1
  //cv::Mat filre = SobelXFourierKernel(image.rows,image.cols);
  //ex2-2
  //cv::Mat filtre = SobelYFourierKernel(image.rows,image.cols);
  //ex2-3
  //cv::Mat filtre = LaplaceFourierKernel(image.rows,image.cols);
  //ex2-4
  //cv::Mat filtre = RehausserFourierKernel(image.rows,image.cols);
  //ex2-5
  cv::Mat filtre = gaussianFourierKernel(image.rows,image.cols,20);
  cv::Mat filtreMagnitude;
  cv::Mat filtrePhase;
  discreteFourierTransform(filtre, filtreMagnitude, filtrePhase);
  for(int i =0; i < image.rows; i++){
    for(int j = 0; j< image.cols; j++){
      imageFourierMagnitude.at<float>(i,j)*= filtreMagnitude.at<float>(i,j);
    }
  }


  // inverse Fourier Transform
  cv::Mat outputImage;
  inverseDiscreteFourierTransform(imageFourierMagnitude, imageFourierPhase, outputImage, CV_8U);



  // display everything
  cv::namedWindow("Input image");
  cv::namedWindow("Magnitude");
  cv::namedWindow("Output image");

  cv::moveWindow("Input image",100, 50);
  cv::moveWindow("Magnitude",700, 50);
  cv::moveWindow("Output image",100, 400);

  cv::imshow("Input image", image);
  cv::imshow("Magnitude", fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imshow("Output image", outputImage);
  cv::waitKey();
 
  // save the images
  cv::imwrite("output/inputImage.jpg",image);
  cv::imwrite("output/magnitude.png", fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imwrite("output/pahse.png", fourierPhaseToDisplay(imageFourierPhase));
  cv::imwrite("output/filteredImage.png",outputImage);

  return 0;
}

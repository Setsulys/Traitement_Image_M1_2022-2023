#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include "fourierTransform.hpp"



//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat sobelXFourierKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

  // line 1
  kernel.at<float>((height/2)-1,(width/2)-1) = -1.0;
  kernel.at<float>((height/2)-1,(width/2))   =  0.0;
  kernel.at<float>((height/2)-1,(width/2)+1) =  1.0;

  // line 2
  kernel.at<float>((height/2),(width/2)-1) = -2.0;
  kernel.at<float>((height/2),(width/2))   =  0.0;
  kernel.at<float>((height/2),(width/2)+1) =  2.0;

  // line 3
  kernel.at<float>((height/2)+1,(width/2)-1) = -1.0;
  kernel.at<float>((height/2)+1,(width/2))   =  0.0;
  kernel.at<float>((height/2)+1,(width/2)+1) =  1.0;

  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat sobelYFourierKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

  // line 1
  kernel.at<float>((height/2)-1,(width/2)-1) = -1.0;
  kernel.at<float>((height/2)-1,(width/2))   = -2.0;
  kernel.at<float>((height/2)-1,(width/2)+1) = -1.0;

  // line 2
  kernel.at<float>((height/2),(width/2)-1) =  0.0;
  kernel.at<float>((height/2),(width/2))   =  0.0;
  kernel.at<float>((height/2),(width/2)+1) =  0.0;

  // line 3
  kernel.at<float>((height/2)+1,(width/2)-1) =  1.0;
  kernel.at<float>((height/2)+1,(width/2))   =  2.0;
  kernel.at<float>((height/2)+1,(width/2)+1) =  1.0;

  return kernel;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat laplacianFourierKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

  // line 1
  kernel.at<float>((height/2)-1,(width/2)-1) = 0.0;
  kernel.at<float>((height/2)-1,(width/2))   = 1.0;
  kernel.at<float>((height/2)-1,(width/2)+1) = 0.0;

  // line 2
  kernel.at<float>((height/2),(width/2)-1) =  1.0;
  kernel.at<float>((height/2),(width/2))   = -4.0;
  kernel.at<float>((height/2),(width/2)+1) =  1.0;

  // line 3
  kernel.at<float>((height/2)+1,(width/2)-1) =  0.0;
  kernel.at<float>((height/2)+1,(width/2))   =  1.0;
  kernel.at<float>((height/2)+1,(width/2)+1) =  0.0;

  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat rehausseurContoursKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

  // line 1
  kernel.at<float>((height/2)-1,(width/2)-1) =  0.0;
  kernel.at<float>((height/2)-1,(width/2))   = -1.0;
  kernel.at<float>((height/2)-1,(width/2)+1) =  0.0;

  // line 2
  kernel.at<float>((height/2),(width/2)-1) = -1.0;
  kernel.at<float>((height/2),(width/2))   =  5.0;
  kernel.at<float>((height/2),(width/2)+1) = -1.0;

  // line 3
  kernel.at<float>((height/2)+1,(width/2)-1) =  0.0;
  kernel.at<float>((height/2)+1,(width/2))   = -1.0;
  kernel.at<float>((height/2)+1,(width/2)+1) =  0.0;

  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat gaussianFourierKernel(const int width, const int height, const double sigma)
{  
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));
  
  float sum = 0.0;
  for(int i=0; i<height; ++i)
    for(int j=0; j<width; ++j){
      float squareDist = pow((-height*0.5)+i,2) + pow((-width*0.5)+j,2);
      kernel.at<float>(i,j) = exp(-squareDist/(2.0*sigma*sigma));
      sum += kernel.at<float>(i,j);
    }
  
  // kernel normalisation
  kernel = kernel / sum;
  
  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat averageFourierKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));
  int kernelSize = 11; // nombre impaire
  for(int i=-kernelSize/2; i<(kernelSize/2)+1; ++i)
    for(int j=-kernelSize/2; j<(kernelSize/2)+1; ++j)
      kernel.at<float>((height/2)+i,(width/2)+j) = 1.0/(kernelSize*kernelSize);

  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat scharrFourierKernel(const int width, const int height)
{
  cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

  // line 1
  kernel.at<float>((height/2)-1,(width/2)-1) = -3.0;
  kernel.at<float>((height/2)-1,(width/2))   =  0.0;
  kernel.at<float>((height/2)-1,(width/2)+1) =  3.0;

  // line 2
  kernel.at<float>((height/2),(width/2)-1) =  0.0;
  kernel.at<float>((height/2),(width/2))   =  0.0;
  kernel.at<float>((height/2),(width/2)+1) =  0.0;

  // line 3
  kernel.at<float>((height/2)+1,(width/2)-1) = -3.0;
  kernel.at<float>((height/2)+1,(width/2))   =  0.0;
  kernel.at<float>((height/2)+1,(width/2)+1) =  3.0;

  return kernel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void applyKernel(cv::Mat &magnitude, const cv::Mat &kernelMagnitude)
{
  for(int i=0; i<magnitude.rows; ++i)
    for(int j=0; j<magnitude.cols; ++j){
      magnitude.at<float>(i,j) *= kernelMagnitude.at<float>(i,j);
    } 
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



  //////////////////////////////////////////////////////
  
  // discrete Fourier Transform
  cv::Mat imageFourierMagnitude, imageFourierPhase;
  discreteFourierTransform(image, imageFourierMagnitude, imageFourierPhase);
  cv::imwrite("output/magnitude_input.png", fourierMagnitudeToDisplay(imageFourierMagnitude));


  // create kernel
  //cv::Mat kernel = sobelXFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);
  //cv::Mat kernel = sobelYFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);
  //cv::Mat kernel = 4 * laplacianFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);
  //cv::Mat kernel = rehausseurContoursKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);
  cv::Mat kernel = gaussianFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows,std::sqrt(8));
  //cv::Mat kernel = averageFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);
  //cv::Mat kernel = scharrFourierKernel(imageFourierMagnitude.cols, imageFourierMagnitude.rows);

  cv::Mat kernelFourierMagnitude, kernelFourierPhase;
  discreteFourierTransform(kernel, kernelFourierMagnitude, kernelFourierPhase);


  // apply kernel
  applyKernel(imageFourierMagnitude,kernelFourierMagnitude);


  // inverse Fourier Transform
  cv::Mat outputImage;
  inverseDiscreteFourierTransform(imageFourierMagnitude, imageFourierPhase, outputImage, CV_8U);



  //////////////////////////////////////////////////////


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
  cv::imwrite("output/magnitude_output.png", fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imwrite("output/filteredImage.png",outputImage);
  cv::imwrite("output/kernelMagnitude.png",fourierMagnitudeToDisplay(kernelFourierMagnitude));

  return 0;
}



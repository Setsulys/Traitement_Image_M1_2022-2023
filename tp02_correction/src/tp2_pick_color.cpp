#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cmath> 


// bad global variables
cv::Mat image;
 
//////////////////////////////////////////////////////////////////////////////////////////////////
void callBackKeyboard(int event, int x, int y, int flags, void* userdata)
{

  (void)userdata; // just to avoid compilation warning of unused variable
  (void)flags;    // just to avoid compilation warning of unused variable

   switch(event){
    case cv::EVENT_LBUTTONDOWN : 
      std::cout << "left button pressed at : " << x << ", " << y << std::endl;
      std::cout << "image color in (R,G,B) : (" << int(image.at<cv::Vec3b>(y,x)[2]) << ", " << int(image.at<cv::Vec3b>(y,x)[1]) << ", " << int(image.at<cv::Vec3b>(y,x)[0]) << ")" << std::endl;
      break;

    case cv::EVENT_RBUTTONDOWN : 
    case cv::EVENT_MBUTTONDOWN : 
    case cv::EVENT_MOUSEMOVE   : 
      break;
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
  image = cv::imread(argv[1]);
  if(image.empty()){
    std::cout << "error loading " << argv[1] << std::endl;
    return -1;
  }

  // display the image
  cv::namedWindow("une image");
  //cv::moveWindow("une image", 2000,20);
  cv::setMouseCallback("une image", callBackKeyboard, NULL);
  cv::imshow("une image", image);
  std::cout << "cliquez sur l'image ou appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  return 1;
}

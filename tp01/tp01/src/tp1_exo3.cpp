#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

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


  // do something
  // ...

int x = image.cols/2.0 -20;
int y = image.rows/2.0 -20;
  
  for(int i = 0; i <40;i++ )
  {
    for(int j = 0 ; j < 40;j++)
    {
      image.at<cv::Vec3b>(i+y,j+x)[2] = 0;
      image.at<cv::Vec3b>(i+y,j+x)[1] = 255;
      image.at<cv::Vec3b>(i+y,j+x)[0] = 0;
    }
  }

  for(int i = 0; i <40;i++ )
  {
    for(int j = 0 ; j <40;j++)
    {
      image.at<cv::Vec3b>(i+y,j+x)[2] =0;
      image.at<cv::Vec3b>(i+y,j+x)[1] =0;
      image.at<cv::Vec3b>(i+y,j+x)[0] =0;
    }
  }
  for(int i = 0; i <40;i++ )
  {
    for(int j = 0 ; j <40;j++)
    {
      image.at<cv::Vec3b>(i+y,j+x)[2] += 5*j;
      image.at<cv::Vec3b>(i+y,j+x)[1] += 5*j;
      image.at<cv::Vec3b>(i+y,j+x)[0] += 5*j;
    }
  }


  cv::Point center((int) image.cols/2.0, (int)image.rows/2.0);
  cv::Scalar ray(0,0,255);
  cv::circle(image,center,40,ray,1);
  // display an image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // save the image
  cv::imwrite("output/tp1ex3.jpg",image);

  return 1;
}

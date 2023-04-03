#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cmath> 


void get_rotation(cv::Mat &R, const float angle_in_degree)
{
  // rotation around the origine
  R = cv::Mat(3, 3, CV_32FC1, cv::Scalar(0.0f));
  const float angle_in_radian = angle_in_degree * M_PI / 180.0;
  R.at<float>(0,0) =  std::cos(angle_in_radian);
  R.at<float>(1,1) =  std::cos(angle_in_radian);
  R.at<float>(1,0) =  std::sin(angle_in_radian);
  R.at<float>(0,1) = -std::sin(angle_in_radian);
  R.at<float>(2,2) =  1.0;
}

void get_translation(cv::Mat &T, const float t_x, const float t_y)
{ 
  // rotation around the origine
  T = cv::Mat(3, 3, CV_32FC1);
  cv::setIdentity(T);
  T.at<float>(0,2) = t_x;
  T.at<float>(1,2) = t_y;
}

void rotate_image(cv::Mat &image, const float angle_in_degree)
{
  //rotation matrix
  cv::Mat R;
  get_rotation(R, angle_in_degree);

  //std::cout << R << std::endl;
  // apply transformation
  cv::warpPerspective( image, image, R, image.size());
}

void translate_image(cv::Mat &image, const float center_x, const float center_y)
{
  // translation matrix
  cv::Mat T;
  get_translation(T, center_x, center_y);

  //std::cout << T << std::endl;
  // apply transformation
  cv::warpPerspective( image, image, T, image.size());
}

void rotate_image_around_point(cv::Mat &image, const float angle_in_degree, const float center_x, const float center_y)
{
  // rotation matrix
  cv::Mat R;
  get_rotation(R, angle_in_degree);

  // translation and inverse translation matrix
  cv::Mat T, Tinv;
  get_translation(T, center_x, center_y);
  get_translation(Tinv, -center_x, -center_y);

  // combine 3 matrices
  cv::Mat M = T*R*Tinv;

  // apply transformation
  cv::warpPerspective( image, image, M, image.size());
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

  // display the image
  cv::namedWindow("une image");
  // cv::moveWindow("une image", 2000,20);
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  

  translate_image(image, 200,10);

  // question 1
  //rotate_image(image, 20);

  // question xx
  //rotate_image_around_point(image, 10, image.cols/2, image.rows/2);

  // question xx
/*  const int nb_run = 20;
  for(int i=0; i<nb_run; ++i)
    rotate_image_around_point(image, 10, image.cols/2, image.rows/2);

  for(int i=0; i<nb_run; ++i)
    rotate_image_around_point(image, -10, image.cols/2, image.rows/2);*/


  // display the image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  // save the image
  cv::imwrite("output/tp2ex1.jpg",image);

  return 1;
}

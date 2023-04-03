#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cmath> 

//////////////////////////////////////////////////////////////////////////////////////////////////
  void get_rotation(cv::Mat &R, const float angle_in_degree){
    float radian = (angle_in_degree/180) * M_PI;
    R.at<float>(0,0) = cos(radian);
    R.at<float>(0,1) = -sin(radian);
    R.at<float>(1,0) = sin(radian);
    R.at<float>(1,1) = cos(radian);
    R.at<float>(2,2) = 1;
    std::cout << R << std::endl;
  }

  void set_rotation(cv::Mat image,const float angle_in_degree){
    cv::Mat R = cv::Mat (3,3,CV_32FC1 , cv::Scalar(0.0f));
    get_rotation(R,angle_in_degree);
    cv::warpPerspective(image,image,R,image.size());
  }
//////////////////////////////////////////////////////////////////////////////////////////////////
  void get_translation(cv::Mat &T, const float t_x, const float t_y){
    T.at<float>(0,0) = 1;
    T.at<float>(1,1) = 1;
    T.at<float>(2,2) = 1;
    T.at<float>(0,2) = t_x;
    T.at<float>(1,2) = t_y;
    std::cout << T << std::endl;
  }

  void set_translation(cv::Mat image,const float t_x, const float t_y){
    cv::Mat R = cv::Mat (3,3,CV_32FC1 , cv::Scalar(0.0f));
    get_translation(R,t_x,t_y);
    cv::warpPerspective(image,image,R,image.size());
  }
  
//////////////////////////////////////////////////////////////////////////////////////////////////

  void rotate_image_around_point(cv::Mat &image,  const float angle_in_degree, const float center_x, const float center_y){
    cv::Mat R = cv::Mat (3,3,CV_32FC1 , cv::Scalar(0.0f));
    get_rotation(R,angle_in_degree);
    cv::Mat T = cv::Mat (3,3,CV_32FC1 , cv::Scalar(0.0f));
    get_translation(T,center_x,center_y);
    cv::Mat Ti = cv::Mat (3,3,CV_32FC1 , cv::Scalar(0.0f));
    get_translation(Ti,-center_x,-center_y);

    cv::Mat rotated = T * R * Ti;
    cv::warpPerspective(image,image,rotated,image.size());
  }

//////////////////////////////////////////////////////////////////////////////////////////////////

  void lot_of_rotation(cv::Mat image,const int values, const float angle_in_degree,const float center_x,const float center_y){
    for(int i =0;i < values;i++){
      rotate_image_around_point(image,angle_in_degree,center_x,center_y);
    }
    for(int i =0;i < values;i++){
     rotate_image_around_point(image,-angle_in_degree,center_x,center_y);
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

  // display the image
  cv::namedWindow("une image");
  // cv::moveWindow("une image", 2000,20);
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  // do something here ...

  //set_rotation(image,45);
  //set_translation(image,image.cols/8.0,image.rows/8.0);
  //rotate_image_around_point(image,30,image.cols/4.0,image.rows/4.0);
  
  lot_of_rotation(image,20,10,image.cols/2.0,image.rows/2.0);

  // display the image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // save the image
  cv::imwrite("output/tp2ex1.jpg",image);

  return 1;
}

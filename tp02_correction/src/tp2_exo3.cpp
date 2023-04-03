#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cmath> 



//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  // check arguments
  if(argc != 3){
    std::cout << "usage: " << argv[0] << " [image_fond_vert] [image_remplacement]" << std::endl;
    return -1;
  }

  // load the first image
  std::cout << "load images ..." << std::endl;
  cv::Mat image = cv::imread(argv[1]);
  if(image.empty()){
    std::cout << "error loading " << argv[1] << std::endl;
    return -1;
  }
  std::cout << "image size : " << image.cols << " x " << image.rows << std::endl;

  // load the second image
  cv::Mat image2 = cv::imread(argv[2]);
  if(image2.empty()){
    std::cout << "error loading " << argv[2] << std::endl;
    return -1;
  }
  std::cout << "image2 size : " << image2.cols << " x " << image2.rows << std::endl;


  // display the image
  cv::namedWindow("une image");
  //cv::moveWindow("une image", 2000,20);
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();
  
  // convert image to LAB
  cv::Mat imageLAB;
  cvtColor(image, imageLAB, cv::COLOR_BGR2Lab);

  // define target color
  // (467, 381) fonctionne bien avec ../input/fond_vert_02.png ../input/wb_auto.png
  cv::Vec3d target_color_Lab = imageLAB.at<cv::Vec3b>(467, 381); 
  std::cout << "target color " << target_color_Lab << std::endl;

  // remove green
  for(int i=0; i<image.rows; ++i)
    for(int j=0; j<image.cols; ++j){
      cv::Vec3d pixel = imageLAB.at<cv::Vec3b>(i,j);
      if( cv::norm(target_color_Lab - pixel) < 40)
        // image.at<cv::Vec3b>(i,j) = cv::Vec3b(255,255,255);
        image.at<cv::Vec3b>(i,j) = image2.at<cv::Vec3b>(i%image2.rows,j%image2.cols);
    }

  // display the image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // save the image
  cv::imwrite("output/tp2ex3.jpg",image);

  return 1;
}

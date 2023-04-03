#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>





//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  // check arguments
  if(argc != 3){
    std::cout << "usage: " << argv[0] << " image1 imag2" << std::endl;
    return -1;
  }

  // load the input image
  std::cout << "load images ..." << std::endl;
  cv::Mat image1 = cv::imread(argv[1]);
  if(image1.empty()){
    std::cout << "error loading " << argv[1] << std::endl;
    return -1;
  }
  std::cout << "image1 size     : " << image1.cols << " x " << image1.rows << std::endl;
  std::cout << "image1 channels : " << image1.channels() << std::endl << std::endl;

  // load the input transparent image
  cv::Mat image2 = cv::imread(argv[2],-1);
  if(image2.empty()){
    std::cout << "error loading " << argv[2] << std::endl;
    return -1;
  }
  std::cout << "image2 size : " << image2.cols << " x " << image2.rows << std::endl;
  std::cout << "image2 channels : " << image2.channels() << std::endl << std::endl;


  // merge the images with transparency
  for(int i=0; i<image1.rows; ++i)
    for(int j=0; j<image1.cols; ++j){
      float alpha = image2.at<cv::Vec4b>(i,j)[3] / 255.0; // percentage of transparency
      for(int c=0; c<3; ++c)
        image1.at<cv::Vec3b>(i,j)[c] = (1.0-alpha)*image1.at<cv::Vec3b>(i,j)[c] + alpha*image2.at<cv::Vec4b>(i,j)[c]; // linear blending
    }


  // display an image
  cv::imshow("une image", image1);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  cv::imwrite("output/tp1ex3.jpg",image1);
  // save the image

  return 1;
}

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <algorithm> // std::clamp



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

  //////////////////////////////////////////////////////
  // read and write pixel values
  std::cout << "pixel value (50,100) BGR : " << (int) image.at<cv::Vec3b>(50,100)[0] << " " << (int) image.at<cv::Vec3b>(50,100)[1] << " " << (int) image.at<cv::Vec3b>(50,100)[2] << std::endl; 


  //////////////////////////////////////////////////////
  // display an image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // colorie un pixel
  image.at<cv::Vec3b>(10,20)[0] = 0;
  image.at<cv::Vec3b>(10,20)[1] = 0;
  image.at<cv::Vec3b>(10,20)[2] = 255;
  // image.at<cv::Vec3b>(11,20) = cv::Vec3b(0,0,255);

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  // une ligne rouge
  for(int j=0; j<image.cols; ++j){
    image.at<cv::Vec3b>(42,j) = cv::Vec3b(0,0,255);
    // image.at<cv::Vec3b>(42,j)[0] = 0;
    // image.at<cv::Vec3b>(42,j)[1] = 0;
    // image.at<cv::Vec3b>(42,j)[2] = 255;
  }

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();
 
  //////////////////////////////////////////////////////
  // brightness
  for(int i=0; i<image.rows; ++i)
    for(int j=0; j<image.cols; ++j)
      for(int c=0; c<3; ++c) {
        // image.at<cv::Vec3b>(i,j)[c] = (unsigned char) std::min(255, std::max(0,int(image.at<cv::Vec3b>(i,j)[c])+50));
        // image.at<cv::Vec3b>(i,j)[c] = cv::saturate_cast<uchar>(int( image.at<cv::Vec3b>(i,j)[c] ) + 50); 
        image.at<cv::Vec3b>(i,j)[c] = (unsigned char) std::clamp(int(image.at<cv::Vec3b>(i,j)[c])+50,0,255);        
      }

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  //////////////////////////////////////////////////////
  // négatif
  for(int i=0; i<image.rows; ++i)
    for(int j=0; j<image.cols; ++j)
      for(int c=0; c<3; ++c)
        image.at<cv::Vec3b>(i,j)[c] = 255 - image.at<cv::Vec3b>(i,j)[c];

  // oubien 
  // image = 255 - image;

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  //////////////////////////////////////////////////////
  cvtColor(image, image, cv::COLOR_BGR2GRAY);

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  //////////////////////////////////////////////////////
  // seuillage
  for(int i=0; i<image.rows; ++i)
    for(int j=0; j<image.cols; ++j)
      if(image.at<unsigned char>(i,j) > 50) image.at<unsigned char>(i,j) = 255;
      else image.at<unsigned char>(i,j) = 0;
 
   // cv::threshold(image, image, 50, 255, cv::THRESH_BINARY);
   // ou
   // image = image > 50 ;

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  // reset image
  image = cv::imread(argv[1]);

  // carré vert centré
  const int square_size = 50;
  for(int i=(image.rows-square_size)/2; i<(image.rows+square_size)/2; ++i)
    for(int j=(image.cols-square_size)/2; j<(image.cols+square_size)/2; ++j)
      image.at<cv::Vec3b>(i,j) = cv::Vec3b(0,255,0);

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // carré dégradé
  for(int i=(image.rows-square_size)/2; i<(image.rows+square_size)/2; ++i)
    for(int j=(image.cols-square_size)/2; j<(image.cols+square_size)/2; ++j){
      unsigned char value = 255*(i-(image.rows-square_size)/2)/square_size;
      image.at<cv::Vec3b>(i,j) = cv::Vec3b(value,value,value);
    }

  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // disque
  constexpr int center_i = 100;
  constexpr int center_j = 200;
  constexpr int radius   = 50;
  constexpr int squared_radius = radius * radius;

  for(int i=-radius; i<radius; ++i)
    for(int j=-radius; j<radius; ++j)
      if(i*i+j*j < squared_radius)
        image.at<cv::Vec3b>(center_i+i,center_j+j) = cv::Vec3b(0,255,255);

  // ou bien cercle avec openCV
  //cv::circle(image, cv::Point2i(center_j,center_i), radius, cv::Scalar(0,255,255), -1);

  // display an image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();


  // save the image
  cv::imwrite("output/tp1ex1.jpg",image);

  return 1;
}

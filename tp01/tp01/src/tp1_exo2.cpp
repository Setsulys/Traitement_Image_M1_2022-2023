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
  int i=50;
  int j=100;
  std::cout << "RGB :( " << (int) image.at<cv::Vec3b>(i,j)[2]<< ", " << (int) image.at<cv::Vec3b>(i,j)[1] <<  ", " <<(int) image.at<cv::Vec3b>(i,j)[0] << " )" <<std::endl;
  cv::imshow("une image", image);

  i = 10;
  j = 20; 
  image.at<cv::Vec3b>(i,j)[2] = 255;
  image.at<cv::Vec3b>(i,j)[1] = 0;
  image.at<cv::Vec3b>(i,j)[0] = 0;
  cv::imshow("une image", image);

  i = 45;
  for( int k  = 0; k < image.cols ; k++)
  {
    image.at<cv::Vec3b>(i,k)[2] = 255;
    image.at<cv::Vec3b>(i,k)[1] = 0;
    image.at<cv::Vec3b>(i,k)[0] = 0;
  }

  cv::imshow("une image", image);
  /*
  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      image.at<cv::Vec3b>(i,j)[2] += 50;
      image.at<cv::Vec3b>(i,j)[1] += 50;
      image.at<cv::Vec3b>(i,j)[0] += 50;
    }
  }*/
  //les valeurs des pixels sont overflow mais comme c'est modulo sa reviens (ex (200 + 100)%255 = 45)

  cv::imshow("une image", image);
  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      image.at<cv::Vec3b>(i,j)[2] = std::min(std::max(image.at<cv::Vec3b>(i,j)[2] + 50,0),255);
      image.at<cv::Vec3b>(i,j)[1] = std::min(std::max(image.at<cv::Vec3b>(i,j)[1] + 50,0),255);
      image.at<cv::Vec3b>(i,j)[0] = std::min(std::max(image.at<cv::Vec3b>(i,j)[0] + 50,0),255);
    }
  }
  cv::imshow("une image", image);


  //negatif de l'image
  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      image.at<cv::Vec3b>(i,j)[2] = std::abs(image.at<cv::Vec3b>(i,j)[2]-255);
      image.at<cv::Vec3b>(i,j)[1] = std::abs(image.at<cv::Vec3b>(i,j)[1]-255);
      image.at<cv::Vec3b>(i,j)[0] = std::abs(image.at<cv::Vec3b>(i,j)[0]-255);
    }
  }
  cv::imshow("une image", image);



  //niveau de gris
  cv::cvtColor(image,image,cv::COLOR_BGR2GRAY);
  cv::imshow("une image", image);


  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      if(image.at<unsigned char>(i,j)>255/2.0)
      {
        image.at<unsigned char>(i,j) = 255;
      }
      else
      {
        image.at<unsigned char>(i,j) =0;
      }
    }
  }

  // display an image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();
  

  // save the image
  cv::imwrite("output/tp1ex2.jpg",image);

  return 1;
}

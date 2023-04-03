#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
cv::Mat derivative_x()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  //kernel.at<double>(0,1) = 0.42;
  // finish it ...
  kernel.at<double>(1,1) = -1.0;
  kernel.at<double>(1,2) = 1.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat derivative_y()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  //kernel.at<double>(0,1) = 0.42;
  // finish it ...
  kernel.at<double>(1,1) = -1.0;
  kernel.at<double>(2,1) = 1.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat laplacian()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  //kernel.at<double>(0,1) = 0.42;
  // finish it ...
  kernel.at<double>(0,1) = 1.0;
  kernel.at<double>(1,0) = 1.0;
  kernel.at<double>(1,2) = 1.0;
  kernel.at<double>(2,1) = 1.0;
  kernel.at<double>(1,1) = -4.0;

  return kernel;
}


///////////////////////////////////////////////////////////////////////////////
cv::Mat rehausseur()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  //kernel.at<double>(0,1) = 0.42;
  // finish it ...
  kernel.at<double>(0,1) = -1.0;
  kernel.at<double>(1,0) = -1.0;
  kernel.at<double>(1,2) = -1.0;
  kernel.at<double>(2,1) = -1.0;
  kernel.at<double>(1,1) = 5.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////

cv::Mat average(const unsigned int n)
{
  cv::Mat kernel(n,n,CV_64F,cv::Scalar(1));
  cv::Mat avg =  1/(pow(n,2))* kernel;
  return avg;
}

///////////////////////////////////////////////////////////////////////////////

cv::Mat gaussienne(const int n, const double sigma)
{
  cv::Mat kernel(n,n,CV_64F,cv::Scalar(1));
  double sum = 0.0;
 
  for(int i =0; i < n;i++)
  {
    for(int j = 0;j < n;j++)
    {
      kernel.at<double>(i,j)= exp(-(pow(i-(n/2),2)+pow(j-(n/2),2))/(2*pow(sigma,2)));
      sum += kernel.at<double>(i,j);
    }
  }
  return kernel / sum;
}

///////////////////////////////////////////////////////////////////////////////

double mediane(std::vector<double> list)
{
  sort(list.begin(),list.end());
  int size = list.size();
  if(size%2 ==0)
  {
    return (list[size/2-1] + list[size/2])/2;
  }
  return (list[size/2]);
}

cv::Mat median_Mat(cv::Mat image)
{
  std::vector<double> vect;
  for(int i = 0;i < image.rows;i++)
  {
    for(int j = 0;j < image.cols;j++)
    {
      vect.push_back(image.at<double>(i,j));
    }
  }
  auto med = mediane(vect);
}

///////////////////////////////////////////////////////////////////////////////
void apply_convolution(const cv::Mat& src, cv::Mat& dst, const cv::Mat &kernel, const unsigned char offset)
{
  dst = cv::Mat(src.size(), CV_8UC1, cv::Scalar(0));

  for(int i=kernel.rows/2; i<dst.rows - (kernel.rows/2); ++i)
    for(int j=kernel.cols/2; j<dst.cols - (kernel.cols/2); ++j){
      double value = 0;
      for(int u=0; u<kernel.rows; ++u)
        for(int v=0; v<kernel.rows; ++v){
          int pos_i = i -(kernel.rows/2) + u;
          int pos_j = j -(kernel.cols/2) + v;
          value += double(src.at<unsigned char>(pos_i,pos_j) * kernel.at<double>(u,v));
        }
      dst.at<unsigned char>(i,j) = std::clamp(value + offset, 0.0, 255.0);
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

  // convert the image to grayscale
  cvtColor(image,image,cv::COLOR_BGR2GRAY);

  // display an image
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", image);
  cv::waitKey();


  // do something here ... ////////
  cv::Mat filtered_image;

  // // derivative x
  // apply_convolution(image, filtered_image, derivative_x(), 128);
  // std::cout << "appuyer sur une touche ..." << std::endl;
  // cv::imshow("image", filtered_image);
  // cv::waitKey();


  // // derivative y
  // apply_convolution(image, filtered_image, derivative_y(), 128);
  // std::cout << "appuyer sur une touche ..." << std::endl;
  // cv::imshow("image", filtered_image);
  // cv::waitKey();


  // // laplacian
  // apply_convolution(image, filtered_image, laplacian(), 128);
  // std::cout << "appuyer sur une touche ..." << std::endl;
  // cv::imshow("image", filtered_image);
  // cv::waitKey();

  // // rehausser
  // apply_convolution(image, filtered_image, rehausseur(), 0);
  // std::cout << "appuyer sur une touche ..." << std::endl;
  // cv::imshow("image", filtered_image);
  // cv::waitKey();

  // // average
  // apply_convolution(image, filtered_image, average(15), 0);
  // std::cout << "appuyer sur une touche ..." << std::endl;
  // cv::imshow("image", filtered_image);
  // cv::waitKey();

    // rehauser
  apply_convolution(image, filtered_image, gaussienne(5,15), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  ////////////////////////////////


  // save images
  cv::imwrite("output/image.jpg",image);


  return 1;
}

/*
2
  f'(x,y) ~ f(x,y) - f(x+1,y) 
              -1        1
4
On fait une convolution
On récupere une matrice dst contenant que des 0 on place la matrice kernel dans la matrice dst et on fait la somme des valeurs au tours du pixel

*/
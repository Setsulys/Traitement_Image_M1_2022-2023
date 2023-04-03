#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <algorithm>



///////////////////////////////////////////////////////////////////////////////
cv::Mat derivative_x()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  kernel.at<double>(1,1) = -1.0;
  kernel.at<double>(1,2) =  1.0;

  return kernel;
}


///////////////////////////////////////////////////////////////////////////////
cv::Mat derivative_x_centered()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  kernel.at<double>(1,0) = -1.0;
  kernel.at<double>(1,2) =  1.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat sobel_x()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  kernel.at<double>(0,0) = -1.0;
  kernel.at<double>(0,1) =  0.0;
  kernel.at<double>(0,2) =  1.0;

  kernel.at<double>(1,0) = -2.0;
  kernel.at<double>(1,1) =  0.0;
  kernel.at<double>(1,2) =  2.0;

  kernel.at<double>(2,0) = -1.0;
  kernel.at<double>(2,1) =  0.0;
  kernel.at<double>(2,2) =  1.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat sobel_y()
{
  return sobel_x().t();
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat laplacian()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  kernel.at<double>(0,0) = 0.0;
  kernel.at<double>(0,1) = 1.0;
  kernel.at<double>(0,2) = 0.0;

  kernel.at<double>(1,0) = 1.0;
  kernel.at<double>(1,1) =-4.0;
  kernel.at<double>(1,2) = 1.0;

  kernel.at<double>(2,0) = 0.0;
  kernel.at<double>(2,1) = 1.0;
  kernel.at<double>(2,2) = 0.0;

  return kernel;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat sharpen()
{
  // create and initialize and 1d array of integers
  cv::Mat kernel(3,3,CV_64F, cv::Scalar(0));

  // build your kernel
  kernel.at<double>(0,0) = 0.0;
  kernel.at<double>(0,1) =-1.0;
  kernel.at<double>(0,2) = 0.0;

  kernel.at<double>(1,0) =-1.0;
  kernel.at<double>(1,1) = 5.0;
  kernel.at<double>(1,2) =-1.0;

  kernel.at<double>(2,0) = 0.0;
  kernel.at<double>(2,1) =-1.0;
  kernel.at<double>(2,2) = 0.0;

  std::cout << kernel << std::endl;

  return kernel;
}


///////////////////////////////////////////////////////////////////////////////
cv::Mat blur(const int n)
{
  return cv::Mat(n,n,CV_64F, cv::Scalar(1.0/(n*n)));
}


///////////////////////////////////////////////////////////////////////////////
cv::Mat gaussian_kernel(int size, double sigma)
{
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_64F);

    int half_size = size / 2;
    double sum = 0.0; // remember, the sum of all elements of the kernel should be zero at the end
    for (int i = -half_size; i <= half_size; i++)
        for (int j = -half_size; j <= half_size; j++)
        {
            double value = exp(-(i * i + j * j) / (2 * sigma * sigma));
            kernel.at<double>(i + half_size, j + half_size) = value;
            sum += value;
        }

    kernel /= sum;
    return kernel;
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


///////////////////////////////////////////////////////////////////////////////
void median_filter(const cv::Mat& src, cv::Mat& dst, const int size)
{
  dst = cv::Mat(src.size(), CV_8UC1, cv::Scalar(0));
  const int half_size = size / 2;

  std::vector<unsigned char> vec(size*size);
  for(int i=half_size; i<dst.rows - half_size; ++i)
    for(int j=half_size; j<dst.cols - half_size; ++j){
      int k=0;
      for(int u=-half_size; u<=half_size; ++u)
        for(int v=-half_size; v<=half_size; ++v){
          vec[k] = src.at<unsigned char>(i+u,j+v);
          k++;
        }

      std::sort(vec.begin(), vec.end());
      dst.at<unsigned char>(i,j) = vec[size*size/2];
    }
}


///////////////////////////////////////////////////////////////////////////////
cv::Mat image_integral(const cv::Mat &image)
{
  cv::Mat dst = cv::Mat(image.size(), CV_32S, cv::Scalar(0));
  for(int i=0; i<dst.rows; ++i)
    for(int j=0; j<dst.cols; ++j)
    {
      dst.at<int>(i,j) = int(image.at<unsigned char>(i,j)) 
                       + dst.at<int>(std::max(i-1,0),j) 
                       + dst.at<int>(i,std::max(j-1,0))
                       - dst.at<int>(std::max(i-1,0),std::max(j-1,0));
    }

    return dst;
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat blur_integral(const cv::Mat &image, const int size)
{
  cv::Mat integral = image_integral(image);
  cv::Mat dst(image.size(), CV_8UC1, cv::Scalar(0));
  
  const int half_size = size / 2;
  for(int i=half_size; i<dst.rows - half_size; ++i)
    for(int j=half_size; j<dst.cols - half_size; ++j){

      dst.at<unsigned char>(i,j) = (integral.at<int>(i-half_size, j-half_size) 
                                  + integral.at<int>(i+half_size, j+half_size) 
                                  - integral.at<int>(i-half_size, j+half_size) 
                                  - integral.at<int>(i+half_size, j-half_size)) / ( (size-1) * (size-1));
    }

    return dst;
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

  // derivative x
  apply_convolution(image, filtered_image, derivative_x(), 128);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // derivative x centered
  apply_convolution(image, filtered_image, derivative_x_centered(), 128);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // x sobel
  apply_convolution(image, filtered_image, sobel_x(), 128);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // y sobel
  apply_convolution(image, filtered_image, sobel_y(), 128);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // laplacian
  apply_convolution(image, filtered_image, laplacian(), 128);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // originale
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", image);
  cv::waitKey();

  // sharpen
  apply_convolution(image, filtered_image, sharpen(), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // blur 1
  apply_convolution(image, filtered_image, blur(5), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // blur 2
  apply_convolution(image, filtered_image, blur(11), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // blur 3
  apply_convolution(image, filtered_image, blur(21), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // gaussian
  apply_convolution(image, filtered_image, gaussian_kernel(21, 10), 0);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // median filter 1
  median_filter(image, filtered_image, 3);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  // median filter 2
  median_filter(image, filtered_image, 7);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();
  


  // blur integral image
  filtered_image = blur_integral(image, 11);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::imshow("image", filtered_image);
  cv::waitKey();

  ////////////////////////////////


  // save images
  cv::imwrite("output/image.jpg",image);


  return 1;
}

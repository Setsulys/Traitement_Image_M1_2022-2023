#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cmath> 


//////////////////////////////////////////////////////////////////////////////////////////////////

void suppr_couleur(const cv::Mat &imageLAB, const cv::Vec3d targetLAB, cv::Mat &image){
  
  for(int i = 0; i < image.rows;i++){
    for(int j =0; j < image.cols;j++){
        float b = imageLAB.at<cv::Vec3b>(i,j)[0] - targetLAB[0];
        float g = imageLAB.at<cv::Vec3b>(i,j)[1] - targetLAB[1];
        float r = imageLAB.at<cv::Vec3b>(i,j)[2] - targetLAB[2];
        if(sqrt(pow(r,2)+ pow(g,2)+ pow(b,2) ) < 50){
          image.at<cv::Vec3b>(i,j)[0]=255;
          image.at<cv::Vec3b>(i,j)[1]=255;
          image.at<cv::Vec3b>(i,j)[2]=255;
          
        }
    }
  }
}


void repl_couleur(const cv::Mat &imageLAB, const cv::Vec3d targetLAB, cv::Mat &image, const cv::Mat &bg_image){
  
  for(int i = 0; i < image.rows;i++){
    for(int j =0; j < image.cols;j++){
        float b = imageLAB.at<cv::Vec3b>(i,j)[0] - targetLAB[0];
        float g = imageLAB.at<cv::Vec3b>(i,j)[1] - targetLAB[1];
        float r = imageLAB.at<cv::Vec3b>(i,j)[2] - targetLAB[2];
        if(sqrt(pow(r,2)+ pow(g,2)+ pow(b,2) ) < 50){
          // image.at<cv::Vec3b>(i,j)[0]=255;
          // image.at<cv::Vec3b>(i,j)[1]=255;
          // image.at<cv::Vec3b>(i,j)[2]=255;
          image.at<cv::Vec3b>(i,j) = bg_image.at<cv::Vec3b>(i%bg_image.rows,j%bg_image.cols);
        }
    }
  }
}

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

  // load the second image
  cv::Mat image2 = cv::imread(argv[2]);
  if(image2.empty()){
    std::cout << "error loading " << argv[2] << std::endl;
    return -1;
  }

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
  cv::Vec3d target_color_Lab = imageLAB.at<cv::Vec3b>(100, 100);
  std::cout << "target color " << target_color_Lab << std::endl;



  // remove green here ...
  repl_couleur(imageLAB,target_color_Lab,image,image2);


  // display the image
  cv::imshow("une image", image);
  std::cout << "appuyer sur une touche ..." << std::endl;
  cv::waitKey();

  // save the image
  cv::imwrite("output/tp2ex3.jpg",image);

  return 1;
}

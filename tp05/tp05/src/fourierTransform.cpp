#include "fourierTransform.hpp"

#include <cstring>

#include <opencv2/imgproc/imgproc.hpp>



// \brief perform a 2D translation of width/2, height/2 of a periodic image, this operation is equivalent to 2 quarquants swap.
// \param image the image to translate
static void translateHalfImage(cv::Mat &image)
{
    // image center
    int cx = image.cols/2;
    int cy = image.rows/2;

    // pointer to the 4 quadrants
    cv::Mat q0(image, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(image, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(image, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(image, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    // swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;                             
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp) ;                     
    q2.copyTo(q1);
    tmp.copyTo(q2);
}


void discreteFourierTransform(const cv::Mat &inputImage, cv::Mat &imageFourierAmplitude, cv::Mat &imageFourierPhase)
{
	// convert "inputImage" in float. 
  // Genarate a new image filled by 0.0. Merge the 2 images in a 2-channel image 'complexI'.
  cv::Mat planes[] = {cv::Mat_<float>(inputImage), cv::Mat::zeros(inputImage.size(), CV_32F)};
  cv::Mat complexImage;
  cv::merge(planes, 2, complexImage);     // Add to the expanded another plane with zeros

	// Fourier transform
  cv::dft(complexImage, complexImage);    // this way the result may fit in the source matrix

  // rearrange the quadrants of Fourier image so that the origin is at the image center
  translateHalfImage(complexImage);

  // separate the channels: planes[0] = Re(DFT(I)), planes[1] = Im((DFT(I))
  cv::Mat imageFourierReal      = cv::Mat(complexImage.size(),CV_32F);
  cv::Mat imageFourierImaginary = cv::Mat(complexImage.size(),CV_32F);
  cv::Mat finalPlanes[2] = {imageFourierReal,imageFourierImaginary};
  cv::split(complexImage, finalPlanes); 

	// extract the amplitude and phase images
	cv::cartToPolar(imageFourierReal, imageFourierImaginary, imageFourierAmplitude, imageFourierPhase);
}


void inverseDiscreteFourierTransform(const cv::Mat &imageFourierMagnitude, const cv::Mat &imageFourierPhase, cv::Mat &outputImage, const int cvType)
{
	// convert amplitude and phase into real an imaginary parts
	cv::Mat imageFourierReal, imageFourierImaginary;
  cv::polarToCart(imageFourierMagnitude, imageFourierPhase, imageFourierReal, imageFourierImaginary);
	
  // convert "padded" in float. Genarate a new image filled by 0.0. Merge the 2 images in a 2-channel image 'complexI'.
  cv::Mat planes[2] = {imageFourierReal, imageFourierImaginary};
  cv::Mat complexImage;
  cv::merge(planes, 2, complexImage);         

  // rearrange the quadrants of Fourier image  so that the origin is at the image center
  translateHalfImage(complexImage);
   
  // Inverse Fourier transform
  cv::dft(complexImage, complexImage, cv::DFT_INVERSE | cv::DFT_SCALE);

  // separate the channels: planes[0] = image, planes[1] = nothing
  cv::split(complexImage, planes);
  planes[0].convertTo(outputImage, cvType); // cvType can be CV_8U, CV_32F, CV_64F, ...
}


cv::Mat fourierMagnitudeToDisplay(const cv::Mat &magnitudeImage)
{
cv::Mat magnitudeImageToDisplay;
magnitudeImage.copyTo(magnitudeImageToDisplay);

magnitudeImageToDisplay += cv::Scalar::all(1);                                      // add '1' to each pixel
cv::log(magnitudeImageToDisplay, magnitudeImageToDisplay);                          // switch to logarithmic scale
cv::normalize(magnitudeImageToDisplay, magnitudeImageToDisplay, 0, 1, cv::NORM_MINMAX);   // Transform the matrix with float values into a
                                                                                    // viewable image form (float between values 0 and 1).
magnitudeImageToDisplay.convertTo(magnitudeImageToDisplay, CV_8UC1, 255, 0);        // converted to uchar

return magnitudeImageToDisplay;
}


cv::Mat fourierPhaseToDisplay(const cv::Mat &phaseImage)
{
  cv::Mat imageFourierPhaseAbs = cv::abs(phaseImage);

  double minValue, maxValue;
  cv::minMaxLoc(imageFourierPhaseAbs,&minValue,&maxValue);
  imageFourierPhaseAbs.convertTo(imageFourierPhaseAbs, CV_8UC1, 0.5*255.0/(maxValue+1), 127);

  return imageFourierPhaseAbs;
}


void removeRing(cv::Mat &image, const float minRadius, const float maxRadius) 
{
// image center
float cx = image.cols/2.0;
float cy = image.rows/2.0;
  
// square the radius
float squaredMaxRadius = maxRadius * maxRadius;
float squaredMinRadius = minRadius * minRadius;

// try all the pixels
for(int i=0; i<image.rows; ++i)	
  for(int j=0; j<image.cols; ++j){
    float radius = pow((float)i-cy,2) + pow((float)j-cx,2);
    if(radius >= squaredMinRadius && radius <= squaredMaxRadius)
      image.at<float>(i,j) = 0.0;
  }
}


void removeQuarquant(cv::Mat &image, const int lineWidth)
{
for(int i=0; i<image.rows; ++i)
  for(int j=0; j<image.cols; ++j)
    if(abs(i-image.rows/2)>lineWidth && abs(j-image.cols/2)>lineWidth)
      image.at<float>(i,j) = 0;
}

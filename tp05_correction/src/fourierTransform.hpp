#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/// \brief get an image and returns its Fourier transform magnitude and phase images
/// \param inputImage the 1 channel image (any type) to be computed
/// \param imageFourierAmplitude amplitude Fourier image (CV_32F) created and filled by the function
/// \param imageFourierPhase phase Fourier image (CV_32F) created and filled by the function
void discreteFourierTransform(const cv::Mat &inputImage, cv::Mat &imageFourierAmplitude, cv::Mat &imageFourierPhase);

/// \brief perform an inverse Fourier transform from magnitude and phase images (CV_32F)
/// \param imageFourierMagnitude input Fourier magnitude image (CV_32F)
/// \param imageFourierPhase input Fourier phase image (CV_32F)
/// \param outputImage output image reconstructed from the magnitude and phase image
/// \param cvType output image type
void inverseDiscreteFourierTransform(const cv::Mat &imageFourierMagnitude, const cv::Mat &imageFourierPhase, cv::Mat &outputImage, const int cvType = CV_8U);

/// \brief generate an amplitude image well suited to be displayed by switching to logarithmic scale => magnitudeDisplay = log(1 + magnitude)
/// \param magnitudeImage input Fourier magnitude image
/// \return final image transformed to logarithmic scale
cv::Mat fourierMagnitudeToDisplay(const cv::Mat &magnitudeImage);


/// \brief generate a phase image well suited to be displayed with a data centering and normalization procedure 
/// \param phaseImage input Fourier phase image
/// \return final phase image (CV_8U) ready for display
cv::Mat fourierPhaseToDisplay(const cv::Mat &phaseImage);


/// \brief set to zero a set of pixels that belong to a ring defined by minRadius and maxRadius centered in the middle of the image.
/// \param image image to be computed (inplace)
/// \param minRadius radius
/// \param maxRadius
void removeRing(cv::Mat &image, const float minRadius, const float maxRadius);

/// \brief removes a large part of the 4 quarquants of a Fourier amplitude image and keep only the main horizontal and vertical lines passing throug the image center.
/// \param image the original image to be transformed (inplace)
/// \param lineWidth width of the horizontal and vertical lines that remain unchanged
void removeQuarquant(cv::Mat &image, const int lineWidth);



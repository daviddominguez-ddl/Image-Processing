// histogram.cpp
// Created By David Dominguez
// 2017-03-11
// update: 2019-08-19


#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

const String keys =
  "{help h usage ? |      | options            }"
  "{@file1         |      | image (a) input    }"
  "{@file2         |      | image (b) output   }";


void histogram(Mat );
int setContrast(int);

int main(int argc, char *argv[]){

  CommandLineParser parser(argc, argv, keys);
  parser.about("histogram 1.0.3");
  
  if (parser.has("help") || argc == 1) {
      parser.printMessage();
      return 0;
    }
  
  String file1 = parser.get<String>(0);
  String file2 = parser.get<String>(1);
  int i,j;


  
  Mat img1 = imread(file1, CV_LOAD_IMAGE_GRAYSCALE);
  Mat img2  = Mat::zeros(img1.rows,img1.cols,CV_8UC1);
    
    for (i = 0; i < img1.rows; i++)
      for (j = 0; j < img1.cols; j++) {
	img2.at<uchar>(i,j) = setContrast((int)img1.at<uchar>(i, j));
      }
    
    histogram(img1); /* print histogram of image*/
    imwrite("dog_grayscale.png",img1);
    imwrite(file2,img2);
    
    return 0;
}
 
void histogram(Mat mat) {

  int frequency_intensity[255];
  int n = 0,i,j,pixel;
  
  for(i = 0; i <= 255; i++)
    frequency_intensity[i] = 0;
  

  for(i = 0; i < mat.rows; i++){
    for(j = 0; j < mat.cols; j++){
      
      pixel = mat.at<uchar>(i,j);
      frequency_intensity[pixel]++;
    }
  }

  for(i = 0; i <= 255; i++){
      cout<< i <<"\t"<<frequency_intensity[i]<<endl;
      n++;
    }
}

int setContrast(int intensity)
{
    int I_MIN[2] = {0,50};
    int I_MAX[2] = {80,140};
    
    int result = 0;
    if(intensity < I_MIN[1])
        result = 2 * intensity;
    
    else if( intensity >= I_MIN[1] &&  intensity <= I_MAX[0] )
        result = I_MAX[0];

    else if( intensity >= I_MAX[0] &&  intensity <= I_MAX[1] )
        result = (2 * intensity) - I_MAX[0];

    else if(intensity >= I_MAX[1])
        result = 255;

    return result;
}

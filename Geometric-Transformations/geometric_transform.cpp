/**
 * Copyright (c) 2019, David Dominguez
 * Contact: david.stutz@rwth-aachen.de, davidstutz.de
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <string>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

const String keys =
  "{help h usage ? |      | options            }"
  "{@image1        |      | image (a) input    }"
  "{@image2        |      | image (b) output   }"
  "{R rotation     | 0.0  | rotation angle     }"
  "{S scale        | 1.0  | scale factor       }"
  "{X              | 0.0  | deformation X axis (image width) }"
  "{Y              | 0.0  | deformation Y axis (image height) }"
  ;


void rotationMat(float R[2][2],float theta);
void scaleMat(float S[2][2],float kx,float ky);
void deformationXMat(float D[2][2],float dx);
void deformationYMat(float D[2][2],float dy);

float linear_interpolation(float x1,float x2,float y1,float y2,float x);
float bilinear_interpolation(float x,float y,float x1,float x2,float y1,float y2,float Z[2][2]);

void inverse2x2(float A[2][2],float I[2][2]);
void product2x1(float A[2][2],float u[2],float v[2]);
void product2x2(float A[2][2],float B[2][2],float C[2][2]);
void copyMat(float From[2][2],float To[2][2]);

int main(int argc, const char* argv[])
{

  CommandLineParser parser(argc, argv, keys);
  parser.about("App tranformation 1.0.0");
  
  if (parser.has("help"))
    {
      parser.printMessage();
      return 0;
    }

  String win1 = "Input";
  String win2 = "Output (Transform)";
  
  /* input file names */
  String img1 = parser.get<String>(0);
  String img2 = parser.get<String>(1);

  /* transformation parameters */
  double rotation = parser.get<double>("R");
  double scale = parser.get<double>("S");
  double dx = parser.get<double>("X");
  double dy = parser.get<double>("Y");
  
  Mat G=imread(img1);
  Mat H;
  
  namedWindow(win1);
  namedWindow(win2);
  
  float T[2][2]={{1,0},{0,1}};
  float U[2][2];
  float TMP1[2][2],TMP2[2][2];
  

  /* image transformation */
  rotationMat(TMP1, rotation * M_PI/180.0);
  product2x2(TMP1,T,TMP2);
  copyMat(TMP2,T);
  
  scaleMat(TMP1,scale,scale);
  product2x2(TMP1,T,TMP2);
  copyMat(TMP2,T);
  
  deformationXMat(TMP1,dx);
  product2x2(TMP1,T,TMP2);
  copyMat(TMP2,T);
  
  deformationYMat(TMP1,dy);
  product2x2(TMP1,T,TMP2);
  copyMat(TMP2,T);

  cout<<"transform application:"<<endl;
  cout<<"Rotation:"<<rotation<<endl;
  cout<<"Scale:"<<scale<<endl;
  cout<<"X:"<<dx<<endl;
  cout<<"Y:"<<dy<<endl;
  cout<<"img(a):"<<img1<<endl;
  cout<<"img(b):"<<img2<<endl;

  float p[2],q[2];

  inverse2x2(T,U);

  int w1=G.size().width-1;
  int h1=G.size().height-1;
  
  float xlim[2],ylim[2];
  int w2,h2;
  int nw,nh;
  int i,j;
  
  /* compute final dimension of transform */
  
  float p1[2] = { 0.0,   0.0 };
  float p2[2] = { (float)G.cols - 1.0,   0.0 };
  float p3[2] = { 0.0, ((float)G.rows - 1.0) };
  float p4[2] = { (float)G.cols - 1.0, ((float)G.rows - 1.0) };
  
  float q1[2],q2[2],q3[2],q4[2];
  
  
  product2x1(T,p1,q1);
  product2x1(T,p2,q2);
  product2x1(T,p3,q3);
  product2x1(T,p4,q4);
  
  xlim[0] = MIN(q1[0],MIN(q2[0],MIN(q3[0],q4[0])));
  xlim[1] = MAX(q1[0],MAX(q2[0],MAX(q3[0],q4[0])));
  ylim[0] = MIN(q1[1],MIN(q2[1],MIN(q3[1],q4[1])));
  ylim[1] = MAX(q1[1],MAX(q2[1],MAX(q3[1],q4[1])));

  
  w2 = (int)(xlim[1]-xlim[0])+1;
  h2 = (int)(ylim[1]-ylim[0])+1;
  
  H = Mat::zeros(h2,w2,CV_8UC3);
  
  float x1,x2,y1,y2;
  float x,y,z1 = 0,z2 = 0,z3 = 0;
  float Z[2][2];
  int contador=0;
  
  int set_color[3]={0,0,0};
  
  for(i=0;i<h2;i++){
    q[1]=i+ylim[0];
    for(j=0;j<w2;j++){
      q[0]=j+xlim[0];			
      product2x1(U,q,p);
      
      if(p[0]>=0 && p[0]<=w1 && p[1]>=0 && p[1]<=h1)
	{
	  x = p[0];
	  y = p[1];
	  x1 = floor(x);	x2=x1+1;
	  y1 = floor(y);	y2=y1+1;

	  Z[0][0]=G.at<cv::Vec3b>(y1,x1)[0];
	  Z[0][1]=G.at<cv::Vec3b>(y1,x2)[0];
	  Z[1][0]=G.at<cv::Vec3b>(y2,x1)[0];
	  Z[1][1]=G.at<cv::Vec3b>(y2,x2)[0];
	  z1 = bilinear_interpolation(x,y,x1,x2,y1,y2,Z);

	  Z[0][0]=G.at<cv::Vec3b>(y1,x1)[1];
	  Z[0][1]=G.at<cv::Vec3b>(y1,x2)[1];
	  Z[1][0]=G.at<cv::Vec3b>(y2,x1)[1];
	  Z[1][1]=G.at<cv::Vec3b>(y2,x2)[1];
	  z2 = bilinear_interpolation(x,y,x1,x2,y1,y2,Z);

	  Z[0][0]=G.at<cv::Vec3b>(y1,x1)[2];
	  Z[0][1]=G.at<cv::Vec3b>(y1,x2)[2];
	  Z[1][0]=G.at<cv::Vec3b>(y2,x1)[2];
	  Z[1][1]=G.at<cv::Vec3b>(y2,x2)[2];
	  z3 = bilinear_interpolation(x,y,x1,x2,y1,y2,Z);
	  
	  H.at<cv::Vec3b>(i,j)[0]=(int)z1;
	  H.at<cv::Vec3b>(i,j)[1]=(int)z2;
	  H.at<cv::Vec3b>(i,j)[2]=(int)z3;
	}
      
    }
  }
  
  imshow(win1,G);
  imshow(win2,H);
  
  imwrite(img2,H);
  
  waitKey(0);

  return 0;
}


void rotationMat(float R[2][2],float theta)
{
  R[0][0] =  cos(theta);
  R[0][1] = -sin(theta);
  R[1][0] =  sin(theta);
  R[1][1] =  cos(theta);
}

void scaleMat(float S[2][2],float kx,float ky)
{
  S[0][0] = kx;
  S[0][1] = 0.0;
  S[1][0] = 0.0;
  S[1][1] = ky;
}

void deformationXMat(float D[2][2],float dx)
{
  D[0][0] = 1.0;
  D[0][1] = dx;
  D[1][0] = 0;
  D[1][1] = 1.0;
}
void deformationYMat(float D[2][2],float dy)
{
  D[0][0] = 1.0;	D[0][1] = 0.0;
  D[1][0] = dy;	D[1][1] = 1.0;
}

float linear_interpolation(float x1,float x2,float y1,float y2,float x)
{
  float y = (x-x1)/(x2-x1)*(y2-y1)+y1;
  return y;
}

float bilinear_interpolation(float x,float y,float x1,float x2,float y1,float y2,float Z[2][2])
{
  float u1 = linear_interpolation(x1,x2,Z[0][0],Z[0][1],x);
  float u2 = linear_interpolation(x1,x2,Z[1][0],Z[1][1],x);
  float z  = linear_interpolation(y1,y2,u1,u2,y);
  return z;
}

void inverse2x2(float A[2][2],float I[2][2])
{
  float det=A[0][0]*A[1][1]-A[0][1]*A[1][0];
  I[0][0] = A[1][1]/det;
  I[1][1] = A[0][0]/det;
  
  I[0][1] = -A[0][1]/det;
  I[1][0] = -A[1][0]/det;
}

void product2x1(float A[2][2],float u[2],float v[2])
{
  v[0]=A[0][0]*u[0]+A[0][1]*u[1];
  v[1]=A[1][0]*u[0]+A[1][1]*u[1];
}

void product2x2(float A[2][2],float B[2][2],float C[2][2])
{
  C[0][0] = A[0][0]*B[0][0]+A[0][1]*B[1][0];
  C[0][1] = A[0][0]*B[0][1]+A[0][1]*B[1][1];
  
  C[1][0] = A[1][0]*B[0][0]+A[1][1]*B[1][0];
  C[1][1] = A[1][0]*B[0][1]+A[1][1]*B[1][1];
}

void copyMat(float From[2][2],float To[2][2])
{
  int i,j;
  for(i=0;i<2;i++)
    for(j=0;j<2;j++)
      To[i][j]=From[i][j];
}

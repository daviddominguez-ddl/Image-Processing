## Change the contrast of grayscale image

### Model
In this post will show how  to change the contrast of grayscale image using OpenCV.  The process use two commonly operations: multiplication and addition with a constant:

![\Large I'(x) = \alpha I(x) + \beta](https://latex.codecogs.com/svg.latex?I%27%28x%29%3D%5Calpha%20I%28x%29%2B%5Cbeta)

where, ![\Large I(x)](https://latex.codecogs.com/svg.latex?I%28x%29) are the source image pixels , ![\Large I(x)](https://latex.codecogs.com/svg.latex?I%27%28x%29) are the output image pixels. The parameters ![\Large \alpha>0](https://latex.codecogs.com/svg.latex?%5Calpha%20%3E0) and ![\Large \beta>0](https://latex.codecogs.com/svg.latex?%5Cbeta) are often called the gain and bias parameters; sometimes these parameters are said to control contrast and brightness respectively.

### Run Code

There are 1 file:
* histogram.cpp

Simply compile and run. There is a Makefile for a fast test.

#### C++
```
make
./histogram <input_file> <output_file>
```
#### Options.

	/* ------ Example 1: ---- */
	{
	histogram 1.0.3
    Usage: histogram file1 file2 

	-?, -h, --help, --usage
		options

	file1
		image (a) input
	file2
		image (b) output
	}
	
#### Example
```
 ./histogram dog.jpg dog_contrast.jpg
```
#### Plotting Histograms
The histogram represents for each color level the number of pixels with that color level. A dark image will have many pixels with low color value and thus the histogram will present a peak in his left part.

![](histogram.jpg)

### Sample Result
Change of image contrast between gray intervals [0,50] and [80,140].

![](dog.jpg)
![](dog_grayscale.jpg)
![](dog_contrast.jpg)

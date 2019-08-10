## Code for geometric transformations on images

### Model
The transformations are applied using matrix multiplication. The value of a pixel is estimated through a bilinear interpolation function.

### Run Code

There are 1 file:
* geometric_transform.cpp

Simply compile and run. There is a Makefile for a fast test.

#### C++
```
make
./transform [params] <input_file> <output_file>
```
#### Options.

	/* ------ Example 1: ---- */
	{
		Usage: transform [params] image1 image2 

	-?, -h, --help, --usage (value:true)
		options
	-R, --rotation (value:0.0)
		rotation angle
	-S, --scale (value:1.0)
		scale factor
	-X (value:0.0)
		deformation X axis (image width)
	-Y (value:0.0)
		deformation Y axis (image height)

	image1
		image (a) input
	image2
		image (b) output
	}
	
#### Example
```
./transform -R=45 -S=1.5 -X=0 -Y=0 lena.jpg lenaT.jpg
```

### Sample Result

![](lena.jpg)
![](lenaT.jpg)

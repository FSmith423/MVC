// From image processor assignment

class Image;

#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include "common.h"

#define RED		0
#define GREEN	1
#define BLUE	2

struct Point {
	int x;
	int y;
	Point(): x(0), y(0)
	{
	}
	Point(int X, int Y): x(X), y(Y)
	{
	}
	bool operator==(const Point& p)
	{
		return (x == p.x && y == p.y); 
	}
	bool operator!=(const Point& p)
	{
		return (!operator==(p));
	}
	bool valid()
	{
		return (x >= 0 && y >= 0);
	}
};

bool close(Point&, Point&);
Point operator-(Point& left, Point& right);
Point operator+(Point& left, Point& right);
std::ostream &operator<<(std::ostream &out_file, Point& thePoint);

class Pixel
{
public:
	Pixel ();
	Pixel (double r, double g, double b);
	Pixel (const Pixel& toCopy);
	Pixel& operator=(const Pixel& toCopy);

	bool isBlack();
	double getColor(int chan);
	void setColor(int chan, double value);

	friend Pixel operator-(Pixel&,Pixel&);
	friend Pixel operator+(Pixel&, Pixel&);
	friend Pixel scale(Pixel, double);
private:
	double		col[3];
};

Pixel operator-(Pixel& left, Pixel& right);
Pixel operator+(Pixel& left, Pixel& right);
Pixel scale(Pixel, double weight);

std::ostream &operator<<(std::ostream &out_file, Pixel& thePixel);

/*
* generic multi channel 8-bit max image class.  can read and write
* 8 and 24 bit uncompressed BMP files and supports some useful OpenGL 
* calls.
*
* get and set pixel methods use doubles from 0.0 to 1.0.  these 
* values are mapped to integer values from 0 to the maximum value
* allowed by the number of bits per channel in the image.
*/
class Image
{
public:
	Image ();
	~Image ();

	// create empty image with specified characteristics
	Image (int width_, int height_);
	Image (int width_, int height_, int bits_);

	// create image and read data from filename
	// use good() or bad() to check success
	Image (const char* filename);

	// copy constructor and assignment operator
	// _deep_ copy!
	Image (const Image& image);
	Image&		operator= (const Image& image);

	// accessors
	int		getWidth ()    { return width;    }
	int		getHeight ()   { return height;   }
	int		getBits ()     { return bits;     }

	// check if the image is valid
	bool		good ();
	bool		bad ();

	// set all the pixel data to zero
	void		clear ();

	// retrieve pixel data.  methods with _ at the
	// end of their name return 0.0 if the x and y
	// coordinates are out of range.  the functions
	// without a _ give an assertion failur for out
	// of bounds coordinates
	double		getPixel  (int x, int y, int channel);
	double		getPixel_ (int x, int y, int channel);
	double		getPixel_ (Point pt, int channel);
	Pixel		getPixel  (int x, int y);
	Pixel		getPixel_ (int x, int y);
	Pixel		getPixel_ (Point pt);
	Pixel&		getPixel  (int x, int y, Pixel& pixel);
	Pixel&		getPixel_ (int x, int y, Pixel& pixel);

	// set pixel data.  set is ignored by functions with a _
	// at the end of the name when the coordinates or the pixel
	// value are out of bounds.  the functions without an _
	// after the name give an assertion failure for out of
	// bounds
	// 
	void		setPixel  (int x, int y, int channel, double value);
	void		setPixel_ (int x, int y, int channel, double value);
	void		setPixel_ (Point pt, int channel, double value);
	void		setPixel  (int x, int y, Pixel& pixel);
	void		setPixel_ (int x, int y, Pixel& pixel);
	void		setPixel_ (Point pt, Pixel& pixel);
	void		setArea_  (Point pt, Pixel& pixel);

	// OpenGL call wrappers
	void		glDrawPixelsWrapper ();
	void		glTexImage2DWrapper ();
	void		glReadPixelsWrapper ();

	// top-level file read and write calls,
	// determines file type
	int		read (const char* filename);
	int		write (const char* filename);

	// BMP specific read and write calls
	int		readBMP (const char* filename);
	int		readBMP (std::ifstream& file);
	int		writeBMP (const char* filename);
	int		writeBMP (std::ofstream& file);


private:
	int		index(int x, int y, int c);
	int		width;
	int		height;
	int		bits;		// number of bits per pixel per channel
	int		maxValue;	// max that can be stored in bits

	unsigned char*	pixels;		// image data

};


#endif // IMAGE_H

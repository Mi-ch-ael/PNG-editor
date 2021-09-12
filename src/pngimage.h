#ifndef PNGIMAGE_H
#define PNGIMAGE_H

#include <png.h>
#include <pngconf.h>
#include <QObject>
#include <QImage>
#include <QRgb>
#include <cmath>
#include <QColor>
#include <QPoint>

//PNGimage::load errors
#define OPEN_FAILURE 1
#define RECOGNISE_FAILURE 2
#define READ_STRUCT_FAILURE 3
#define INFO_STRUCT_FAILURE 4
#define IO_INIT_FAILURE 5
#define READ_FAILURE 6
#define END_READ_FAILURE 7
#define UNSUPPORTED_PNG_FORMAT 8
#define MALLOC_FAILURE 9

//PNGimage::write errors
#define CREATE_FAILURE 1
#define WRITE_STRUCT_FAILURE 2
#define WINFO_STRUCT_FAILURE 3
#define WIO_INIT_FAILURE 4
#define HEADER_WRITE_FAILURE 5
#define BYTES_WRITE_FAILURE 6
#define END_OF_WRITE_FAILURE 7

//PNGimage::put_pixel & PNGimage::draw_line errors
#define OUT_OF_RANGE 1
#define INVALID_WIDTH 2

//which fractal to draw?
#define ICE_FRACTAL 0.4
#define TRIANGLES 0.5
#define SQUARES 0.0

//rotate error
#define MALLOC_ERROR 1

//
class PNGimage : public QObject
{
    Q_OBJECT

 private:
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_structp write_ptr;
    png_infop info_ptr;
    png_infop info_ptr_before;
    png_infop write_info;
    int number_of_passes;
    png_bytep *row_pointers;


public:
    explicit PNGimage(QObject *parent = 0);
    ~PNGimage();
    int load(QString filename);
    int write(QString filename);
    QImage* convert_to_qimage();
    int put_pixel(int x, int y, QColor* color);
    int put_pixel(int x, int y, int r, int g, int b, int a = 255);
    int draw_line(int x_start,int y_start,int x_end,int y_end, int width, QColor* color);
    void draw_rectangle(QPoint* p1, QPoint* p2, int width,
                       QColor* line_color, QColor* fill_color);
    int draw_ice_frame(QColor* color, double omega, double t);
    void draw_rectangle_fragment(int width, QColor* color, int x, int y, double r, double alpha, int iter, int forward);
    void draw_rectangle_base(int width, QColor* color, int x, int y, double r, double alpha, int iter);
    void draw_ice_fragment(int width, QColor* color, int x, int y, double r, double alpha, double pattern, int iter);
    void divide_image(int H, int V, int line_width, QColor* color);
    int rotate(QPoint* p1, QPoint* p2, double alpha);

    int getImageWidth(); //
    int getImageHeight();

    png_infop getPNG_info();
    png_infop getPNG_info_before();

signals:

public slots:

};







#endif // PNGIMAGE_H

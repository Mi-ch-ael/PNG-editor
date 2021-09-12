#include "pngimage.h"

PNGimage::PNGimage(QObject *parent) :
    QObject(parent)
{
    this->png_ptr = NULL;
    this->info_ptr = NULL;
    this->info_ptr_before = NULL;
    this->write_ptr = NULL;
    this->write_info = NULL;
    this->row_pointers = NULL;
}

PNGimage::~PNGimage(){
    png_infop end_info = NULL;

    png_destroy_info_struct(png_ptr, &info_ptr_before);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    if(write_ptr && write_info){
          png_destroy_write_struct(&write_ptr, &write_info);
    }
    else{
        if(write_ptr) png_destroy_write_struct(&write_ptr, NULL);
        else{
            if(write_info) png_destroy_write_struct(NULL, &write_info);
            else png_destroy_write_struct(NULL, NULL);
        }
    }

    if(row_pointers != NULL){
       for (int y = 0; y < this->height; y++){
           if(this->row_pointers[y] != NULL) free(this->row_pointers[y]);
       }
       free(this->row_pointers);
    }
 }

 int PNGimage::load(QString filename){
     char header[8];
     QByteArray qb = filename.toLocal8Bit();
     char* name = qb.data();

     FILE* f = fopen(name, "rb");

     if(!f){
         return OPEN_FAILURE;
     }

     fread(header, 8, 1, f);

     if(png_sig_cmp((png_bytep)header, 0, 8)){  // Лишнее?
         fclose(f);
         return RECOGNISE_FAILURE;
     }

     this->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);


     if(!(this->png_ptr)){
         fclose(f);
         return READ_STRUCT_FAILURE;
     }

     this->info_ptr = png_create_info_struct(this->png_ptr);

     //=====================================
     info_ptr_before = png_create_info_struct(this->png_ptr);
     //=====================================

     if(!(this->info_ptr) || !(this->info_ptr_before)){
         fclose(f);
         return INFO_STRUCT_FAILURE;
     }

     if (setjmp(png_jmpbuf(this->png_ptr))){
             fclose(f);
             return IO_INIT_FAILURE;
     }

     png_init_io(this->png_ptr, f);

     png_set_sig_bytes(this->png_ptr, 8);
     png_read_info(this->png_ptr, this->info_ptr);

     //==================================

     this->info_ptr_before->color_type = this->info_ptr->color_type;
     this->info_ptr_before->channels = this->info_ptr->channels;
     this->info_ptr_before->bit_depth = this->info_ptr->bit_depth;
     this->info_ptr_before->mod_time = this->info_ptr->mod_time;
     this->info_ptr_before->interlace_type = this->info_ptr->interlace_type;
     this->info_ptr_before->pixel_depth = this->info_ptr->pixel_depth;



     //==================================


     this->width = png_get_image_width(this->png_ptr, this->info_ptr);
     this->height = png_get_image_height(this->png_ptr, this->info_ptr);
     this->color_type = png_get_color_type(this->png_ptr, this->info_ptr);
     this->bit_depth = png_get_bit_depth(this->png_ptr, this->info_ptr);

     this->number_of_passes = png_set_interlace_handling(this->png_ptr);

     if(color_type == PNG_COLOR_TYPE_PALETTE){
         fclose(f);
         return UNSUPPORTED_PNG_FORMAT;
     }
     if(bit_depth == 16){
         png_set_strip_16(png_ptr);

     }
     if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
         png_set_expand_gray_1_2_4_to_8(png_ptr);
     }
     if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
         png_set_tRNS_to_alpha(png_ptr);
     }
     if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY){
         png_set_filler(png_ptr, 255, PNG_FILLER_AFTER);
     }
     if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GA){
         png_set_gray_to_rgb(png_ptr);
     }

     png_read_update_info(this->png_ptr, this->info_ptr);

     if (setjmp(png_jmpbuf(this->png_ptr))){
             fclose(f);
             return READ_FAILURE;
     }

     this->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * this->height);
     if(!(this->row_pointers)){
         fclose(f);
         return MALLOC_FAILURE;
     }
     for(int i = 0; i < this->height; ++i) row_pointers[i] = NULL;
     for (int y = 0; y < this->height; y++){
         this->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(this->png_ptr, this->info_ptr));
         if(!(this->row_pointers[y])){
             fclose(f);
             return MALLOC_FAILURE;
         }
     }

     png_read_image(this->png_ptr, this->row_pointers);

     if (setjmp(png_jmpbuf(this->png_ptr))){
             fclose(f);
             return END_READ_FAILURE;
     }

     png_read_end(NULL, NULL);

     fclose(f);
     return 0;
 }


 int PNGimage::write(QString filename){
     QByteArray qb = filename.toLocal8Bit();
     char* name = qb.data();

     FILE* f = fopen(name, "wb");
     if(!f){
         return CREATE_FAILURE;
     }

     this->write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
     if(!(this->write_ptr)){
         fclose(f);
         return WRITE_STRUCT_FAILURE;
     }

     this->write_info = png_create_info_struct(this->write_ptr);
     if(!(this->write_info)){
         fclose(f);
         return WINFO_STRUCT_FAILURE;
     }

     if (setjmp(png_jmpbuf(this->write_ptr))){
             fclose(f);
             return WIO_INIT_FAILURE;
     }

     png_init_io(this->write_ptr, f);

     if (setjmp(png_jmpbuf(this->write_ptr))){
           fclose(f);
           return HEADER_WRITE_FAILURE;
     }

     png_set_IHDR(write_ptr, write_info, width, height,
                  info_ptr->bit_depth, /*color_type,*/
                  PNG_COLOR_TYPE_RGBA,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_BASE,
                  PNG_FILTER_TYPE_BASE);

     png_write_info(this->write_ptr, this->write_info);


     if (setjmp(png_jmpbuf(this->write_ptr))){
             fclose(f);
             return BYTES_WRITE_FAILURE;
     }

     png_write_image(this->write_ptr, this->row_pointers);

     if (setjmp(png_jmpbuf(this->write_ptr))){
           fclose(f);
           return END_OF_WRITE_FAILURE;
     }

     png_write_end(this->write_ptr, NULL);

     fclose(f);
     return 0;
 }


 QImage* PNGimage::convert_to_qimage(){
     int x, y;
     QRgb rgb;
     QImage* result = new QImage(info_ptr->width, info_ptr->height, QImage::Format_ARGB32);
     if(!result) return result;

     for (y = 0; y < (int)info_ptr->height; y++) {
             png_byte* row = row_pointers[y];
             for (x = 0; x < (int)info_ptr->width; x++) {
                 png_byte *ptr = &(row[x * 4]);
                 rgb = (unsigned int)((ptr[3] << 8*3) + (ptr[0] << 8*2) +
                         (ptr[1] << 8*1) + (ptr[2] << 8*0));
                 result->setPixel(x, y, rgb);
             }
     }
     return result;
 }


 int PNGimage::put_pixel(int x, int y, int r, int g, int b, int a){
     if( (x < 0) || (y < 0) || (x >= (int)info_ptr->width) || (y >= (int)info_ptr->height) ){
         return OUT_OF_RANGE;
     }
     png_bytep ptr = &(row_pointers[y][x * 4]);
     ptr[0] = r;
     ptr[1] = g;
     ptr[2] = b;
     ptr[3] = a;
     return 0;
 }


 int PNGimage::put_pixel(int x, int y, QColor* color){
     int r = color->red();
     int g = color->green();
     int b = color->blue();
     int a = color->alpha();
     if( (x < 0) || (y < 0) || (x >= (int)info_ptr->width) || (y >= (int)info_ptr->height) ){
         return OUT_OF_RANGE;
     }
     png_bytep ptr = &(row_pointers[y][x * 4]);
     ptr[0] = r;
     ptr[1] = g;
     ptr[2] = b;
     ptr[3] = a;
     return 0;
 }

int PNGimage::draw_line(int x_start, int y_start, int x_end, int y_end, int width,
                        QColor* color){
    int return_value = 0;
    int eps;
    if(width < 0 || width % 2 == 0 || width > 7) return INVALID_WIDTH;
    eps = (width - 1) / 2;
    while(eps){
        return_value = (return_value | draw_line(x_start-eps, y_start, x_end-eps, y_end, 1, color));
        return_value = (return_value | draw_line(x_start+eps, y_start, x_end+eps, y_end, 1, color));
        return_value = (return_value | draw_line(x_start, y_start-eps, x_end, y_end-eps, 1, color));
        return_value = (return_value | draw_line(x_start, y_start+eps, x_end, y_end+eps, 1, color));
        width -= 2;
        eps = (width - 1) / 2;
    }
    int x_mid, y_mid;
    return_value = (return_value | put_pixel(x_start,y_start,color));
    return_value = (return_value | put_pixel(x_end, y_end, color));
    if( abs(x_start - x_end) > 1 || abs(y_start - y_end) > 1 ){
        x_mid = ceil( (x_start + x_end)/2 );
        y_mid = ceil( (y_start + y_end)/2 );
        draw_line(x_start, y_start, x_mid, y_mid, 1, color);
        draw_line(x_mid, y_mid, x_end, y_end, 1, color);
    }
    return return_value;
}

void PNGimage::draw_rectangle(QPoint* p1, QPoint* p2, int width,
                   QColor* line_color, QColor* fill_color){
      int x1 = p1->x();
      int y1 = p1->y();
      int x2 = p2->x();
      int y2 = p2->y();
      int x_max, x_min, y_max, y_min;
      draw_line(x1, y1, x1, y2, width, line_color);
      draw_line(x1, y1, x2, y1, width, line_color);
      draw_line(x2, y2, x1, y2, width, line_color);
      draw_line(x2, y2, x2, y1, width, line_color);

      int eps = (width - 1) / 2;
      for(int i = -eps; i <= eps; ++i){
          for(int j = -eps; j <= eps; ++j){
              put_pixel(x1 + i, y1 + j, line_color);
              put_pixel(x1 + i, y2 + j, line_color);
              put_pixel(x2 + i, y1 + j, line_color);
              put_pixel(x2 + i, y2 + j, line_color);
          }
      }

      if(fill_color){

          x_max = std::max(x1,x2);
          y_max = std::max(y1, y2);
          x_min = std::min(x1, x2);
          y_min = std::min(y1, y2);

          for(int i = x_min + eps + 1; i <= x_max - eps - 1; ++i){
              for(int j = y_min+eps+1; j <= y_max-eps-1; ++j){
                  put_pixel(i, j, fill_color);
              }
          }
      }

}


void PNGimage::draw_ice_fragment(int width,QColor* color,int x,int y,
                                double r, double alpha,
                                double pattern, int iter){

    int x2 = round(x + r*cos(alpha));
    int y2 = round(y + r*sin(alpha));
    int x_mid = round((x + x2)/2);
    int y_mid = round((y + y2)/2);
    r = r/2;
    double r_mid = 2*pattern*r;
    int cx = round(x_mid + r_mid * cos(alpha + M_PI/2));
    int cy = round(y_mid + r_mid * sin(alpha + M_PI/2));

    draw_line(x, y, x2, y2, width, color);
    draw_line(x_mid, y_mid, cx, cy, width, color);
    --iter;

    if (iter > 0){
        draw_ice_fragment(width, color, x, y, r, alpha, pattern, iter);
        draw_ice_fragment(width, color,x_mid,y_mid,r, alpha, pattern, iter);
        draw_ice_fragment(width,color,x_mid,y_mid,r_mid,alpha+M_PI/2, pattern, iter);
        draw_ice_fragment(width, color, cx, cy, r_mid,alpha-M_PI/2, pattern, iter);
    }


}

void PNGimage::draw_rectangle_fragment(int width, QColor* color, int x, int y, double r,
                                       double alpha, int iter, int forward){

    double k = 1.0/3.0;
    QPoint p1(x, y);
    QPoint p2(round(x+r*cos(alpha)+r*sin(alpha)),round(y+r*cos(alpha)+r*sin(alpha)));
    draw_rectangle(&p1, &p2, width, color, NULL);
    --iter;
    if(iter > 0){
        if(forward){
            draw_rectangle_fragment(width, color,
                                    round(x+2*k*r*cos(alpha)),round(y+2*k*r*sin(alpha)),
                                    2*k*r, alpha, iter, forward);

        }
        else{
            draw_rectangle_fragment(width, color,
                                    round(x-r*k*cos(alpha)),round(y-r*k*sin(alpha)),
                                    2*k*r,alpha, iter, forward);
        }
    }
}

void PNGimage::draw_rectangle_base(int width, QColor* color, int x, int y, double r, double alpha, int iter){
     double k = 1.0/3.0;

     QPoint p1(round(x+k*r*cos(alpha)) , round(y+k*r*sin(alpha)));
     QPoint p2(round(x+2*k*r*cos(alpha)+k*r*sin(alpha)),
               round(y+2*k*r*sin(alpha)+k*r*cos(alpha)));
     draw_rectangle(&p1, &p2, width,
                    color, NULL);
     draw_rectangle_fragment(width, color,
                             round(x+(2.0/9.0)*r*cos(alpha)), round(y+(2.0/9.0)*r*sin(alpha)),
                             (2.0/9.0)*r, alpha, iter, 0);
     draw_rectangle_fragment(width, color,
                             round(x+(5.0/9.0)*r*cos(alpha)), round(y+(5.0/9.0)*r*sin(alpha)),
                             (2.0/9.0)*r, alpha, iter, 1);

}

int PNGimage::draw_ice_frame(QColor* color,
                             double omega, double t){
    int width = 3;
    int flag = t ? 1 : 0;
    if(!flag) t = 0.4;

    int return_value = 0;

    int n_vertical = ceil(t*(info_ptr->height) /
                     (omega*std::min(info_ptr->height,info_ptr->width)));
    int n_horizontal = ceil(t*(info_ptr->width) /
                     (omega*std::min(info_ptr->height,info_ptr->width)));

    int length_vertical = round(info_ptr->height / n_vertical);
    int length_horizontal = round(info_ptr->width / n_horizontal);


    int it_h = ceil(log2(length_horizontal*0.1));
    int it_v = ceil(log2(length_vertical*0.1));
    int iter = std::max(it_h, it_v);
    if(iter < 2) return_value = 1;

    draw_line(0, 0, (info_ptr->width)-1, 0, width, color);
    draw_line(0, (info_ptr->height)-1, (info_ptr->width)-1, (info_ptr->height)-1,
              width, color);
    draw_line((info_ptr->width)-1,0,(info_ptr->width)-1, (info_ptr->height)-1,
              width, color);
    draw_line(0,0,0,(info_ptr->height)-1, width, color);

    if(!flag){
        //draw rectangles
        for(int i = 0; i < n_horizontal; ++i){
            draw_rectangle_base(width, color, i*length_horizontal, 0, length_horizontal, 0.0, iter);
            draw_rectangle_base(width, color, info_ptr->width-i*length_horizontal,
                                info_ptr->height-1, length_horizontal, M_PI, iter);
        }
        for(int i = 0; i < n_vertical; ++i){
            draw_rectangle_base(width, color, 0, i*length_vertical,length_vertical,
                                M_PI_2, iter);
            draw_rectangle_base(width, color, info_ptr->width-1,
                                info_ptr->height-i*length_vertical,length_vertical,
                                -M_PI_2, iter);
        }
        return return_value;
    }

    for(int i = 0; i < n_horizontal; ++i){

                draw_ice_fragment(width, color, i*length_horizontal,0,
                                 length_horizontal, 0.0, t, iter);

                draw_ice_fragment(width, color,
                                 (info_ptr->width)-i*length_horizontal,
                                  (info_ptr->height)-1,
                                 length_horizontal, M_PI, t, iter);
    }
    for(int i = 0; i < n_vertical; ++i){

                draw_ice_fragment(width, color, 0,
                                 (info_ptr->height)-i*length_vertical,
                                 length_vertical, -M_PI_2,t, iter);

                draw_ice_fragment(width, color, (info_ptr->width)-1,
                                 i*length_vertical,
                                 length_vertical, M_PI_2,t,iter);
    }

    return return_value;

}
void PNGimage::divide_image(int H, int V, int line_width, QColor* color)
{

   for(int i = 1; i*info_ptr->width/H < info_ptr->width; i++)
        draw_line(i*info_ptr->width/H, 0, i*info_ptr->width/H, info_ptr->height, line_width, color);

   for(int j = 1; j*info_ptr->height/V < info_ptr->height; j++)
       draw_line(0, j*info_ptr->height/V, info_ptr->width, j*info_ptr->height/V, line_width, color);

}


int PNGimage::rotate(QPoint* p1, QPoint* p2, double alpha){
    int x1 = p1->x();
    int y1 = p1->y();
    int x2 = p2->x();
    int y2 = p2->y();
    int x_mid = round((x1 + x2)/2);
    int y_mid = round((y1 + y2)/2);
    int x_max = std::max(x1, x2);
    int x_min = std::min(x1, x2);
    int y_max = std::max(y1, y2);
    int y_min = std::min(y1, y2);
    int delta_x, delta_y;
    x2 = x_max; x1 = x_min; y2 = y_max; y1 = y_min;
    png_bytepp tmp_buf;
    png_bytep arr;

    tmp_buf = (png_bytepp)malloc((y2 - y1 + 1)*sizeof(png_bytep));
    if(!tmp_buf) return MALLOC_ERROR;
    for(int i = 0; i < y2-y1+1; ++i){
        tmp_buf[i] = (png_bytep)malloc((x2-x1+1)*4*sizeof(png_byte));
        if(!tmp_buf[i]){
            for(int j = 0; j < i; ++j){
                free(tmp_buf[j]);
            }
            free(tmp_buf);
            return MALLOC_ERROR;
        }
    }

    for(int i = 0; i < y2-y1+1; ++i){
        for(int j = 0; j < 4*(x2-x1+1); ++j){
            tmp_buf[i][j] = row_pointers[y1+i][4*x1+j];
        }
    }

    for(int i = y1; i < y2+1; ++i){
        for(int j = x1; j < x2+1; ++j){
            put_pixel(j, i, 0, 0, 0, 0);
        }
    }


    for(int i = 0; i < y2-y1+1; ++i){
        for(int j = 0; j < x2-x1+1; ++j){
            delta_x = x1 + j - x_mid;
            delta_y = y1 + i - y_mid;
            arr = &(tmp_buf[i][4*j]);
            put_pixel(round(x_mid+delta_x*cos(alpha)-delta_y*sin(alpha)),
                      round(y_mid+delta_x*sin(alpha)+delta_y*cos(alpha)),
                      arr[0], arr[1], arr[2], arr[3]);
        }
    }

    for(int i = 0; i < y2-y1+1; ++i){
        free(tmp_buf[i]);
    }
    free(tmp_buf);
    return 0;
}


int PNGimage::getImageWidth()
{
   return width;
}

int PNGimage::getImageHeight()
{
    return height;
}

png_infop PNGimage::getPNG_info()
{
  return info_ptr;
}

png_infop PNGimage::getPNG_info_before()
{
  return info_ptr_before;
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QLineEdit>
#include<QLabel>
#include<QComboBox>
#include<QColor>
#include<QFileDialog>
#include<QtWidgets>
#include<QCursor>


#include "pngimage.h"

//Load error reports
#define LOAD__REPORT_HEADER "Opening failure"
#define LOAD__OPEN_FAILURE_REPORT "File could not be opened. Make sure you have the rights to read this file."
#define LOAD__RECOGNISE_FAILURE_REPORT "File has not been recognised as a PNG. File loading terminated."
#define LOAD__PREPARE_READ_FAILURE_REPORT "An error occured while preparing to read a PNG file. File loading terminated."
#define LOAD__READ_FAILURE_REPORT "Failed to read file."
#define LOAD__MALLOC_FAILURE_REPORT "Failed to read file due to memory allocation failure."
#define LOAD__UNSUPPORTED_REPORT "Sorry, paletted images are currently not supported. File can not be loaded."

//Save error reports
#define SAVE__REPORT_HEADER "Save failure"
#define SAVE__FAILURE_REPORT "Failed to save file."

//Small frame report
#define FRAME__WARNING_HEADER "Note"
#define FRAME__SMALL_WARNING "The image is too small for the frame to have proper detalization. If you want to appreciate the pattern, pick a bigger picture. However, a frame has been drawn anyway."

//Rotate error reports
#define ROTATE__ERROR_REPORT "Rotating failed due to memory allocation failure."

#define FORM_TITLE "Simple PNG editor"

//About
#define ABOUT QString("Simple image editor by Mikhail Eiswald\n") + \
              QString("\n") + QString("Short manual:\n") + \
              QString("Press folder icon to load an image. ") + \
              QString("Click on the image to specify a point. ") + \
              QString("Press Rectangle button to draw a rectangle with two specified") + \
              QString(" points as opposite corners. ") + \
              QString("Press Rotate button to rotate a specified") + \
              QString(" region. Press Frame button to draw a frame. ") + \
              QString("Press Divide button to divide image. ") + \
              QString("Use tools on the bar to set line color,") + \
              QString(" line width, frame pattern, angle of ") + \
              QString("rotation and so on. Press Save icon to ") + \
              QString("save your picture. Press dice icon to ") + \
              QString("see the information about the picture.")


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PNGimage* my_image;
   // PaintWidget *wgt;  // pointer to widget for image

    QAction* m_actionOpen;
    QAction* m_actionSave;
    QAction* m_actionClose;
    QAction* m_actionProperty;
    QAction* m_actionAbout;

    QAction* m_actionDivide;
    QAction* m_actionRotate;
    QAction* m_actionRect;
    QAction* m_actionFrame;


   QMenu *pmnuFile;
   QMenu *pmnuAbout;

   QLineEdit* pm_X_num;   // num Divide Horizontal
   QLineEdit* pm_Y_num;   // num Divide Vertical

   int m_H_num;
   int m_V_num;

   QComboBox* pm_Rotate;  // index 0 - 0; 1 - 90; 2 - 180; 3 - 270 degree

   QComboBox* m_colorComboBox;
   QComboBox* m_colorFillComboBox;
   QColor* palette;


   QComboBox* m_lineComboBox;
//   int m_widthLine;   // width line of rectangle

   QComboBox* m_patternComboBox;



   QComboBox* m_widthFrameComboBox; // width  of Frame
   int m_numFrame;   // type of Frame

   QPoint m_point1; // coner 1
   QPoint m_point2; // coner 2


   // Mouse position
   QLabel* m_plblX1;
   QLabel* m_plblY1;

   QLabel* m_plblX2;
   QLabel* m_plblY2;

   // Draw Image
   QPixmap pix;
   QLabel* lb;
   QScrollArea* scrollarea;

   QCursor* cursor;

   QString m_filename;
   png_infop info_ptr;
   png_infop info_ptr_before;

private:

    QString stringCOLOR[8];
    Ui::MainWindow *ui;

protected:
   void mousePressEvent(QMouseEvent *pe);
   void redrawImage();

protected slots:

    void slotOpen();
    bool slotSave();
    void slotProperty();
    void slotAbout();

    void slotDivide();
    void slotRotate();
    void slotRectangle();
    void slotFrame();

signals:
};


#endif // MAINWINDOW_H

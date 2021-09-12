#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include<QPalette>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->setWindowTitle(FORM_TITLE);

    my_image = NULL;

   scrollarea = new QScrollArea(this);
   lb = new QLabel(scrollarea);

   scrollarea->setWidget(lb);
   setCentralWidget(scrollarea);

   stringCOLOR[0] = QString("PNG_COLOR_TYPE_GRAY");
   stringCOLOR[2] = QString("PNG_COLOR_TYPE_RGB");
   stringCOLOR[4] = QString("PNG_COLOR_TYPE_GRAY_ALPHA");
   stringCOLOR[6] = QString("PNG_COLOR_TYPE_RGB_ALPHA");

   // create palette
   palette = new QColor[8];
   palette[0].setNamedColor("#000000");
   palette[1].setNamedColor("#ff0000");
   palette[2].setNamedColor("#ffa500");
   palette[3].setNamedColor("#ffff00");
   palette[4].setNamedColor("#00ff00");
   palette[5].setNamedColor("#0000ff");
   palette[6].setNamedColor("#808080");
   palette[7].setNamedColor("#ffffff");


    m_actionOpen = new QAction(tr("&Open"), this);
    m_actionOpen->setIcon(QPixmap(":/images/images/open.png"));
    m_actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(slotOpen()));

    m_actionSave = new QAction(tr("&Save"), this);
    m_actionSave->setIcon(QPixmap(":/images/images/save.png"));
    m_actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(m_actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    m_actionProperty = new QAction(tr("&Property"), this);
    m_actionProperty->setIcon(QPixmap(":/images/images/test.png"));
    m_actionProperty->setShortcut(QKeySequence("Ctrl+I"));
    connect(m_actionProperty, SIGNAL(triggered()), this, SLOT(slotProperty()));

    m_actionClose = new QAction(tr("&Close"), this);
    connect(m_actionClose, SIGNAL(triggered()), this, SLOT(close()));

    m_actionAbout = new QAction(tr("&About"), this);
    connect(m_actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

    m_actionDivide = new QAction(tr("Divide"), this);
    m_actionDivide->setIcon(QPixmap(":/images/images/Div.png"));
    m_actionDivide->setShortcut(QKeySequence("Ctrl+D"));
    connect(m_actionDivide, SIGNAL(triggered()), this, SLOT(slotDivide()));

    m_actionRotate = new QAction(tr("Rotate"), this);
    m_actionRotate->setIcon(QPixmap(":/images/images/Rotate.png"));
    m_actionRotate->setShortcut(QKeySequence("Ctrl+R"));
    connect(m_actionRotate, SIGNAL(triggered()), this, SLOT(slotRotate()));

    m_actionRect = new QAction(tr("Rectangle"), this);
    m_actionRect->setIcon(QPixmap(":/images/images/Rect.png"));
    m_actionRect->setShortcut(QKeySequence("Ctrl+T"));
    connect(m_actionRect, SIGNAL(triggered()), this, SLOT(slotRectangle()));

    m_actionFrame = new QAction(tr("Frame"), this);
    m_actionFrame->setIcon(QPixmap(":/images/images/Frame_3.png"));
    m_actionFrame->setShortcut(QKeySequence("Ctrl+F"));
    connect(m_actionFrame, SIGNAL(triggered()), this, SLOT(slotFrame()));

    // create Menu

    pmnuFile = new QMenu("&File", menuBar());
    pmnuFile->addAction(m_actionOpen);
    pmnuFile->addAction(m_actionSave);
    pmnuFile->addAction(m_actionProperty);
    pmnuFile->addSeparator();
    pmnuFile->addAction(m_actionClose);

    pmnuAbout = new QMenu("&About", menuBar());
    pmnuAbout->addAction(m_actionAbout);


    menuBar()->addMenu(pmnuFile);
    menuBar()->addMenu(pmnuAbout);


    // create Widgets for StatusBar
    m_plblX1 = new QLabel(this);
    m_plblY1 = new QLabel(this);

    m_plblX2 = new QLabel(this);
    m_plblY2 = new QLabel(this);

    m_plblX1->setFixedWidth(80);
    m_plblY1->setFixedWidth(80);
    m_plblX2->setFixedWidth(80);
    m_plblY2->setFixedWidth(80);

    // create Widgets for ToolBar

    pm_X_num = new QLineEdit(0, this);
    pm_X_num->setValidator(new QIntValidator(1, 100, this));
    pm_X_num->setText("1");
    pm_Y_num = new QLineEdit(0, this);
    pm_Y_num->setValidator(new QIntValidator(1, 100, this));
    pm_Y_num->setText("1");

    QLabel* pLabel_H = new QLabel(" H: ", this);
    QLabel* pLabel_V = new QLabel(" V: ", this);

    pm_Rotate = new QComboBox(this);
    pm_Rotate->addItem("0", 0);
    pm_Rotate->addItem("90", 1);
    pm_Rotate->addItem("180", 2);
    pm_Rotate->addItem("270", 3);

    QLabel* pLabel_color = new QLabel(" Color: ", this);

    m_colorComboBox = new QComboBox(this);
    m_colorComboBox->addItem(QIcon(":/images/images/Black.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Red.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Orange.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Yellow.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Green.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Blue.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/Gray.png"), "");
    m_colorComboBox->addItem(QIcon(":/images/images/White.png"), "");

    QLabel* pLabel_colorFill = new QLabel(" Color Fill: ", this);

    m_colorFillComboBox = new QComboBox(this);
    m_colorFillComboBox->addItem(QIcon(":/images/images/Rect.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Black.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Red.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Orange.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Yellow.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Green.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Blue.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/Gray.png"), "");
    m_colorFillComboBox->addItem(QIcon(":/images/images/White.png"), "");

    QLabel* pLabe_Line = new QLabel(" Line: ", this);

    m_lineComboBox = new QComboBox(this);
    m_lineComboBox->addItem(QIcon(":/images/images/Line_1.png"), "1");
    m_lineComboBox->addItem(QIcon(":/images/images/Line_2.png"), "3");
    m_lineComboBox->addItem(QIcon(":/images/images/Line_3.png"), "5");

    m_lineComboBox->setFixedWidth(60);

    QLabel* pLabe_Pattern = new QLabel(" Pattern: ", this);

    m_patternComboBox = new QComboBox(this);
    m_patternComboBox->addItem("Squares");
    m_patternComboBox->addItem("Triangles");
    m_patternComboBox->addItem("Ice fractal");


    m_patternComboBox->setFixedWidth(90);

    QLabel* pLabe_width = new QLabel(" Width: ", this);

    m_widthFrameComboBox = new QComboBox(this);
    m_widthFrameComboBox->addItem("5%", 1);
    m_widthFrameComboBox->addItem("10%", 2);
    m_widthFrameComboBox->addItem("15%", 3);


    // add Actions and Widgets into ToolBar

    ui->mainToolBar->addAction(m_actionOpen);
    ui->mainToolBar->addAction(m_actionSave);
    ui->mainToolBar->addAction(m_actionProperty);

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(m_actionDivide);

    pm_X_num->setFixedWidth(40);
    pm_Y_num->setFixedWidth(40);

    ui->mainToolBar->addWidget(pLabel_H);
    ui->mainToolBar->addWidget(pm_X_num);
    ui->mainToolBar->addWidget(pLabel_V);
    ui->mainToolBar->addWidget(pm_Y_num);

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(m_actionRotate);
    ui->mainToolBar->addWidget(pm_Rotate);

    ui->mainToolBar->addSeparator();


    // create additional ToolBar
    QToolBar* additionalToolBar = new QToolBar(this);

    additionalToolBar->setObjectName(QStringLiteral("additionalToolBar"));
    this->addToolBar(Qt::TopToolBarArea, additionalToolBar);


   additionalToolBar->addAction(m_actionRect);
   additionalToolBar->addWidget(pLabel_color);

   additionalToolBar->addWidget(m_colorComboBox);
   additionalToolBar->addWidget(pLabel_colorFill);

   additionalToolBar->addWidget(m_colorFillComboBox);
   additionalToolBar->addWidget(pLabe_Line);
   additionalToolBar->addWidget(m_lineComboBox);
   additionalToolBar->addSeparator();

   additionalToolBar->addAction(m_actionFrame);
   additionalToolBar->addWidget(pLabe_Pattern);

   additionalToolBar->addWidget(m_patternComboBox);

   additionalToolBar->addWidget(pLabe_width);
   additionalToolBar->addWidget(m_widthFrameComboBox);


   // StatusBar
   statusBar()->setFixedHeight(30);

   statusBar()->addWidget(m_plblX1);
   statusBar()->addWidget(m_plblY1);
   statusBar()->addWidget(m_plblX2);
   statusBar()->addWidget(m_plblY2);


   // Write start text for StatusBar
   m_plblX1->setText("X1= 0");
   m_plblY1->setText("Y1= 0");
   m_plblX2->setText("X2= 0");
   m_plblY2->setText("Y2= 0");

}


void MainWindow::mousePressEvent(QMouseEvent *pe)
{
   static char count = 0;

   int pos_X,pos_Y;

   if(lb->pixmap() == NULL)    return;

   pos_X = lb->mapFromGlobal(cursor->pos()).x();
   pos_Y = lb->mapFromGlobal(cursor->pos()).y();

   if(pos_X >= my_image->getImageWidth() || pos_X < 0) return;
   if(pos_Y >= my_image->getImageHeight() || pos_Y < 0) return;

   if((pe->buttons() & Qt::LeftButton) && (count == 0))
   {
     m_plblX1->setText("X1= " + QString().setNum(pos_X));
     m_plblY1->setText("Y1= " + QString().setNum(pos_Y));

     m_point1.setX(pos_X);
     m_point1.setY(pos_Y);

     count = 1;
   }
   else
   {
     m_plblX2->setText("X2= " + QString().setNum(pos_X));
     m_plblY2->setText("Y2= " + QString().setNum(pos_Y));

     m_point2.setX(pos_X);
     m_point2.setY(pos_Y);

     count = 0;
   }

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOpen()
{
   QString filename = QFileDialog::getOpenFileName(this, "Open Dialog", "", "*.png");

   if(filename == "") return;

   delete lb;

   delete my_image;
   my_image = NULL;

   m_point1.setX(0);
   m_point1.setY(0);
   m_point2.setX(0);
   m_point2.setY(0);

   m_plblX1->setText("X1= " + QString().setNum(m_point1.x()));
   m_plblY1->setText("Y1= " + QString().setNum(m_point1.y()));
   m_plblX2->setText("X2= " + QString().setNum(m_point2.x()));
   m_plblY2->setText("Y2= " + QString().setNum(m_point2.y()));

   lb = new QLabel(scrollarea);

       int result;
       QString header;
       QString message;

       my_image = new PNGimage(this);

       result = my_image->load(filename);

       if(result) {
           header = QString(LOAD__REPORT_HEADER);
           switch(result){
             case OPEN_FAILURE:
               message = QString(LOAD__OPEN_FAILURE_REPORT);
               break;
             case RECOGNISE_FAILURE:
               message = QString(LOAD__RECOGNISE_FAILURE_REPORT);
               break;
             case READ_STRUCT_FAILURE:
             case INFO_STRUCT_FAILURE:
             case IO_INIT_FAILURE:
               message = QString(LOAD__PREPARE_READ_FAILURE_REPORT);
               break;
             case READ_FAILURE:
             case END_READ_FAILURE:
               message = QString(LOAD__READ_FAILURE_REPORT);
               break;
             case UNSUPPORTED_PNG_FORMAT:
               message = QString(LOAD__UNSUPPORTED_REPORT);
               break;
             case MALLOC_FAILURE:
               message = QString(LOAD__MALLOC_FAILURE_REPORT);
               break;
           }

           QMessageBox::warning(this,header,message);
           setWindowTitle(FORM_TITLE);

           delete my_image;
           my_image = NULL;
       }
       else{

           QImage* addr = my_image->convert_to_qimage();
           if(!addr){
               QMessageBox::warning(this, "Error", "Image could not be displayed.");
               delete my_image;
               my_image = NULL;
               return;
           }

           lb->setPixmap(QPixmap::fromImage(*addr));

           delete addr;

           scrollarea->setWidget(lb);

           m_filename = filename;
           setWindowTitle(m_filename);

       }

}


bool MainWindow::slotSave()
{
   if(!lb->pixmap()) return false;
   int result;

   QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "*.png");
   if(filename == "") return false;

   result = my_image->write(filename);

   if(result == 0)
   {
     m_filename = filename;
     return true;
   }
   else{
       QString message = QString(SAVE__FAILURE_REPORT);
       QString header = QString(SAVE__REPORT_HEADER);
       QMessageBox::warning(this, header, message);
       return false;
   }
}

void MainWindow::slotProperty()
{

    if(!lb->pixmap()) return;

    info_ptr = my_image->getPNG_info();
    info_ptr_before = my_image->getPNG_info_before();
    if(!info_ptr || !info_ptr_before) return;

    QString str =  "File name: " + m_filename + "\n";
    str += "Size W x H: " + QString::number(my_image->getImageWidth()) + " x " + QString::number(my_image->getImageHeight()) +"\n\n";
    str += "Before conversion \n";
    str += "Color : " + stringCOLOR[info_ptr_before->color_type] + "\n";
    str += "Bit depth:  " + QString::number(info_ptr_before->bit_depth) + "\n";
    str += "Channels:  " + QString::number(info_ptr_before->channels) + "\n";
    str += "Interlace type:  " + QString::number(info_ptr_before->interlace_type) + "\n";
    str += "Pixel depth:  " + QString::number(info_ptr_before->pixel_depth) + "\n\n";



    str += "After conversion: \n";
    str += "Color : PNG_COLOR_TYPE_RGB_ALPHA\n";
    str += "Bit depth:  " + QString::number(info_ptr->bit_depth) + "\n";
    str += "Channels:  " + QString::number(info_ptr->channels) + "\n";
    str += "Interlace type:  " + QString::number(info_ptr->interlace_type) + "\n";
    str += "Pixel depth:  " + QString::number(info_ptr->pixel_depth) + "\n";

    QMessageBox::information(0, "Properties", str);


}

void MainWindow::slotAbout()
{
    QMessageBox::information(this, "About", ABOUT);
}

void MainWindow::slotDivide()
{

    int H, V;
    int widthLine;

    H =  pm_X_num->text().toInt();
    V = pm_Y_num->text().toInt();
    widthLine = m_lineComboBox->currentText().toInt();


 // is pixmap ready?
 if(lb->pixmap() == NULL)    return;

 // is divide correct?
 if((H > my_image->getImageWidth())  || (H < 1) ||
    (V > my_image->getImageHeight()) || (V < 1)) return;


 my_image->divide_image(H, V, widthLine, palette + m_colorComboBox->currentIndex());

 // Redraw my_image
 redrawImage();

}


void MainWindow::slotRotate()
{

 if(!lb->pixmap()) return;
 int result = my_image->rotate(&m_point1, &m_point2,
                  (pm_Rotate->currentIndex())*M_PI_2);
 if(result){
     QString message = QString(ROTATE__ERROR_REPORT);
     QMessageBox::critical(this, "Error", message);
 }
 this->redrawImage();
}

void MainWindow::slotRectangle()
{

    int widthLine;
    int indexColor;
    int indexFillColor;

    widthLine = m_lineComboBox->currentText().toInt();
    indexColor = m_colorComboBox->currentIndex();
    indexFillColor = m_colorFillComboBox->currentIndex();
    //
    if(lb->pixmap() == NULL)    return;


    my_image->draw_rectangle(&m_point1, &m_point2, widthLine, palette + indexColor,
                             indexFillColor > 0 ? palette+(indexFillColor - 1) : NULL);


    // Redraw my_image
    redrawImage();
}

void MainWindow::slotFrame()
{
    if(!lb->pixmap()) return;

    double pattern;
    double omega;

    switch(m_patternComboBox->currentIndex()){
    case 0:
        pattern = SQUARES;
        break;
    case 1:
        pattern = TRIANGLES;
        break;
    case 2:
        pattern = ICE_FRACTAL;
    }

    omega = (m_widthFrameComboBox->currentIndex()+1)/20.0;

    int result = my_image->draw_ice_frame(&(palette[m_colorComboBox->currentIndex()]),
            omega, pattern);
    this->redrawImage();
    if(result){
        QString title = QString(FRAME__WARNING_HEADER);
        QString message = QString(FRAME__SMALL_WARNING);
        QMessageBox::information(this, title, message);
    }

}


void MainWindow::redrawImage()
{
    delete lb;

   lb = new QLabel(scrollarea);


    QImage* addr = my_image->convert_to_qimage();
    if(!addr){
        QMessageBox::warning(this, "Error", "Image could not be displayed.");
        delete my_image;
        my_image = NULL;
        return;
    }

    lb->setPixmap(QPixmap::fromImage(*addr));

    delete addr;

    scrollarea->setWidget(lb);

}







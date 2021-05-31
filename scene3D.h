#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include "method1_init.h"
#include "method_compute.h"

class Scene3D : public QGLWidget
{
   Q_OBJECT
private:
   GLfloat xRot;
   GLfloat yRot;
   GLfloat zRot;
   GLfloat zTra;
   GLfloat nSca;

   QPoint ptrMousePosition;

   void scale_plus();
   void scale_minus();
   void rotate_up();
   void rotate_down();
   void rotate_left();
   void rotate_right();
   void translate_down();
   void translate_up();
   void defaultScene();

   void drawAxis();

   int func_id;
   double a_x;
   double a_y;
   double b_x;
   double b_y;

   double f(double, double);
   double *values;
   double *file_values;
   double *derivatives;
   double **state;
   double scale_parameter;

protected:
   void initializeGL();
   void resizeGL(int nWidth, int nHeight);
   void paintGL();

   void mousePressEvent(QMouseEvent *pe);
   void mouseMoveEvent(QMouseEvent *pe);
   void wheelEvent(QWheelEvent *pe);
   void keyPressEvent(QKeyEvent *pe);

public:
   Scene3D(QWidget *parent);
   ~Scene3D();
   int parse_command_line(int argc, char *argv[]);
   double Pf1(double x, double y, int n_appr_x, int n_appr_y);
   double Errf(double x, double y, int n_appr_x, int n_appr_y);
   int Pf1_init(int n_appr_x, double a_x, double b_x, int n_appr_y, double a_y, double b_y);
   const char *f_name;
   int n_appr_x;
   int n_appr_y;
   int n;
   double max_buf;
   double max_buf_1;
   double max_buf_2;
   int disturb;
   bool show_graph_1;
   bool show_graph_err;
   bool initialized;
public slots:
   void change_func();
   void add_n_appr();
   void add_n_appr_x();
   void add_n_appr_y();
   void reduce_n_appr();
   void reduce_n_appr_x();
   void reduce_n_appr_y();
   void show_method_1();
   void show_err();
   void scale_up();
   void scale_down();
   void add_disturb();
   void sub_disturb();
   void add_fineness();
   void reduce_fineness();
};

#endif
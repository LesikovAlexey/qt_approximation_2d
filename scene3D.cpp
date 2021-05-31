#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "scene3D.h"
#define DEFAULT_A -10
#define DEFAULT_B 10
const static float pi = 3.141593, k = pi / 180;

static double f_0(double x, double y)
{
   return 1;
}

static double f_1(double x, double y)
{
   return x;
}

static double f_2(double x, double y)
{
   return y;
}

static double f_3(double x, double y)
{
   return x + y;
}

static double f_4(double x, double y)
{
   return sqrt(x * x + y * y);
}

static double f_5(double x, double y)
{
   return x * x + y * y;
}

static double f_6(double x, double y)
{
   return exp(x * x - y * y);
}

static double f_7(double x, double y)
{
   return 1 / (25 * (x * x + y * y) + 1);
}

Scene3D::Scene3D(QWidget *parent) : QGLWidget(parent)
{
   xRot = -90;
   yRot = 0;
   zRot = 0;
   zTra = 0;
   nSca = 1;
   a_x = DEFAULT_A;
   b_x = DEFAULT_B;
   a_y = DEFAULT_A;
   b_y = DEFAULT_B;
   n = 100;
   n_appr_x = 25;
   n_appr_y = 25;
   show_graph_1 = 1;
   show_graph_err = 0;
   scale_parameter = 1;
   disturb = 0;
   max_buf = 0;
   max_buf_1 = 0;
   max_buf_2 = 0;
   initialized = 0;
   func_id = 6;

   change_func();

   state = (double **)malloc(n_appr_x * n_appr_y * sizeof(double));
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         state[i * n_appr_y + j] = (double *)malloc(16 * sizeof(double));
      }
   }
   derivatives = (double *)malloc(2 * (n_appr_x + n_appr_y) * sizeof(double));
   values = (double *)malloc(n_appr_x * n_appr_y * sizeof(double));
}

Scene3D::~Scene3D()
{
   free(values);
   free(derivatives);

   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
}

int Scene3D::Pf1_init(int n_appr_x, double a_x, double b_x, int n_appr_y, double a_y, double b_y)
{

   double h_x = (b_x - a_x) / (n_appr_x - 1);
   double h_y = (b_y - a_y) / (n_appr_y - 1);
   state = (double **)malloc(n_appr_x * n_appr_y * sizeof(double *));
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         state[i * n_appr_y + j] = (double *)malloc(16 * sizeof(double));
      }
   }
   values = (double *)realloc(values, n_appr_x * n_appr_y * sizeof(double));
   derivatives = (double *)realloc(derivatives, 2 * (n_appr_x + n_appr_y) * sizeof(double));
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         values[i * n_appr_y + j] = 0;
      }
   }
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         for (int k = 0; k < 16; k++)
         {
            state[i * n_appr_y + j][k] = 0;
         }
      }
   }
   for (int i = 0; i < 2 * (n_appr_x + n_appr_y); i++)
   {
      derivatives[i] = 0;
   }
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         values[i * n_appr_y + j] = f(a_x + i * h_x, a_y + j * h_y);
      }
   }

   for (int i = 0; i < 2 * n_appr_y; i += 2)
   {
      derivatives[i] = (f(a_x + h_x / 1e10, a_y + (i / 2) * h_y) - f(a_x, a_y + (i / 2) * h_y)) / (h_x / 1e10);
      derivatives[i + 1] = (f(b_x, a_y + (i / 2) * h_y) - f(b_x - h_x / 1e10, a_y + (i / 2) * h_y)) / (h_x / 1e10);
   }

   for (int i = 0; i < 2 * n_appr_x; i += 2)
   {
      derivatives[2 * n_appr_y + i] = (f(a_x + (i / 2) * h_x, a_y + h_y / 1e10) - f(a_x + (i / 2) * h_x, a_y)) / (h_y / 1e10);
      derivatives[2 * n_appr_y + i + 1] = (f(a_x + (i / 2) * h_x, b_y) - f(a_x + (i / 2) * h_x, b_y - h_y / 1e10)) / (h_y / 1e10);
   }
   method1_init(n_appr_x, a_x, b_x, n_appr_y, a_y, b_y, values, derivatives, state);
   return 0;
}

double Scene3D::Pf1(double x, double y, int n_appr_x, int n_appr_y)
{
   return method_compute(n_appr_x, a_x, b_x, x, n_appr_y, a_y, b_y, y, state);
}

double Scene3D::Errf(double x, double y, int n_appr_x, int n_appr_y)
{
  return fabs(Pf1(x, y, n_appr_x, n_appr_y) - f(x, y));
}

double Scene3D::f(double x, double y)
{

   switch (func_id)
   {
   case 0:
      f_name = "k = 0   f (x, y) = 1";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_0(x, y) + disturb * 0.1 * max_buf;
      return (f_0(x, y));
   case 1:
      f_name = "k = 1   f (x, y) = x";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_1(x, y) + disturb * 0.1 * max_buf;
      return (f_1(x, y));
   case 2:
      f_name = "k = 2   f (x, y) = y";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_2(x, y) + disturb * 0.1 * max_buf;
      return (f_2(x, y));
   case 3:
      f_name = "k = 3   f (x, y) = x + y";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_3(x, y) + disturb * 0.1 * max_buf;
      return (f_3(x, y));
   case 4:
      f_name = "k = 4   f (x, y) = sqrt(x * x + y * y)";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_4(x, y) + disturb * 0.1 * max_buf;
      return (f_4(x, y));
   case 5:
      f_name = "k = 5   f (x, y) = x * x + y * y";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_5(x, y) + disturb * 0.1 * max_buf;
      return (f_5(x, y));
   case 6:
      f_name = "k = 6   f (x, y) = exp(x * x - y * y)";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_6(x, y) + disturb * 0.1 * max_buf;
      return (f_6(x, y));
   case 7:
      f_name = "k = 7   f (x, y) = 1 / (25 * (x * x + y * y) + 1)";
      if (fabs(x - a_x - ((b_x - a_x) / (n_appr_x - 1)) * (n_appr_x / 2)) < (b_x - a_x) / (10 * n_appr_x) && fabs(y - a_y - ((b_y - a_y) / (n_appr_y - 1)) * (n_appr_y / 2)) < (b_y - a_y) / (10 * n_appr_y))
         return f_7(x, y) + disturb * 0.1 * max_buf;
      return (f_7(x, y));
   default:
      return 0;
   }
}

int Scene3D::parse_command_line(int argc, char *argv[])
{
   FILE *input_file;
   if (argc == 1)
      return 0;

   if (argc == 2)
      return -1;

   if (sscanf(argv[1], "%lf", &a_x) != 1 || sscanf(argv[2], "%lf", &b_x) != 1 || sscanf(argv[4], "%d", &func_id) != 1 || b_x - a_x < 1.e-6 || (sscanf(argv[3], "%d", &n_appr_x) != 1) || n_appr_x <= 9 || func_id < 0 || func_id > 6)
      return -2;
   return 0;
}

void Scene3D::change_func()
{
   func_id = (func_id + 1) % 7;

   switch (func_id)
   {
   case 0:
      f_name = "k = 0   f (x) = 1";
      break;
   case 1:
      f_name = "k = 1   f (x) = x";
      break;
   case 2:
      f_name = "k = 2   f (x) = x * x";
      break;
   case 3:
      f_name = "k = 3   f (x) = x * x * x";
      break;
   case 4:
      f_name = "k = 4   f (x) = x * x * x * x";
      break;
   case 5:
      f_name = "k = 5   f (x) = exp(x)";
      break;
   case 6:
      f_name = "k = 6   f (x) = 1 / (25 * x * x + 1)";
      break;
   }
   initialized = 0;
   updateGL();
}

void Scene3D::add_n_appr()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   n_appr_x += 10;
   n_appr_y += 10;
   initialized = 0;
   updateGL();
}

void Scene3D::add_n_appr_x()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   n_appr_x += 10;
   initialized = 0;
   updateGL();
}

void Scene3D::add_n_appr_y()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   n_appr_y += 10;
   initialized = 0;
   updateGL();
}

void Scene3D::reduce_n_appr()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   if (n_appr_x - 10 > 9)
   {
      n_appr_x -= 10;
   }
   if (n_appr_y - 10 > 9)
   {
      n_appr_y -= 10;
   }
   initialized = 0;
   updateGL();
}

void Scene3D::reduce_n_appr_x()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   if (n_appr_x - 10 > 9)
   {
      n_appr_x -= 10;
   }
   initialized = 0;
   updateGL();
}

void Scene3D::reduce_n_appr_y()
{
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         free(state[i * n_appr_y + j]);
      }
   }
   free(state);
   if (n_appr_y - 10 > 9)
   {
      n_appr_y -= 10;
   }
   initialized = 0;
   updateGL();
}

void Scene3D::add_fineness()
{
   if (n * 2 < 1000)
   {
      n *= 2;
   }
   updateGL();
}

void Scene3D::reduce_fineness()
{
   if (n / 2 > 40)
   {
      n /= 2;
   }
   updateGL();
}

void Scene3D::add_disturb()
{
   disturb += 1;
   initialized = 0;
   updateGL();
}

void Scene3D::sub_disturb()
{
   disturb -= 1;
   initialized = 0;
   updateGL();
}

void Scene3D::show_method_1()
{
   show_graph_1 = (show_graph_1 + 1) % 2;
   initialized = 0;
   updateGL();
}

void Scene3D::show_err()
{
   show_graph_err = (show_graph_err + 1) % 2;
   show_graph_1 = 0;
   initialized = 0;
   updateGL();
}
void Scene3D::scale_up()
{
   scale_parameter *= 2;
   updateGL();
}
void Scene3D::scale_down()
{
   if (scale_parameter > 1)
   {
      scale_parameter /= 2;
      updateGL();
   }
}
void Scene3D::initializeGL()
{
   qglClearColor(Qt::white);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glEnable(GL_CULL_FACE);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   a_x = DEFAULT_A;
   b_x = DEFAULT_B;
   a_y = DEFAULT_A;
   b_y = DEFAULT_B;
   n = 50;
   n_appr_x = 25;
   n_appr_y = 25;
   show_graph_1 = 0;
   show_graph_err = 0;
   scale_parameter = 1;
   disturb = 0;
   max_buf = 0;
   max_buf_1 = 0;
   max_buf_2 = 0;
   initialized = 0;
   func_id = 0;

   state = (double **)malloc(n_appr_x * n_appr_y * sizeof(double));
   for (int i = 0; i < n_appr_x; i++)
   {
      for (int j = 0; j < n_appr_y; j++)
      {
         state[i * n_appr_y + j] = (double *)malloc(16 * sizeof(double));
      }
   }
   derivatives = (double *)malloc(2 * (n_appr_x + n_appr_y) * sizeof(double));
   values = (double *)malloc(n_appr_x * n_appr_y * sizeof(double));
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio = (GLfloat)nHeight / (GLfloat)nWidth;
   if (nWidth >= nHeight)
      glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -10.0, 1.0);
   else
      glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -10.0, 1.0);

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::paintGL()
{
   double x1, x2, y1, y2, z1, z2, z3, z4;
   double delta_x = (b_x - a_x) / n;
   double delta_y = (b_y - a_y) / n;
   double buf;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(nSca, nSca, nSca);
   glTranslatef(0.0f, zTra, 0.0f);
   glRotatef(xRot, 1.0f, 0.0f, 0.0f);
   glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   glRotatef(zRot, 0.0f, 0.0f, 1.0f);

   drawAxis();
   x1 = a_x;
   y1 = a_y;
   buf = y1;
   max_buf = 0;
   if (show_graph_err == 0)
   {
      for (x2 = x1 + delta_x; x2 - b_x < 1.e-6; x2 += delta_x)
      {
         for (y2 = y1 + delta_y; y2 - b_y < 1.e-6; y2 += delta_y)
         {
            z1 = f(x1, y1);
            z2 = f(x1, y2);
            z3 = f(x2, y1);
            z4 = f(x2, y2);
            if (max_buf < fabs(z1)) max_buf = fabs(z1);
            if (max_buf < fabs(z2)) max_buf = fabs(z2);
            if (max_buf < fabs(z3)) max_buf = fabs(z3);
            if (max_buf < fabs(z4)) max_buf = fabs(z4);
            glBegin(GL_LINE_LOOP);
            glColor4f(0, 0, 0, 70);
            glVertex3f(x1, y1, z1);
            glVertex3f(x1, y2, z2);
            glVertex3f(x2, y1, z3);
            glVertex3f(x2, y2, z4);
            glEnd();
            y1 = y2;
         }
         x1 = x2, y1 = buf;
      }
      if (show_graph_1 == 1)
      {
         if (initialized == 0)
         {
            Pf1_init(n_appr_x, a_x, b_x, n_appr_y, a_y, b_y);
            initialized = 1;
         }
         x1 = a_x;
         y1 = a_y;
         buf = y1;
         for (x2 = x1 + delta_x; x2 - b_x < 1.e-6; x2 += delta_x)
         {
            for (y2 = y1 + delta_y; y2 - b_y < 1.e-6; y2 += delta_y)
            {
               z1 = Pf1(x1, y1, n_appr_x, n_appr_y);
               z2 = Pf1(x1, y2, n_appr_x, n_appr_y);
               z3 = Pf1(x2, y1, n_appr_x, n_appr_y);
               z4 = Pf1(x2, y2, n_appr_x, n_appr_y);
               glBegin(GL_LINE_LOOP);
               glColor4f(0, 0, 50, 70);
               glVertex3f(x1, y1, z1);
               glVertex3f(x1, y2, z2);
               glVertex3f(x2, y1, z3);
               glVertex3f(x2, y2, z4);
               glEnd();
               y1 = y2;
            }
            x1 = x2, y1 = buf;
         }
      }
   }
   if (show_graph_err == 1)
   {
      if (initialized == 0)
      {
         Pf1_init(n_appr_x, a_x, b_x, n_appr_y, a_y, b_y);
         initialized = 1;
      }
      x1 = a_x;
      y1 = a_y;
      buf = y1;
      for (x2 = x1 + delta_x; x2 - b_x < 1.e-6; x2 += delta_x)
      {
         for (y2 = y1 + delta_y; y2 - b_y < 1.e-6; y2 += delta_y)
         {
            z1 = Errf(x1, y1, n_appr_x, n_appr_y);
            z2 = Errf(x1, y2, n_appr_x, n_appr_y);
            z3 = Errf(x2, y1, n_appr_x, n_appr_y);
            z4 = Errf(x2, y2, n_appr_x, n_appr_y);
            if (max_buf_1 < fabs(z1)) max_buf_1 = fabs(z1);
            if (max_buf_1 < fabs(z2)) max_buf_1 = fabs(z2);
            if (max_buf_1 < fabs(z3)) max_buf_1 = fabs(z3);
            if (max_buf_1 < fabs(z4)) max_buf_1 = fabs(z4);
            glBegin(GL_LINE_LOOP);
            glColor4f(0, 0, 50, 70);
            glVertex3f(x1, y1, z1);
            glVertex3f(x1, y2, z2);
            glVertex3f(x2, y1, z3);
            glVertex3f(x2, y2, z4);
            glEnd();
            y1 = y2;
         }
         x1 = x2, y1 = buf;
      }
   }
}

void Scene3D::mousePressEvent(QMouseEvent *pe)
{
   ptrMousePosition = pe->pos();
}

void Scene3D::mouseMoveEvent(QMouseEvent *pe)
{
   xRot += 180 / nSca * (GLfloat)(pe->y() - ptrMousePosition.y()) / height();
   zRot += 180 / nSca * (GLfloat)(pe->x() - ptrMousePosition.x()) / width();

   ptrMousePosition = pe->pos();

   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent *pe)
{
   if ((pe->delta()) > 0)
      scale_plus();
   else if ((pe->delta()) < 0)
      scale_minus();

   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent *pe)
{
   switch (pe->key())
   {
   case Qt::Key_Plus:
      scale_plus();
      break;

   case Qt::Key_Equal:
      scale_plus();
      break;

   case Qt::Key_Minus:
      scale_minus();
      break;

   case Qt::Key_Up:
      rotate_up();
      break;

   case Qt::Key_Down:
      rotate_down();
      break;

   case Qt::Key_Left:
      rotate_left();
      break;

   case Qt::Key_Right:
      rotate_right();
      break;

   case Qt::Key_Z:
      translate_down();
      break;

   case Qt::Key_X:
      translate_up();
      break;

   case Qt::Key_Space:
      defaultScene();
      break;

   case Qt::Key_Escape:
      this->close();
      break;
   }

   updateGL();
}

void Scene3D::scale_plus()
{
   nSca = nSca * 1.1;
}

void Scene3D::scale_minus()
{
   nSca = nSca / 1.1;
}

void Scene3D::rotate_up()
{
   xRot += 1.0;
}

void Scene3D::rotate_down()
{
   xRot -= 1.0;
}

void Scene3D::rotate_left()
{
   zRot += 1.0;
}

void Scene3D::rotate_right()
{
   zRot -= 1.0;
}

void Scene3D::translate_down()
{
   zTra -= 0.05;
}

void Scene3D::translate_up()
{
   zTra += 0.05;
}

void Scene3D::defaultScene()
{
   xRot = -90;
   yRot = 0;
   zRot = 0;
   zTra = 0;
   nSca = 1;
}

void Scene3D::drawAxis()
{
   glLineWidth(3.0f);

   glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(a_x, 0.0f, 0.0f);
   glVertex3f(b_x, 0.0f, 0.0f);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(-10, -10, 0.0f);
   glVertex3f(-10, -10, 5);
   glEnd();

   QColor halfGreen(0, 128, 0, 255);
   qglColor(halfGreen);
   glBegin(GL_LINES);
   glVertex3f(0.0f, a_y, 0.0f);
   glVertex3f(0.0f, b_y, 0.0f);

   glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
   glVertex3f(0.0f, 0.0f, 10.0f);
   glVertex3f(0.0f, 0.0f, -10.0f);
   glEnd();
}

void Window::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);
   painter.setPen("black");
   painter.drawText(0, 20, scene->f_name);
   painter.drawText(0, 40, "appr points on x = " + QString::number(scene->n_appr_x) + " appr points on y = " + QString::number(scene->n_appr_y));
   painter.drawText(0, 60, "draw points = " + QString::number(scene->n));

   if (scene->show_graph_1 == 1 || scene->show_graph_err == 1)
   {
      painter.setPen("blue");
      painter.drawText(0, 80, "- method 1");
      if (scene->show_graph_err == 1)
      {
         painter.drawText(0, 100, "Err1 = " + QString::number(scene->max_buf_1));
      }
   }
   painter.drawText(0, 200, "disturbance = " + QString::number(scene->disturb));
   update();
}
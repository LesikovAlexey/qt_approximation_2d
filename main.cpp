
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QtOpenGL>

#include "scene3D.h"

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QMainWindow *window = new QMainWindow;
   Scene3D *scene1 = new Scene3D(window);
   QMenuBar *tool_bar = new QMenuBar(window);
   QAction *action;
   Window *text_area = new Window(window);
   text_area->scene = scene1;
   if (scene1->parse_command_line(argc, argv))
   {
      QMessageBox::warning(0, "Wrong input arguments!",
                           "Wrong input arguments!");
      return -1;
   }
   action = tool_bar->addAction("&Change function", scene1, SLOT(change_func()));
   action->setShortcut(QString("Ctrl+C"));

   action = tool_bar->addAction("&Add appr points", scene1, SLOT(add_n_appr()));
   action->setShortcut(QString("Ctrl++"));

   action = tool_bar->addAction("&Reduce appr points", scene1, SLOT(reduce_n_appr()));
   action->setShortcut(QString("Ctrl+-"));

   action = tool_bar->addAction("&Show method 1", scene1, SLOT(show_method_1()));
   action->setShortcut(QString("1"));

   action = tool_bar->addAction("&Show method 2", scene1, SLOT(show_method_2()));
   action->setShortcut(QString("2"));

   action = tool_bar->addAction("&Show error graph", scene1, SLOT(show_err()));
   action->setShortcut(QString("3"));

   action = tool_bar->addAction("&Add disturbance", scene1, SLOT(add_disturb()));
   action->setShortcut(QString("Alt++"));

   action = tool_bar->addAction("&Subtract disturbance", scene1, SLOT(sub_disturb()));
   action->setShortcut(QString("Alt+-"));

   action = tool_bar->addAction("&Scale +", scene1, SLOT(scale_up()));
   action->setShortcut(QString("+"));

   action = tool_bar->addAction("&Scale -", scene1, SLOT(scale_down()));
   action->setShortcut(QString("-"));

   action = tool_bar->addAction("&Add appr points on x", scene1, SLOT(add_n_appr_x()));
   action = tool_bar->addAction("&Add appr points on y", scene1, SLOT(add_n_appr_y()));
   action = tool_bar->addAction("&Reduce appr points on x", scene1, SLOT(reduce_n_appr_x()));
   action = tool_bar->addAction("&Reduce appr points on y", scene1, SLOT(reduce_n_appr_y()));
   action = tool_bar->addAction("&Add fineness", scene1, SLOT(add_fineness()));
   action = tool_bar->addAction("&Reduce fineness", scene1, SLOT(reduce_fineness()));

   action = tool_bar->addAction("E&xit", window, SLOT(close()));
   action->setShortcut(QString("Ctrl+X"));

   tool_bar->setMaximumHeight(30);

   window->setMenuBar(tool_bar);
   window->setCentralWidget(scene1);
   window->setWindowTitle("Graph");

   window->resize(1000, 1000);
   window->show();
   text_area->show();

   return app.exec();
}

#ifndef QT_CEF_POC_CEFEMBEDWINDOW_H_
#define QT_CEF_POC_CEFEMBEDWINDOW_H_

#include <QtWidgets>
#include "cef_widget.h"

class CefEmbedWindow : public QWindow {
  Q_OBJECT
 public:
  CefEmbedWindow(QPointer<CefWidget> cef_widget, QWindow *parent = 0);
 protected:
  void moveEvent(QMoveEvent *);
  void resizeEvent(QResizeEvent *);
 private:
  QPointer<CefWidget> cef_widget_;
};

#endif // QT_CEF_POC_CEFEMBEDWINDOW_H_

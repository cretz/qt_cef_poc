#ifndef QT_CEF_POC_CEFWIDGET_H_
#define QT_CEF_POC_CEFWIDGET_H_

#include <QWidget>
#include "cef.h"

class CefWidget : public QWidget {
  Q_OBJECT
 public:
  CefWidget(Cef *cef, QWidget *parent = 0);

  void EmbedBrowser();

 private:
  Cef *cef_;
};

#endif // QT_CEF_POC_CEFWIDGET_H_

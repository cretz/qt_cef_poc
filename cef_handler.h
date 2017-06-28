#ifndef QT_CEF_POC_HANDLER_H_
#define QT_CEF_POC_HANDLER_H_

#include <QtWidgets>
#include "include/cef_client.h"

class CefHandler : public CefClient, public CefDisplayHandler {
 public:
  explicit CefHandler(QPointer<QMainWindow> main_win,
                      QPointer<QLineEdit> url_line_edit);

  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }

  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) override;

  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& url) override;

 private:
  QPointer<QMainWindow> main_win_;
  QPointer<QLineEdit> url_line_edit_;

  IMPLEMENT_REFCOUNTING(CefHandler);
};

#endif // QT_CEF_POC_HANDLER_H_

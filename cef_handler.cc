#include "cef_handler.h"

CefHandler::CefHandler(QPointer<QMainWindow> main_win,
                       QPointer<QLineEdit> url_line_edit,
                       QPointer<QWidget> browser_widg) {
  main_win_ = main_win;
  url_line_edit_ = url_line_edit;
  browser_widg_ = browser_widg;
}

void CefHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                               const CefString &title) {
  if (main_win_) {
    main_win_->setWindowTitle(QString::fromWCharArray(title.c_str()) +
                              " - QT CEF POC");
  }
}

void CefHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const CefString &url) {
  if (frame->IsMain() && url_line_edit_) {
    url_line_edit_->setText(QString::fromWCharArray(url.c_str()));
  }
}

void CefHandler::OnGotFocus(CefRefPtr<CefBrowser> browser) {
  if (url_line_edit_) {
    url_line_edit_->clearFocus();
  }
}

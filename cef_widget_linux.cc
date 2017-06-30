#include "cef_widget.h"
#include "include/cef_client.h"
#include "cef_handler.h"

QPointer<QWidget> CefWidget::EmbedBrowser(QMainWindow *main_win,
                                          QLineEdit *url_line_edit) {
  CefWindowInfo win_info;
  auto win = new QWindow;
  win_info.SetAsChild((CefWindowHandle) win->winId(),
                      CefRect(0, 0, width(), height()));
  CefBrowserSettings settings;
  CefRefPtr<CefHandler> handler(new CefHandler(main_win,
                                               url_line_edit,
                                               this));
  browser_ = CefBrowserHost::CreateBrowserSync(win_info,
                                               handler,
                                               CefString("http://example.com"),
                                               settings,
                                               nullptr);
  return QWidget::createWindowContainer(win, main_win);
}


void CefWidget::UpdateSize() {
  if (browser_) {
    auto browser_host = browser_->GetHost();
    browser_host->NotifyMoveOrResizeStarted();
  }
}

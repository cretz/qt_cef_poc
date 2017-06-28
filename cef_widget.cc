#include "cef_widget.h"
#include <Windows.h>
#include "include/cef_client.h"
#include "cef_handler.h"

CefWidget::CefWidget(Cef *cef, QWidget *parent) : QWidget(parent) {
  cef_ = cef;
}

CefWidget::~CefWidget() {
  if (browser_) {
    browser_->GetHost()->CloseBrowser(true);
  }
}

void CefWidget::EmbedBrowser(QMainWindow *main_win,
                             QLineEdit *url_line_edit) {
  CefWindowInfo win_info;
  win_info.SetAsChild((CefWindowHandle) winId(),
                      RECT { 0, 0, width(), height() });
  CefBrowserSettings settings;
  CefRefPtr<CefHandler> handler(new CefHandler(main_win, url_line_edit));
  browser_ = CefBrowserHost::CreateBrowserSync(win_info,
                                    handler,
                                    CefString("http://google.com"),
                                    settings,
                                    nullptr);
}

void CefWidget::LoadUrl(const QString &url) {
  if (browser_) {
    browser_->GetMainFrame()->LoadURL(CefString(url.toStdWString()));
  }
}

void CefWidget::moveEvent(QMoveEvent *event) {
  this->updateSize();
}

void CefWidget::resizeEvent(QResizeEvent *event) {
  this->updateSize();
}

void CefWidget::updateSize() {
  // TODO: abstract, this is win only
  // Basically just set the browser handle to the same dimensions as widget
  if (browser_) {
    auto browser_host = browser_->GetHost();
    auto browser_win = browser_host->GetWindowHandle();
    SetWindowPos(browser_win, (HWND) this->winId(), 0, 0,
                 this->width(), this->height(),
                 SWP_NOZORDER);
    browser_host->NotifyMoveOrResizeStarted();
  }
}

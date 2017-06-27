#include "cef_widget.h"

#include "include/cef_client.h"

CefWidget::CefWidget(Cef *cef, QWidget *parent) : QWidget(parent) {
  cef_ = cef;
}

CefWidget::~CefWidget() {
}

void CefWidget::EmbedBrowser() {
  CefWindowInfo win_info;
  win_info.SetAsChild((CefWindowHandle) winId(),
                      RECT { 0, 0, width(), height() });
  CefBrowserSettings settings;
  CefRefPtr<CefClient> handler(NULL);
  CefBrowserHost::CreateBrowserSync(win_info,
                                    handler,
                                    CefString("http://google.com"),
                                    settings,
                                    nullptr);
}

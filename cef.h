#ifndef QT_CEF_POC_CEF_H_
#define QT_CEF_POC_CEF_H_

class Cef {
 public:
  Cef();
  ~Cef();
  // If >= 0, this is a child and not the UI window
  int EarlyExitCode();
  void Tick();

 private:
  int early_exit_code_;
};

#endif // QT_CEF_POC_CEF_H_

#include "stdafx.h"

class HTTPDownloader : public QThread
{
  Q_OBJECT

public:
  HTTPDownloader(const QString& aURL, QWidget* aParent);
  ~HTTPDownloader();

protected:
  void run() override;

private:
  void* curl;
  std::string mURL;

  std::string download();

signals:
  void resultReady(const QString&);
};

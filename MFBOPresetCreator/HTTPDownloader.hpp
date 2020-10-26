#ifndef HTTPDOWNLOADER_HPP
#define HTTPDOWNLOADER_HPP
#include <string>

class HTTPDownloader
{
public:
  HTTPDownloader();
  ~HTTPDownloader();

  std::string download(const std::string& url);

private:
  void* curl;
};
#endif /* HTTPDOWNLOADER_HPP */
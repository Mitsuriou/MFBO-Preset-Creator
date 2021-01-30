#include "HTTPRequesterGet.h"

// UTIL function
size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
  std::string data((const char*)ptr, (size_t)size * nmemb);
  *((std::stringstream*)stream) << data << std::endl;
  return size * nmemb;
}

HTTPRequesterGet::HTTPRequesterGet(const QString& aURL, QWidget* aParent)
  : QThread(aParent)
  , mURL(aURL.toStdString())
{
  curl_global_init(CURL_GLOBAL_ALL);
  this->mCURL = curl_easy_init();
}

HTTPRequesterGet::~HTTPRequesterGet()
{
  curl_easy_cleanup(this->mCURL);
  curl_global_cleanup();
}

void HTTPRequesterGet::run()
{
  emit resultReady(QString::fromStdString(this->download()));
}

std::string HTTPRequesterGet::download()
{
  // Basic fetching setup
  curl_easy_setopt(this->mCURL, CURLOPT_URL, this->mURL.c_str());
  curl_easy_setopt(this->mCURL, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(this->mCURL, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(this->mCURL, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(this->mCURL, CURLOPT_ACCEPT_ENCODING, "deflate");

  // Output setup
  std::stringstream lOutputStream;
  curl_easy_setopt(this->mCURL, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(this->mCURL, CURLOPT_WRITEDATA, &lOutputStream);

  // Perform the request and get the return code
  CURLcode lRes{curl_easy_perform(this->mCURL)};

  if (lRes != CURLE_OK)
  {
    //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(lRes));
    return "fetch_error";
  }

  return lOutputStream.str();
}

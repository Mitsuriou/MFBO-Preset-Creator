#include "HTTPRequesterFile.h"

// UTIL function
static size_t write_file(void* ptr, size_t size, size_t nmemb, void* stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
  return written;
}

HTTPRequesterFile::HTTPRequesterFile(const QString& aURL, QWidget* aParent)
  : QThread(aParent)
  , mURL(aURL.toStdString())
{
  curl_global_init(CURL_GLOBAL_ALL);
  mCURL = curl_easy_init();
}

HTTPRequesterFile::~HTTPRequesterFile()
{
  curl_easy_cleanup(mCURL);
  curl_global_cleanup();
}

void HTTPRequesterFile::run()
{
  emit resultReady(this->download());
}

bool HTTPRequesterFile::download()
{
  // Basic fetching setup
  curl_easy_setopt(mCURL, CURLOPT_URL, this->mURL.c_str());
  curl_easy_setopt(mCURL, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(mCURL, CURLOPT_WRITEFUNCTION, write_file);
  // Handle HTTP redirection
  curl_easy_setopt(mCURL, CURLOPT_FOLLOWLOCATION, 1L);

  // Output setup
  auto lPath = QString("%1/wizard.exe").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
  FILE* lFileStream = fopen(lPath.toStdString().c_str(), "wb+");
  if (lFileStream)
  {
    curl_easy_setopt(mCURL, CURLOPT_WRITEDATA, lFileStream);

    // Execute the HTTP request
    CURLcode lRes{curl_easy_perform(mCURL)};

    // Close the file stream
    fclose(lFileStream);

    return true;
  }

  return false;
}

#include "HTTPRequesterFile.h"

// UTIL function
static size_t write_file(void* ptr, size_t size, size_t nmemb, void* stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
  return written;
}

HTTPRequesterFile::HTTPRequesterFile(const QString& aURL, const QString& aSavedFilePath, QWidget* aParent)
  : QThread(aParent)
  , mURL(aURL.toStdString())
  , mSavedFilePath(aSavedFilePath.toStdString())
{
  curl_global_init(CURL_GLOBAL_ALL);
  this->mCURL = curl_easy_init();
}

HTTPRequesterFile::~HTTPRequesterFile()
{
  curl_easy_cleanup(this->mCURL);
  curl_global_cleanup();
}

void HTTPRequesterFile::run()
{
  emit resultReady(this->download());
}

bool HTTPRequesterFile::download()
{
  // Basic fetching setup
  curl_easy_setopt(this->mCURL, CURLOPT_URL, this->mURL.c_str());
  curl_easy_setopt(this->mCURL, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(this->mCURL, CURLOPT_WRITEFUNCTION, write_file);
  // Handle HTTP redirection
  curl_easy_setopt(this->mCURL, CURLOPT_FOLLOWLOCATION, 1L);

  // Output setup
  FILE* lFileStream;
  fopen_s(&lFileStream, this->mSavedFilePath.c_str(), "wb+");

  if (lFileStream)
  {
    curl_easy_setopt(this->mCURL, CURLOPT_WRITEDATA, lFileStream);

    // Perform the request and get the return code
    CURLcode lRes{curl_easy_perform(this->mCURL)};

    // Close the file stream
    fclose(lFileStream);

    if (lRes != CURLE_OK)
    {
      //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(lRes));
      return false;
    }

    return true;
  }

  return false;
}

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iomanip>
#include <iostream>
#include <jukeb0x/music/music.h>
#include <sstream>

namespace music {

std::string GetYoutubeID(Track track) {
  int durationInSeconds = track.duration;
  std::string searchQuery = "'" + track.title + "' " + track.artist;

  std::vector<SearchResults> results = ytSearch(searchQuery, 10);

  if (results.size() == 0) {
    std::cerr << "No songs found for " << searchQuery << std::endl;
    return "";
  }
}

// TODO: scrape the response body to extract info for the SearchResults structs
std::vector<SearchResults> ytSearch(const std::string &query, int limit) {
  std::vector<SearchResults> results;
  std::string ytQuery = makeYouTubeSearchQuery(query);

  httplib::Client cli("https://www.youtube.com");

  httplib::Headers headers = {{"Accept-Language", "en"}};

  auto res = cli.Get(ytQuery, headers);

  if (!res) {
    std::cerr << "HTTP request failed: No response" << std::endl;
    return results;
  }

  if (res->status != httplib::StatusCode::OK_200) {
    std::cerr << "Failed to make request to youtube: " << res.error()
              << std::endl;
    return results;
  }

  std::string body = res->body;

  std::cout << body << std::endl;

  // std::cout << ytSearchUrl << std::endl;
}

std::string makeYouTubeSearchQuery(const std::string &query) {
  return "/results?search_query=" + urlEncode(query);
}

std::string urlEncode(const std::string &value) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (char c : value) {
    if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' ||
        c == '.' || c == '~') {
      escaped << c;
    } else if (c == ' ') {
      escaped << '+';
    } else {
      unsigned int code = static_cast<unsigned char>(c);
      escaped << '%' << std::setw(2) << std::uppercase << code;
    }
  }
  return escaped.str();
}
} // namespace music

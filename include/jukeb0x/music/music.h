#pragma once

#include <string>
#include <vector>

namespace music {

struct Track {
  std::string title;
  std::string artist;
  std::vector<std::string> artists;
  std::string album;
  int duration = 0;
};

struct SearchResults {
  std::string id;
  std::string title;
  std::string url;
  std::string uploader;
  std::string source = "youtube";
  bool live;
};

std::string GetYoutubeID(Track track);
std::vector<SearchResults> ytSearch(const std::string &query, int limit);
std::string makeYouTubeSearchQuery(const std::string &query);
std::string urlEncode(const std::string &value);

} // namespace music

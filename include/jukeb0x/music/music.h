#pragma once

#include <string>
#include <vector>

namespace music {

struct Track {
  std::string title;
  std::string artist;
  std::vector<std::string> artists;
  std::string album;
  int duration;
};

std::string GetYoutubeID(Track track);
} // namespace music

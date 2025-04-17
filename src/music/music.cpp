#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iomanip>
#include <iostream>
#include <jukeb0x/music/music.h>
#include <nlohmann/json.hpp>
#include <sstream>
using json = nlohmann::json;

namespace music {

int ConvertStringDurationToSeconds(std::string durationStr) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(durationStr);

  while (std::getline(tokenStream, token, ':')) {
    tokens.push_back(token);
  }

  if (tokens.size() == 1) {
    try {
      int seconds = std::stoi(tokens[0]);
      return seconds;
    } catch (std::exception &e) {
      std::cerr << "Error converting to seconds: " << e.what() << std::endl;
    }
  } else if (tokens.size() == 2) {
    int minutes = std::stoi(tokens[0]);
    int seconds = std::stoi(tokens[1]);
    return (minutes * 60) + seconds;
  } else if (tokens.size() == 3) {
    int hours = std::stoi(tokens[0]);
    int minutes = std::stoi(tokens[1]);
    int seconds = std::stoi(tokens[2]);
    return ((hours * 60) * 60) + (minutes * 60) + seconds;
  }

  return 0;
}

std::string GetYoutubeID(Track track) {
  int durationMatchThreshold = 5;
  int durationInSeconds = track.duration;
  std::string searchQuery = "'" + track.title + "' " + track.artist;

  std::vector<SearchResults> results = ytSearch(searchQuery, 10);

  if (results.size() == 0) {
    std::cerr << "No songs found for " << searchQuery << std::endl;
    return "";
  }

  for (const auto &res : results) {
    int allowedRangeStart = durationInSeconds - durationMatchThreshold;
    int allowedRangeEnd = durationInSeconds + durationMatchThreshold;
    int resultSongDuration = ConvertStringDurationToSeconds(res.duration);

    if (resultSongDuration >= allowedRangeStart &&
        resultSongDuration <= allowedRangeEnd) {
      return res.id;
    }
  }

  std::cerr << "Could not find a song for search result: " << searchQuery
            << std::endl;
  return "";
}

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

  std::string startMarker = "var ytInitialData = ";
  std::string endMarker = ";</script>";

  size_t startPos = body.find(startMarker);
  if (startPos == std::string::npos) {
    std::cerr << "Could not find start marker 'var ytInitialData = '"
              << std::endl;
    startMarker = "window[\"ytInitialData\"] = ";
    startPos = body.find(startMarker);
    if (startPos == std::string::npos) {
      std::cerr << "Could not find alternative start marker either."
                << std::endl;
      return results;
    }
  }

  startPos += startMarker.length();

  size_t endPos = body.find(endMarker, startPos);
  if (endPos == std::string::npos) {
    std::cerr << "Could not find end marker ';</script>'" << std::endl;
    // Try finding just the next </script> and backtrack for the semicolon
    endPos = body.find("</script>", startPos);
    if (endPos != std::string::npos) {
      size_t lastSemicolon = body.rfind(';', endPos);
      if (lastSemicolon != std::string::npos && lastSemicolon > startPos) {
        endPos = lastSemicolon;
      } else {
        std::cerr << "Could not find reliable end point for JSON." << std::endl;
        return results;
      }
    } else {
      std::cerr << "Could not find any script end tag after start marker."
                << std::endl;
      return results;
    }
  }

  if (startPos >= endPos) {
    std::cerr << "Invalid start/end positions for JSON data." << std::endl;
    return results;
  }

  std::string jsonDataStr = body.substr(startPos, endPos - startPos);

  ParseJson(jsonDataStr, results, limit);

  return results;
}

void ParseJson(const std::string &jsonDataStr,
               std::vector<SearchResults> &results, int limit) {
  // --- Parse the JSON ---
  json jsonData;
  try {
    jsonData = json::parse(jsonDataStr);
  } catch (json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
  }

  // --- Navigate JSON and Extract Data ---
  try {
    if (!jsonData.contains("contents") ||
        !jsonData["contents"].contains("twoColumnSearchResultsRenderer") ||
        !jsonData["contents"]["twoColumnSearchResultsRenderer"].contains(
            "primaryContents") ||
        !jsonData["contents"]["twoColumnSearchResultsRenderer"]
                 ["primaryContents"]
                     .contains("sectionListRenderer") ||
        !jsonData["contents"]["twoColumnSearchResultsRenderer"]
                 ["primaryContents"]["sectionListRenderer"]
                     .contains("contents")) {
      std::cerr << "Error: Unexpected JSON structure for primary contents."
                << std::endl;
    }

    const auto &primaryContents =
        jsonData["contents"]["twoColumnSearchResultsRenderer"]
                ["primaryContents"]["sectionListRenderer"]["contents"];

    int count = 0;
    for (const auto &section : primaryContents) {
      if (count >= limit)
        break;

      if (!section.contains("itemSectionRenderer") ||
          !section["itemSectionRenderer"].contains("contents")) {
        continue; // Skip sections without the expected structure
      }

      const auto &items = section["itemSectionRenderer"]["contents"];
      for (const auto &item : items) {
        if (count >= limit)
          break;

        // Check if this item is a standard video result
        if (item.contains("videoRenderer")) {
          const auto &video = item["videoRenderer"];
          SearchResults sr;

          if (video.contains("videoId") && video["videoId"].is_string()) {
            sr.id = video["videoId"].get<std::string>();
            sr.url = "https://www.youtube.com/watch?v=" + sr.id;

          } else
            continue; // Skip if no videoId

          if (video.contains("title") && video["title"].contains("runs") &&
              video["title"]["runs"].is_array() &&
              !video["title"]["runs"].empty()) {
            sr.title = video["title"]["runs"][0]["text"].get<std::string>();
          }

          if (video.contains("ownerText") &&
              video["ownerText"].contains("runs") &&
              video["ownerText"]["runs"].is_array() &&
              !video["ownerText"]["runs"].empty()) {
            sr.uploader =
                video["ownerText"]["runs"][0]["text"].get<std::string>();
          }

          if (video.contains("lengthText") &&
              video["lengthText"].contains("simpleText") &&
              !video["lengthText"]["simpleText"].empty()) {
            sr.duration = video["lengthText"]["simpleText"].get<std::string>();
          }

          sr.live = false;
          if (video.contains("badges") && video["badges"].is_array()) {
            for (const auto &badge : video["badges"]) {
              if (badge.contains("metadataBadgeRenderer") &&
                  badge["metadataBadgeRenderer"].contains("style") &&
                  badge["metadataBadgeRenderer"]["style"] ==
                      "BADGE_STYLE_TYPE_LIVE_NOW") {
                sr.live = true;
                break;
              }
            }
          }
          // Alternative check in overlays (sometimes used for live)
          if (!sr.live && video.contains("thumbnailOverlays") &&
              video["thumbnailOverlays"].is_array()) {
            for (const auto &overlay : video["thumbnailOverlays"]) {
              if (overlay.contains("thumbnailOverlayTimeStatusRenderer") &&
                  overlay["thumbnailOverlayTimeStatusRenderer"].contains(
                      "style") &&
                  overlay["thumbnailOverlayTimeStatusRenderer"]["style"] ==
                      "LIVE") {
                sr.live = true;
                break;
              }
            }
          }

          results.push_back(sr);
          count++;
        }
      }
    }

  } catch (json::exception &e) {
    std::cerr << "JSON navigation/access error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Standard exception during JSON processing: " << e.what()
              << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception during JSON processing." << std::endl;
  }
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

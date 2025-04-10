#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace wav {

using TagsMap = std::map<std::string, std::string>;

struct Disposition {
  std::optional<int> default_flag;
  std::optional<int> dub;
  std::optional<int> original;
  std::optional<int> comment;
  std::optional<int> lyrics;
  std::optional<int> karaoke;
  std::optional<int> forced;
  std::optional<int> hearing_impaired;
  std::optional<int> visual_impaired;
  std::optional<int> clean_effects;
  std::optional<int> attached_pic;
  std::optional<int> timed_thumbnails;
  std::optional<int> non_diegetic;
  std::optional<int> captions;
  std::optional<int> descriptions;
  std::optional<int> metadata;
  std::optional<int> dependent;
  std::optional<int> still_image;
  std::optional<int> multilayer; //
};

struct StreamInfo {
  int index = -1;
  std::string codec_name;
  std::string codec_long_name;
  std::string codec_type;
  std::optional<std::string> sample_fmt;
  std::optional<std::string> sample_rate;
  std::optional<int> channels;
  std::optional<std::string> channel_layout;
  std::optional<int> bits_per_sample;
  std::string duration;
  std::optional<std::string> bit_rate;
  Disposition disposition;
  TagsMap tags;
};

struct FormatInfo {
  std::string filename;
  int nb_streams = 0;
  std::string format_name;
  std::string format_long_name;
  std::string start_time;
  std::string duration;
  std::string size;
  std::string bit_rate;
  TagsMap tags;
};

struct MediaProbeInfo {
  std::vector<StreamInfo> streams;
  FormatInfo format;
};

MediaProbeInfo GetMetadata(const std::string &filePath);
} // namespace wav

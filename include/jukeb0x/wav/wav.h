#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace wav {

using TagsMap = std::map<std::string, std::string>;

struct Disposition {
  int default_flag;
  int dub;
  int original;
  int comment;
  int lyrics;
  int karaoke;
  int forced;
  int hearing_impaired;
  int visual_impaired;
  int clean_effects;
  int attached_pic;
  int timed_thumbnails;
  int non_diegetic;
  int captions;
  int descriptions;
  int metadata;
  int dependent;
  int still_image;
  int multilayer;

  Disposition() = default;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
    Disposition, default_flag, dub, original, comment, lyrics, karaoke, forced,
    hearing_impaired, visual_impaired, clean_effects, attached_pic,
    timed_thumbnails, non_diegetic, captions, descriptions, metadata, dependent,
    still_image, multilayer)

struct StreamInfo {
  int index = -1;
  std::string codec_name;
  std::string codec_long_name;
  std::string codec_type;
  std::string sample_fmt;
  std::string sample_rate;
  int channels;
  std::string channel_layout;
  int bits_per_sample;
  std::string duration;
  std::string bit_rate;
  Disposition disposition;
  TagsMap tags;

  StreamInfo() = default;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(StreamInfo, index, codec_name,
                                                codec_long_name, codec_type,
                                                sample_fmt, sample_rate,
                                                channels, channel_layout,
                                                bits_per_sample, duration,
                                                bit_rate, disposition, tags)

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

  FormatInfo() = default;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FormatInfo, filename,
                                                nb_streams, format_name,
                                                format_long_name, start_time,
                                                duration, size, bit_rate, tags)

struct MediaProbeInfo {
  std::vector<StreamInfo> streams;
  FormatInfo format;

  MediaProbeInfo() = default;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MediaProbeInfo, streams, format)

MediaProbeInfo GetMetadata(const std::string &filePath);
} // namespace wav

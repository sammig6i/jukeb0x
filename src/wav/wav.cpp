#include <jukeb0x/utils/utils.h>
#include <jukeb0x/wav/wav.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace wav {

// TODO: error handling for json inputs since these will be provided by users
MediaProbeInfo GetMetadata(const std::string &filePath) {
  MediaProbeInfo metadata;

  std::string cmd =
      "ffprobe -v quiet -print_format json -show_format -show_streams " +
      filePath;

  std::string ffprobe = utils::exec(cmd.c_str());
  json j = json::parse(ffprobe);
  metadata = j.template get<wav::MediaProbeInfo>();

  return metadata;
}

} // namespace wav

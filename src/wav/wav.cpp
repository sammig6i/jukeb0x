#include <jukeb0x/wav/wav.h>

namespace wav {

MediaProbeInfo GetMetadata(const std::string &filePath) {
  MediaProbeInfo metadata;
  /*
   * TODO:
   * run ffprobe command to get the metadata of the file
   * map the metadata to the MediaProbeInfo data struct
   * change all keys in the tages for FormatInfo and StreamInfo[0] to lowercase
   * return the metadata
   */

  return metadata;
}

} // namespace wav

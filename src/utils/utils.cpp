#include <array>
#include <filesystem>
#include <iostream>
#include <jukeb0x/music/music.h>
#include <jukeb0x/utils/utils.h>
#include <jukeb0x/wav/wav.h>
#include <string>
namespace fs = std::filesystem;

namespace utils {

// execute shell commands
std::string exec(const char *cmd) {
  std::string result = "";
  std::array<char, 256> buffer;

  FILE *pipe = popen(cmd, "r");
  if (!pipe) {
    std::runtime_error("popen() failed.");
  }

  while (std::fgets(buffer.data(), buffer.size(), pipe) != NULL) {
    result += buffer.data();
  }

  int exit_status = pclose(pipe);
  if (exit_status != 0) {
    std::cerr << "Command failed with exit code: " << exit_status << std::endl;
  }

  return result;
}

void save(const std::string &filePath) {
  std::error_code ec;

  fs::path file = fs::absolute(filePath, ec);
  if (ec) {
    std::cerr << "Error getting absolute path of file: " << ec.message()
              << std::endl;
    return;
  }

  if (fs::is_directory(file, ec)) {
    if (ec) {
      std::cerr << "Error checking if path is a directory: " << ec.message()
                << std::endl;
      return;
    }

    std::cout << "Savings songs from the directory: " << file << std::endl;

    for (const auto &entry : fs::directory_iterator(file)) {
      if (entry.is_regular_file(ec)) {
        std::cout << "\nFile in directory " << file << ": " << entry.path()
                  << std::endl;
        saveSong(entry.path());
      }

      if (ec) {
        std::cout << "Error saving song " << entry.path() << ": "
                  << ec.message() << std::endl;
      }
    }
  } else if (fs::is_regular_file(file, ec)) {
    if (ec) {
      std::cerr << "Error checking if path is a regular file: " << ec.message()
                << std::endl;
      return;
    }

    std::cout << "file is a regular file: " << file << std::endl;
    saveSong(file);
  } else {
    std::cout << "File path is neither a regular file nor a directory: " << file
              << std::endl;
  }
}

void saveSong(const std::string &filePath) {
  /*
   * TODO: handle converting, saving and processing the song in .wav format
   * 1. get metadata from file using ffprobe
   * 2. Save metadata for Title, Artist and Album to a track data type
   * 3. get the Youtube ID of the audio file
   * process the file by creating the mel spectrogram operations then store the
   * spectrogram to the database
   * move the new wav file to the processed songs directory
   */

  std::string inputFilePath = "'" + filePath + "'";
  wav::MediaProbeInfo metadata = wav::GetMetadata(inputFilePath);
  float duration = 0.0f;
  try {
    duration = std::stof(metadata.format.duration);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  const auto &tags = metadata.format.tags;
  auto title = tags.find("title");
  auto artist = tags.find("artist");
  auto album = tags.find("album");

  music::Track track = {(title != tags.end() ? title->second : ""),
                        (artist != tags.end() ? artist->second : ""),
                        {},
                        (album != tags.end() ? album->second : ""),
                        static_cast<int>(duration)};

  std::cout << "Title: " << track.title << std::endl;
  std::cout << "Artist: " << track.artist << std::endl;
  std::cout << "Album: " << track.album << std::endl;
  std::cout << "Duration: " << track.duration << std::endl;

  std::string ytID = music::GetYoutubeID(track);
  std::cout << "Youtube ID: " << ytID << std::endl;
}

} // namespace utils

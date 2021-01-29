// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "Utils.hpp"

#include <array>
#include <string_view>

#include <fmt/core.h>

namespace asmc {

std::string readNextLineFromGzip(gzFile& gzFileHandle) {

  std::array<char, 512> buffer = {};
  std::string line;

  char* successful_read = nullptr;
  do {
    successful_read = gzgets(gzFileHandle, buffer.data(), static_cast<int>(buffer.size()));

    if (successful_read != Z_NULL) {
      line += buffer.data();
    }

  } while (!line.empty() && line.back() != '\n' && !gzeof(gzFileHandle));

  if (!line.empty() && line.back() == '\n') {
    line.pop_back();
  }

  return line;
}

/**
 * Look for a file that might have one of several possible extensions. Given a partial path to a file, for instance,
 * /some/path/to/a/file, and a list of possible extensions {.ext1, .ext2, .ext3}, check whether the full paths
 *   /some/path/to/a/file.ext1
 *   /some/path/to/a/file.ext2
 *   /some/path/to/a/file.ext3
 * exist or not.
 *
 * The first such file that exists is returned as a filesystem path object. If no such file exists, a
 * default-constructed path object is returned, which will return "false" to an existence check.
 *
 * @param baseName the path to the file, minus the extension
 * @param possibleExts a vector of possible file extensions
 * @return the first file "baseName + ext" that exists, or a default-constructed filesystem path object, if none exist
 */
fs::path getFileInDirWithExt(std::string_view baseName, const std::vector<std::string>& possibleExts) {

  auto filePath = fs::path{};

  for(const auto& ext : possibleExts) {
    if(auto fileName = fmt::format("{}{}", baseName, ext); fs::exists(fileName)) {
      filePath = fs::path{fileName};
      break;
    }
  }

  return filePath;
}

} // namespace asmc

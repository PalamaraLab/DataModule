// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_UTILS_HPP
#define DATA_MODULE_UTILS_HPP

#include <algorithm>
#include <filesystem>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include <zlib.h>

namespace asmc {

namespace fs = std::filesystem;

/**
 * Read the next line from a gzip file.
 *
 * @param gzFileHandle handle to a file opened with zlib's gzopen
 * @return a string containing the next line contained in the gzip file, without a trailing newline character
 */
std::string readNextLineFromGzip(gzFile& gzFileHandle);

/**
 * Split a string of text into a vector of strings, splitting by a given delimiter.
 *
 * @param text the text to split
 * @param del the delimiter to split by
 * @return a vector of substrings of text, split by del
 */
std::vector<std::string> splitTextByDelimiter(std::string_view text, std::string_view del);

} // namespace asmc

#endif // DATA_MODULE_UTILS_HPP

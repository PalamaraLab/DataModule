// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_UTILS_HPP
#define DATA_MODULE_UTILS_HPP

#include <algorithm>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include <zlib.h>

namespace asmc {

/**
 * Read the next line from a gzip file.
 *
 * @param gzFileHandle handle to a file opened with zlib's gzopen
 * @return a string contating the next line contained in the gzip file, without a trailing newline character
 */
std::string readNextLineFromGzip(gzFile& gzFileHandle);

} // namespace asmc

#endif // DATA_MODULE_UTILS_HPP

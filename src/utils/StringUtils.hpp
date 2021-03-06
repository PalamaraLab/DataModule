// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_STRING_UTILS_HPP
#define DATA_MODULE_STRING_UTILS_HPP

#include <string>
#include <string_view>
#include <vector>

namespace asmc {

/**
 * Split a string of text into a vector of strings, splitting by a given delimiter.
 *
 * @param text the text to split
 * @param del the delimiter to split by
 * @return a vector of substrings of text, split by del
 */
std::vector<std::string> splitTextByDelimiter(std::string_view text, std::string_view del);

/**
 * Remove whitespace characters '\n', ' ', '\t', '\r' from the end of a string
 *
 * @param s the string to strip whitespace from the back of
 * @return the string with whitespace stripped from the back
 */
std::string stripBack(std::string s);

} // namespace asmc

#endif // DATA_MODULE_STRING_UTILS_HPP

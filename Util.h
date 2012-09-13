/**
 * Contains a number of utility functions.
 */
#ifndef DOTWRITER_UTIL_H_
#define DOTWRITER_UTIL_H_

#include <string>

namespace DotWriter {

/**
 * Taken from:
 * http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string
 */
void ReplaceAll(std::string& str, const std::string& from,
  const std::string& to);

/**
 * Takes a string and escapes necessary characters for output to a
 * DOT file.
 * 
 * Currently, it just escapes quotation marks with a forward slash.
 *
 * This is used to sanitize IDs and labels during DOT file writing.
 */
std::string SanitizeString(std::string& label);

}  // namespace DotWriter

#endif

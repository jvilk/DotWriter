#include "Util.h"

namespace DotWriter {

/**
 * Taken from:
 * http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string
 */
void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty())
        return;

    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

/**
 * Takes a string and escapes necessary characters for output to a
 * DOT file.
 *
 * Currently, it just escapes quotation marks with a forward slash,
 * and changes newlines to \n.
 */
std::string SanitizeString(std::string& label) {
    // escape a double quote
    ReplaceAll(label, "\"", "\\\"");
    // newline -> \n
    ReplaceAll(label, "\n", "\\n");
    return label;
}

}  // namespace DotWriter

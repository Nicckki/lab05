#include "formatter_ex.h"
#include "../formatter_lib/formatter.h"

std::string formatter_ex(const std::string& s) {
    return formatter(s) + " [ex]";
}

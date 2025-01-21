#include <limits>
#include "rapidjson/document.h"

int db_string_to_vector(const char* p, int str_len, float* vector, int* p_count) {
    if (!p || !vector || !p_count || str_len <= 0) {
        return -1; // Error
    }

    rapidjson::Document doc;
    rapidjson::ParseResult result = doc.Parse(p, static_cast<size_t>(str_len));
    if (!result || !doc.IsArray()) {
        return -1;
    }

    size_t size = doc.Size();
    if (size > static_cast<size_t>(std::numeric_limits<int>::max()) || size > 2000) {
        return -1;
    }

    for (size_t i = 0; i < size; i++) {
        if (!doc[i].IsNumber()) {
            return -1;
        }
        vector[i] = static_cast<float>(doc[i].GetDouble());
    }

    *p_count = static_cast<int>(size);
    return 0; // Success
}

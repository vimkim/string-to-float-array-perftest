#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <limits>

namespace {
    constexpr size_t MAX_VECTOR_SIZE = 2000;
    constexpr size_t NUMBER_BUFFER_SIZE = 64;
    constexpr size_t INITIAL_VECTOR_CAPACITY = 128;
    constexpr std::string_view WHITESPACE = " \t\n\r";

    bool should_continue_parsing(size_t pos, size_t current_size, const std::string_view& input) {
        return pos < input.size() && current_size < MAX_VECTOR_SIZE;
    }

    bool has_valid_ending(size_t pos, const std::string_view& input, const std::vector<float>& result) {
        if (pos >= input.size() || input[pos] != ']') {
            return false;
        }
        size_t end = input.find_first_not_of(WHITESPACE, pos + 1);
        return end == std::string_view::npos && !result.empty();
    }

    size_t extract_number(const std::string_view& input, size_t pos, std::string& number_buffer) {
        number_buffer.clear();
        size_t number_end = pos;

        while (number_end < input.size() &&
               input[number_end] != ',' &&
               input[number_end] != ']' &&
               number_buffer.size() < NUMBER_BUFFER_SIZE - 1) {
            if (!std::isspace(input[number_end])) {
                number_buffer.push_back(input[number_end]);
            }
            ++number_end;
        }

        return (number_buffer.empty() || number_buffer.size() >= NUMBER_BUFFER_SIZE - 1)
               ? std::string_view::npos
               : number_end;
    }

    bool parse_and_add_number(const std::string& number_str, std::vector<float>& result) {
        try {
            result.push_back(std::stof(number_str));
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

    size_t validate_delimiter(const std::string_view& input, size_t pos) {
        pos = input.find_first_not_of(WHITESPACE, pos);
        if (pos == std::string_view::npos) {
            return std::string_view::npos;
        }

        if (input[pos] == ']') {
            return pos;
        }
        if (input[pos] != ',') {
            return std::string_view::npos;
        }
        return pos + 1;
    }
}

std::optional<std::vector<float>> db_string_to_vector(std::string_view input) {
    size_t pos = input.find_first_not_of(WHITESPACE);
    if (pos == std::string_view::npos || input[pos] != '[') {
        return std::nullopt;
    }

    std::vector<float> result;
    result.reserve(INITIAL_VECTOR_CAPACITY);

    std::string number_buffer;
    number_buffer.reserve(NUMBER_BUFFER_SIZE);

    pos = pos + 1;
    while (should_continue_parsing(pos, result.size(), input)) {
        pos = input.find_first_not_of(WHITESPACE, pos);
        if (pos == std::string_view::npos) {
            return std::nullopt;
        }
        if (input[pos] == ']') {
            break;
        }

        size_t number_end = extract_number(input, pos, number_buffer);
        if (number_end == std::string_view::npos) {
            return std::nullopt;
        }

        if (!parse_and_add_number(number_buffer, result)) {
            return std::nullopt;
        }

        pos = validate_delimiter(input, number_end);
        if (pos == std::string_view::npos) {
            return std::nullopt;
        }
        if (input[pos - 1] == ']') {
            break;
        }
    }

    return has_valid_ending(pos, input, result)
           ? std::make_optional(std::move(result))
           : std::nullopt;
}

int db_string_to_vector(const char* p, int str_len, float* vector, int* p_count) {
    if (!p || !vector || !p_count || str_len <= 0) {
        return -1; // Error
    }

    auto result = db_string_to_vector(std::string_view(p, static_cast<size_t>(str_len)));
    if (!result || result->size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return -1;
    }

    std::copy(result->begin(), result->end(), vector);
    *p_count = static_cast<int>(result->size());
    return 0; // Success
}

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <iomanip>

// Function declaration
int db_string_to_vector(const char* p, int str_len, float* vector, int* p_count);

// Test data generation
std::string generate_test_string(int num_floats) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << "[";
    for (int i = 0; i < num_floats; ++i) {
        if (i > 0) ss << ",";
        ss << dis(gen);
    }
    ss << "]";
    return ss.str();
}

struct TestResult {
    double total_time_ms;
    double avg_time_ms;
    double min_time_ms;
    double max_time_ms;
    int successful_parses;
    int failed_parses;
    std::vector<double> individual_times_ms;
};

TestResult run_test(const std::vector<std::string>& test_strings) {
    TestResult result = {0.0, 0.0, std::numeric_limits<double>::max(), 0.0, 0, 0};
    result.individual_times_ms.reserve(test_strings.size());
    
    std::vector<float> output_vector(2000);
    int count;
    
    for (const auto& input : test_strings) {
        auto start = std::chrono::high_resolution_clock::now();
        
        int ret = db_string_to_vector(
            input.c_str(),
            static_cast<int>(input.length()),
            output_vector.data(),
            &count
        );
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_ms = duration.count() / 1000.0;
        
        result.individual_times_ms.push_back(time_ms);
        result.total_time_ms += time_ms;
        result.min_time_ms = std::min(result.min_time_ms, time_ms);
        result.max_time_ms = std::max(result.max_time_ms, time_ms);
        
        if (ret == 0) {
            result.successful_parses++;
        } else {
            result.failed_parses++;
        }
    }
    
    result.avg_time_ms = result.total_time_ms / test_strings.size();
    return result;
}

void print_results(const TestResult& result) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Performance Results:" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << "Total time: " << result.total_time_ms << " ms" << std::endl;
    std::cout << "Average time per string: " << result.avg_time_ms << " ms" << std::endl;
    std::cout << "Min time: " << result.min_time_ms << " ms" << std::endl;
    std::cout << "Max time: " << result.max_time_ms << " ms" << std::endl;
    std::cout << "Successful parses: " << result.successful_parses << std::endl;
    std::cout << "Failed parses: " << result.failed_parses << std::endl;
    std::cout << "Throughput: " 
              << (1000.0 * result.successful_parses / result.total_time_ms) 
              << " strings/second" << std::endl;
    
    // Calculate percentiles
    auto times = result.individual_times_ms;
    std::sort(times.begin(), times.end());
    size_t size = times.size();
    
    std::cout << "\nPercentiles:" << std::endl;
    std::cout << "50th: " << times[size * 50 / 100] << " ms" << std::endl;
    std::cout << "90th: " << times[size * 90 / 100] << " ms" << std::endl;
    std::cout << "95th: " << times[size * 95 / 100] << " ms" << std::endl;
    std::cout << "99th: " << times[size * 99 / 100] << " ms" << std::endl;
}

int main() {
    const int NUM_STRINGS = 1000;
    const int FLOATS_PER_STRING = 2000;
    
    std::cout << "Generating " << NUM_STRINGS << " test strings with "
              << FLOATS_PER_STRING << " floats each..." << std::endl;
    
    std::vector<std::string> test_strings;
    test_strings.reserve(NUM_STRINGS);
    
    for (int i = 0; i < NUM_STRINGS; ++i) {
        test_strings.push_back(generate_test_string(FLOATS_PER_STRING));
    }
    
    std::cout << "\nRunning performance test..." << std::endl;
    auto result = run_test(test_strings);
    print_results(result);
    
    return 0;
}

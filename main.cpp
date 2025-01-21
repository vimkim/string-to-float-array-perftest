#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <iomanip>

// Function declarations for both implementations
int db_string_to_vector(const char* p, int str_len, float* vector, int* p_count);

// Test data generation
std::string generate_test_string(int num_floats) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << "[";
    for (int i = 0; i < num_floats; ++i) {
        if (i > 0) ss << ",";
        ss << dis(gen);
    }
    ss << "]";
    return ss.str();
}

// Performance measurement structure
struct PerfResult {
    double total_time_ms;
    double avg_time_ms;
    int successful_parses;
    int failed_parses;
};

// Test runner
PerfResult run_test(const std::vector<std::string>& test_data) {
    PerfResult result = {0.0, 0.0, 0, 0};
    std::vector<float> output_vector(2000);
    int count = 0;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (const auto& input : test_data) {
        int ret = db_string_to_vector(
            input.c_str(),
            static_cast<int>(input.length()),
            output_vector.data(),
            &count
        );
        
        if (ret == 0 /* NO_ERROR */) {
            result.successful_parses++;
        } else {
            result.failed_parses++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time
    );
    
    result.total_time_ms = duration.count() / 1000.0;
    result.avg_time_ms = result.total_time_ms / test_data.size();
    
    return result;
}

int main() {
    const int NUM_STRINGS = 1000;
    const int FLOATS_PER_STRING = 2000;
    
    // Generate test data
    std::cout << "Generating " << NUM_STRINGS << " test strings with "
              << FLOATS_PER_STRING << " floats each..." << std::endl;
    
    std::vector<std::string> test_data;
    test_data.reserve(NUM_STRINGS);
    
    for (int i = 0; i < NUM_STRINGS; ++i) {
        test_data.push_back(generate_test_string(FLOATS_PER_STRING));
    }
    
    // Run tests
    std::cout << "\nRunning performance test...\n" << std::endl;
    
    PerfResult result = run_test(test_data);
    
    // Report results
    std::cout << "Performance Results:" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Total time: " << result.total_time_ms << " ms" << std::endl;
    std::cout << "Average time per string: " << result.avg_time_ms << " ms" << std::endl;
    std::cout << "Successful parses: " << result.successful_parses << std::endl;
    std::cout << "Failed parses: " << result.failed_parses << std::endl;
    std::cout << "Throughput: " 
              << (1000.0 * result.successful_parses / result.total_time_ms) 
              << " strings/second" << std::endl;
    
    return 0;
}

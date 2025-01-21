# Vector Parser Performance Test

This project compares the performance of two different implementations for parsing string-formatted float arrays:

1. A custom manual parser using C++ string operations
2. A RapidJSON-based parser

Both implementations handle arrays of floats in the format: `[1.0, 2.0, 3.0, ...]`

## Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- Ninja build system (recommended) or Make

## Project Structure

```txt
vector_parser_test/
├── CMakeLists.txt              # Build configuration
├── src/
│   ├── db_vector.cpp           # Manual parser implementation
│   ├── db_vector_rapidjson.cpp # RapidJSON-based implementation
│   └── perf_test.cpp          # Performance test suite
└── include/                    # Header files directory
```

## Building the Project

```bash
# Create and enter build directory
mkdir build
cd build

# Configure with Ninja (recommended)
cmake -GNinja ..

# Or configure with Make
cmake ..

# Build
cmake --build .
```

## Running the Tests

The project provides several targets for running performance tests:

```bash
# Run manual implementation test
cmake --build . --target run_manual

# Run RapidJSON implementation test
cmake --build . --target run_rapidjson

# Run both tests sequentially
cmake --build . --target run_perf_tests
```

## Performance Metrics

The test suite measures and reports:

- Total execution time
- Average time per string
- Minimum and maximum parse times
- Success/failure counts
- Throughput (strings/second)
- Latency percentiles (50th, 90th, 95th, 99th)

## Test Configuration

Default test parameters:

- Number of test strings: 1,000
- Floats per string: 2,000
- Float range: [-1000.0, 1000.0]
- Float precision: 6 decimal places

## Implementation Details

### Manual Parser (`db_vector.cpp`)

- Custom string parsing using C++ standard library
- No external dependencies
- Configurable maximum vector size (default: 2000)
- Whitespace tolerant
- Memory pre-allocation optimization

### RapidJSON Parser (`db_vector_rapidjson.cpp`)

- Uses RapidJSON library for parsing
- JSON-compliant parsing
- Automatic type conversion
- Same size limits as manual parser

## Results

```
⬢ [Docker]  zsh ❯ cmake --build build --target run_perf_tests
[1/1] cd /home/vimkim/temp/rapidjson_perftest/string-to-float-array-p..._perftest/string-to-float-array-perftest/build --target run_rapidjson
Running both performance tests...
[1/1] cd /home/vimkim/temp/rapidjson_perftest/string-to-float-array-perftest/build && /nix/store/38cffsqqx823crf1i4bcf6zz1qz1hgpd-cmake-3.30.
5/bin/cmake -E echo Running\ manual\ implementation\ test... && /home/vimkim/temp/rapidjson_perftest/string-to-float-array-perftest/build/per
f_test_manual
Running manual implementation test...
Generating 1000 test strings with 2000 floats each...

Running performance test...
Performance Results:
===================
Total time: 1736.679 ms
Average time per string: 1.737 ms
Min time: 1.706 ms
Max time: 1.960 ms
Successful parses: 1000
Failed parses: 0
Throughput: 575.812 strings/second

Percentiles:
50th: 1.725 ms
90th: 1.787 ms
95th: 1.845 ms
99th: 1.865 ms
n
[1/1] cd /home/vimkim/temp/rapidjson_perftest/string-to-float-array-perftest/build && /nix/store/38cffsqqx823crf1i4bcf6zz1qz1hgpd-cmake-3.30.5/bin/cmake -E echo Running\ RapidJSON\ implementation\ test... && /home/vimkim/temp/rapidjson_perftest/string-to-float-array-perftest/build/perf_test_rapidjson
Running RapidJSON implementation test...
Generating 1000 test strings with 2000 floats each...

Running performance test...
Performance Results:
===================
Total time: 1044.767 ms
Average time per string: 1.045 ms
Min time: 1.026 ms
Max time: 1.275 ms
Successful parses: 1000
Failed parses: 0
Throughput: 957.151 strings/second

Percentiles:
50th: 1.039 ms
90th: 1.078 ms
95th: 1.098 ms
99th: 1.108 ms
```

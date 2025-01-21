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

```
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

## Error Handling

Both implementations return:

- 0 (success) when parsing succeeds
- -1 (error) for:
  - Invalid input format
  - Memory allocation failures
  - Size limit violations
  - Number format errors

## Contributing

When contributing, please ensure:

1. Tests pass for both implementations
2. Performance metrics are included in pull requests
3. Code follows existing style and conventions

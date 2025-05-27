# 🧰 cpp-utils

[![DOI](https://zenodo.org/badge/991307682.svg)](https://zenodo.org/badge/latestdoi/991307682)

A lightweight C++ utility module for multithreaded pipeline components and JSON handling.
Includes a flexible `StageBase` class for real-time data processing and tools for loading, validating, and writing structured JSON files using [RapidJSON](https://github.com/Tencent/rapidjson).

## 📑 Citation

If you use this software, please use the GitHub **“Cite this repository”** button at the top(-right) of this page.

## 🔧 Features

### ✅ JSON Tools (`jsontools.h`)
- Load and validate JSON documents with optional schema support
- Save structured JSON objects to disk
- Automatically timestamp JSON metadata
- Scan directories for JSON or config files

### 💻 Cli Tools (`clitools.h`)
- Simple status bar

### 📷 OpenCV Tools (`opencvtools.h`)
- Batch Video Iterator, iterate over a batch of videos frame-by-frame

### 🧵 StageBase Template
A generic, thread-safe, multi-stage processing base class for building C++ pipelines.  
Features:
- Threaded input/output FIFOs
- Customizable `ProcessFunction`
- Hooks for completion callbacks
- Runtime-safe termination and thread joining

> **Note**: The general layout of `StageBase` was developed as part of Cheng Minghao's PhD work.


## Integration

Include this repo as a submodule or CMake interface library in larger projects, such as:

- [flirmulticamera](https://github.com/HenrikTrom/flirmulticamera)
- [detection-inference](https://github.com/HenrikTrom/detection-inference)
- [pose-inference](https://github.com/HenrikTrom/pose-inference)


## Dependencies

- C++17
- OpenCV 4.10.0
- spdlog
- [RapidJSON](https://github.com/Tencent/rapidjson) (included or linked)
- Standard C++ libraries (`<mutex>`, `<thread>`, `<queue>`, `<chrono>`, etc.)

## Example Usage

### Load and validate JSON

```cpp
cpp_utils::Document config;
cpp_utils::load_json_with_schema("config.json", "schema.json", 8192, config);
````

### Use the pipeline base

```cpp
class MyStage : public cpp_utils::StageBase<MyInput, MyOutput> {
    bool ProcessFunction(MyInput& input, MyOutput& output) override {
        // process input -> output
        return true;
    }
};
```

## 👥 Acknowledgments

* JSON loading and schema support based on [RapidJSON](https://github.com/Tencent/rapidjson)
* Multi-threaded stage system was initially designed in **Cheng Minghao's** PhD and adapted for this work.

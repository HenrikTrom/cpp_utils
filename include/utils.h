#include <filesystem>
#include <spdlog/spdlog.h>

namespace cpp_utils{

/**
 * @brief Retrieves a fixed number of filenames from a directory matching a given extension.
 *
 * This function scans the specified directory for regular files that match the provided file extension.
 * It returns the stem (filename without path or extension) of the first `BATCH_SIZE` matching files
 * in lexicographic order (as provided by the filesystem iterator).
 *
 * @tparam BATCH_SIZE Number of filenames to retrieve.
 *
 * @param[in] resources Path to the directory containing the files.
 * @param[in] extension File extension to match (e.g., ".mp4", ".json").
 *
 * @return std::array<std::string, BATCH_SIZE> Array of filename stems (without extension).
 *
 * @throws std::runtime_error If fewer than `BATCH_SIZE` matching files are found in the directory.
 *
 * @note This function logs each found file using spdlog and reports errors via logging as well.
 *
 * @example
 *     auto names = get_filenames<4>("./data", ".mp4");
 *     might return: {"video1", "video2", "video3", "video4"}
 */


template<size_t BATCH_SIZE>
std::array<std::string, BATCH_SIZE> get_filenames(
    const std::string &resources, const std::string &extension
){
    std::array<std::string, BATCH_SIZE> fnames;
    uint8_t count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(resources)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            fnames.at(count) = entry.path().stem().string();
            spdlog::info("Found {}", entry.path().string());
            count++;
        }
        if (count==BATCH_SIZE){
            break;
        }
    }
    if (count < BATCH_SIZE) {
        std::string error_msg = "Not enough files in the directory: "+resources +
            ", only "+std::to_string(count)+" found, should be "+std::to_string(BATCH_SIZE);
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
    }
    return fnames;
}

} // namespace cpp_utils
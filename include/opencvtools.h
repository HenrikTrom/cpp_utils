#pragma once

#include <array>             
#include <string>
#include <stdexcept>     
#include <opencv2/opencv.hpp> 
#include "utils.h"

namespace cpp_utils{

/**
 * @brief A synchronous iterator for reading multiple video files frame-by-frame.
 * 
 * This templated class is designed to open and synchronously iterate over a batch of video files.
 * The iterator allows frame-by-frame access across all videos in the batch simultaneously.
 * 
 * @tparam BATCH_SIZE Number of video files to open and iterate synchronously.
 */
template<size_t BATCH_SIZE>
class SyncVideoIterator
{
public:
    /**
     * @brief Constructs a SyncVideoIterator.
     * 
     * This constructor attempts to open `BATCH_SIZE` video files located in the given resource path,
     * with filenames specified in the `fnames` array.
     * 
     * @param[in] resources Directory path where the video files are stored.
     * @param[in] fnames Array of video file names (without the ".mp4" extension).
     * 
     * @throws std::runtime_error if any file does not exist, fails to open, or if frame counts differ.
     */
    SyncVideoIterator(const std::string &resources, const std::array<std::string, BATCH_SIZE> &fnames) {
        for (uint16_t cidx = 0; cidx < BATCH_SIZE; cidx++) {
            std::string filename = resources + fnames.at(cidx) + ".mp4";
            if (!std::filesystem::exists(filename)) {
                std::string error_msg = "File " + filename + " does not exist";
                spdlog::error(error_msg);
                throw std::runtime_error(error_msg);
            }
            spdlog::info("Opening file: " + filename);
            this->video_readers.at(cidx) = cv::VideoCapture(filename);
        }

        for (uint16_t cidx = 0; cidx < BATCH_SIZE; cidx++) {
            if (!this->video_readers.at(cidx).isOpened()) {
                std::string error_msg = "Unable to open video file: " + fnames.at(cidx);
                spdlog::error(error_msg);
                throw std::runtime_error(error_msg);
            }
        }

        for (uint16_t cidx = 1; cidx < BATCH_SIZE; cidx++) {
            int length1 = this->video_readers.at(cidx - 1).get(cv::CAP_PROP_FRAME_COUNT);
            int length2 = this->video_readers.at(cidx).get(cv::CAP_PROP_FRAME_COUNT);
            if (length1 != length2) {
                std::string error_msg = "Video files have different number of frames: " +
                    fnames.at(cidx - 1) + " has " + std::to_string(length1) + " frames, while " +
                    fnames.at(cidx) + " has " + std::to_string(length2) + " frames";
                spdlog::error(error_msg);
                throw std::runtime_error(error_msg);
            }
        }

        this->frame_count = this->video_readers.at(0).get(cv::CAP_PROP_FRAME_COUNT);
    }

    ~SyncVideoIterator() = default;

    /**
     * @brief Reads the next frame from all video files.
     * 
     * Stores the next frame from each video into the provided array of `cv::Mat`.
     * If the end of the video is reached, it automatically resets to the beginning.
     * 
     * @param[out] cpuImgs Output array to store the next frame from each video.
     */
    void get_next(std::array<cv::Mat, BATCH_SIZE> &cpuImgs) {
        if (this->frame_idx == this->frame_count) {
            this->reset();
        }

        for (uint16_t cidx = 0; cidx < BATCH_SIZE; cidx++) {
            this->video_readers.at(cidx).read(cpuImgs.at(cidx));
        }

        this->frame_idx++;
    }

    /**
     * @brief Returns the total number of frames in each video.
     * 
     * Assumes all videos have the same number of frames.
     * 
     * @return std::size_t Number of frames in the videos.
     */
    std::size_t get_framecount() {
        return this->frame_count;
    }
    /**
     * @brief Returns width and height of the videos.
     * 
     * Assumes all videos have the same width and height.
     * 
     * @param[out] width width of each video.
     * @param[out] height height of each video.
     * 
     * @return std::size_t Number of frames in the videos.
     */
    void get_wh(int &width, int &height) {
        width = static_cast<int>(this->video_readers.at(0).get(cv::CAP_PROP_FRAME_WIDTH));
        height = static_cast<int>(this->video_readers.at(0).get(cv::CAP_PROP_FRAME_HEIGHT));
    }

    /**
     * @brief Resets all video readers to the first frame.
     * 
     * This function rewinds all video files back to the beginning and resets the internal frame index.
     */
    void reset() {
        spdlog::info("Resetting Video Captures");
        this->frame_idx = 0;
        for (uint16_t cidx = 0; cidx < BATCH_SIZE; cidx++) {
            this->video_readers.at(cidx).set(cv::CAP_PROP_POS_FRAMES, 0);
        }
    }

private:
    std::array<cv::VideoCapture, BATCH_SIZE> video_readers; // OpenCV video readers
    std::size_t frame_count; // Number of frames in each video
    std::size_t frame_idx{0}; // Current frame index
};

} // namespace cpp_utils

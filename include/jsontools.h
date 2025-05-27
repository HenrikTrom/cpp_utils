#pragma once

#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <spdlog/spdlog.h>

using namespace rapidjson;

namespace cpp_utils{

/**
 * @brief Reads json file as rapidjson document
 *
 * @param [in] filepath filepath of json-file
 * @param [in] buffer Buffer for reading the json file
 * @param [out] document Document to return
 * \return true if file exists, false otherwise and prints error.
 */
bool read_json_document(const std::string& filepath, const int& buffer, Document& document);

/**
 * @brief Reads json file as rapidjson document and compares it to schema
 *
 * @param [in] filepath filepath of json-file
 * @param [in] schemapath filepath of the schema.json
 * @param [in] buffer Buffer for reading the json file
 * @param [out] document Document to return
 * \return true if schema could get applied to document, false otherwise
 */
bool load_json_with_schema(
    const std::string& docpath, const std::string& schemapath, 
    const int& buffer, Document& document
);

/**
 * @brief Saves images of frame by idx.jpg at filepath
 *
 * @param [in] filepath Filename and location
 * @param [in] document json to save
 * @param [in] int_buffer buffer for saving (65536)
 * \return true if document could get saved, false otherwise.
 */
bool save_rapidjson(const std::string& filepath, const Document& document, const int& int_buffer);

/**
 * @brief Gets current 'timestamp' as "%Y-%m-%d_%H:%M:%S"
 *
 * \return Timestamp
 */
std::string get_timestamp();

/**
 * @brief Scans directory for files with given extension
 *
 * @param [in] directory Loaction to scan 
 * @param [in] ext Extension to search for
 * @param [out] filepaths Files to be returned
 * \return true if operation was successful, false otherwise
 */
bool scan_for_files(const std::string directory, const std::string ext, std::vector<std::string>& filepaths);

} // namespace cpp_utils

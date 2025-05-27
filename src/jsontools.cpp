#include "jsontools.h"

namespace cpp_utils{

bool read_json_document(const std::string& filepath, const int& buffer, Document& document)
{
    if (!std::filesystem::exists(filepath)){
        std::string error_msg = "File does not exist: "+filepath;
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
        return false;
    }
    FILE* file = fopen(filepath.c_str(), "rb");
    if (!file) {
        std::string error_msg = "Failed to open file: "+filepath;
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
        return false;
    }
    char cbuffer[buffer];
    FileReadStream fileStream(file, cbuffer, sizeof(cbuffer));
    document.ParseStream(fileStream);
    fclose(file);
    // Check parsing
    if (document.HasParseError()) {
        std::string error_msg = "JSON parsing failed. ("+filepath+")"+std::to_string(document.GetParseError());
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
        return false;
    }

    if (!document.IsObject()){
        std::string error_msg = "JSON document is not an object. ("+filepath+")";
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
        return false;
    }
    return true;
}

bool load_json_with_schema(
    const std::string& docpath, const std::string& schemapath, 
    const int& buffer, Document& document
)
{
    Document schema_doc;
    if (!read_json_document(schemapath, buffer, schema_doc)){return false;};
    if (!read_json_document(docpath, buffer, document)){return false;};

    SchemaDocument schema(schema_doc);
    SchemaValidator validator(schema);
    if (!document.Accept(validator)) {
        StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        std::string error_msg = "JSON-SCHEMA-ERROR: "+docpath+" does not match "+schemapath;
        spdlog::error(error_msg);
        spdlog::error("Invalid schema: "+ std::string(sb.GetString()));
        spdlog::error("Invalid keyword: "+ std::string(validator.GetInvalidSchemaKeyword()));
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        spdlog::error("Invalid keyword: "+ std::string(sb.GetString()));
        throw std::runtime_error(error_msg);
        return false;
    }
    return true;
}

bool save_rapidjson(const std::string& filepath, const Document& document, const int& int_buffer)
{
    FILE* fp = fopen(filepath.c_str(), "w");
    
    char writeBuffer[int_buffer];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    
    PrettyWriter<FileWriteStream> writer(os);
    if (!document.Accept(writer)){
        return false;
    };
    
    fclose(fp);
    return true;
}

std::string get_timestamp(){
    // timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S");
    std::string timestamp = oss.str();
    return timestamp;
}

bool scan_for_files(const std::string directory, const std::string ext, std::vector<std::string>& filepaths){
    filepaths.clear();
    for (auto &p : std::filesystem::recursive_directory_iterator(directory))
    {
        if (p.path().extension() == ext)
            filepaths.push_back(p.path().string());
    }
    return true;
}

} // namespace cpp_utils

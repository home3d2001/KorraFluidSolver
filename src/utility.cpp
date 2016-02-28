#include <utility.h>

bool
ParseJson(
	const char* filepath,
    Json::Value& root
    )
{
    // -- Open file
    std::string jsonText;
    std::string line;
    std::ifstream jsonFile(filepath);
    if (jsonFile.is_open() == false) {
        std::cout << "JSON file not open." << std::endl;
        return false;
    }
    while (std::getline(jsonFile, line)) {
        jsonText += line;
    }

    // -- Parse json
    Json::Reader reader;
    bool success = reader.parse(jsonText, root, false);
    if (!success)
    {
        std::cout << "Parsing failed." << std::endl;

        // report to the user the failure and their locations in the document.
        std::cout  << reader.getFormatedErrorMessages() << std::endl;
        return false;
    }

    return true;
}



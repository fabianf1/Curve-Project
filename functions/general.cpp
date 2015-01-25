// Sort of generic functions
#include "functions.h"
//
sf::String int2string(const int &x){
    std::stringstream type;
    type << x;
    return type.str();
}
// Thank https://github.com/LaurentGomila/SFML/wiki/Source:-Load-Image-From-Resource
sf::Image LoadImageFromResource(const std::string& name){
    HRSRC rsrcData = FindResource(NULL, name.c_str(), RT_RCDATA);
    if (!rsrcData)
        throw std::runtime_error("Failed to find resource.");

    DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
    if (rsrcDataSize <= 0)
        throw std::runtime_error("Size of resource is 0.");

    HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
    if (!grsrcData)
        throw std::runtime_error("Failed to load resource.");

    LPVOID firstByte = LockResource(grsrcData);
    if (!firstByte)
        throw std::runtime_error("Failed to lock resource.");

    sf::Image image;
    if (!image.loadFromMemory(firstByte, rsrcDataSize))
        throw std::runtime_error("Failed to load image from memory.");
    return image;
}

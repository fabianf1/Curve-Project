// Contains the implementation of resource loading things
#include "main.h"

// Windows implementation. Uses the .rc to embed resources in exe and then load it using code below
// Implementation based on https://github.com/LaurentGomila/SFML/wiki/Source:-Load-Image-From-Resource
#ifdef _WIN32
    #include <Windows.h>
    // Image Loader
    sf::Image Resource::loadImage(const std::string& name){
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
    // Font Loader
    sf::Font Resource::loadFont(const std::string& name){
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

        sf::Font font;
        if (!font.loadFromMemory(firstByte, rsrcDataSize))
            throw std::runtime_error("Failed to load font from memory.");

        return font;
    }
// Linux implementation
#else
    //TBD
#endif // _WIN32

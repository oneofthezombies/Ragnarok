#pragma once

/*

Decorator Pattern

*/

#include <string>

struct ISound {
    virtual std::string getPath() const = 0;
    virtual bool isDirectory() const = 0;
};

struct Sound : ISound {
    virtual std::string getPath() const { return "resources/sounds/"; }
    virtual bool isDirectory() const { return true; }
};

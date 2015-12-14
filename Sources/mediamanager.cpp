#include "mediamanager.h"

MediaManager::MediaManager()
{

}

bool MediaManager::isOpen()
{
    return _isOpen;
}

std::string MediaManager::path() const
{
    return _path;
}

std::string MediaManager::name() const
{
    return _name;
}

std::string MediaManager::ext() const
{
    return _ext;
}

std::string MediaManager::fullPath() const
{
    return _path + _name + _ext;
}

void MediaManager::setPath(const std::string &path)
{
    _path = path;
}

void MediaManager::setName(const std::string &name)
{
    _name = name;
}

void MediaManager::setExt(const std::string &ext)
{
    _ext = ext;
}


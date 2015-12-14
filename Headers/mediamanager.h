#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <iostream>

class MediaManager
{
public:
    MediaManager();
    virtual void loadFile() = 0;
    virtual void saveFile() = 0;
    virtual bool isOpen();
    virtual std::string path() const;
    virtual std::string name() const;
    virtual std::string ext() const;
    virtual std::string fullPath() const;
    virtual void setPath(const std::string &path);
    virtual void setName(const std::string &name);
    virtual void setExt(const std::string &ext);


protected:
    std::string _path;
    std::string _name;
    std::string _ext;
    bool _isOpen;
};

class FileManager : public MediaManager
{
public:
    FileManager(){_ext = ".txt";}
    virtual void loadFile() = 0;
    virtual void saveFile() = 0;
};

#endif // MEDIAMANAGER_H

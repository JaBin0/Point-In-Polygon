#pragma once
#include <string_view>
#include <iostream>

class SImage {
    int _width; 
    int _height; 
    int _nrChannels;
    unsigned char* _data;
    bool _valid;
public:
    SImage(std::string_view img);
    ~SImage();

    inline int width() const {return _width;}
    inline int height() const {return _height;}
    inline int channels() const {return _nrChannels;}
    inline unsigned char* data() const {return _data;}

    explicit operator bool() const {
        return _valid;
    }
};
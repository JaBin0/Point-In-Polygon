#include "Settings.h"

Settings::Settings() 
{}

Settings* Settings::instance() {
    static Settings* instance = nullptr;
    if(instance == nullptr) {
        instance = new Settings();
    }
    return instance;
}
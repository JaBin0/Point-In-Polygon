#pragma once
#include <inttypes.h>
#include <Graphic/color.h>
#include <string_view>

class Settings {
    Settings();

public:
    static Settings* instance();

    std::string_view title = "Point in Polygon";

    float screenWidth = 960.0f; //1280.0f;

    float screenHeight = 600.0f; //800.0f;

    Color backgroundColor1 = 0x161f2bff;

    Color backgroundColor2 = 0x111a26ff;
    Color backgroundColor3 = 0x1e2733ff;
    Color PolygonColor = 0xe28218ff;
    Color PointInsideColor = 0xb94610ff;
    Color PointOutsideColor = 0x3d5a6cff;
    Color TextColor = 0x4d4d4dff;

    float radius = 4.0f;

    float margin = 10.0f;
};
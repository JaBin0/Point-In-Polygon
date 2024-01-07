#pragma once
#include <glad/gl.h>
#include <unordered_map>

#include <glm/matrix.hpp>

#include <inttypes.h>
#include <string_view>
#include <string>
#include <vector>
#include <array>

// Struct which combine 
struct GBuffer {
    uint32_t vao;
    uint32_t vbo;
};

class Renderer {
    // Currently used shader pipeline
    uint32_t _activePipeline;

    // Projection matrix (in case of this app orthographic)
    glm::mat4 _projection;

protected:
    struct Character {
        unsigned int textureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    std::unordered_map<char, Character> _font;

    void loadFont(std::string_view fontFile);

public:
    // Constructors && Destructor
    Renderer() = delete;
    Renderer(GLADloadfunc procAddress);
    ~Renderer();

    /* Read content of the file and return it via string object */
    std::string readFile(std::string_view path);

    /* Create graphic pipeline base on the specified vertex and fragment shader files */
    uint32_t createShaderProgram(std::string_view vertexShaderFile, std::string_view fragmentShaderFile);

    // Create buffer in the graphic memory with given parameters
    GBuffer createBuffer(const std::vector<float> data, const std::vector<uint16_t> bufferDes, uint32_t size = 0xFFFFFFFF, const bool isStatic = true);

    // Load and register image within graphic system. Returns texture ID assign by openGL driver 
    uint32_t createTexture(std::string_view textureFile);

    // Update graphic buffer define by the buffer argument with given data starting from specific index. 
    // vertexSize describe data if for example single vertex consists of more then single float ex. point (x, y)
    bool updateBuffer(const GBuffer buffer, const std::vector<float> data, const uint16_t vertexSize, const uint32_t startIndex = 0);

    // Change currently active shader pipeline
    void useProgram(uint32_t shaderPipeline);

    // Update vec4 uniform in the currently active shader pipeline 
    void setUniform4f(std::string_view name, std::array<float, 4> data);

    // Update vec2 uniform in the currently active shader pipeline 
    void setUniform2f(std::string_view name, std::array<float, 2> data);

    // Update float uniform in the currently active shader pipeline 
    void setUniform1f(std::string_view name, float data);
    
    void setUniform1b(std::string_view name, bool data);

    // Set point size in the graphic context
    void setPointSize(float size);

    // Draw single static point from the buffer 
    void drawStaticPoint(GBuffer buffer, uint32_t index);

    void drawStaticRectangle(GBuffer buffer, uint32_t index);

    void drawStaticImage(GBuffer buffer, uint32_t index, uint32_t image);

    void drawStaticLine(GBuffer buffer, uint32_t startIndex, uint32_t numberOfPoints);

    void drawText2D(GBuffer buffer, std::string text, float x, float y, float scale);

    void clear();

public:
    static const uint32_t INVALID_SHADER_PIPELINE = 0xFFFFFFFF;

    static const uint32_t INVALID_VALUE = 0xFFFFFFFF;
};
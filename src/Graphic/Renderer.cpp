#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "Renderer.h"
#include "simage.h"
#include <Config/Settings.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

Renderer::Renderer(GLADloadfunc procAddress) 
    : _activePipeline {0}
    , _projection {glm::mat4(1.0f)}
{
    // Load openGL function pointers
    if(!gladLoadGL(procAddress)) {
        std::cout << "ERROR: OpenGL function pointers could not be retrieve by GLAD" << std::endl;
        return;        
    }

    Settings* settings = Settings::instance();
    const Color& bgColor = settings->backgroundColor1;
    glClearColor(bgColor.r(), bgColor.g(), bgColor.b(), bgColor.a());
    glViewport(0, 0, settings->screenWidth, settings->screenHeight);

    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _projection = glm::ortho(0.0f, settings->screenWidth, 0.0f, settings->screenHeight, 0.0f, 100.0f);

    loadFont("Assets/fonts/arial.ttf");
}

Renderer::~Renderer() {

}

void Renderer::loadFont(std::string_view fontFile) {
    FT_Library ft;
    if(FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init freetype library" << std::endl;
    }

    FT_Face face;
    if(FT_New_Face(ft, fontFile.data(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load the font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            GL_RED, 
            face->glyph->bitmap.width, 
            face->glyph->bitmap.rows, 
            0, 
            GL_RED, 
            GL_UNSIGNED_BYTE, 
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        _font.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

std::string Renderer::readFile(std::string_view path) {
    std::fstream file(path.data(), std::ios_base::in);
    std::string content;
    if(!file.is_open()) {
        std::cout << "File <" << path << "> could not been opened correctly "<< std::endl;
        return content;
    }

    std::getline(file, content, '\0');
    return content;    
}

uint32_t Renderer::createShaderProgram(std::string_view vertexShaderFile, std::string_view fragmentShaderFile) {
    uint32_t shaderProgram = Renderer::INVALID_SHADER_PIPELINE;
    std::stringstream msg {};
    msg << "Creating shader program <" << vertexShaderFile << "> : <" << fragmentShaderFile << " ... ";

    int success;
    char infoLog[512];

    // =========== Vertex shader ===========
    std::string vertexShaderSource = readFile(vertexShaderFile);
    const char* vertexSource = vertexShaderSource.data();
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        msg << "Failed" << std::endl
            << "ERROR::VERTEX_SHADER:" << std::endl
            << infoLog << std::endl;
        glDeleteShader(vertexShader);
        std::cout << msg.str();
        return shaderProgram;
    }

    // =========== Fragment shader ===========
    std::string fragmentShaderSource = readFile(fragmentShaderFile);
    const char* fragmentSource = fragmentShaderSource.data();
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        msg << "Failed" << std::endl
            << "ERROR::FRAGMENT_SHADER:" << std::endl
            << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        std::cout << msg.str();
        return shaderProgram;
    }

    // =========== Program shader ===========
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        msg << "Failed" << std::endl
            << "ERROR::LINKING_PROGRAM:" << std::endl
            << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        shaderProgram = Renderer::INVALID_SHADER_PIPELINE;
        std::cout << msg.str();
        return shaderProgram;
    }

    msg << "Success" << std::endl;
    std::cout << msg.str();
    return shaderProgram;
}

GBuffer Renderer::createBuffer(const std::vector<float> data, const std::vector<uint16_t> bufferDes, uint32_t size, const bool isStatic) {
    if(size == 0xFFFFFFFF) {size = data.size();}
    GBuffer buffer {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};
    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    uint32_t dataType = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data.data(), dataType);
    uint16_t vertexSize = std::accumulate(bufferDes.begin(), bufferDes.end(), 0);
    
    uint32_t offset = 0;
    for(uint16_t i = 0; i < bufferDes.size(); i++) {
        glVertexAttribPointer(i, bufferDes.at(i), GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += bufferDes.at(i);
    }
    glBindVertexArray(0);
    return buffer;
}

uint32_t Renderer::createTexture(std::string_view textureFile) {
    SImage img(textureFile);
    if(!img) {
        return 0;
    }
    
    uint32_t texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

bool Renderer::updateBuffer(const GBuffer buffer, const std::vector<float> data, const uint16_t vertexSize, const uint32_t startIndex) {
    bool result = false;
    if(buffer.vao == Renderer::INVALID_VALUE || buffer.vbo == Renderer::INVALID_VALUE) {
        std::cout << "ERROR::Renderer::updateBuffer - vao or vbo is invalid " << std::endl;
        return false; 
    }

    if(data.size() == 0 || ((data.size() % vertexSize) != 0)) {
        std::cout << "ERROR::Renderer::updateBuffer - data is empty or data size is not fully devideable by dataDes " << std::endl;
        return false; 
    }

    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    // std::cout << "SizeOf data: " <<  data.size() * sizeof(float) << std::endl;     
    glBufferSubData(GL_ARRAY_BUFFER,  startIndex * vertexSize * sizeof(float), data.size() * sizeof(float), data.data());
    // glBufferSubData(GL_ARRAY_BUFFER,  0, data.size() * sizeof(float), data.data());
    glBindVertexArray(0);
    result = true;
    return result;
}

void Renderer::useProgram(uint32_t shaderPipeline) {
    _activePipeline = shaderPipeline;
    glUseProgram(shaderPipeline);
    if(_activePipeline != 0) {
        glUniformMatrix4fv(glGetUniformLocation(_activePipeline, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
    } 
}

void Renderer::setUniform4f(std::string_view name, const std::array<float, 4> data) {
    glUniform4fv(glGetUniformLocation(_activePipeline, name.data()), 1, &data[0]);
}

void Renderer::setUniform2f(std::string_view name, const std::array<float, 2> data) {
    glUniform2fv(glGetUniformLocation(_activePipeline, name.data()), 1, &data[0]);
}

void Renderer::setUniform1f(std::string_view name, const float data) {
    glUniform1f(glGetUniformLocation(_activePipeline, name.data()), data);
}

void Renderer::setUniform1b(std::string_view name, bool data) {
    glUniform1i(glGetUniformLocation(_activePipeline, name.data()), data ? 1 : 0);
}

void Renderer::setPointSize(const float size) {
    glPointSize(size);
}

void Renderer::drawStaticPoint(GBuffer buffer, uint32_t index) {
    glBindVertexArray(buffer.vao);
    glDrawArrays(GL_POINTS, index, 1);
    glBindVertexArray(0);
}

void Renderer::drawStaticRectangle(GBuffer buffer, uint32_t index) {
    glBindVertexArray(buffer.vao);
    glDrawArrays(GL_TRIANGLES, index * 6, index * 6 + 6);
    glBindVertexArray(0);
}

void Renderer::drawStaticImage(GBuffer buffer, uint32_t index, uint32_t image) {
    glBindVertexArray(buffer.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);
    glDrawArrays(GL_TRIANGLES, index * 6, index * 6 + 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Renderer::drawStaticLine(GBuffer buffer, uint32_t startIndex, uint32_t numberOfPoints) {
    glBindVertexArray(buffer.vao);
    glDrawArrays(GL_LINES, startIndex, numberOfPoints);
    glBindVertexArray(0);
}

void Renderer::drawText2D(GBuffer buffer, std::string text, float x, float y, float scale) {
    // Active corresponding render state
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(buffer.vao);

    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++) {
        Character ch = _font[*c];
        float xpos = x + ch.Bearing.x * (scale/48.0f);
        float ypos = y - (ch.Size.y - ch.Bearing.y) * (scale/48.0f);

        float w = ch.Size.x * (scale/48.0f);
        float h = ch.Size.y * (scale/48.0f);

        // update VBO for each character
        float vertices[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos,     ypos,     0.0f, 1.0f},
            {xpos + w, ypos,     1.0f, 1.0f},

            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos + w, ypos +h,  1.0f, 0.0f}
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * (scale/48.0f);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>

#include "math.h"

#pragma pack(push, 1)
struct bitmapHeader
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t bitmapOffset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
};
#pragma pack(pop)

struct Texture
{
    uint32_t* pixels;
    int width;
    int height;
};

struct IndexBuffer
{
    int a, b, c;
};

struct VertexBuffer
{
    Vec3 vertice; 
    Vec3 normal; 
    Vec2 textureCoord;
};

struct Mesh
{
    uint32_t vao;
    int numVertices;
    int numTexCoords;
    int numNormals;
    int numIndex; 
    Vec3* vertices;
    Vec2* textureCoords;
    Vec3* normals;
    int* indices;
    IndexBuffer* vertexIndex;
    IndexBuffer* textureIndex;
    IndexBuffer* normalIndex;
    VertexBuffer* vertexBuffer;
    VertexBuffer* fVertexBuffer;
    Texture tex;
    uint32_t texId;
    Matrix model;
    bool should_render = true;
};

struct Terrain
{
    unsigned int vao;
    VertexBuffer* vertexBuffer;
    int*  indexBuffer;
    int numIndex;
    Texture tex;
    uint32_t texId;
};

char* ReadFile(const char* file_path);
Texture LoadBMP(const char* filename);
void LoadOBJFileIndex(Mesh* mesh, const char* filePhat, const char* texFileName);
void GenerateTerrain(Terrain* terrain,
                     int posX,
                     int posZ,
                     int numCols,
                     int numRows,
                     int cellSpacing,
                     const char* texFileName);

#endif

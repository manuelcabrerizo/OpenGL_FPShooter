#ifndef COLLADAPARSER_H
#define COLLADAPARSER_H

#include "tinyxml.h"
// c and c++ libraries
#include <vector>

std::vector<int> ParseStringIntVector(char* string);
std::vector<float> ParseStringFloatVector(char* string);
bool LoadColladaFile(unsigned int* vao,
                     unsigned int* textId,
                     int* numberVertices,
                     const char* xmlFilePath,
                     const char* textureFilePath,
                     bool haveEBO);

#endif

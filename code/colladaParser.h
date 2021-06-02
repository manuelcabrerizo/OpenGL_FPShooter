#ifndef COLLADAPARSER_H
#define COLLADAPARSER_H

#include "tinyxml.h"
#include "math.h"
// c and c++ libraries
#include <vector>

struct ReturnValues
{
    std::vector<Vec3>  a;
    std::vector<IVec3> b;
};

std::vector<int> ParseStringIntVector(char* string);
std::vector<float> ParseStringFloatVector(char* string);
std::vector<char*> ParseStringStringVector(char* string);
ReturnValues GetJointsIdsAndWeight(TiXmlElement* controllers);
bool LoadColladaFile(unsigned int* vao,
                     unsigned int* textId,
                     int* numberVertices,
                     const char* xmlFilePath,
                     const char* textureFilePath,
                     bool haveEBO);

#endif

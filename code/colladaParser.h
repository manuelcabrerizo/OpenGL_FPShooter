#ifndef COLLADAPARSER_H
#define COLLADAPARSER_H

#include "tinyxml.h"

// c and c++ libraries
#include <stdlib.h>
#include <vector>
#include <string.h>


bool LoadColladaFile(const char* xmlFilePath);

#endif

std::vector<float> ParseStringFloatVector(char* string)
{
    char* pNext;
    std::vector<float> vectorFloats;
    float firstFloat = strtof(string, &pNext);
    vectorFloats.push_back(firstFloat);
    while(true)
    {
        char* pActual = pNext;
        float nextFloat = strtof(pActual, &pNext);
        if(pActual == pNext)
        {
            break;
        }
        vectorFloats.push_back(nextFloat);
    }
    return vectorFloats;
}

bool LoadColladaFile(const char* xmlFilePath)
{
    // open the xml file
    TiXmlDocument xmlDoc;
    if(!xmlDoc.LoadFile(xmlFilePath))
    {
        return false;
    }

    // then we have to get a pointer to the 
    // root element of the xml file
    TiXmlElement* pRoot = xmlDoc.RootElement();

    // first we are going to try loading the data 
    // of the collada file
    TiXmlElement* geometries  = 0;
    TiXmlElement* animations  = 0;
    TiXmlElement* controllers = 0;
    TiXmlElement* visualScene = 0;

    for(TiXmlElement* e = pRoot->FirstChildElement();
        e != NULL;
        e = e->NextSiblingElement())
    {
        if(strcmp(e->Value(), "library_geometries") == 0)
        {
            geometries = e;
        }
        if(strcmp(e->Value(), "library_animations") == 0)
        {
            animations = e;
        }
        if(strcmp(e->Value(), "library_controllers") == 0)
        {
            controllers = e;
        }
        if(strcmp(e->Value(), "library_visual_scenes") == 0)
        {
            visualScene = e;
        }
    }
    if(geometries == NULL  ||
       animations == NULL  ||
       controllers == NULL ||
       visualScene == NULL)
    {
        return false;
    }

    // procces the geometry data
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> textures;

    // LOADING::VERTICES::NORMALS::TEXTURECOODS
    char* verticesString = (char*)geometries->FirstChildElement()->FirstChildElement()->FirstChildElement()->FirstChildElement()->GetText(); 
    char* normalsString  = (char*)geometries->FirstChildElement()->FirstChildElement()->FirstChildElement()->NextSiblingElement()->FirstChildElement()->GetText();
    char* textureString  = (char*)geometries->FirstChildElement()->FirstChildElement()->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement()->GetText(); 
    vertices = ParseStringFloatVector(verticesString);
    normals  = ParseStringFloatVector(normalsString);
    textures = ParseStringFloatVector(textureString);



    return true;
}

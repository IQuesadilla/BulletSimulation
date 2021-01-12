#include "daeload.h"

using namespace tinyxml2;

daeloader::daeloader()
{

}

daeloader::~daeloader()
{

}

bool daeloader::parse(XMLNode* parent)
{    
    for (XMLNode *node = parent->FirstChild(); node != NULL; node = node->NextSibling())
    {
        auto eparent = node->ToElement();
        if (eparent != NULL)
        {
            std::cout << "Name: " << eparent->Name() << std::endl;
            
            for (const XMLAttribute* attrib = eparent->FirstAttribute(); attrib != NULL; attrib = attrib->Next())
            {
                    std::cout << "   Attribute: " << attrib->Name() << " : " << attrib->Value() << std::endl;
            }
        }
        else
        {
            if (std::string(parent->Value()) != "p" && std::string(parent->Value()) != "float_array")
                std::cout << "      Value: " << node->Value() << std::endl;
            else
                std::cout << "      Value: Alot of numbers" << std::endl;
        }

        if (!node->NoChildren()) parse(node);
    }
    return false;
}

void daeloader::import(std::string path)
{
    XMLDocument doc;
	std::cout << doc.LoadFile(path.c_str()) << std::endl;
    parse(doc.FirstChildElement("COLLADA"));
}
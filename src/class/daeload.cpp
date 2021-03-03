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
            if (std::string(parent->Value()) == "float_array")
			{
				std::stringstream parser; parser << node->Value();
				std::cout << "		Value: P or FLOAT_ARRAY" << std::endl;
                for (float parsed = 0.0f; !parser.eof(); parser >> parsed)
					indices.push_back(parsed);
				std::cout << "		Value: P or FLOAT_ARRAY loaded" << std::endl;
			}
			else if (std::string(parent->Value()) == "p")
				std::cout << "      Value: A large number" << std::endl;
            else
			{
				std::cout << "      Value: " << node->Value() << std::endl;
			}
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
	std::cout << "INDICES: " << indices.size() << std::endl;
}

std::shared_ptr<_shader> daeloader::getShader()
{
	return shaderptr;
}
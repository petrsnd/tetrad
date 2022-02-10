#pragma once
#include "JsonSpirit/json_spirit.h"

#include <string>


namespace Implbits {

class IJsonSerializable
{
public:
    virtual ~IJsonSerializable() {}
    virtual json_spirit::mObject JsonEncode() const = 0;
    virtual void JsonDecode( json_spirit::mObject& json ) = 0;
};

} // Implbits

#include <LV_Registration.h>

#include <vec2.hpp>
#include <vec3.hpp>

#include <Data_Structures/Vector.h>
#include <Data_Structures/List.h>

#include <Variable_Loader.h>

using namespace LV;


void LV::register_types(LV::Object_Constructor& _object_constructor)
{
    _object_constructor.register_type<LV::Variable_Loader_Stub>();
}

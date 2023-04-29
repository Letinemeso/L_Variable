#include "Type_Manager.h"

using namespace LV;


LDS::Map<std::string, Type_Utility> Type_Manager::m_registred_types;



void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility)
{
	L_DEBUG_FUNC_NOARG([&]()
	{
        regtypes_t::Iterator check = m_registred_types.find(_type_name);
        L_ASSERT(!check.is_ok());
	});

    m_registred_types.insert(_type_name, _utility);
}



bool Type_Manager::validate(const std::string &_type_name, const std::string &_value_as_string)
{
    regtypes_t::Iterator utility_it = m_registred_types.find(_type_name);
    L_ASSERT(utility_it.is_ok());

    const Type_Utility& utility = *utility_it;

	return utility.validation_func(_value_as_string);
}

void Type_Manager::parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to)
{
    regtypes_t::Iterator utility_it = m_registred_types.find(_type_name);
    L_ASSERT(utility_it.is_ok());

    const Type_Utility& utility = *utility_it;

	L_DEBUG_FUNC_NOARG([&]()	//	crashes if validation is failed
	{
		for(unsigned int i=0; i<_values_as_string.size(); ++i)
			L_ASSERT(utility.validation_func(_values_as_string[i]));
	});

	utility.parse_func(_allocate_to, _values_as_string);
}

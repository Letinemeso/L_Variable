#include "Type_Manager.h"

using namespace LV;


std::map<std::string, Type_Utility> Type_Manager::m_registred_types;



void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility)
{
	L_DEBUG_FUNC_NOARG([&]()
	{
		regtypes_t::iterator check = m_registred_types.find(_type_name);
		L_ASSERT(check == m_registred_types.end());
	});

	m_registred_types.emplace(_type_name, _utility);
}



bool Type_Manager::validate(const std::string &_type_name, const std::string &_value_as_string)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	L_ASSERT(utility_it != m_registred_types.end());

	const Type_Utility& utility = utility_it->second;

	return utility.validation_func(_value_as_string);
}

void Type_Manager::parse(const std::string& _type_name, const std::string& _value_as_string, void* _allocate_to)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	L_ASSERT(utility_it != m_registred_types.end());

	const Type_Utility& utility = utility_it->second;

	L_ASSERT(utility.validation_func(_value_as_string));

	utility.parse_func(_allocate_to, _value_as_string);
}

void Type_Manager::assign(const std::string& _type_name, void* _data_vptr, void* _assign_to)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	if(utility_it == m_registred_types.end())
		return;

	const Type_Utility& utility = utility_it->second;

	utility.assign_func(_assign_to, _data_vptr);
}

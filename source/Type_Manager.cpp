#include "Type_Manager.h"

using namespace LV;


Type_Manager::Registred_Types_Map Type_Manager::m_registred_types;
Type_Manager::Types_Aliases_Map Type_Manager::m_types_aliases;



void Type_Manager::register_basic_types()
{
    Type_Utility type_utility;

    //  int
    type_utility.validation_func =
        [](const std::string& _val)
    {
        unsigned int i=0;
        if(_val[0] == '+' || _val[0] == '-')
            ++i;
        for(; i<_val.size(); ++i)
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        *((int*)_variable_vptr) = std::stoi(_values_as_string[0]);
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<std::string> result;
        result.push(std::to_string(*((int*)_variable_vptr)));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<int>();
    type_utility.clear_func = __construct_default_clear_function<int>();
    type_utility.copy_func = __construct_default_copy_function<int>();
    register_type("int", type_utility);


    //  LDS::Vector<int>
    type_utility.validation_func =
        [](const std::string& _val)
    {
        for(unsigned int i=0; i<_val.size(); ++i)
        {
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        }
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        LDS::Vector<int>& vector = *((LDS::Vector<int>*)_variable_vptr);
        vector.clear();
        vector.resize(_values_as_string.size());
        for(unsigned int i=0; i<_values_as_string.size(); ++i)
            vector.push(std::stoi(_values_as_string[i]));
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Vector<int>& variable = *((LDS::Vector<int>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size());
        for(unsigned int i=0; i<variable.size(); ++i)
            result.push(std::to_string(variable[i]));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Vector<int>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Vector<int>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Vector<int>>();
    register_type("Int_Vector", type_utility);
    register_type_alias("Int_Vector", "LDS::Vector<int>");


    //  unsigned int
    type_utility.validation_func =
        [](const std::string& _val)
    {
        for(unsigned int i=0; i<_val.size(); ++i)
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        *((int*)_variable_vptr) = std::stoi(_values_as_string[0]);
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<std::string> result;
        result.push(std::to_string(*((unsigned int*)_variable_vptr)));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<unsigned int>();
    type_utility.clear_func = __construct_default_clear_function<unsigned int>();
    type_utility.copy_func = __construct_default_copy_function<unsigned int>();
    register_type("uint", type_utility);
    register_type_alias("uint", "unsigned int");


    //  LDS::Vector<unsigned int>
    type_utility.validation_func =
        [](const std::string& _val)
    {
        for(unsigned int i=0; i<_val.size(); ++i)
        {
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        }
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        LDS::Vector<unsigned int>& vector = *((LDS::Vector<unsigned int>*)_variable_vptr);
        vector.clear();
        vector.resize(_values_as_string.size());
        for(unsigned int i=0; i<_values_as_string.size(); ++i)
            vector.push(std::stoi(_values_as_string[i]));
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Vector<unsigned int>& variable = *((LDS::Vector<unsigned int>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size());
        for(unsigned int i=0; i<variable.size(); ++i)
            result.push(std::to_string(variable[i]));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Vector<unsigned int>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Vector<unsigned int>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Vector<unsigned int>>();
    register_type("Uint_Vector", type_utility);
    register_type_alias("Uint_Vector", "LDS::Vector<unsigned int>");


    //  bool
    type_utility.validation_func =
        [](const std::string& _val)
    {
        if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
            return true;
        return false;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        bool& var = *((bool*&)_variable_vptr);

        if(_values_as_string[0] == "true" || _values_as_string[0] == "+" || _values_as_string[0] == "1")
            var = true;
        else if(_values_as_string[0] == "false" || _values_as_string[0] == "-" || _values_as_string[0] == "0")
            var = false;
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        bool variable = *((unsigned int*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.push(variable ? "+" : "-");
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<bool>();
    type_utility.clear_func = __construct_default_clear_function<bool>();
    type_utility.copy_func = __construct_default_copy_function<bool>();
    register_type("bool", type_utility);


    //  LDS::Vector<bool>
    type_utility.validation_func =
        [](const std::string& _val)
    {
        if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
            return true;
        return false;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        LDS::Vector<bool>& vector = *((LDS::Vector<bool>*)_variable_vptr);
        vector.clear();
        vector.resize(_values_as_string.size());
        for(unsigned int i=0; i<_values_as_string.size(); ++i)
        {
            if(_values_as_string[i] == "true" || _values_as_string[i] == "+" || _values_as_string[i] == "1")
                vector.push(true);
            else if(_values_as_string[i] == "false" || _values_as_string[i] == "-" || _values_as_string[i] == "0")
                vector.push(false);
        }
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Vector<bool>& variable = *((LDS::Vector<bool>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size());
        for(unsigned int i=0; i<variable.size(); ++i)
            result.push(variable[i] ? "+" : "-");
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Vector<bool>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Vector<bool>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Vector<bool>>();
    register_type("Bool_Vector", type_utility);
    register_type_alias("Bool_Vector", "LDS::Vector<bool>");


    //  std::string
    type_utility.validation_func =
        [](const std::string& _val)
    {
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        *((std::string*)_variable_vptr) = _values_as_string[0];
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<std::string> result;
        result.push(*((std::string*)_variable_vptr));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<std::string>();
    type_utility.clear_func = __construct_default_clear_function<std::string>();
    type_utility.copy_func = __construct_default_copy_function<std::string>();
    register_type("string", type_utility);
    register_type_alias("string", "std::string");


    //  LDS::Vector<std::string>
    type_utility.validation_func =
        [](const std::string& _val)
    {
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        LDS::Vector<std::string>& vector = *((LDS::Vector<std::string>*)_variable_vptr);
        vector.clear();
        vector = _values_as_string;
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Vector<std::string>& variable = *((LDS::Vector<std::string>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size());
        for(unsigned int i=0; i<variable.size(); ++i)
            result.push(variable[i]);
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Vector<std::string>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Vector<std::string>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Vector<std::string>>();
    register_type("String_Vector", type_utility);
    register_type_alias("String_Vector", "LDS::Vector<std::string>");


    //  float
    type_utility.validation_func =
        [](const std::string& _val)
    {
        if(_val == ".")
            return false;

        unsigned int dots_count = 0;
        unsigned int i=0;
        if(_val[0] == '+' || _val[0] == '-')
            ++i;
        for(; i<_val.size(); ++i)
        {
            if(_val[i] == '.')
            {
                ++dots_count;
                continue;
            }
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        }

        if(dots_count > 1)
            return false;

        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        *((float*)_variable_vptr) = std::stof(_values_as_string[0]);
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<std::string> result;
        result.push(std::to_string(*((float*)_variable_vptr)));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<float>();
    type_utility.clear_func = __construct_default_clear_function<float>();
    type_utility.copy_func = __construct_default_copy_function<float>();
    register_type("float", type_utility);


    //  LDS::Vector<float>
    type_utility.validation_func =
        [](const std::string& _val)
    {
        if(_val == ".")
            return false;

        unsigned int dots_count = 0;
        unsigned int i=0;
        if(_val[0] == '+' || _val[0] == '-')
            ++i;
        for(; i<_val.size(); ++i)
        {
            if(_val[i] == '.')
            {
                ++dots_count;
                continue;
            }
            if(_val[i] < '0' || _val[i] > '9')
                return false;
        }

        if(dots_count > 1)
            return false;

        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        LDS::Vector<float>& vector = *((LDS::Vector<float>*)_variable_vptr);
        vector.clear();
        vector.resize(_values_as_string.size());
        for(unsigned int i=0; i<_values_as_string.size(); ++i)
            vector.push(std::stof(_values_as_string[i]));
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Vector<float>& variable = *((LDS::Vector<float>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size());
        for(unsigned int i=0; i<variable.size(); ++i)
            result.push(std::to_string(variable[i]));
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Vector<float>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Vector<float>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Vector<float>>();
    register_type("Float_Vector", type_utility);
    register_type_alias("Float_Vector", "LDS::Vector<float>");


    //  LDS::Map<std::string, std::string> - had to name differently (String_To_String_Map) due to macros not seeing comma as a part of the type
    //  String_To_String_Map
    type_utility.validation_func =
        [](const std::string& _val)
    {
        return true;
    };
    type_utility.parse_func =
        [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
    {
        L_ASSERT(_values_as_string.size() % 2 == 0);

        LDS::Map<std::string, std::string>& map = *((LDS::Map<std::string, std::string>*)_variable_vptr);
        map.clear();
        for(unsigned int i=0; i<_values_as_string.size(); i += 2)
            map.insert(_values_as_string[i], _values_as_string[i + 1]);
    };
    type_utility.serialize_func =
        [](void* _variable_vptr)
    {
        const LDS::Map<std::string, std::string>& variable = *((LDS::Map<std::string, std::string>*)_variable_vptr);
        LDS::Vector<std::string> result;
        result.resize(variable.size() * 2);
        for(LDS::Map<std::string, std::string>::Const_Iterator it = variable.iterator(); !it.end_reached(); ++it)
        {
            result.push(it.key());
            result.push(*it);
        }
        return result;
    };
    type_utility.allocate_func = __construct_default_allocate_function<LDS::Map<std::string, std::string>>();
    type_utility.clear_func = __construct_default_clear_function<LDS::Map<std::string, std::string>>();
    type_utility.copy_func = __construct_default_copy_function<LDS::Map<std::string, std::string>>();
    register_type("String_To_String_Map", type_utility);
}

void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility, bool _override)
{
    Registred_Types_Map::Iterator maybe_registred_type = m_registred_types.find(_type_name);

    if(!maybe_registred_type.is_ok())
    {
        m_registred_types.insert(_type_name, _utility);
        register_type_alias(_type_name, _type_name);
    }
    else if(_override)
    {
        *maybe_registred_type = _utility;
    }
}

void Type_Manager::register_type_alias(const std::string& _type_name, const std::string& _type_alias)
{
    Types_Aliases_Map::Iterator maybe_alias = m_types_aliases.find(_type_alias);

    if(maybe_alias.is_ok())
    {
        L_ASSERT(*maybe_alias == _type_name);
        return;
    }

    m_types_aliases.insert(_type_alias, _type_name);
}



const std::string& Type_Manager::get_default_type_name(const std::string& _alias)
{
    Types_Aliases_Map::Iterator type_alias_it = m_types_aliases.find(_alias);
    L_ASSERT(type_alias_it.is_ok());

    return *type_alias_it;
}

bool Type_Manager::type_is_registered(const std::string& _type_name)
{
    Types_Aliases_Map::Iterator maybe_type_alias_it = m_types_aliases.find(_type_name);
    if(!maybe_type_alias_it.is_ok())
        return false;

    const std::string& original_name = *maybe_type_alias_it;
    return m_registred_types.find(original_name).is_ok();
}

const Type_Utility& Type_Manager::get_type_utility(const std::string& _type_name)
{
    const std::string& original_name = get_default_type_name(_type_name);

    Registred_Types_Map::Iterator utility_it = m_registred_types.find(original_name);
    L_ASSERT(utility_it.is_ok());

    return *utility_it;
}


bool Type_Manager::validate(const std::string &_type_name, const std::string &_value_as_string)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.validation_func);

	return utility.validation_func(_value_as_string);
}

void Type_Manager::parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.parse_func);
    L_ASSERT(utility.validation_func);

	L_DEBUG_FUNC_NOARG([&]()	//	crashes if validation is failed
	{
		for(unsigned int i=0; i<_values_as_string.size(); ++i)
			L_ASSERT(utility.validation_func(_values_as_string[i]));
	});

	utility.parse_func(_allocate_to, _values_as_string);
}

LDS::Vector<std::string> Type_Manager::serialize(const std::string& _type_name, void* _variable)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.serialize_func);

    return utility.serialize_func(_variable);
}

Type_Utility::Allocate_Result Type_Manager::allocate(const std::string& _type_name, unsigned int _amount)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.allocate_func);

    return utility.allocate_func(_amount);
}

void Type_Manager::clear(const std::string& _type_name, void* _variable_vptr)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.clear_func);

    utility.clear_func(_variable_vptr);
}

void Type_Manager::copy(const std::string& _type_name, void* _variable_vptr_0, void* _variable_vptr_1)
{
    const Type_Utility& utility = get_type_utility(_type_name);

    L_ASSERT(utility.copy_func);

    utility.copy_func(_variable_vptr_0, _variable_vptr_1);
}

#include "Type_Manager.h"

using namespace LV;


Type_Manager::Registred_Types_Map Type_Manager::m_registred_types;



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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new int[_amount]);
        result.size = sizeof(int) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        int* ptr = (int*)_variable_vptr;
        delete[] ptr;
    };
    register_type("int", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new unsigned int[_amount]);
        result.size = sizeof(unsigned int) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        unsigned int* ptr = (unsigned int*)_variable_vptr;
        delete[] ptr;
    };
    register_type("unsigned int", type_utility);
    register_type("uint", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new LDS::Vector<unsigned int>[_amount]);
        result.size = sizeof(LDS::Vector<unsigned int>) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<unsigned int>* ptr = (LDS::Vector<unsigned int>*)_variable_vptr;
        delete[] ptr;
    };
    register_type("LDS::Vector<unsigned int>", type_utility);
    register_type("LDS::Vector<uint>", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new bool[_amount]);
        result.size = sizeof(bool) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        bool* ptr = (bool*)_variable_vptr;
        delete[] ptr;
    };
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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new LDS::Vector<bool>[_amount]);
        result.size = sizeof(LDS::Vector<bool>) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<bool>* ptr = (LDS::Vector<bool>*)_variable_vptr;
        delete[] ptr;
    };
    register_type("LDS::Vector<bool>", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new std::string[_amount]);
        result.size = sizeof(std::string) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        std::string* ptr = (std::string*)_variable_vptr;
        delete[] ptr;
    };
    register_type("std::string", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new LDS::Vector<std::string>[_amount]);
        result.size = sizeof(LDS::Vector<std::string>) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<std::string>* ptr = (LDS::Vector<std::string>*)_variable_vptr;
        delete[] ptr;
    };
    register_type("LDS::Vector<std::string>", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new float[_amount]);
        result.size = sizeof(float) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        float* ptr = (float*)_variable_vptr;
        delete[] ptr;
    };
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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new LDS::Vector<float>[_amount]);
        result.size = sizeof(LDS::Vector<float>) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        LDS::Vector<float>* ptr = (LDS::Vector<float>*)_variable_vptr;
        delete[] ptr;
    };
    register_type("LDS::Vector<float>", type_utility);


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
    type_utility.allocate_func =
        [](unsigned int _amount)
    {
        Type_Utility::Allocate_Result result;
        result.ptr = (void*)(new LDS::Map<std::string, std::string>[_amount]);
        result.size = sizeof(LDS::Map<std::string, std::string>) * _amount;
        return result;
    };
    type_utility.clear_func =
        [](void* _variable_vptr)
    {
        LDS::Map<std::string, std::string>* ptr = (LDS::Map<std::string, std::string>*)_variable_vptr;
        delete[] ptr;
    };
    register_type("String_To_String_Map", type_utility);
}

void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility, bool _override)
{
    Registred_Types_Map::Iterator maybe_registred_type = m_registred_types.find(_type_name);

    if(!maybe_registred_type.is_ok())
        m_registred_types.insert(_type_name, _utility);
    else if(_override)
        *maybe_registred_type = _utility;
}



bool Type_Manager::type_is_registered(const std::string& _type_name)
{
    return m_registred_types.find(_type_name).is_ok();
}

const Type_Utility& Type_Manager::get_type_utility(const std::string& _type_name)
{
    Registred_Types_Map::Iterator utility_it = m_registred_types.find(_type_name);
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

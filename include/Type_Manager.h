#pragma once

#include <string>

#include <Data_Structures/Vector.h>
#include <Data_Structures/Map.h>

#include <L_Debug/L_Debug.h>


namespace LV
{

	struct Type_Utility
	{
        typedef bool(*Validation_Function)(const std::string& _values_as_string);
        typedef void(*Parse_Function)(void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string);
        typedef LDS::Vector<std::string>(*Serialize_Function)(void* _variable_vptr);

        Validation_Function validation_func = nullptr;
        Parse_Function parse_func = nullptr;
        Serialize_Function serialize_func = nullptr;

        Type_Utility() { }
        Type_Utility(Validation_Function _validation_func, Parse_Function _parse_func, Serialize_Function _serialize_func)
            : validation_func(_validation_func), parse_func(_parse_func), serialize_func(_serialize_func) { }
	};


	class Type_Manager final
	{
	private:
        using Registred_Types_Map = LDS::Map<std::string, Type_Utility>;
        static Registred_Types_Map m_registred_types;

	public:
        static void register_basic_types();
        static void register_type(const std::string& _type_name, const Type_Utility& _utility, bool _override = false);

	public:
		static bool validate(const std::string& _type_name, const std::string& _value_as_string);
        static void parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to);
        static LDS::Vector<std::string> serialize(const std::string& _type_name, void* _variable);

	public:
		Type_Manager() = delete;
		Type_Manager(const Type_Manager&) = delete;
		Type_Manager(Type_Manager&&) = delete;

	};

}

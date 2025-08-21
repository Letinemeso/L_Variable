#pragma once

#include <string>

#include <Data_Structures/Vector.h>
#include <Data_Structures/Map.h>

#include <L_Debug/L_Debug.h>


namespace LV
{

	struct Type_Utility
	{
        struct Allocate_Result
        {
            void* ptr = nullptr;
            unsigned int size = 0;
        };

        typedef bool(*Validation_Function)(const std::string& _values_as_string);
        typedef void(*Parse_Function)(void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string);
        typedef LDS::Vector<std::string>(*Serialize_Function)(void* _variable_vptr);
        typedef Allocate_Result(*Allocate_Function)(unsigned int _amount);
        typedef void(*Clear_Function)(void* _variable_vptr);
        typedef void(*Copy_Function)(void* _variable_vptr_0, void* _variable_vptr_1);

        Validation_Function validation_func = nullptr;
        Parse_Function parse_func = nullptr;
        Serialize_Function serialize_func = nullptr;
        Allocate_Function allocate_func = nullptr;
        Clear_Function clear_func = nullptr;
        Copy_Function copy_func = nullptr;

        Type_Utility() { }
	};


    template<typename _Type>
    Type_Utility::Allocate_Function __construct_default_allocate_function()
    {
        return
            [](unsigned int _amount)
            {
                Type_Utility::Allocate_Result result;
                result.ptr = (void*)(new _Type[_amount]);
                result.size = sizeof(_Type) * _amount;
                return result;
            };
    }

    template<typename _Type>
    Type_Utility::Clear_Function __construct_default_clear_function()
    {
        return
            [](void* _variable_vptr)
            {
                _Type* ptr = (_Type*)_variable_vptr;
                delete[] ptr;
            };
    }

    template<typename _Type>
    Type_Utility::Copy_Function __construct_default_copy_function()
    {
        return
            [](void* _variable_vptr_0, void* _variable_vptr_1)
            {
                _Type* ptr_0 = (_Type*)_variable_vptr_0;
                _Type* ptr_1 = (_Type*)_variable_vptr_1;
                *ptr_0 = *ptr_1;
            };
    }


	class Type_Manager final
	{
	private:
        using Registred_Types_Map = LDS::Map<std::string, Type_Utility>;
        static Registred_Types_Map m_registred_types;

	public:
        static void register_basic_types();
        static void register_type(const std::string& _type_name, const Type_Utility& _utility, bool _override = false);

	public:
        static bool type_is_registered(const std::string& _type_name);
        static const Type_Utility& get_type_utility(const std::string& _type_name);

		static bool validate(const std::string& _type_name, const std::string& _value_as_string);
        static void parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to);
        static LDS::Vector<std::string> serialize(const std::string& _type_name, void* _variable);
        static Type_Utility::Allocate_Result allocate(const std::string& _type_name, unsigned int _amount);
        static void clear(const std::string& _type_name, void* _variable_vptr);
        static void copy(const std::string& _type_name, void* _variable_vptr_0, void* _variable_vptr_1);

	public:
		Type_Manager() = delete;
		Type_Manager(const Type_Manager&) = delete;
		Type_Manager(Type_Manager&&) = delete;

	};

}

#ifndef TYPE_MANAGER_H
#define TYPE_MANAGER_H

#include <string>
#include <map>

#include <Vector.h>

#include <Debug/Debug.h>

namespace LV
{

	struct Type_Utility
	{
		typedef bool(*validation_func_t)(const std::string& _valuea_as_string);
		typedef void(*parse_func_t)(void*& _variable_vptr, const LDS::Vector<std::string>& _values_as_string);
		typedef void(*assign_func_t)(void*& _variable_vptr, void* _data_ptr);

		validation_func_t validation_func = nullptr;
		parse_func_t parse_func = nullptr;
		assign_func_t assign_func = nullptr;

		Type_Utility(validation_func_t _validation_func, parse_func_t _parse_func, assign_func_t _assign_func)
			: validation_func(_validation_func), parse_func(_parse_func), assign_func(_assign_func) { }
	};

	class Type_Manager final
	{
	private:
		typedef std::map<std::string, Type_Utility> regtypes_t;
		static std::map<std::string, Type_Utility> m_registred_types;

	public:
		static void register_type(const std::string& _type_name, const Type_Utility& _utility);

	public:
		static bool validate(const std::string& _type_name, const std::string& _value_as_string);
		static void parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to);
		static void assign(const std::string& _type_name, void* _data_vptr, void* _assign_to);

	public:
		Type_Manager() = delete;
		Type_Manager(const Type_Manager&) = delete;
		Type_Manager(Type_Manager&&) = delete;

	};

}

#endif // TYPE_MANAGER_H

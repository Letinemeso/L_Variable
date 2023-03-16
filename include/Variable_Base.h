#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <map>

#include "Type_Manager.h"
#include "MDL_Reader.h"

#ifndef VARIABLE

	#define DECLARE_VARIABLE \
		public: \
			static std::string get_estimated_type(); \
			static std::string get_estimated_history(); \
			std::string get_actual_type() const override; \
			void assign_values(const LV::MDL_Variable_Stub& _stub) override

	#define INIT_FIELDS(variable_type, parent_type) \
		std::string variable_type::get_estimated_type() { return std::string(#variable_type); } \
		std::string variable_type::get_actual_type() const { return std::string(#variable_type); }  \
		std::string variable_type::get_estimated_history() { return parent_type::get_estimated_history() + "/" + std::string(#variable_type); } \
		void variable_type::assign_values(const LV::MDL_Variable_Stub& _stub) { \
			parent_type::assign_values(_stub); \
			m_type_history = m_type_history + "/" + std::string(#variable_type);

	#define ADD_FIELD(type, field_reference) { \
			std::map<std::string, LDS::Vector<std::string>>::const_iterator check = _stub.fields.find(#field_reference); \
			if(check != _stub.fields.cend())  \
				LV::Type_Manager::parse(#type, check->second, (void*)(&field_reference)); \
		}

    #define ADD_CHILD(child_reference) \
            { \
                std::map<std::string, LV::MDL_Variable_Stub>::const_iterator check = _stub.childs.find(#child_reference); \
                if(check != _stub.childs.cend())  \
                    (child_reference).assign_values(check->second); \
            }

    #define ADD_CHILD(child_name, child_reference) \
            { \
                std::map<std::string, LV::MDL_Variable_Stub>::const_iterator check = _stub.childs.find(child_name); \
                if(check != _stub.childs.cend())  \
                    (child_reference).assign_values(check->second); \
            }

	#define FIELDS_END M_on_values_assigned(); }

#endif


namespace LV
{

	class Variable_Base
	{
	protected:
		std::string m_type_history;

	public:
		static std::string get_estimated_type() { return ""; };
		static std::string get_estimated_history() { return ""; };
		virtual std::string get_actual_type() const { return ""; }
		const std::string& get_actual_history() const { return m_type_history; }

		virtual void assign_values(const MDL_Variable_Stub& /*_stub*/) { m_type_history = ""; }

	protected:
		virtual void M_on_values_assigned() { };

	public:
		Variable_Base();
		virtual ~Variable_Base();

	public:
		void set_name(const std::string& _name);
		const std::string& get_name() const;

	};


	template<typename T>
	T* cast_variable(Variable_Base* _var)
	{
		if(_var == nullptr)
			return nullptr;

        std::string T_type = T::get_estimated_history();
        std::string Var_type = _var->get_actual_history();

		if(Var_type.size() < T_type.size())
			return nullptr;

		if(Var_type.substr(0, T_type.size()) != T_type)
			return nullptr;

		return (T*)_var;
	}

	template<typename T>
	const T* cast_variable(const Variable_Base* _var)
	{
		if(_var == nullptr)
			return nullptr;

        std::string T_type = T::get_estimated_history();
        std::string Var_type = _var->get_actual_history();

		if(Var_type.size() < T_type.size())
			return nullptr;

		if(Var_type.substr(0, T_type.size()) != T_type)
			return nullptr;

		return (T*)_var;
	}

}


#endif // TYPE_H

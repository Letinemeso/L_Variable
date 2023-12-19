#ifndef TYPE_H
#define TYPE_H

#include <string>

#include <Data_Structures/Map.h>

#include <Type_Manager.h>
#include <MDL_Reader.h>

#ifndef VARIABLE
#define VARIABLE

    #define DECLARE_VARIABLE                                                    \
        public:                                                                 \
            static std::string get_estimated_type();                            \
            static std::string get_estimated_history();                         \
            std::string get_actual_type() const override;                       \
			void assign_values(const LV::MDL_Variable_Stub& _stub) override



    #define INIT_FIELDS(variable_type, parent_type)                                                                                                 \
        std::string variable_type::get_estimated_type() { return std::string(#variable_type); }                                                     \
        std::string variable_type::get_actual_type() const { return std::string(#variable_type); }                                                  \
        std::string variable_type::get_estimated_history() { return parent_type::get_estimated_history() + "/" + std::string(#variable_type); }     \
        void variable_type::assign_values(const LV::MDL_Variable_Stub& _stub) {                                                                     \
            parent_type::assign_values(_stub);                                                                                                      \
			m_type_history = m_type_history + "/" + std::string(#variable_type);



    #define ADD_FIELD(type, field_reference) {                                                                              \
            LDS::Map<std::string, LDS::Vector<std::string>>::Const_Iterator check = _stub.fields.find(#field_reference);    \
            if(check.is_ok())                                                                                               \
                LV::Type_Manager::parse(#type, *check, (void*)(&field_reference));                                          \
		}



    #define ADD_CHILD(child_name, child_ptr)                                        \
            {                                                                       \
                Childs_Container_Type::Iterator check = m_childs.find(child_name);  \
                L_ASSERT(!check.is_ok());                                           \
                m_childs.insert(child_name, (LV::Variable_Base**)&(child_ptr));     \
            }



    #define FIELDS_END }

#endif


namespace LV
{

	class Variable_Base
	{
    public:
        using Childs_Container_Type = LDS::Map<std::string, Variable_Base**>;

	protected:
		std::string m_type_history;
        Childs_Container_Type m_childs;

	public:
		static std::string get_estimated_type() { return ""; };
		static std::string get_estimated_history() { return ""; };
		virtual std::string get_actual_type() const { return ""; }
		const std::string& get_actual_history() const { return m_type_history; }

		virtual void assign_values(const MDL_Variable_Stub& /*_stub*/) { m_type_history = ""; }
        virtual void on_values_assigned() { };
        void init_childs(const MDL_Variable_Stub& _stub);
        inline const Childs_Container_Type& get_childs() const { return m_childs; }

	public:
		Variable_Base();
		virtual ~Variable_Base();

	};


	template<typename T>
	const T* cast_variable(const Variable_Base* _var)
	{
		if(_var == nullptr)
			return nullptr;

        const std::string T_type = T::get_estimated_history();
        const std::string& var_type = _var->get_actual_history();

        if(var_type.size() < T_type.size())
			return nullptr;

        for(unsigned int i=0; i<T_type.size(); ++i)
            if(T_type[i] != var_type[i])
                return nullptr;

		return (T*)_var;
	}

    template<typename T>
    T* cast_variable(Variable_Base* _var)
    {
        return (T*)cast_variable<T>((const Variable_Base*)_var);
    }

}


#endif // TYPE_H

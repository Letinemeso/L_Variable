#pragma once

#include <string>

#include <Stuff/Mask.h>
#include <Data_Structures/List.h>
#include <Data_Structures/Map.h>

#include <Type_Manager.h>
#include <MDL_Reader.h>

#ifndef VARIABLE
#define VARIABLE

    #define INIT_VARIABLE(variable_type, parent_type)                                                                            \
        private:    \
            using Current_Type = variable_type; \
            using Parent_Type = parent_type;    \
        public:                                                                                                                     \
            static std::string get_estimated_type() { return std::string(#variable_type); }                                                 \
            static std::string get_estimated_history() { return parent_type::get_estimated_history() + "/" + get_estimated_type(); } \
            std::string get_actual_type() const override { return get_estimated_type(); }                                     \
            std::string get_actual_history() const override { return get_estimated_history(); }


    #define INIT_FIELDS                                                                                               \
        void M_assign_values(const LV::MDL_Variable_Stub& _stub) override {                                           \
            Parent_Type::M_assign_values(_stub);

    #define ADD_FIELD(type, field_reference)                                                                                \
            {                                                                                                                   \
                LV::MDL_Variable_Stub::Fields_Map::Const_Iterator check = _stub.fields.find(#field_reference);    \
                if(check.is_ok())                                                                                               \
                    LV::Type_Manager::parse(#type, *check, (void*)(&field_reference));                                          \
            }

    #define FIELDS_END \
            }


    #define INIT_CHILDS \
        public:         \
            LV::Variable_Base::Childs_Container_Type get_childs() override { \
                LV::Variable_Base::Childs_Container_Type childs_result = Parent_Type::get_childs();

    #define ADD_CHILD(child_name, child_ptr)    \
                M_register_child(childs_result, child_name, (LV::Variable_Base**)(&child_ptr));

    #define CHILDS_END \
                return childs_result; \
            }


    #define INIT_CHILDS_LISTS  \
        public: \
            LV::Variable_Base::Childs_Lists get_childs_lists() override {    \
                LV::Variable_Base::Childs_Lists childs_lists_result = Parent_Type::get_childs_lists();

    #define ADD_CHILDS_LIST(mask, childs_list_ptr) \
                childs_lists_result.push_back(Childs_List_Data(mask, childs_list_ptr));

    #define CHILDS_LISTS_END \
                return childs_lists_result; \
            }


#endif


namespace LV
{

	class Variable_Base
	{
    public:
        friend class Object_Constructor;

    public:
        using Childs_Container_Type = LDS::Map<std::string, Variable_Base**>;

    public:
        struct Child_Data
        {
            std::string name;
            LV::Variable_Base* child_ptr = nullptr;
        };
        using Childs_List = LDS::List<Child_Data>;

        struct Childs_List_Data
        {
            Childs_List_Data(const std::string& _mask, Childs_List* _ptr) : name_mask(_mask), childs_list_ptr(_ptr) { }
            LST::Mask name_mask;
            Childs_List* childs_list_ptr = nullptr;
        };
        using Childs_Lists = LDS::List<Childs_List_Data>;

	public:
        static std::string get_estimated_type() { return ""; }
        static std::string get_estimated_history() { return ""; }
        virtual std::string get_actual_type() const { return ""; }
        virtual std::string get_actual_history() const { return "/LV::Variable_Base"; }

        virtual Childs_Container_Type get_childs() { return {}; }
        virtual Childs_Lists get_childs_lists() { return {}; }

    protected:
        void M_register_child(Childs_Container_Type& _container, const std::string& _child_name, Variable_Base** _child_ptr);

    protected:
        virtual void M_assign_values(const MDL_Variable_Stub& _stub) {  }
        void M_init_childs(const MDL_Variable_Stub& _stub);

    public:
        void assign_values(const MDL_Variable_Stub& _stub);

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

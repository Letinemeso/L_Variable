#pragma once

#include <string>
#include <cstring>

#include <Stuff/Mask.h>
#include <Stuff/Function_Wrapper.h>
#include <Data_Structures/List.h>
#include <Data_Structures/Map.h>

#include <Type_Manager.h>
#include <MDL_Reader.h>

#ifndef VARIABLE
#define VARIABLE

    #define INIT_VARIABLE(variable_type, parent_type) \
        private: \
            static inline std::string m_type; \
            static inline std::string m_history; \
        private: \
            using Current_Type = variable_type; \
            using Parent_Type = parent_type; \
        public: \
            static const std::string& get_estimated_type() \
            { \
                if(variable_type::m_type.size() == 0) \
                    variable_type::m_type = #variable_type; \
                return variable_type::m_type; \
            } \
            static const std::string& get_estimated_history() \
            { \
                if(variable_type::m_history.size() == 0) \
                    variable_type::m_history = parent_type::get_estimated_history() + "/" + #variable_type; \
                return variable_type::m_history; \
            } \
            const std::string& get_actual_type() const override { return variable_type::get_estimated_type(); } \
            const std::string& get_actual_history() const override { return variable_type::get_estimated_history(); }


    #define INIT_FIELDS \
        LV::Variable_Base::Fields_Data_List get_fields() override { \
            LV::Variable_Base::Fields_Data_List fields_result = Parent_Type::get_fields();

    #define ADD_FIELD(field_type, field_reference) \
            { \
                LV::Variable_Base::Field_Data field_data; \
                field_data.name = #field_reference; \
                field_data.type_name = #field_type; \
                void* field_ptr = &field_reference; \
                field_data.init_func = [field_ptr](const LV::MDL_Variable_Stub& _stub) \
                { \
                    LV::MDL_Variable_Stub::Fields_Map::Const_Iterator check = _stub.fields.find(#field_reference); \
                    if(check.is_ok()) \
                        LV::Type_Manager::parse(#field_type, *check, field_ptr); \
                }; \
                field_data.serialization_func = [field_ptr]() \
                { \
                    return LV::Type_Manager::serialize(#field_type, field_ptr); \
                }; \
                fields_result.push_back(field_data); \
            }

    #define ADD_FIELD_RENAMED(field_type, field_name, field_reference) \
            { \
                    LV::Variable_Base::Field_Data field_data; \
                    field_data.name = field_name; \
                    field_data.type_name = #field_type; \
                    void* field_ptr = &field_reference; \
                    field_data.init_func = [field_ptr](const LV::MDL_Variable_Stub& _stub) \
                    { \
                        LV::MDL_Variable_Stub::Fields_Map::Const_Iterator check = _stub.fields.find(field_name); \
                        if(check.is_ok()) \
                            LV::Type_Manager::parse(#field_type, *check, field_ptr); \
                    }; \
                    field_data.serialization_func = [field_ptr]() \
                    { \
                        return LV::Type_Manager::serialize(#field_type, field_ptr); \
                    }; \
                    fields_result.push_back(field_data); \
            }

    #define FIELDS_END \
                return fields_result; \
            }


    #define INIT_CHILDS \
        public: \
            LV::Variable_Base::Childs_Container_Type get_childs() override { \
                LV::Variable_Base::Childs_Container_Type childs_result = Parent_Type::get_childs();

    #define ADD_CHILD(child_name, child_ptr) \
                M_register_child(childs_result, child_name, (LV::Variable_Base**)(&child_ptr));

    #define CHILDS_END \
                return childs_result; \
            }


    #define INIT_CHILDS_LISTS \
        public: \
            LV::Variable_Base::Childs_Lists get_childs_lists() override { \
                LV::Variable_Base::Childs_Lists childs_lists_result = Parent_Type::get_childs_lists();

    #define ADD_CHILDS_LIST(mask, childs_list_ptr) \
                childs_lists_result.push_back(Childs_List_Data(mask, childs_list_ptr));

    #define CHILDS_LISTS_END \
                return childs_lists_result; \
            }


    #define OVERRIDE_ON_VALUES_ASSIGNED protected: void M_on_values_assigned() override;

    #define ON_VALUES_ASSIGNED_IMPLEMENTATION(type) void type::M_on_values_assigned()


#endif


namespace LV
{

	class Variable_Base
	{
    public:
        friend class Object_Constructor;

    public:
        using Childs_Container_Type = LDS::Map<std::string, Variable_Base**>;
        using Init_Field_Func = LST::Function<void(const MDL_Variable_Stub&)>;
        using Serialize_Field_Func = LST::Function<LDS::Vector<std::string>()>;

    public:
        struct Field_Data
        {
            std::string name;
            std::string type_name;
            Init_Field_Func init_func;
            Serialize_Field_Func serialization_func;
        };
        using Fields_Data_List = LDS::List<Field_Data>;

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

    private:
        static inline std::string m_type;
        static inline std::string m_history;

	public:
        static const std::string& get_estimated_type();
        static const std::string& get_estimated_history();
        virtual const std::string& get_actual_type() const;
        virtual const std::string& get_actual_history() const;

        virtual Fields_Data_List get_fields() { return {}; }
        virtual Childs_Container_Type get_childs() { return {}; }
        virtual Childs_Lists get_childs_lists() { return {}; }

    protected:
        void M_register_child(Childs_Container_Type& _container, const std::string& _child_name, Variable_Base** _child_ptr);

    protected:
        void M_assign_values(const MDL_Variable_Stub& _stub);
        void M_init_childs(const MDL_Variable_Stub& _stub);
        virtual void M_on_values_assigned() { }

        void M_pack_fields(MDL_Variable_Stub& _stub);
        void M_pack_childs(MDL_Variable_Stub& _stub);
        void M_pack_childs_lists(MDL_Variable_Stub& _stub);

    protected:
        void clear_childs_list(Childs_List& _list);

    public:
        void assign_values(const MDL_Variable_Stub& _stub);
        MDL_Variable_Stub pack();

	public:
		Variable_Base();
		virtual ~Variable_Base();

	};





	template<typename T>
	const T* cast_variable(const Variable_Base* _var)
	{
		if(_var == nullptr)
			return nullptr;

        const std::string& T_type = T::get_estimated_history();
        const std::string& var_type = _var->get_actual_history();

        if(var_type.size() < T_type.size())
			return nullptr;

        if(std::memcmp(T_type.data(), var_type.data(), T_type.size()) != 0)
            return nullptr;

		return (T*)_var;
	}

    template<typename T>
    inline T* cast_variable(Variable_Base* _var)
    {
        return (T*)cast_variable<T>((const Variable_Base*)_var);
    }

}

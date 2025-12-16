#pragma once

#include <Data_Structures/List.h>
#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>
#include <Stuff/Mask.h>

#include <Variable_Base.h>

namespace LV
{

    class Object_Constructor
    {
    public:
        using Construction_Func_Type = LST::Function<LV::Variable_Base*()>;
        using Initialization_Func_Type = LST::Function<void(LV::Variable_Base*)>;

    private:
        struct Object_Construction_Tools final
        {
            Construction_Func_Type construction_func;
            Initialization_Func_Type initialization_func;
        };
        using Registred_Types_Container = LDS::Map<std::string, Object_Construction_Tools>;

    public:
        class Tools_Configurator final
        {
        private:
            Object_Construction_Tools& m_type_stuff_ref;

        private:
            Tools_Configurator() = delete;
            void operator=(const Tools_Configurator& _other) = delete;
            void operator=(Tools_Configurator&& _other) = delete;

        public:
            Tools_Configurator(const Tools_Configurator& _other) : m_type_stuff_ref(_other.m_type_stuff_ref) { }
            Tools_Configurator(Tools_Configurator&& _other) : m_type_stuff_ref(_other.m_type_stuff_ref) { }

        private:
            friend class Object_Constructor;
            Tools_Configurator(Object_Construction_Tools& _ref) : m_type_stuff_ref(_ref) { }

        public:
            ~Tools_Configurator() { }

        public:
            Tools_Configurator& override_constructor_func(Construction_Func_Type _func);
            Tools_Configurator& override_initialization_func(Initialization_Func_Type _func);

        };

    private:
        Registred_Types_Container m_registred_types;

    public:
        Object_Constructor();
        ~Object_Constructor();

    public:
        template<typename Variable_Base_Child_Type>
        Tools_Configurator register_type(const std::string& _override_name = Variable_Base_Child_Type::get_estimated_type());
        template<typename Variable_Base_Child_Type>
        Tools_Configurator register_parent_type();

    private:
        std::string M_extract_type_from_history(const std::string& _type_history) const;
        void M_construct_childs_arrays(LV::Variable_Base* _object, const LV::Variable_Base::Childs_List_Data& _childs_list_data, const MDL_Variable_Stub& _mdl_stub) const;
        void M_initialize_constructed_object(LV::Variable_Base* _object, const std::string& _type_history) const;

    public:
        LV::Variable_Base* construct(const std::string& _type_name) const;
        LV::Variable_Base* construct(const MDL_Variable_Stub& _mdl_stub) const;

    };



    template<typename Variable_Base_Child_Type>
    Object_Constructor::Tools_Configurator Object_Constructor::register_type(const std::string& _override_name)
    {
        std::string type_name = _override_name;
        if(type_name.size() == 0)
            type_name = Variable_Base_Child_Type::get_estimated_type();

        L_ASSERT(type_name.size() > 0);

        Registred_Types_Container::Iterator maybe_registred_type = m_registred_types.find(type_name);
        if(maybe_registred_type.is_ok())
            return Tools_Configurator(*maybe_registred_type);

        Object_Construction_Tools construction_tools;
        construction_tools.construction_func = [this](){ return new Variable_Base_Child_Type; };

        maybe_registred_type = m_registred_types.insert_and_get_iterator(type_name, construction_tools);
        return Tools_Configurator(*maybe_registred_type);
    }

    template<typename Variable_Base_Child_Type>
    Object_Constructor::Tools_Configurator Object_Constructor::register_parent_type()
    {
        std::string type_name = Variable_Base_Child_Type::get_estimated_type();

        Registred_Types_Container::Iterator maybe_registred_type = m_registred_types.find(type_name);
        if(maybe_registred_type.is_ok())
            return Tools_Configurator(*maybe_registred_type);

        Object_Construction_Tools construction_tools;
        maybe_registred_type = m_registred_types.insert_and_get_iterator(type_name, construction_tools);
        return Tools_Configurator(*maybe_registred_type);
    }

}

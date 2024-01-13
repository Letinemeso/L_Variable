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
        using Childs_Map = LDS::Map<std::string, LV::Variable_Base*>;
        using Childs_Map_Extraction_Func = LST::Function<Childs_Map&(LV::Variable_Base*)>;

    private:
        struct Childs_Array_Construction_Tools final
        {
            LST::Mask name_mask;
            Childs_Map_Extraction_Func childs_extraction_func;
        };
        using Childs_Array_Construction_Tools_List = LDS::List<Childs_Array_Construction_Tools>;

        struct Object_Construction_Tools final
        {
            Construction_Func_Type construction_func;
            Initialization_Func_Type initialization_func;
            Childs_Array_Construction_Tools_List childs_array_construction_tools_list;
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
            Tools_Configurator& add_childs_array(const std::string& _childs_name_mask, const Childs_Map_Extraction_Func& _extraction_func);

        };

    private:
        Registred_Types_Container m_registred_types;

    public:
        Object_Constructor();
        ~Object_Constructor();

    public:
        template<typename Variable_Base_Child_Type>
        Tools_Configurator register_type(const std::string& _override_name = Variable_Base_Child_Type::get_estimated_type());

    private:
        std::string M_extract_type_from_history(const std::string& _type_history) const;
        void M_construct_childs_arrays(LV::Variable_Base* _object, const Childs_Array_Construction_Tools& _tools, const MDL_Variable_Stub& _mdl_stub) const;
        void M_initialize_constructed_object(LV::Variable_Base* _object, const std::string& _type_history, const MDL_Variable_Stub& _mdl_stub) const;

    public:
        LV::Variable_Base* construct(const MDL_Variable_Stub& _mdl_stub) const;

    };



    template<typename Variable_Base_Child_Type>
    Object_Constructor::Tools_Configurator Object_Constructor::register_type(const std::string& _override_name)
    {
        Object_Construction_Tools construction_tools;

        construction_tools.construction_func = [this](){ return new Variable_Base_Child_Type; };

        std::string type_name = _override_name;
        if(type_name.size() == 0)
            type_name = Variable_Base_Child_Type::get_estimated_type();

        L_ASSERT(type_name.size() > 0);
        L_ASSERT(m_registred_types.find(type_name).is_ok() == false);

        m_registred_types.insert(type_name, construction_tools);

        Object_Construction_Tools& construction_tools_reference = *m_registred_types.find(type_name);

        return Tools_Configurator(construction_tools_reference);
    }

}

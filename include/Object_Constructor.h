#ifndef OBJECT_CONSTRUCTOR_H
#define OBJECT_CONSTRUCTOR_H

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Variable_Base.h>

namespace LV
{

    class Object_Constructor
    {
    public:
        using construction_func_type = LST::Function<LV::Variable_Base*()>;
        using initialization_func_type = LST::Function<void(LV::Variable_Base*)>;

    private:
        struct Object_Construction_Tools final
        {
            construction_func_type construction_func;
            initialization_func_type initialization_func;
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
            Tools_Configurator& override_constructor_func(construction_func_type _func);
            Tools_Configurator& override_initialization_func(initialization_func_type _func);

        };

    private:
        Registred_Types_Container m_registred_types;

    public:
        Object_Constructor();
        ~Object_Constructor();

    public:
        template<typename Variable_Base_Child_Type>
        Tools_Configurator register_type(const std::string& _override_name = Variable_Base_Child_Type::get_estimated_type());

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

#endif // OBJECT_CONSTRUCTOR_H

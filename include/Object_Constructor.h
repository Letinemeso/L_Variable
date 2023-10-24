#ifndef OBJECT_CONSTRUCTOR_H
#define OBJECT_CONSTRUCTOR_H

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Variable_Base.h>

namespace LV
{

    class Object_Constructor
    {
    private:
        using constructor_func_type = LST::Function<LV::Variable_Base*(const LV::MDL_Variable_Stub&)>;

    private:
        LDS::Map<std::string, constructor_func_type> m_registred_types;

    public:
        Object_Constructor();
        ~Object_Constructor();

    public:
        template<typename Variable_Base_Child_Type>
        void register_type(const std::string& _override_name = std::string());

    public:
        LV::Variable_Base* construct(const MDL_Variable_Stub& _mdl_stub) const;

    };



    template<typename Variable_Base_Child_Type>
    void Object_Constructor::register_type(const std::string& _override_name)
    {
        constructor_func_type construction_func = [](const LV::MDL_Variable_Stub& _mdl_stub)
        {
            LV::Variable_Base* result = new Variable_Base_Child_Type;
            return result;
        };

        std::string construction_func_name = _override_name;
        if(construction_func_name.size() == 0)
            construction_func_name = Variable_Base_Child_Type::get_estimated_type();

        L_ASSERT(construction_func_name.size() > 0);

        m_registred_types.insert((std::string&&)construction_func_name, (constructor_func_type&&)construction_func);
    }

}

#endif // OBJECT_CONSTRUCTOR_H

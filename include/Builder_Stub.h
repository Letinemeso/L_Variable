#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Variable_Base.h>


#ifndef BUILDER_STUB
#define BUILDER_STUB

    #define INIT_BUILDER_STUB(Type_To_Construct) \
    protected:  \
        using Constructed_Type = Type_To_Construct;     \
        LV::Variable_Base* M_construct_product() const override;    \
        void M_init_constructed_product(LV::Variable_Base* _product) const override;    \
    public: \
        static inline Constructed_Type* cast_product(LV::Variable_Base* _ptr)  \
        {   \
            L_ASSERT(LV::cast_variable<Constructed_Type>(_ptr));    \
            return (Constructed_Type*)_ptr; \
        }   \
    public:

    #define BUILDER_STUB_CONSTRUCTION_FUNC(Stub_Type)  LV::Variable_Base* Stub_Type::M_construct_product() const

    #define BUILDER_STUB_CONSTRUCTION_FUNC_DEFAULT_IMPL { return new Constructed_Type; }

    #define BUILDER_STUB_CONSTRUCTION_FUNC_CUSTOM_IMPL(RETURN_VALUE) { return RETURN_VALUE; }


    #define BUILDER_STUB_INITIALIZATION_FUNC(Stub_Type) void Stub_Type::M_init_constructed_product(LV::Variable_Base* _product) const

    #define BUILDER_STUB_PARENT_INITIALIZATION Parent_Type::M_init_constructed_product(_product)

    #define BUILDER_STUB_CAST_PRODUCT Constructed_Type* product = (Constructed_Type*)_product

#endif


namespace LV
{

    class Builder_Stub : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LV::Builder_Stub, LV::Variable_Base)

    private:
        LST::Function<void(Variable_Base*)> m_extract_data_func;

    public:
        inline void set_extract_data_func(const LST::Function<void(Variable_Base*)> _func) { m_extract_data_func = _func; }

    protected:
        virtual LV::Variable_Base* M_construct_product() const = 0;
        virtual void M_init_constructed_product(LV::Variable_Base* /*_product*/) const {}

    public:
        LV::Variable_Base* construct() const;

    };

}

#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Variable_Base.h>


namespace LV
{

    class Builder_Stub : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

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

#pragma once

#include <L_Debug/L_Debug.h>
#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>


namespace LV
{

    class Variable_Loader : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LV::Variable_Loader, LV::Variable_Base)

    public:
        using Variables_Map = LDS::Map<std::string, LV::Variable_Base*>;

    private:
        Variables_Map m_variables;

    public:
        Variable_Loader();
        ~Variable_Loader();

        Variable_Loader(const Variable_Loader&) = delete;
        Variable_Loader(Variable_Loader&&) = delete;

    public:
        void add_variable(const std::string& _name, LV::Variable_Base* _variable, bool _rewrite = false);   //  takes ownership
        void remove_variable(const std::string& _name);

        void clear();

    public:
        template<typename _Variable_Type>
        _Variable_Type* get_variable(const std::string& _name) const;

    };

    template<typename _Variable_Type>
    _Variable_Type* Variable_Loader::get_variable(const std::string& _name) const
    {
        Variables_Map::Const_Iterator maybe_variable_it = m_variables.find(_name);
        L_ASSERT(maybe_variable_it.is_ok());

        L_ASSERT(LV::cast_variable<_Variable_Type>(*maybe_variable_it));
        _Variable_Type* result = (_Variable_Type*)*maybe_variable_it;

        return result;
    }


    class Variable_Loader_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LV::Variable_Loader_Stub, LV::Builder_Stub)

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("*", &variables_stubs)
        CHILDS_LISTS_END

    public:
        LV::Variable_Base::Childs_List variables_stubs;

    public:
        ~Variable_Loader_Stub();

    protected:
        INIT_BUILDER_STUB(Variable_Loader)

    };

}

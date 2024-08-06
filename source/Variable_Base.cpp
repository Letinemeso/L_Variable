#include "Variable_Base.h"

using namespace LV;


Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

}



const std::string& Variable_Base::get_estimated_type()
{
    if(m_type.size() == 0)
        m_type = "LV::Variable_Base";
    return m_type;
}

const std::string& Variable_Base::get_estimated_history()
{
    if(m_history.size() == 0)
        m_history = "/LV::Variable_Base";
    return m_history;
}

const std::string& Variable_Base::get_actual_type() const
{
    return Variable_Base::get_estimated_type();
}

const std::string& Variable_Base::get_actual_history() const
{
    return Variable_Base::get_estimated_type();
}



void Variable_Base::M_register_child(Childs_Container_Type& _container, const std::string& _child_name, Variable_Base** _child_ptr)
{
    Childs_Container_Type::Iterator check = _container.find(_child_name);
    L_ASSERT(!check.is_ok());
    _container.insert(_child_name, _child_ptr);
}



void Variable_Base::M_init_childs(const MDL_Variable_Stub& _stub)
{
    Childs_Container_Type childs = get_childs();

    for(LDS::Map<std::string, Variable_Base**>::Iterator it = childs.iterator(); !it.end_reached(); ++it)
    {
        L_ASSERT(*(*it) != nullptr);

        const std::string& name = it.key();

        LDS::Map<std::string, LV::MDL_Variable_Stub>::Const_Iterator maybe_child_stub = _stub.childs.find(name);
        L_ASSERT(maybe_child_stub.is_ok());

        (**it)->assign_values(*maybe_child_stub);
    }
}



void Variable_Base::clear_childs_list(Childs_List& _list)
{
    for(Childs_List::Iterator it = _list.begin(); !it.end_reached(); ++it)
        delete it->child_ptr;
    _list.clear();
}



void Variable_Base::assign_values(const MDL_Variable_Stub& _stub)
{
    M_assign_values(_stub);
    M_init_childs(_stub);
}

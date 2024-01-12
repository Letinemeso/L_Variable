#include "Variable_Base.h"

using namespace LV;


Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

}


void Variable_Base::M_register_child(const std::string& _child_name, Variable_Base** _child_ptr)
{
    Childs_Container_Type::Iterator check = m_childs.find(_child_name);
    L_ASSERT(!check.is_ok());
    m_childs.insert(_child_name, _child_ptr);
}

void Variable_Base::M_assign_values(const MDL_Variable_Stub& /*_stub*/)
{
    m_type_history = "";
}

void Variable_Base::M_init_childs(const MDL_Variable_Stub& _stub)
{
    for(LDS::Map<std::string, Variable_Base**>::Iterator it = m_childs.iterator(); !it.end_reached(); ++it)
    {
        L_ASSERT(*(*it) != nullptr);

        const std::string& name = it.key();

        LDS::Map<std::string, LV::MDL_Variable_Stub>::Const_Iterator maybe_child_stub = _stub.childs.find(name);
        L_ASSERT(maybe_child_stub.is_ok());

        (**it)->assign_values(*maybe_child_stub);
    }
}



void Variable_Base::assign_values(const MDL_Variable_Stub& _stub)
{
    M_assign_values(_stub);
    M_init_childs(_stub);
}

#include "Variable_Base.h"

using namespace LV;


Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

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



void Variable_Base::assign_values(const MDL_Variable_Stub& _stub)
{
    M_assign_values(_stub);
    M_init_childs(_stub);
}

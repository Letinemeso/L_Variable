#include "Variable_Base.h"

using namespace LV;


Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

}



void Variable_Base::init_childs(const MDL_Variable_Stub& _stub)
{
    for(LDS::Map<std::string, Variable_Base**>::Iterator it = m_childs.iterator(); !it.end_reached(); ++it)
    {
        L_ASSERT(*(*it) != nullptr);

        const std::string& name = it.key();

        LDS::Map<std::string, LV::MDL_Variable_Stub>::Const_Iterator maybe_child_stub = _stub.childs.find(name);
        L_ASSERT(maybe_child_stub.is_ok());

        (**it)->assign_values(*maybe_child_stub);
        (**it)->init_childs(*maybe_child_stub);
    }
}

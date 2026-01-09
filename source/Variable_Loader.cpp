#include <Variable_Loader.h>


using namespace LV;


Variable_Loader::Variable_Loader()
{

}

Variable_Loader::~Variable_Loader()
{
    clear();
}



void Variable_Loader::add_variable(const std::string& _name, LV::Variable_Base* _variable, bool _rewrite)
{
    L_ASSERT(_variable);
    L_ASSERT(_name.size() > 0);

    Variables_Map::Iterator maybe_loaded_variable_it = m_variables.find(_name);
    if(!maybe_loaded_variable_it.is_ok())
        return m_variables.insert(_name, _variable);

    L_ASSERT(_rewrite);     //  if rewrite is not allowed, program should crash if variable with same name already loaded

    delete *maybe_loaded_variable_it;
    *maybe_loaded_variable_it = _variable;
}

void Variable_Loader::remove_variable(const std::string& _name)
{
    Variables_Map::Iterator maybe_loaded_variable_it = m_variables.find(_name);
    L_ASSERT(!maybe_loaded_variable_it.is_ok());

    delete *maybe_loaded_variable_it;
    m_variables.erase(maybe_loaded_variable_it);
}


void Variable_Loader::clear()
{
    for(Variables_Map::Iterator it = m_variables.iterator(); !it.end_reached(); ++it)
        delete *it;

    m_variables.clear();
}





Variable_Loader_Stub::~Variable_Loader_Stub()
{
    for(LV::Variable_Base::Childs_List::Iterator it = variables_stubs.begin(); !it.end_reached(); ++it)
        delete it->child_ptr;
}



BUILDER_STUB_CONSTRUCTION_FUNC(Variable_Loader_Stub) BUILDER_STUB_CONSTRUCTION_FUNC_DEFAULT_IMPL

BUILDER_STUB_INITIALIZATION_FUNC(Variable_Loader_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    for(LV::Variable_Base::Childs_List::Const_Iterator it = variables_stubs.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(LV::cast_variable<LV::Builder_Stub>(it->child_ptr));
        product->add_variable(it->name, ((LV::Builder_Stub*)it->child_ptr)->construct());
    }
}

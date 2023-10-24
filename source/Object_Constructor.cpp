#include <Object_Constructor.h>

using namespace LV;


Object_Constructor::Object_Constructor()
{

}

Object_Constructor::~Object_Constructor()
{

}



LV::Variable_Base* Object_Constructor::construct(const MDL_Variable_Stub& _mdl_stub) const
{
    LDS::Map<std::string, LDS::Vector<std::string>>::Const_Iterator maybe_type_field = _mdl_stub.fields.find("type");
    L_ASSERT(maybe_type_field.is_ok());
    L_ASSERT((*maybe_type_field).size() == 1);

    std::string type_str = (*maybe_type_field)[0];

    LDS::Map<std::string, constructor_func_type>::Const_Iterator maybe_registred_type = m_registred_types.find(type_str);
    L_ASSERT(maybe_registred_type.is_ok());

    const constructor_func_type& construction_func = *maybe_registred_type;

    LV::Variable_Base* result = construction_func(_mdl_stub);
    result->assign_values(_mdl_stub);

    const LV::Variable_Base::Childs_Container_Type& results_childs = result->get_childs();

    for(LV::Variable_Base::Childs_Container_Type::Const_Iterator it = results_childs.iterator(); !it.end_reached(); ++it)
    {
        auto maybe_child_stub = _mdl_stub.childs.find(it.key());
        L_ASSERT(maybe_child_stub.is_ok());

        L_ASSERT(**it == nullptr);

        **it = construct(*maybe_child_stub);
    }

    result->on_values_assigned();

    return result;
}

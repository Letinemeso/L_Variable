#include <Object_Constructor.h>

using namespace LV;


Object_Constructor::Tools_Configurator& Object_Constructor::Tools_Configurator::override_constructor_func(construction_func_type _func)
{
    m_type_stuff_ref.construction_func = _func;
    return *this;
}

Object_Constructor::Tools_Configurator& Object_Constructor::Tools_Configurator::override_initialization_func(initialization_func_type _func)
{
    m_type_stuff_ref.initialization_func = _func;
    return *this;
}



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

    Registred_Types_Container::Const_Iterator maybe_registred_type = m_registred_types.find(type_str);
    L_ASSERT(maybe_registred_type.is_ok());

    const construction_func_type& construction_func = maybe_registred_type->construction_func;
    L_ASSERT(construction_func);

    LV::Variable_Base* result = construction_func();
    result->assign_values(_mdl_stub);

    const LV::Variable_Base::Childs_Container_Type& results_childs = result->get_childs();

    for(LV::Variable_Base::Childs_Container_Type::Const_Iterator it = results_childs.iterator(); !it.end_reached(); ++it)
    {
        auto maybe_child_stub = _mdl_stub.childs.find(it.key());

        if(!maybe_child_stub.is_ok())
            continue;

        L_ASSERT(**it == nullptr);

        **it = construct(*maybe_child_stub);
    }

    result->on_values_assigned();

    const initialization_func_type& initialization_func = maybe_registred_type->initialization_func;

    if(initialization_func)
        initialization_func(result);

    return result;
}

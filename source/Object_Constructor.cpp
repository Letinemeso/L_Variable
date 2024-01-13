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



void Object_Constructor::M_initialize_constructed_object(LV::Variable_Base* _object, const std::string& _type_history) const
{
    if(_type_history.size() <= 1)
        return;

    unsigned int slash_index = _type_history.size() - 1;
    for(; slash_index > 0; --slash_index)
    {
        if(_type_history[slash_index] == '/')
            break;
    }

    std::string type = _type_history.substr(slash_index + 1);
    std::string remaining_history = _type_history.substr(0, slash_index);

    M_initialize_constructed_object(_object, remaining_history);

    Registred_Types_Container::Const_Iterator maybe_registred_type = m_registred_types.find(type);  //  maybe not the most efficient way of doing this
    if(!maybe_registred_type.is_ok())
        return;

    const initialization_func_type& initialization_func = maybe_registred_type->initialization_func;

    if(initialization_func)
        initialization_func(_object);
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
    result->M_assign_values(_mdl_stub);

    const LV::Variable_Base::Childs_Container_Type& results_childs = result->get_childs();

    for(LV::Variable_Base::Childs_Container_Type::Const_Iterator it = results_childs.iterator(); !it.end_reached(); ++it)
    {
        auto maybe_child_stub = _mdl_stub.childs.find(it.key());

        if(!maybe_child_stub.is_ok())
            continue;

        L_ASSERT(**it == nullptr);  //  this may be wrong. if child is not nullptr, assign_values should be called (probably)

        **it = construct(*maybe_child_stub);
    }

    M_initialize_constructed_object(result, result->get_actual_history());

    return result;
}

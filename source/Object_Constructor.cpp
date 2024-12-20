#include <Object_Constructor.h>

using namespace LV;


Object_Constructor::Tools_Configurator& Object_Constructor::Tools_Configurator::override_constructor_func(Construction_Func_Type _func)
{
    m_type_stuff_ref.construction_func = _func;
    return *this;
}

Object_Constructor::Tools_Configurator& Object_Constructor::Tools_Configurator::override_initialization_func(Initialization_Func_Type _func)
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



std::string Object_Constructor::M_extract_type_from_history(const std::string& _type_history) const
{
    unsigned int slash_index = _type_history.size() - 1;
    for(; slash_index > 0; --slash_index)
    {
        if(_type_history[slash_index] == '/')
            break;
    }

    return _type_history.substr(slash_index + 1);
}

void Object_Constructor::M_construct_childs_arrays(LV::Variable_Base* _object, const LV::Variable_Base::Childs_List_Data& _childs_list_data, const MDL_Variable_Stub& _mdl_stub) const
{
    const LST::Mask& name_mask = _childs_list_data.name_mask;

    LV::Variable_Base::Childs_List& childs_list = *_childs_list_data.childs_list_ptr;

    for(LV::MDL_Variable_Stub::Child_Names_Order_List::Const_Iterator mdl_child_names_it = _mdl_stub.child_names_order.begin(); !mdl_child_names_it.end_reached(); ++mdl_child_names_it)
    {
        const std::string& child_name = *mdl_child_names_it;

        if(!name_mask.compare(child_name))
            continue;

        LV::MDL_Variable_Stub::Childs_Map::Const_Iterator child_it = _mdl_stub.childs.find(child_name);
        L_ASSERT(child_it.is_ok());     //  just in case

        LV::Variable_Base* child = construct(*child_it);

        childs_list.push_back({child_name, child});
    }
}

void Object_Constructor::M_initialize_constructed_object(LV::Variable_Base* _object, const std::string& _type_history, const MDL_Variable_Stub& _mdl_stub) const
{
    if(_type_history.size() <= 1)
        return;

    std::string type = M_extract_type_from_history(_type_history);
    std::string remaining_history = _type_history.substr(0, _type_history.size() - 1 - type.size());

    M_initialize_constructed_object(_object, remaining_history, _mdl_stub);

    Registred_Types_Container::Const_Iterator maybe_registred_type = m_registred_types.find(type);  //  maybe not the most efficient way of doing this
    if(!maybe_registred_type.is_ok())
        return;

    const Initialization_Func_Type& initialization_func = maybe_registred_type->initialization_func;

    if(initialization_func)
        initialization_func(_object);

    _object->M_on_values_assigned();
}



LV::Variable_Base* Object_Constructor::construct(const MDL_Variable_Stub& _mdl_stub) const
{
    LDS::Map<std::string, LDS::Vector<std::string>>::Const_Iterator maybe_type_field = _mdl_stub.fields.find("type");
    L_ASSERT(maybe_type_field.is_ok());
    L_ASSERT((*maybe_type_field).size() == 1);

    std::string type_str = (*maybe_type_field)[0];

    Registred_Types_Container::Const_Iterator maybe_registred_type = m_registred_types.find(type_str);
    L_ASSERT(maybe_registred_type.is_ok());

    const Construction_Func_Type& construction_func = maybe_registred_type->construction_func;
    L_ASSERT(construction_func);

    LV::Variable_Base* result = construction_func();
    result->M_assign_values(_mdl_stub);

    const LV::Variable_Base::Childs_Container_Type& results_childs = result->get_childs();

    for(LV::Variable_Base::Childs_Container_Type::Const_Iterator it = results_childs.iterator(); !it.end_reached(); ++it)
    {
        auto maybe_child_stub = _mdl_stub.childs.find(it.key());

        if(!maybe_child_stub.is_ok())
            continue;

        LV::Variable_Base*& child_ptr_ref = **it;

        if(child_ptr_ref == nullptr)
            child_ptr_ref = construct(*maybe_child_stub);
        else
        {
            child_ptr_ref->assign_values(*maybe_child_stub);
            M_initialize_constructed_object(child_ptr_ref, child_ptr_ref->get_actual_history(), *maybe_child_stub);
        }
    }

    LV::Variable_Base::Childs_Lists childs_lists = result->get_childs_lists();
    for(LV::Variable_Base::Childs_Lists::Iterator it = childs_lists.begin(); !it.end_reached(); ++it)
        M_construct_childs_arrays(result, *it, _mdl_stub);

    std::string real_object_type = M_extract_type_from_history(result->get_actual_history());
    std::string type_history = result->get_actual_history().substr(0, result->get_actual_history().size() - 1 - real_object_type.size()) + '/' + type_str;  //  object type can be overridden

    M_initialize_constructed_object(result, type_history, _mdl_stub);

    return result;
}

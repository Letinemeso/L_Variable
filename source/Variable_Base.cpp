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



void Variable_Base::M_assign_values(const MDL_Variable_Stub& _stub)
{
    Fields_Data_List fields = get_fields();
    for(Fields_Data_List::Iterator it = fields.begin(); !it.end_reached(); ++it)
    {
        const Field_Data& field_data = *it;
        field_data.init_func(_stub);
    }
}

void Variable_Base::M_init_childs(const MDL_Variable_Stub& _stub)
{
    Childs_Container_Type childs = get_childs();

    for(Childs_Container_Type::Iterator it = childs.iterator(); !it.end_reached(); ++it)
    {
        L_ASSERT(*(*it) != nullptr);

        const std::string& name = it.key();

        MDL_Variable_Stub::Childs_Map::Const_Iterator maybe_child_stub = _stub.childs.find(name);
        L_ASSERT(maybe_child_stub.is_ok());

        (**it)->assign_values(*maybe_child_stub);
    }
}


void Variable_Base::M_pack_fields(MDL_Variable_Stub& _stub)
{
    Fields_Data_List fields = get_fields();
    for(Fields_Data_List::Iterator it = fields.begin(); !it.end_reached(); ++it)
    {
        const Field_Data& field_data = *it;
        _stub.fields.insert(field_data.name, field_data.serialization_func());
    }
}

void Variable_Base::M_pack_childs(MDL_Variable_Stub& _stub)
{
    Childs_Container_Type childs = get_childs();
    for(Childs_Container_Type::Iterator it = childs.iterator(); !it.end_reached(); ++it)
    {
        _stub.child_names_order.push_back(it.key());
        _stub.childs.insert(it.key(), (**it)->pack());
    }
}

void Variable_Base::M_pack_childs_lists(MDL_Variable_Stub& _stub)
{
    Childs_Lists childs_lists = get_childs_lists();
    for(Childs_Lists::Iterator lists_it = childs_lists.begin(); !lists_it.end_reached(); ++lists_it)
    {
        Childs_List& childs_list = *lists_it->childs_list_ptr;
        for(Childs_List::Iterator it = childs_list.begin(); !it.end_reached(); ++it)
        {
            _stub.child_names_order.push_back(it->name);
            _stub.childs.insert(it->name, it->child_ptr->pack());
        }
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
    M_on_values_assigned();
}

MDL_Variable_Stub Variable_Base::pack()
{
    MDL_Variable_Stub result;

    M_pack_fields(result);
    M_pack_childs(result);
    M_pack_childs_lists(result);

    return result;
}

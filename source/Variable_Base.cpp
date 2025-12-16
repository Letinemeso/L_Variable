#include "Variable_Base.h"

#include <Object_Constructor.h>

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


void Variable_Base::M_copy_fieds(LV::Variable_Base* _to)
{
    Fields_Data_List this_fields = get_fields();
    Fields_Data_List result_fields = _to->get_fields();
    L_ASSERT(this_fields.size() == result_fields.size());

    Fields_Data_List::Iterator this_it = this_fields.begin();
    Fields_Data_List::Iterator result_it = result_fields.begin();

    while(!this_it.end_reached() && !result_it.end_reached())
    {
        Field_Data& this_fd = *this_it;
        Field_Data& result_fd = *result_it;

        L_ASSERT(this_fd.name == result_fd.name);
        L_ASSERT(this_fd.type_name == result_fd.type_name);

        this_fd.copy_func(result_fd.field_ptr);

        ++this_it;
        ++result_it;
    }
}

void Variable_Base::M_copy_childs(LV::Variable_Base* _to)
{
    Childs_Container_Type this_childs = get_childs();
    Childs_Container_Type copy_childs = _to->get_childs();

    for(Childs_Container_Type::Iterator it = copy_childs.iterator(); !it.end_reached(); ++it)
    {
        LV::Variable_Base** child_ptr = *it;

        Childs_Container_Type::Iterator corresponding_child_it = this_childs.find(it.key());
        L_ASSERT(corresponding_child_it.is_ok());

        LV::Variable_Base* this_child = **corresponding_child_it;
        if(!this_child)
            continue;

        if(!*child_ptr)
            *child_ptr = this_child->construct_copy();
        else
            this_child->M_init_copy(*child_ptr);
    }
}

void Variable_Base::M_copy_childs_lists(LV::Variable_Base* _to)
{
    Childs_Lists this_childs_lists = get_childs_lists();
    Childs_Lists copy_childs_lists = _to->get_childs_lists();

    Childs_Lists::Iterator this_list_it = this_childs_lists.begin();
    Childs_Lists::Iterator copy_list_it = copy_childs_lists.begin();

    while(!this_list_it.end_reached() && !copy_list_it.end_reached())
    {
        Childs_List& this_list = *this_list_it->childs_list_ptr;
        Childs_List& copy_list = *copy_list_it->childs_list_ptr;

        L_ASSERT(copy_list.size() == 0);

        for(Childs_List::Iterator it = this_list.begin(); !it.end_reached(); ++it)
        {
            copy_list.push_back({});
            Child_Data& child_data = *copy_list.end();
            child_data.name = it->name;
            child_data.child_ptr = it->child_ptr->construct_copy();
        }

        ++this_list_it;
        ++copy_list_it;
    }
}

void Variable_Base::M_init_copy(LV::Variable_Base* _copy)
{
    L_ASSERT(_copy);

    M_copy_fieds(_copy);
    M_copy_childs(_copy);
    M_copy_childs_lists(_copy);
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

Variable_Base* Variable_Base::construct_copy()
{
    L_ASSERT(m_object_constructor);

    Variable_Base* result = m_object_constructor->construct( get_actual_type() );
    M_init_copy(result);

    result->M_on_values_assigned();

    return result;
}

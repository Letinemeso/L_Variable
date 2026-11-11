#include <MDL_Variable.h>

using namespace LV;


MDL_Variable::MDL_Variable()
{

}

MDL_Variable::MDL_Variable(const MDL_Variable& _other)
    : m_fields(_other.m_fields)
    , m_childs(_other.m_childs)
    , m_child_names_order(_other.m_child_names_order)
{

}

MDL_Variable::MDL_Variable(MDL_Variable&& _other)
    : m_fields(LST::move(_other.m_fields))
    , m_childs(LST::move(_other.m_childs))
    , m_child_names_order(LST::move(_other.m_child_names_order))
{

}

void MDL_Variable::operator=(const MDL_Variable& _other)
{
    m_fields = _other.m_fields;
    m_childs = _other.m_childs;
    m_child_names_order = _other.m_child_names_order;
}

void MDL_Variable::operator=(MDL_Variable&& _other)
{
    m_fields = LST::move(_other.m_fields);
    m_childs = LST::move(_other.m_childs);
    m_child_names_order = LST::move(_other.m_child_names_order);
}



void MDL_Variable::merge_with(const MDL_Variable& _other)
{
    for(Fields_Map::Const_Iterator it = _other.m_fields.iterator(); !it.end_reached(); ++it)
    {
        const Field_Data_Vector& other_fields = *it;

        Fields_Map::Iterator this_fields_it = m_fields.find_or_insert(it.key());
        *this_fields_it = other_fields;
    }

    for(Childs_Map::Const_Iterator it = _other.m_childs.iterator(); !it.end_reached(); ++it)
    {
        Childs_Map::Iterator maybe_child_it = m_childs.find(it.key());
        if(maybe_child_it.is_ok())
            maybe_child_it->merge_with(*it);
        else
            m_childs.insert( it.key(), *it );
    }

    for(unsigned int i = 0; i < _other.m_child_names_order.size(); ++i)
    {
        const std::string& child_name = _other.m_child_names_order[i];

        if(!m_child_names_order.contains( child_name ))
            m_child_names_order.push( child_name );
    }
}

void MDL_Variable::merge_with(MDL_Variable&& _other)
{
    for(Fields_Map::Iterator it = _other.m_fields.iterator(); !it.end_reached(); ++it)
    {
        Field_Data_Vector& other_fields = *it;

        Fields_Map::Iterator this_fields_it = m_fields.find_or_insert(it.key());
        *this_fields_it = LST::move(other_fields);
    }

    for(Childs_Map::Iterator it = _other.m_childs.iterator(); !it.end_reached(); ++it)
    {
        Childs_Map::Iterator maybe_child_it = m_childs.find(it.key());
        if(maybe_child_it.is_ok())
            maybe_child_it->merge_with( LST::move(*it) );
        else
            m_childs.insert( it.key(), LST::move(*it) );
    }

    for(unsigned int i = 0; i < _other.m_child_names_order.size(); ++i)
    {
        const std::string& child_name = _other.m_child_names_order[i];

        if(!m_child_names_order.contains( child_name ))
            m_child_names_order.push( child_name );
    }
}

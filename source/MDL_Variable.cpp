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

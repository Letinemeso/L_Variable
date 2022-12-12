#include "Variable_Base.h"


void Variable_Base::assign_values(const Str_Values &)
{

}



Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

}



void Variable_Base::set_name(const std::string& _name)
{
	m_name = _name;
}

const std::string& Variable_Base::get_name() const
{
	return m_name;
}

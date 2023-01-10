#include "Variable_Base.h"

using namespace LV;


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

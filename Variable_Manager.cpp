#include "Variable_Manager.h"


Variable_Manager::Variable_Manager()
{

}

Variable_Manager::~Variable_Manager()
{
	Variables_Map::iterator it = m_variables.begin();
	while(it != m_variables.end())
	{
		delete it->second;
		++it;
	}
}



Variable_Manager::Type Variable_Manager::M_get_type(const std::string &_value) const
{
	if(_value.size() == 0)
		return Type::Unknown;

	if(_value[0] == '\"' && _value[_value.size() - 1] == '\"')
		return Type::String;

	if(_value == "true" || _value == "false")
		return Type::Bool;

	bool have_only_digits = true;
	unsigned int dots_amount = 0;
	for(unsigned int i=0; i<_value.size(); ++i)
	{
		if(_value[i] == '.')
		{
			++dots_amount;
			continue;
		}
		if(_value[i] < '0' || _value[i] > '9')
		{
			have_only_digits = false;
			break;
		}
	}

	if(have_only_digits && dots_amount == 0)
		return Type::Int;

	if(have_only_digits && dots_amount == 1)
		return Type::Float;

	return Type::Unknown;
}



void Variable_Manager::add_value(const Variable_Stub &_stub)
{
	if(_stub.name.size() == 0)
		return;

	Type type = M_get_type(_stub.value);

	if(type == Type::Unknown)
		return;

	Variable_Base* var_ptr = nullptr;

	switch (type) {
	case(Type::Int):
		var_ptr = new Int_Variable;
		break;
	case(Type::Bool):
		var_ptr = new Bool_Variable;
		break;
	case(Type::Float):
		var_ptr = new Float_Variable;
		break;
	case(Type::String):
		var_ptr = new String_Variable;
		break;
	default:
		break;
	}

	if(var_ptr == nullptr)
		return;

	var_ptr->set_name(_stub.name);

	if(type == Type::String)
		var_ptr->assign_values({{"value", _stub.value.substr(1, _stub.value.size() - 2)}});
	else
		var_ptr->assign_values({{"value", _stub.value}});

	m_variables.emplace(_stub.name, var_ptr);
}

void Variable_Manager::add_values(const std::list<Variable_Stub> &_raw_values)
{
	std::list<Variable_Stub>::const_iterator it = _raw_values.begin();
	while(it != _raw_values.end())
	{
		add_value(*it);
		++it;
	}
}

Variable_Base* Variable_Manager::get_variable(const std::string &_variable_name)
{
	Variables_Map::iterator it = m_variables.find(_variable_name);
	if(it == m_variables.end())
		return nullptr;
	return it->second;
}

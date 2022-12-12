#include "Variable_Manager.h"


Variable_Manager::Variable_Manager()
{

}

Variable_Manager::~Variable_Manager()
{
	std::list<Variable_Base*>::iterator it = m_variables.begin();
	while(it != m_variables.end())
	{
		delete *it;
		++it;
	}
}



Variable_Manager::Type Variable_Manager::M_get_type(const std::string &_value) const
{
	if(_value.size() == 0)
		return Type::Unknown;

	if(_value == "[]")
		return Type::Array;

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



Variable_Base* Variable_Manager::add_variable(const Variable_Stub &_stub, Array_Variable* _parent)
{
	Type type = M_get_type(_stub.value);

	if(type == Type::Unknown)
		return nullptr;

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
	case(Type::Array):
		var_ptr = new Array_Variable;
		break;
	default:
		break;
	}

	if(var_ptr == nullptr)
		return nullptr;

	var_ptr->set_name(_stub.name);

	if(type == Type::String)
		var_ptr->assign_values({{"value", _stub.value.substr(1, _stub.value.size() - 2)}});
	else
		var_ptr->assign_values({{"value", _stub.value}});

	m_variables.push_back(var_ptr);

	if(type == Type::Array)
		add_variables(_stub.childs, cast_variable<Array_Variable>(var_ptr));

	if(_parent != nullptr)
		_parent->add_child(var_ptr);

	return var_ptr;
}

void Variable_Manager::add_variables(const std::list<Variable_Stub> &_raw_values, Array_Variable* _parent)
{
	std::list<Variable_Stub>::const_iterator it = _raw_values.begin();
	while(it != _raw_values.end())
	{
		add_variable(*it, _parent);
		++it;
	}
}

Variable_Base* Variable_Manager::get_variable(const std::string &_variable_name)
{
	std::list<Variable_Base*>::iterator it = m_variables.begin();
	while(it != m_variables.end())
	{
		if((*it)->get_name() == _variable_name)
			return *it;
		++it;
	}
	return nullptr;
}


void Variable_Manager::add_variable(Variable_Base* _variable)
{
	std::list<Variable_Base*>::iterator it = m_variables.begin();
	while(it != m_variables.end())
	{
		if(*it == _variable)
			return;
		++it;
	}

	m_variables.push_back(_variable);

	Array_Variable* maybe_array = cast_variable<Array_Variable>(_variable);
	if(maybe_array == nullptr)
		return;

	it = maybe_array->get_childs().begin();
	while(it != maybe_array->get_childs().end())
	{
		add_variable(*it);
		++it;
	}
}

void Variable_Manager::exclude_variable(Variable_Base* _variable)
{
	std::list<Variable_Base*>::iterator it = m_variables.begin();
	while(it != m_variables.end())
	{
		if(*it == _variable)
		{
			m_variables.erase(it);
			_variable->set_parent(nullptr);
			break;
		}
		++it;
	}

	if(it == m_variables.end())
		return;

	Array_Variable* maybe_array = cast_variable<Array_Variable>(_variable);
	if(maybe_array == nullptr)
		return;

	while(maybe_array->get_childs().size() > 0)
		maybe_array->remove_child(*maybe_array->get_childs().begin());
}

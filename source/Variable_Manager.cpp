//#include "Variable_Manager.h"

//using namespace LV;


//Variable_Manager::Variable_Manager()
//{

//}

//Variable_Manager::~Variable_Manager()
//{
//	std::list<Variable_Base*>::iterator it = m_variables.begin();
//	while(it != m_variables.end())
//	{
//		delete *it;
//		++it;
//	}
//}



//Variable_Base* Variable_Manager::add_variable(const Variable_Stub &_stub, Array_Variable* _parent)
//{
//	Variable_Base* var_ptr = nullptr;


//	if(var_ptr == nullptr)
//		return nullptr;

//	var_ptr->set_name(_stub.name);



//	m_variables.push_back(var_ptr);



//	if(_parent != nullptr)
//		_parent->add_child(var_ptr);

//	return var_ptr;
//}

//void Variable_Manager::add_variables(const std::list<Variable_Stub> &_raw_values, Array_Variable* _parent, const std::string& _section)
//{
//	if(_section != "")
//	{
//		Variable_Manager* section = M_get_subsection(_section, true);
//		section->add_variables(_raw_values, _parent);
//		return;
//	}

//	std::list<Variable_Stub>::const_iterator it = _raw_values.begin();
//	while(it != _raw_values.end())
//	{
//		add_variable(*it, _parent);
//		++it;
//	}
//}


//void Variable_Manager::add_variables(const OMFL_Reader &_omfl_reader)
//{
//	using Data_t = OMFL_Reader::Parsed_Data_t;
//	const Data_t& data = _omfl_reader.parsed_data();

//	Data_t::const_iterator it = data.cbegin();
//	while(it != data.cend())
//	{
//		add_variables(it->second, nullptr, it->first);
//		++it;
//	}
//}


//Variable_Base* Variable_Manager::get_variable(const std::string &_variable_name)
//{
//	std::list<Variable_Base*>::iterator it = m_variables.begin();
//	while(it != m_variables.end())
//	{
//		if((*it)->get_name() == _variable_name)
//			return *it;
//		++it;
//	}
//	return nullptr;
//}

//Variable_Manager* Variable_Manager::get_section(const std::string &_name)
//{
//	if(_name == "")
//		return this;

//	return M_get_subsection(_name, false);
//}


//void Variable_Manager::add_variable(Variable_Base* _variable)
//{
//	std::list<Variable_Base*>::iterator it = m_variables.begin();
//	while(it != m_variables.end())
//	{
//		if(*it == _variable)
//			return;
//		++it;
//	}

//	m_variables.push_back(_variable);

//	Array_Variable* maybe_array = cast_variable<Array_Variable>(_variable);
//	if(maybe_array == nullptr)
//		return;

//	it = maybe_array->get_childs().begin();
//	while(it != maybe_array->get_childs().end())
//	{
//		add_variable(*it);
//		++it;
//	}
//}

//void Variable_Manager::exclude_variable(Variable_Base* _variable)
//{
//	std::list<Variable_Base*>::iterator it = m_variables.begin();
//	while(it != m_variables.end())
//	{
//		if(*it == _variable)
//		{
//			m_variables.erase(it);
//			_variable->set_parent(nullptr);
//			break;
//		}
//		++it;
//	}

//	if(it == m_variables.end())
//		return;

//	Array_Variable* maybe_array = cast_variable<Array_Variable>(_variable);
//	if(maybe_array == nullptr)
//		return;

//	while(maybe_array->get_childs().size() > 0)
//		maybe_array->remove_child(*maybe_array->get_childs().begin());
//}

#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H

#include <list>
#include <map>

#include "Variable_Base.h"

#include "Int_Variable.h"
#include "Bool_Variable.h"
#include "Float_Variable.h"
#include "String_Variable.h"


struct Variable_Stub
{
	std::string name;
	std::string value;
};


class Variable_Manager
{
private:
	enum class Type
	{
		Unknown,
		Int,
		Float,
		Bool,
		String
	};

private:
	using Variables_Map = std::map<std::string, Variable_Base*>;
	Variables_Map m_variables;

public:
	Variable_Manager();
	~Variable_Manager();

private:
	Type M_get_type(const std::string& _value) const;

public:
	void add_value(const Variable_Stub& _stub);
	void add_values(const std::list<Variable_Stub>& _raw_values);
	Variable_Base* get_variable(const std::string& _variable_name);

};


#endif // VARIABLE_MANAGER_H

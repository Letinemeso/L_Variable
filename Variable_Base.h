#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <map>

#include "Type_Manager.h"


typedef std::map<std::string, std::string> Str_Values;

#ifndef VARIABLE

	#define VARIABLE public: void assign_values(const Str_Values& _str_values) override

	#define INIT_FIELDS(variable_type, parent_type) \
		void variable_type::assign_values(const Str_Values& _str_values) { \
			parent_type::assign_values(_str_values);

	#define ADD_FIELD(type, field_reference, field_name) { \
		Str_Values::const_iterator check = _str_values.find(field_name); \
		if(check != _str_values.cend())  \
			Type_Manager::parse(#type, check->second, (void*)(&field_reference)); \
		}

	#define FIELDS_END }

#endif


class Variable_Base
{
private:
	std::string m_name;

public:
	virtual void assign_values(const Str_Values& _str_values) { };

};

class Variable_Test : public Variable_Base
{
public:
	VARIABLE;

private:
	int ass = 0;

public:


};

INIT_FIELDS(Variable_Test, Variable_Base)
ADD_FIELD(int, ass, "ass")
FIELDS_END

#endif // TYPE_H

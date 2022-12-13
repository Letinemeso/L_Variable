#ifndef OMFL_READER_H
#define OMFL_READER_H

#include <string>
#include <list>
#include <map>
#include <fstream>


struct Variable_Stub
{
	std::string name;
	std::string value;
	std::list<Variable_Stub> childs;

	bool ok() { return value.size() > 0; }
	bool is_section() { return !ok() && name.size() > 0; }
};


class OMFL_Reader
{
public:
	using Parsed_Data_t = std::map<std::string, std::list<Variable_Stub>>;

private:
	Parsed_Data_t m_parsed_data;

private:
	std::string m_current_section;

private:
	Variable_Stub M_parse_line(const std::string& _raw_data, unsigned int& _offset) const;
	std::string M_parse_word(const std::string& _line, unsigned int _offset) const;
	void M_parse_section(const std::string& _line, Variable_Stub& _result) const;
	void M_parse_variable(const std::string& _line, Variable_Stub& _result) const;
	std::string M_parse_string(const std::string& _line) const;
	void M_parse_array(const std::string& _array, Variable_Stub& _result) const;
	std::string M_parse_array_variable(const std::string& _array, unsigned int& _offset) const;
	std::string M_parse_all_array_variables(const std::string _array) const;
	bool M_variable_name_is_valid(const std::string& _name) const;
	unsigned int M_find_next_sumbol_pos(const std::string& _str, unsigned int _offset, char _symbol) const;

public:
	void parse(const std::string& _raw_data);

	const Parsed_Data_t& parsed_data() const;

};


#endif // OMFL_READER_H

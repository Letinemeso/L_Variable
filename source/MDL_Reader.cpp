#include "MDL_Reader.h"

using namespace LV;


MDL_Reader::MDL_Reader()
{

}

MDL_Reader::~MDL_Reader()
{

}



void MDL_Reader::parse_file(const std::string &_path)
{
	std::string raw_data = M_extract_from_file(_path);
	unsigned int raw_size = raw_data.size();

	unsigned int offset = 0;
	while(offset < raw_data.size())
	{
		std::string line = M_extract_line(raw_data, offset);
		offset += line.size() + 1;

		while(M_line_is_empty(line) && offset < raw_size)
		{
			line = M_extract_line(raw_data, offset);
			offset += line.size() + 1;
		}

		if(offset >= raw_size)
			break;

		MDL_Variable_Stub stub;
		stub.type = M_parse_type(line);
		L_ASSERT(stub.type.size() > 0);
		stub.name = M_parse_name(line);
		L_ASSERT(stub.name.size() > 0);

		L_DEBUG_FUNC_NOARG([&]()
		{
			std::list<MDL_Variable_Stub>::const_iterator it = m_stubs.cbegin();
			while(it != m_stubs.cend())
			{
				L_ASSERT(it->name != stub.name);
				++it;
			}
		});

		std::string data = M_extract_variable_data(raw_data, offset);
		stub.fields = M_parse_fields(data);

		m_stubs.push_back((MDL_Variable_Stub&&)stub);
	}
}

void MDL_Reader::add_stub(const MDL_Variable_Stub& _stub)
{
	L_DEBUG_FUNC_NOARG([&]()
	{
		std::list<MDL_Variable_Stub>::const_iterator it = m_stubs.cbegin();
		while(it != m_stubs.cend())
		{
			L_ASSERT(it->name != _stub.name);
			++it;
		}
	});

	m_stubs.push_back(_stub);
}

void MDL_Reader::add_stub(MDL_Variable_Stub&& _stub)
{
	L_DEBUG_FUNC_NOARG([&]()
	{
		std::list<MDL_Variable_Stub>::const_iterator it = m_stubs.cbegin();
		while(it != m_stubs.cend())
		{
			L_ASSERT(it->name != _stub.name);
			++it;
		}
	});

	m_stubs.push_back((MDL_Variable_Stub&&)_stub);
}

void MDL_Reader::clear()
{
	m_stubs.clear();
}

void MDL_Reader::save_to_file(const std::string &_path) const
{
	std::ofstream file(_path + ".mdl", std::ofstream::binary);
	L_ASSERT(file.is_open());

	file << "# This file was auto generated #\n\n";

	std::list<MDL_Variable_Stub>::const_iterator stub_it = m_stubs.cbegin();
	while(stub_it != m_stubs.end())
	{
		file << "<" << stub_it->type << "> " << "|" << stub_it->name << "|\n\\\n";

		for(MDL_Variable_Stub::fields_t::const_iterator variable_it = stub_it->fields.cbegin(); variable_it != stub_it->fields.cend(); ++variable_it)
		{
			file << "\t|" << variable_it->first << "|\n\t\\\n\t\t";

			for(unsigned int i=0; i<variable_it->second.size(); ++i)
				file << "\"" << variable_it->second[i] << "\" ";

			file << "\n\t/\n";
		}

		file << "/\n\n";

		++stub_it;
	}

	file.close();
}



const std::list<MDL_Variable_Stub>& MDL_Reader::stubs() const
{
	return m_stubs;
}



std::string MDL_Reader::M_extract_from_file(const std::string &_path)
{
	std::ifstream file(_path + ".mdl", std::ifstream::binary);
	L_ASSERT(file.is_open());

	file.seekg(0, std::ifstream::end);
	unsigned int length = file.tellg();
	file.seekg(0, std::ifstream::beg);

	std::string result;
	result.resize(length);
	for(unsigned int i=0; i<length; ++i)
		result[i] = file.get();

	file.close();

	M_preprocess(result);

	return result;
}

void MDL_Reader::M_preprocess(std::string &_raw) const
{
	for(unsigned int i=0; i<_raw.size(); ++i)
	{
		if(_raw[i] == '#')
		{
			_raw[i] = ' ';
			++i;
			for(; i < _raw.size(); ++i)
			{
				if(_raw[i] == '#')
				{
					_raw[i] = ' ';
					break;
				}
				_raw[i] = ' ';
			}
			L_ASSERT(i < _raw.size());
		}
	}
}


unsigned int MDL_Reader::M_find_symbol(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size(); ++_offset)
		if(_str[_offset] == _symbol)
			break;
	return _offset;
}


bool MDL_Reader::M_line_is_empty(const std::string &_line) const
{
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		return false;
	}

	return true;
}

bool MDL_Reader::M_line_is_opener(const std::string &_line) const
{
	bool found_opener = false;
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		if(_line[i] == '\\' && found_opener == false)
			found_opener = true;
		else
			return false;
	}

	return found_opener;
}

bool MDL_Reader::M_line_is_closer(const std::string &_line) const
{
	bool found_closer = false;
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		if(_line[i] == '/' && found_closer == false)
			found_closer = true;
		else
			return false;
	}

	return found_closer;
}

std::string MDL_Reader::M_extract_variable_data(const std::string &_raw, unsigned int& _offset) const
{
	unsigned int data_start = _offset;

	M_skip_past_opener(_raw, data_start);

	unsigned int data_end = data_start;
	M_skip_past_closer(_raw, data_end, true);

	for(; data_end != 0 && _raw[data_end] != '/'; --data_end) { }
	L_ASSERT(data_end > 0);
	--data_end;

	L_ASSERT(data_start < _raw.size() && data_end < _raw.size());

	_offset = data_end;
	M_skip_past_closer(_raw, _offset, false);

	return _raw.substr(data_start, data_end - data_start);
}

void MDL_Reader::M_skip_past_opener(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
	while(_offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, _offset);
		_offset += line.size() + 1;
		if(M_line_is_opener(line))
			return;
		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));
	}
	L_ASSERT(false);	//	opener wasn't found
}

void MDL_Reader::M_skip_past_closer(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
	unsigned int openers = 0;

	while(_offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, _offset);
		_offset += line.size() + 1;
		if(M_line_is_closer(line))
		{
			if(openers == 0)
				return;
			else
				--openers;
			continue;
		}

		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));

		if(M_line_is_opener(line))
			++openers;
	}
	L_ASSERT(false);	//	closer wasn't found
}

std::string MDL_Reader::M_extract_line(const std::string &_raw, unsigned int _offset) const
{
	unsigned int end = M_find_symbol(_raw, _offset, '\n');
	return _raw.substr(_offset, end - _offset);
}


std::string MDL_Reader::M_parse_type(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '<') + 1;
	unsigned int end = M_find_symbol(_line, start, '>');

	L_ASSERT(start < _line.size() && end < _line.size());

	return _line.substr(start, end - start);
}

std::string MDL_Reader::M_parse_name(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '|') + 1;
	unsigned int end = M_find_symbol(_line, start, '|');

	if(start < _line.size() && end < _line.size())
		return _line.substr(start, end - start);
	return {};
}

MDL_Variable_Stub::fields_t MDL_Reader::M_parse_fields(const std::string &_raw_data) const
{
	unsigned int offset = 0;

	MDL_Variable_Stub::fields_t result;

	while(offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, offset);
		offset += line.size() + 1;

		if(M_line_is_empty(line))
			continue;

		std::string name = M_parse_name(line);

		L_ASSERT(name.size() > 0);
		L_ASSERT(result.find(name) == result.end());

		std::string simple_data_raw_str = M_extract_variable_data(_raw_data, offset);

		LDS::Vector<std::string> values = M_parse_simple_data(simple_data_raw_str);

		result.emplace(name, (LDS::Vector<std::string>&&)values);
	}

	return result;
}

unsigned int MDL_Reader::M_parse_amount(const std::string &_raw_data) const
{
	unsigned int result = 0;
	for(unsigned int i=0; i<_raw_data.size(); ++i)
	{
		if(_raw_data[i] == '\"')
			++result;
	}

	L_ASSERT(result % 2 == 0);

	return result / 2;
}

LDS::Vector<std::string> MDL_Reader::M_parse_simple_data(const std::string &_raw_data) const
{
	unsigned int amount = M_parse_amount(_raw_data);
	L_ASSERT(amount > 0);

	LDS::Vector<std::string> result;
	result.resize(amount);

	unsigned int offset = 0;
	for(unsigned int i=0; i<amount; ++i)
	{
		offset = M_find_symbol(_raw_data, offset, '\"') + 1;
		unsigned int end = M_find_symbol(_raw_data, offset, '\"');

		std::string str_value;
		if(end > offset)
			str_value = _raw_data.substr(offset, end - offset);

		result.push(str_value);

		offset = end + 1;
	}

	return result;
}
































































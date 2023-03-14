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
        std::string name = M_parse_name(line);
        L_ASSERT(name.size() > 0);

        L_ASSERT(m_stubs.find(name) == m_stubs.end());

		std::string data = M_extract_variable_data(raw_data, offset);
        stub = M_parse_stub(data);

        m_stubs.emplace((std::string&&)name, (MDL_Variable_Stub&&)stub);
	}
}

void MDL_Reader::add_stub(const MDL_Variable_Stub& _stub)
{
//	L_ASSERT(m_stubs.find(_stub.name) == m_stubs.end());

//	m_stubs.emplace(_stub.name, _stub);
}

void MDL_Reader::add_stub(MDL_Variable_Stub&& _stub)
{
//	L_ASSERT(m_stubs.find(_stub.name) == m_stubs.end());

//	m_stubs.emplace(_stub.name, (MDL_Variable_Stub&&)_stub);
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

	for(Stub_Map::const_iterator stub_it = m_stubs.cbegin(); stub_it != m_stubs.end(); ++stub_it)
	{
        file << "|" << stub_it->first << "|\n\\\n";

        for(std::map<std::string, LDS::Vector<std::string>>::const_iterator variable_it = stub_it->second.fields.cbegin(); variable_it != stub_it->second.fields.cend(); ++variable_it)
		{
			file << "\t|" << variable_it->first << "|\n\t\\\n\t\t";

			for(unsigned int i=0; i<variable_it->second.size(); ++i)
				file << "\"" << variable_it->second[i] << "\" ";

			file << "\n\t/\n";
		}

		file << "/\n\n";
	}

	file.close();
}



const MDL_Reader::Stub_Map& MDL_Reader::stubs() const
{
	return m_stubs;
}

const MDL_Variable_Stub& MDL_Reader::get_stub(const std::string &_name) const
{
	 Stub_Map::const_iterator it = m_stubs.find(_name);
	 L_ASSERT(it != m_stubs.end());
	 return it->second;
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
		if(_raw[i] != '#')
			continue;

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


unsigned int MDL_Reader::M_find_symbol(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size(); ++_offset)
		if(_str[_offset] == _symbol)
			break;
	return _offset;
}

unsigned int MDL_Reader::M_find_symbol_from_end(const std::string &_str, int _offset, char _symbol) const
{
    for(; _offset > 0; --_offset)
        if(_str[_offset] == _symbol)
            break;
    if(_offset == -1)
        return _str.size();
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

	for(; data_end != 0; --data_end)
	{
		if(data_end > _raw.size())
			continue;
		if(_raw[data_end] == '/')
			break;
	}
	L_ASSERT(data_end > 0);
	--data_end;

	L_ASSERT(data_start < _raw.size() && data_end < _raw.size());

	_offset = data_end;
	M_skip_past_closer(_raw, _offset, false);

	return _raw.substr(data_start, data_end - data_start);
}

void MDL_Reader::M_skip_past_opener(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
    bool slash_inside_value = false;

	while(_offset < _raw_data.size())
	{
        std::string line = M_extract_line(_raw_data, _offset);
        _offset += line.size() + 1;

        unsigned int quotes_amount = 0;
        for(unsigned int i=0; i<line.size(); ++i)
            quotes_amount += (unsigned int)(line[i] == '\"');
        if(quotes_amount % 2 == 1)
            slash_inside_value = !slash_inside_value;

        if(M_line_is_opener(line) && !slash_inside_value)
			return;
		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));
	}
	L_ASSERT(false);	//	opener wasn't found
}

void MDL_Reader::M_skip_past_closer(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
	unsigned int openers = 0;
    bool backslash_inside_value = false;

	while(_offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, _offset);
		_offset += line.size() + 1;

        unsigned int quotes_amount = 0;
        for(unsigned int i=0; i<line.size(); ++i)
            quotes_amount += (unsigned int)(line[i] == '\"');
        if(quotes_amount % 2 == 1)
            backslash_inside_value = !backslash_inside_value;

        if(M_line_is_closer(line) && !backslash_inside_value)
		{
			if(openers == 0)
				return;
			else
				--openers;
			continue;
		}

		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));

        if(M_line_is_opener(line) && !backslash_inside_value)
			++openers;
	}
	L_ASSERT(false);	//	closer wasn't found
}

std::string MDL_Reader::M_extract_line(const std::string &_raw, unsigned int _offset) const
{
	unsigned int end = M_find_symbol(_raw, _offset, '\n');
	return _raw.substr(_offset, end - _offset);
}


std::string MDL_Reader::M_parse_name(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '|') + 1;
//	unsigned int end = M_find_symbol(_line, start, '|');
    unsigned int end = M_find_symbol_from_end(_line, _line.size() - 1, '|');

	if(!(start < _line.size() && end < _line.size()))
		return {};

	L_DEBUG_FUNC_NOARG([&]()
	{
		std::string line = _line;
		for(unsigned int i=start-1; i < end + 1; ++i)
			line[i] = ' ';
		L_ASSERT(M_line_is_empty(line));
	});

	return _line.substr(start, end - start);
}

MDL_Variable_Stub MDL_Reader::M_parse_stub(const std::string &_raw_data) const
{
	unsigned int offset = 0;

    MDL_Variable_Stub result;

	while(offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, offset);
		offset += line.size() + 1;

		if(M_line_is_empty(line))
			continue;

		std::string name = M_parse_name(line);

        if(name[0] == '|' && name[name.size() - 1] == '|')
        {
            name = M_parse_name(name);

            L_ASSERT(name.size() > 0);
            L_ASSERT(result.fields.find(name) == result.fields.end());

//            unsigned int child_offset = offset;

            std::string raw_child_data = M_extract_variable_data(_raw_data, offset);
            MDL_Variable_Stub child = M_parse_stub(raw_child_data);

            result.childs.emplace((std::string&&)name, (MDL_Variable_Stub&&)child);
        }
        else
        {
            std::string simple_data_raw_str = M_extract_variable_data(_raw_data, offset);
            LDS::Vector<std::string> values = M_parse_simple_data(simple_data_raw_str);
            result.fields.emplace(name, (LDS::Vector<std::string>&&)values);
        }
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

	L_DEBUG_FUNC_NOARG([&]()	//	check if line(s) with data contains nothing but data
	{
		std::string data_check = _raw_data;
		unsigned int offset_check = 0;
		for(unsigned int i=0; i<amount; ++i)
		{
			offset_check = M_find_symbol(data_check, offset_check, '\"') + 1;
			unsigned int end = M_find_symbol(data_check, offset_check, '\"');

			std::string str_value;
			if(end > offset_check)
			{
				for(unsigned int j=offset_check - 1; j<end + 1; ++j)
					data_check[j] = ' ';
			}

			offset_check = end + 1;
		}

		for(unsigned int i=0; i<data_check.size(); ++i)
			L_ASSERT(data_check[i] == ' ' || data_check[i] == '\t' || data_check[i] == '\n' || data_check[i] == '\r');
	});

	return result;
}
































































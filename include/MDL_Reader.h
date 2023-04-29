#ifndef MDL_READER_H
#define MDL_READER_H

#include <string>
#include <fstream>

#include <Data_Structures/Vector.h>
#include <Data_Structures/Map.h>

#include <L_Debug/L_Debug.h>


namespace LV
{

	struct MDL_Variable_Stub
    {
        LDS::Map<std::string, LDS::Vector<std::string>> fields;
        LDS::Map<std::string, MDL_Variable_Stub> childs;

		MDL_Variable_Stub(){}
        MDL_Variable_Stub(const MDL_Variable_Stub& _other) : fields(_other.fields) {}
        MDL_Variable_Stub(MDL_Variable_Stub&& _other) : fields((LDS::Map<std::string, LDS::Vector<std::string>>&&)_other.fields), childs((LDS::Map<std::string, MDL_Variable_Stub>&&)_other.childs) {}
        void operator =(MDL_Variable_Stub&& _other) { fields = (LDS::Map<std::string, LDS::Vector<std::string>>&&)_other.fields; childs = (LDS::Map<std::string, MDL_Variable_Stub>&&)_other.childs; }
	};

	class MDL_Reader final
	{
	public:
        typedef LDS::Map<std::string, MDL_Variable_Stub> Stub_Map;

	private:
		Stub_Map m_stubs;

	public:
		MDL_Reader();
		~MDL_Reader();

		MDL_Reader(const MDL_Reader& _other) = delete;
		MDL_Reader(MDL_Reader&& _other) = delete;

	public:
		void parse_file(const std::string& _path);
        void add_stub(const std::string& _name, const MDL_Variable_Stub& _stub);
        void add_stub(const std::string& _name, MDL_Variable_Stub&& _stub);
		void clear();
		void save_to_file(const std::string& _path) const;

	public:
		const Stub_Map& stubs() const;
		const MDL_Variable_Stub& get_stub(const std::string& _name) const;

    private:
        void M_save_stub_to_file(std::ofstream& _file, const std::string& _name, const MDL_Variable_Stub& _stub, unsigned int _nesting_level = 0) const;

		std::string M_extract_from_file(const std::string& _path);
		void M_preprocess(std::string& _raw) const;

        unsigned int M_find_symbol(const std::string& _str, unsigned int _offset, char _symbol) const;
        unsigned int M_find_symbol_from_end(const std::string& _str, int _offset, char _symbol) const;

		bool M_line_is_empty(const std::string& _line) const;
		bool M_line_is_opener(const std::string& _line) const;
		bool M_line_is_closer(const std::string& _line) const;
		std::string M_extract_variable_data(const std::string& _raw, unsigned int& _offset) const;
		void M_skip_past_opener(const std::string& _raw_data, unsigned int& _offset, bool _ignore_lines = false) const;
		void M_skip_past_closer(const std::string& _raw_data, unsigned int& _offset, bool _ignore_lines = false) const;
		std::string M_extract_line(const std::string& _raw, unsigned int _offset) const;

		std::string M_parse_name(const std::string& _line) const;
        MDL_Variable_Stub M_parse_stub(const std::string& _raw_data) const;
		unsigned int M_parse_amount(const std::string& _raw_data) const;
		LDS::Vector<std::string> M_parse_simple_data(const std::string& _raw_data) const;

	};

}

#endif // MDL_READER_H

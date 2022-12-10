#ifndef TYPE_H
#define TYPE_H

#include <string>

class Variable
{
protected:
	void* m_data = nullptr;

	void* data() { return m_data; }

public:
	virtual std::string type() const { return ""; };

public:
	operator bool() const
	{
		return m_data != nullptr;
	}

	virtual void operator=(const Variable& _var) = 0;

public:
	Variable(){ }

	virtual ~Variable()
	{
	}

};



class Int : public Variable
{
public:
	Int()
	{

	}

	Int(int _data)
	{
		m_data = new int;
		*(int*)m_data = _data;
	}

	void operator=(int _data)
	{
		delete (int*)m_data;
		m_data = new int;
		*(int*)m_data = _data;
	}

	~Int()
	{
		delete (int*)m_data;
	}

public:
	std::string type() const override
	{
		return "Int";
	}

public:
	void operator=(const Variable& _var) override
	{
		if(_var.type() != type())
			return;

		delete (int*)m_data;
		m_data = new int(*(int*)((Int&)_var).m_data);
	}

public:
	int& operator*()
	{
		return *(int*)m_data;
	}
	const int& operator*() const
	{
		return *(int*)m_data;
	}

};

#endif // TYPE_H

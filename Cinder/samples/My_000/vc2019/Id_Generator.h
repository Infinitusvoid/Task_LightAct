#pragma once

class Id_generator
{
public:
	Id_generator(const Id_generator&) = delete;

	static Id_generator& Get()
	{
		static Id_generator instance;
		return instance;
	}
	static int next_id() {
		return Get().inter_next_id();
	}
private:
	int inter_next_id()
	{
		m_num += 1;
		return m_num;
	}
	Id_generator() {}
	int m_num = -1;
};


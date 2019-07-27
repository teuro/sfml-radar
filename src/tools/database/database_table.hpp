#ifndef _TABLE_CLASS_HPP
#define _TABLE_CLASS_HPP

#include <string>

class Database_table {
protected:
	explicit Database_table(std::string table_name);
	
	~Database_table();

	std::string table_name;
};

#endif

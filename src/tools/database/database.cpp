#include "database.hpp"

namespace Database {
	static Settings s;
}

Queryresult Database::get_result(std::string query) {
    sqlite3pp::database db(s.database_name.c_str());
	
    sqlite3pp::query qry(db, query.c_str());
	
    Queryresult qrslt(qry);
	
    return qrslt;
}

std::string Database::bind_param(std::string query_string, std::map <std::string, std::string> variables) {
	if (query_string.length() == 0) {
		throw std::logic_error("Empty query string not able to perform");
	}
	
	for (const auto& item : variables) {
		std::string::size_type pos = query_string.find(s.search_term, 0);
		
		if (pos == std::string::npos) {
			throw std::logic_error("Query string '" + query_string + "' does not contain more " + s.search_term + " check Your querys");
		}
		
		std::string part1 = query_string.substr(0, pos);
		std::string part2 = query_string.substr(pos, s.search_term.length());
		std::string part3 = query_string.substr(pos + s.search_term.length());
		
		part2 = item.first + s.bind_term + item.second;
		
		query_string = part1 + part2 + part3;
	}
	
	return query_string;
}

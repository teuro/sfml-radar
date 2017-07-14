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

std::string Database::bind_param(std::string query_string, std::list <std::string> variables) {
	if (query_string.length() == 0) {
		throw std::logic_error("Empty query string not able to perform");
	}
	
	for (const auto& item : variables) {
		query_string = Tools::replace(query_string, "?", item);
	}
	
	return query_string;
}

std::string Database::bind_param(std::string query_string, std::map <std::string, std::string> variables) {
	if (query_string.length() == 0) {
		throw std::logic_error("Empty query string not able to perform");
	}
	
	for (const auto& item : variables) {
		std::string replace = item.first + s.bind_term + item.second;
		query_string = Tools::replace(query_string, s.search_term, replace);
	}
	
	return query_string;
}

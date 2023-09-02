#include "database.hpp"

Database::Database() { }

void Database::insert(std::string table_name, std::list <std::string> columns, std::list <std::string> values) {
	sqlite3pp::database db("databases/radar.db");
	
	std::string query = "INSERT INTO " +  table_name + " (";
	
	for (auto ci: columns) {
		query += ci + ", ";
	}
	
	query = query.substr(0, query.length() -2);
	query += ") VALUES (";
	
	for (auto vi: values) {
		query += "'" + vi + "', ";
	}
	
	query = query.substr(0, query.length() -2);
	query += ")";
	
	db.execute(query.c_str());
}

void Database::update(std::string table_name, std::map <std::string, std::string> values, std::string condition) {
	sqlite3pp::database db("databases/radar.db");
	
	std::string query = "UPDATE " +  table_name + " SET ";
	
	for (auto ci: values) {
		query += ci.first + " = '" + ci.second + "' ";
	}
	
	query += "WHERE " + condition;
	
	db.execute(query.c_str());
}

Queryresult Database::get_result(std::string query) {
    sqlite3pp::database db("databases/radar.db");
	
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
		std::string replace = item.first + " = " + item.second;
		query_string = Tools::replace(query_string, "? = ?", replace);
	}
	
	return query_string;
}

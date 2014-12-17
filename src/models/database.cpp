#include "database.hpp"

Queryresult Database::get_result(std::string query) {
    //std::clog << query << std::endl;
    sqlite3pp::database db("radar.db");
    sqlite3pp::query qry(db, query.c_str());
    Queryresult qrslt(qry);

    return qrslt;
}

std::string Database::bind_param(std::string query_string, std::stack <std::string> variables) {
    while (true) {
        if (variables.size()) {
            std::string::size_type pos = query_string.find("?", 0);

            std::string part1 = query_string.substr(0, pos);
            std::string part2 = query_string.substr(pos, 1);
            std::string part3 = query_string.substr(pos+1);

            part2 = variables.top();
            variables.pop();

            query_string = part1 + part2 + part3;
        } else {
            return query_string;
        }
    }
}

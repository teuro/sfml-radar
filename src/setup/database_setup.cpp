#include "database_setup.hpp"

void Setup::create_tables() {
	sqlite3pp::database db("databases/radar.db");
	
	std::string games = "CREATE TABLE IF NOT EXISTS games (id INT(6), date  DATETIME default CURRENT_TIMESTAMP, PRIMARY KEY(id))";
	std::string planes = "CREATE TABLE IF NOT EXISTS planes (id INT(6), callsign TEXT, time_in DATETIME, time_out DATETIME, points INT(10), game_id INT(6), PRIMARY KEY(id), FOREIGN KEY (game_id) REFERENCES games (id))";
	std::string clearances = "CREATE TABLE IF NOT EXISTS clearances (id INT(6), clearance TEXT, plane_id INT(6), PRIMARY KEY(id), FOREIGN KEY (plane_id) REFERENCES planes (id))";
	
	db.execute(games.c_str());
	db.execute(planes.c_str());
	db.execute(clearances.c_str());
}
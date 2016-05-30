#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../tools/settings.hpp"

class Player {
public:
    Player(Settings& s);
    ~Player();
    void update();
	
	void set_firstname(std::string firstname);
	void set_surname(std::string surname);
	void set_email(std::string email);
	void set_username(std::string username);
	
	std::string get_firstname();
	std::string get_surname();
	std::string get_email();
	std::string get_username();
protected:
private:
    Settings& settings;
	
	std::string firstname;
	std::string surname;
	std::string email;
	std::string username;
	std::string password_hash;
	std::string password_salt;
};

#endif // PLAYER_HPP

#include "player.hpp"

Player::Player(Settings& s) : settings(s) { }

Player::~Player() { }

void Player::update(double elapsed) { }

void Player::set_firstname(std::string firstname) 	{ this->firstname = firstname; }
void Player::set_surname(std::string surname) 		{ this->surname = surname; }
void Player::set_email(std::string email) 			{ this->email = email; }
void Player::set_username(std::string username) 	{ this->username = username; }

std::string Player::get_firstname() 	{ return this->firstname; }
std::string Player::get_surname() 		{ return this->surname; }
std::string Player::get_email() 		{ return this->email;}
std::string Player::get_username() 		{ return this->username;}

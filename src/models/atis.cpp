#include "atis.hpp"

Atis::Atis(Settings& s) : settings(s) { }

Atis::~Atis() {

}

void Atis::update(double elapsed) {
    build_xml();
}

void Atis::set_departure_runway(std::string dep_rwy) {
    this->departure_runway = dep_rwy;
}

void Atis::set_landing_runway(std::string lnd_rwy) {
    this->landing_runway = lnd_rwy;
}

void Atis::set_transition_altitude(int tr_alt) {
    this->transition_altitude = tr_alt;
}

void Atis::set_transition_level(int tr_lvl) {
    this->transition_level = tr_lvl;
}

std::string Atis::get_departure_runway() {
    return this->departure_runway;
}

std::string Atis::get_landing_runway() {
    return this->landing_runway;
}

int Atis::get_transition_level() {
    return this->transition_level;
}

int Atis::get_transition_altitude() {
    return this->transition_altitude;
}

void Atis::build_xml() {
    this->document.LoadFile("layout.xml");
    TiXmlElement* root = document.RootElement();

    document.RootElement()->Clear();

    TiXmlElement* elements = new TiXmlElement("elements");
    TiXmlElement* element1 = new TiXmlElement("metar");
    TiXmlElement* element2 = new TiXmlElement("input");

    TiXmlElement* e_metar = new TiXmlElement("element");
    e_metar->SetAttribute("id", "metar");
    e_metar->SetAttribute("class", "data");
    e_metar->SetAttribute("name", "Metar");
    TiXmlText* t_metar = new TiXmlText("EFHK 301250 27006KT 2000 +RA BKN012 03/02 Q0998");
    element1->LinkEndChild(t_metar);
    e_metar->LinkEndChild(element1);

    TiXmlElement* e_input = new TiXmlElement("element");
    e_input->SetAttribute("id", "input");
    e_input->SetAttribute("class", "data");
    e_input->SetAttribute("name", "Input");
    TiXmlText* t_input = new TiXmlText(this->command.c_str());
    element2->LinkEndChild(t_input);
    e_input->LinkEndChild(element2);

    elements->LinkEndChild(e_metar);
    elements->LinkEndChild(e_input);
    root->LinkEndChild(elements);
    document.SaveFile();
}

void Atis::set_command(std::string command) {
    this->command = command;
}

std::string Atis::get_command() {
    return this->command;
}

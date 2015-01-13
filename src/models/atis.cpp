#include "atis.hpp"

Atis::Atis(Settings& s) : settings(s) {
    this->transition_altitude = 5000;
    this->transition_level = 50;
    this->departure_runway = "22R";
    this->landing_runway = "15";
}

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

    TiXmlElement* element = new TiXmlElement("element");
    element->SetAttribute("id", "input");
    element->SetAttribute("class", "data");

    TiXmlElement* name = new TiXmlElement("name");
    name->LinkEndChild(new TiXmlText("Input"));

    element->LinkEndChild(name);
    elements->LinkEndChild(element);

    TiXmlElement* element1 = new TiXmlElement("element");
    element1->SetAttribute("id", "metar");
    element1->SetAttribute("class", "data");

    TiXmlElement* name1 = new TiXmlElement("name");
    name1->LinkEndChild(new TiXmlText("Metar"));

    element1->LinkEndChild(name1);

    elements->LinkEndChild(element1);

    root->LinkEndChild(elements);

    document.SaveFile();
}

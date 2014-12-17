#ifndef COMMAND_HPP
#define COMMAND_HPP

class Command {
public:
    Command(int a, int v) : action(a), value(v) { }
    ~Command() { }

    int get_value() { return this->value; }
    int get_action() { return this->action; }
protected:
private:
    int action;
    int value;
};


#endif // COMMAND_HPP

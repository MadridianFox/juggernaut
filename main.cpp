#include <iostream>
#include <utility>
#include <sstream>

#define NUM_COMMANDS 4
#define NUM_MODES 5
class State;
class Mode{
private:
    uint8_t max_strikes = 0;
    std::string name;
public:
    explicit Mode(uint8_t max, std::string name);
    bool checkDead(State* state);
    bool checkModeName(const std::string &name);
    std::string getName();
};

class State{
private:
    uint8_t strikes = 0;
    Mode* current_mode = nullptr;
public:
    uint8_t strikesCount(){
        return this->strikes;
    }
    void addStrike(){
        if(current_mode) {
            this->strikes++;
            std::cout << "Strike! You are shotted " << (int) this->strikesCount() << " times." << std::endl;
            if(current_mode->checkDead(this)){
                std::cout << "You are died =(" << std::endl;
                this->reset();
            }
        }else{
            std::cout << "Chose mode first" << std::endl;
        }
    }
    void reset(){
        this->strikes = 0;
        std::cout << "Reset." << std::endl;
    }
    void setMode(Mode* mode){
        this->reset();
        this->current_mode = mode;
    }
};

Mode::Mode(uint8_t max, std::string name){
    this->max_strikes = max;
    this->name = std::move(name);
}
bool Mode::checkDead(State* state){
    return state->strikesCount() >= this->max_strikes;
}
bool Mode::checkModeName(const std::string &name){
    return this->name == name;
}
std::string Mode::getName(){
    std::ostringstream out;
    out << this->name << " (" << (int)this->max_strikes << " strikes)";
    return out.str();
}

/**
 * @interface
 */
class Cmd{
public:
    virtual bool checkCommandName(std::string cmd) = 0;
    virtual bool doCommand(State *state) = 0;
};

class ResetCmd: public Cmd{
public:
    bool checkCommandName(std::string cmd) override {
        return cmd == "reset";
    }
    bool doCommand(State *state) override {
        state->reset();
        return true;
    }
};

class ShotCmd: public Cmd{
public:
    bool checkCommandName(std::string cmd) override {
        return cmd == "shot";
    }
    bool doCommand(State *state) override {
        state->addStrike();
        return true;
    }
};

class ExitCmd: public Cmd{
public:
    bool checkCommandName(std::string cmd) override {
        return cmd == "exit";
    }
    bool doCommand(State *state) override {
       return false;
    }
};

class ModeCmd: public Cmd{
private:
    Mode** modes;
public:
    explicit ModeCmd(Mode** modes){
        this->modes = modes;
    }
    bool checkCommandName(std::string cmd) override {
        return cmd == "mode";
    }
    bool doCommand(State *state) override {
        std::cout << "Available modes:" << std::endl;
        for(uint8_t i=0; i < NUM_MODES; i++){
            std::cout << "  " << this->modes[i]->getName() << std::endl;
        }
        std::cout << "chose the mode: ";
        std::string mode_name;
        std::cin >> mode_name;
        for(uint8_t i=0; i < NUM_MODES; i++){
            if(this->modes[i]->checkModeName(mode_name)){
                state->setMode(modes[i]);
                break;
            }
        }
        return true;
    }
};


int main() {
    State *state = new State();
    Cmd** commands = new Cmd*[NUM_COMMANDS];
    Mode** modes = new Mode*[NUM_MODES];

    commands[0] = new ResetCmd();
    commands[1] = new ShotCmd();
    commands[2] = new ExitCmd();

    modes[0] = new Mode(1, "1");
    modes[1] = new Mode(5, "2");
    modes[2] = new Mode(10, "3");
    modes[3] = new Mode(20, "4");
    modes[4] = new Mode(40, "5");

    commands[3] = new ModeCmd(modes);

    std::string cmd;
    bool doLoop = true;

    while(doLoop){
        std::cout << "just do it: ";
        std::cin >> cmd;
        for(uint8_t i=0; i < NUM_COMMANDS; i++){
            if(commands[i]->checkCommandName(cmd)){
                doLoop = commands[i]->doCommand(state);
                break;
            }
        }
    }
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
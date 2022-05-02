#include "../include/Studio.h"
#include "../include/Trainer.h"
#include "../include/Workout.h"
#include "../include/Customer.h"
#include "../include/Trainer.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
Studio::Studio(): customerCounter(0) ,open(true),trainers() ,workout_options() ,actionsLog()
{
}

Studio::Studio(const Studio& other):customerCounter(other.customerCounter) ,open(other.open),trainers() ,workout_options() ,actionsLog() {
    for (Trainer* t : other.trainers) {
        trainers.push_back(new Trainer(*t));
    }
    for (BaseAction* ba : other.actionsLog) {
        actionsLog.push_back(ba->clone());
    }
    for(Workout w:other.workout_options){
        workout_options.push_back(Workout(w));
    }
}

Studio& Studio::operator=(const Studio& other)
{
    if (this != &other) {
        clear();
        for (Trainer* t : other.trainers) {
            trainers.push_back(new Trainer(*t));
        }
        for (BaseAction* ba : other.actionsLog) {
            actionsLog.push_back(ba->clone());
        }
        customerCounter = other.customerCounter;
        for(Workout w:other.workout_options){
            workout_options.push_back(Workout(w));
        }
        open = other.open;
    }
    return *this;
}

Studio::Studio(Studio&& other): customerCounter(other.customerCounter) ,open(other.open),trainers()
        ,workout_options()
        ,actionsLog()
{
    trainers=move(other.trainers);
    actionsLog=move(other.actionsLog);
    workout_options=move(other.workout_options);
    other.clear();
}

Studio& Studio::operator=(Studio&& other)
{
    if (this != &other) {
        clear();
        trainers=move(other.trainers);
        open = other.open;
        actionsLog=move(other.actionsLog);
        customerCounter = other.customerCounter;
        workout_options=move(other.workout_options);
        other.open = false;
        other.customerCounter = 0;
        other.clear();

    }
    return *this;
}
Studio::~Studio()
{
    clear();

}

void Studio::clear() {


    for (Trainer *t: trainers) {
        delete t;
        t = nullptr;
    }

    for (BaseAction* ba : actionsLog) {
        delete ba;
        ba= nullptr;
    }
    workout_options.clear();
    trainers.clear();
    actionsLog.clear();
    open = false;
    customerCounter = 0;
}



Studio::Studio(const std::string& configFilePath):customerCounter(0),open(true),trainers(),workout_options()
,actionsLog()
{
    int workoutCounter = 0;
    int numoftrainer;
    std::string line;
    std::ifstream reader;
    std::vector<int> capacities;
    unsigned int index = 1;
    reader.open(configFilePath);
    if (!reader) {
        cout << "Wrong file name" << endl;
        exit(1);
    }
    while (getline(reader, line)) {
        if (line[0] == '#' || line[0]==' ') {
            continue;
        }
        else if (index == 1) {
            stringstream ss;
            ss << line;
            ss >> numoftrainer;
            index++;
        }
        else if (index == 2 && line!="") {
            string s="";
            int num;
            for (unsigned int i = 0; i < (unsigned int)line.length(); i++) {
                if (line[i] == ',') {
                    stringstream ss(s);
                    ss >> num;
                    capacities.push_back(num);
                    s = "";
                }else
                    s = s + line[i];
            }
            stringstream ss(s);
            ss >> num;
            capacities.push_back(num);
            index++;
        }
        else if (index == 3 &&line!="") {
            int price=0;
            std::string name;
            WorkoutType type;
            std::string delimeter = ", ";
            size_t index = 0;
            index = line.find(delimeter);
            name = line.substr(0, index);
            line.erase(0, index + delimeter.length());
            index = line.find(delimeter);
            if (line.substr(0, index) == "Anaerobic") { type = ANAEROBIC; }
            else if (line.substr(0, index) == "Cardio") { type = CARDIO; }
            else { type = MIXED; }
            line.erase(0, index + delimeter.length());
            stringstream p(line);
            p >> price;
            Workout workout(workoutCounter, name, price, type);
            workout_options.push_back(workout);
            workoutCounter++;
        }
    }
    for (unsigned int i = 0; i < (unsigned int)capacities.size(); i++) {
        Trainer* t1 = new Trainer(capacities[i]);
        t1->setId(i);
        trainers.push_back(t1);
    }
}
void Studio::start()
{
    cout << "Studio is now open!" << endl;
    string input, str;
    getline(cin >> str, input);
    while (str != "closeall") {
        if (str.find("order") != string::npos) {
            stringstream ss(input);
            int num;
            ss >> num;
            BaseAction* b = new Order(num);
            actionsLog.push_back(b);
            b->act(*this);
            b->setString("order" + input+ " ");
        }
        if (str.find("open") != string::npos) {
            string s = "";
            int index = 1;
            while (input[index] != ' ') {
                s = s + input[index];
                index++;
            }
            stringstream ss(s);
            int num;
            ss >> num;
            s = input.substr(index + 1);
            vector<string> names;
            vector<string> types;
            bool flag = true;
            while (flag) {
                string s2 = "";
                string s3 = "";
                for (int i = 0; s[i] != ','; i++) {
                    s2 = s2 + s[i];
                }
                names.push_back(s2);
                s3 = s3 + s[s2.length() + 1] + s[s2.length() + 2] + s[s2.length() + 3];
                types.push_back(s3);
                if (s2.length() + 5 > s.length())
                    flag = false;
                else
                    s = s.substr(s2.length() + 5);
            }
            vector<Customer*> cust;
            for (unsigned int i = 0; i < (unsigned int)names.size(); i++) {
                if (types[i] == "swt")
                    cust.push_back(new SweatyCustomer(names[i], customerCounter));
                if (types[i] == "chp")
                    cust.push_back(new CheapCustomer(names[i], customerCounter));
                if (types[i] == "mcl")
                    cust.push_back(new HeavyMuscleCustomer(names[i], customerCounter));
                if (types[i] == "fbd")
                    cust.push_back(new FullBodyCustomer(names[i], customerCounter));
                customerCounter++;
            }
            BaseAction* b = new OpenTrainer(num, cust);
            b->act(*this);
            b->setString(str + input + " ");
            this->actionsLog.push_back(b);
            


        }
        if (str.find("move") != string::npos) {
            string s = "";
            unsigned int index = 1;
            int num;
            int src, dst, custid;
            for (unsigned int i = 0; i < 3; i++) {
                while (input[index] != ' ' && index < (unsigned int)input.length()) {
                    s = s + input[index];
                    index++;
                }
                index++;
                stringstream ss(s);
                ss >> num;
                s = "";
                if (i == 0)src = num;
                else if (i == 1)dst = num;
                else if (i == 2)custid = num;
            }
            BaseAction* b = new MoveCustomer(src, dst, custid);
            b->act(*this);
            b->setString("move " + std::to_string(src) + " " + std::to_string(dst) + " " + std::to_string(custid) );
            this->actionsLog.push_back(b);
        }
        if (str.find("close") != string::npos) {
            string s = "";
            unsigned int index = 1;
            int num;
            while (index < (unsigned int)input.length() && input[index] != ' ') {
                s = s + input[index];
                index++;
            }
            index++;
            stringstream ss(s);
            ss >> num;
            BaseAction* b = new Close(num);
            b->act(*this);
            b->setString("close " + std::to_string(num) + " " );
            actionsLog.push_back(b);
        }
        if (str.find("workout_options") != string::npos) {
            BaseAction* b = new PrintWorkoutOptions();
            b->act(*this);
            b->setString("workout_options " );
            actionsLog.push_back(b);
        }
        if (str.find("status") != string::npos) {
            string s = "";
            unsigned int index = 1;
            int num;
            while (index < (unsigned int)input.length() && input[index] != ' ') {
                s = s + input[index];
                index++;
            }
            index++;
            stringstream ss(s);
            ss >> num;
            BaseAction* b = new PrintTrainerStatus(num);
            b->act(*this);
            b->setString("status " + std::to_string(num) + " ");
            actionsLog.push_back(b);
        }
        if (str.find("log") != string::npos) {
            BaseAction* b = new PrintActionsLog();
            b->act(*this);
            b->setString("log " );
            actionsLog.push_back(b);

        }
        if (str.find("backup") != string::npos) {
            BaseAction* b = new BackupStudio();
            actionsLog.push_back(b);
            b->setString("backup " );
            b->act(*this);

        }
        if (str.find("restore") != string::npos) {
            BaseAction* b = new RestoreStudio();
            b->act(*this);
            b->setString("restore ");
            actionsLog.push_back(b);
        }

        getline(cin >> str, input);
    }
    BaseAction* b = new CloseAll();
    b->act(*this);
    b->setString("closeAll ");
    actionsLog.push_back(b);
    return;

}

int Studio::getNumOfTrainers() const
{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid)
{
    for (Trainer* t : trainers) {
        if (t->getId() == tid) {
            return t;
        }
    }
    return nullptr;
}

Workout Studio::getWorkout(int wid)
{
    Workout w1 = Workout (1,"",1,MIXED);
    for (Workout w : workout_options) {
        if (w.getId() == wid) {
            return w;
        }
    }
    return w1;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const
{
    return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
    return workout_options;
}
std::vector<Trainer*> Studio::getTrainers()
{
    return this->trainers;
}

void Studio::addAction(BaseAction* action)
{
    actionsLog.push_back(action);
}



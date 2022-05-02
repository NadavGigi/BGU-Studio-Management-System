/*valgrind --leak-check=full --show-reachable=yes                                                           bin/main ExmapleInput.txt*/


#pragma once
#ifndef STUDIO_H_
#define STUDIO_H_

#include "Action.h"
#include "Workout.h"
#include "Trainer.h"

#include <vector>
#include <string>
class Trainer;
class BaseAction;

class Studio {
public:
    Studio();//empty constructor
    Studio(const Studio& other);
    Studio& operator=(const Studio& other);
    Studio(Studio&& other);
    Studio& operator=(Studio&& other);
    virtual ~Studio();
    void clear();
    Studio(const std::string& configFilePath);
    void start();
    void start(vector<string> actions, vector<string> value);
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    Workout getWorkout(int wid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainers();
    void addAction(BaseAction* action);
private:
    int customerCounter;
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector <BaseAction*> actionsLog;
};

#endif
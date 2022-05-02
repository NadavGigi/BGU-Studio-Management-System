#pragma once
#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Studio.h"
#include "Trainer.h"
enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction {
public:
    BaseAction();
    BaseAction(string error_m,ActionStatus status_,string forlog_):errorMsg(error_m),status(status_),forlog(forlog_){};
    ActionStatus getStatus() const;
    virtual ~BaseAction()  = default;
    virtual void act(Studio& studio) = 0;
    virtual std::string toString() const = 0;
    void setString(std::string s);
    string getStringStatus() const;
    virtual BaseAction* clone() const = 0;
    std::string getStringForLog() const;
protected:
    void complete();
    void error(std::string errorMsg);
    void setStatus(ActionStatus status);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
    std::string forlog;

};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer*>& customersList);
    OpenTrainer(const  OpenTrainer& other):BaseAction(other.getErrorMsg(),other.getStatus(),other.getStringForLog()),trainerId(other.trainerId),customers(){
        for(Customer* c:other.customers)
            customers.push_back(c->clone());
    };
    ~OpenTrainer();
    void act(Studio& studio);
    std::string toString() const;
    virtual OpenTrainer* clone() const { return new OpenTrainer(*this); };
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    ~Order(){};
    void act(Studio& studio);
    std::string toString() const;
    virtual Order* clone() const { return new Order(*this); };
private:
    const int trainerId;

};


class MoveCustomer : public BaseAction {
public:
    ~MoveCustomer (){};
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio& studio);
    std::string toString() const;
    virtual MoveCustomer* clone() const { return new MoveCustomer(*this); };
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    ~Close (){};
    Close(int id);
    void act(Studio& studio);
    std::string toString() const;
    virtual Close* clone() const { return new Close(*this); };
private:
    const int trainerId;

};


class CloseAll : public BaseAction {
public:
    ~CloseAll(){};
    CloseAll();
    void act(Studio& studio);
    std::string toString() const;
    virtual CloseAll* clone() const { return new CloseAll(*this); };
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    ~PrintWorkoutOptions(){};
    PrintWorkoutOptions();
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintWorkoutOptions* clone() const { return new PrintWorkoutOptions(*this); };
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    ~PrintTrainerStatus(){};
    PrintTrainerStatus(int id);
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintTrainerStatus* clone() const { return new PrintTrainerStatus(*this); };
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    ~PrintActionsLog(){};
    PrintActionsLog();
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintActionsLog* clone() const { return new PrintActionsLog(*this); };
private:
};


class BackupStudio : public BaseAction {
public:
    ~BackupStudio(){};
    BackupStudio();
    void act(Studio& studio);
    std::string toString() const;
    virtual BackupStudio* clone() const { return new BackupStudio(*this); };
private:
};


class RestoreStudio : public BaseAction {
public:
    ~RestoreStudio(){};
    RestoreStudio();
    void act(Studio& studio);
    std::string toString() const;
    virtual RestoreStudio* clone() const { return new RestoreStudio(*this); };

};


#endif
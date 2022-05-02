#pragma once
#pragma once
#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"
#include <iostream>
class Customer {
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout>& workout_options) = 0;
    virtual ~ Customer() = default;
    virtual Customer* clone() const = 0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};

class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    ~SweatyCustomer(){}
    virtual SweatyCustomer* clone() const { return new SweatyCustomer(*this); };

    std::vector<int> order(const std::vector<Workout>& workout_options);
    std::string toString() const;
private:
};



class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    ~CheapCustomer(){}
    virtual CheapCustomer* clone() const { return new CheapCustomer(*this); };
    std::vector<int> order(const std::vector<Workout>& workout_options);
    std::string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    ~HeavyMuscleCustomer(){}
    virtual HeavyMuscleCustomer* clone() const { return new HeavyMuscleCustomer(*this); };

    std::vector<int> order(const std::vector<Workout>& workout_options);
    std::string toString() const;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    ~FullBodyCustomer(){}
    virtual FullBodyCustomer* clone() const { return new FullBodyCustomer(*this); };

    std::vector<int> order(const std::vector<Workout>& workout_options);
    std::string toString() const;
private:
};


#endif
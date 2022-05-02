#pragma once
#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Workout.h"
#include "Customer.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer {
public:
    Trainer(int t_capacity);
    Trainer(const Trainer& other);
    Trainer& operator=(const Trainer& other);
    Trainer(Trainer&& other);
    Trainer& operator=(Trainer&& other);
    virtual ~Trainer();
    void clear();
    void setOpen(bool flag);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int calculateSalary();
    int getSalary();
    bool isOpen();
    int getId();
    void setId(int ID);
    void addOrders(std::vector<OrderPair> orders);
    bool getSpace();

    void setSalary(int salary);

private:
    int id;
    int salary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif
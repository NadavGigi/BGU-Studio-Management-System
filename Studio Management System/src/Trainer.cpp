#include "../include/Trainer.h"
#include "../include/Workout.h"



Trainer::Trainer(int t_capacity) :id(0),salary(0),capacity(t_capacity),open(false),customersList(),orderList()
{
}

Trainer::Trainer(const Trainer& other): id(other.id),salary(other.salary),capacity(other.capacity),open(other.open),customersList(),orderList()
{
    for (unsigned int i = 0; i < other.customersList.size(); i++) {
        addCustomer(other.customersList[i]->clone());
    }
    for (unsigned int i = 0; i < other.orderList.size(); i++) {
        orderList.push_back(OrderPair(other.orderList[i]));
    }

}

Trainer::Trainer(Trainer&& other) :id(0),salary(other.salary),capacity(other.capacity),open(other.open),customersList(),orderList()
{
    customersList=move(other.customersList);
    orderList=move(other.orderList);
    other.clear();
}

Trainer& Trainer::operator=(const Trainer& other)
{
    if (this != &other) {
        clear();
        for (unsigned int i = 0; i < other.customersList.size(); i++) {
            addCustomer(other.customersList[i]->clone());
        }
        for (unsigned int i = 0; i < other.orderList.size(); i++) {
            this->orderList.push_back(OrderPair(other.orderList[i]));
        }
        open = other.open;
        id = other.id;
        salary = other.salary;

    }
    return *this;

}

Trainer& Trainer::operator=(Trainer&& other)
{
    if (this != &other) {
        clear();
        for (unsigned int i = 0; i < other.customersList.size(); i++) {
            addCustomer(other.customersList[i]);
        }
        for (unsigned int i = 0; i < other.orderList.size(); i++) {
            this->orderList.push_back(other.orderList[i]);
        }
        open = other.open;
        id = other.id;
        salary = other.salary;

        other.clear();
    }
    return *this;
}
Trainer::~Trainer()
{

    clear();
}

void Trainer::clear() {

    for (Customer* customer: customersList){
       if(customer!=nullptr){
            delete customer;
            customer=nullptr;
        }

    }
    this->orderList.clear();
    this->capacity = 0;
    this->id = 0;
    this->open = false;
}
void Trainer::setOpen(bool flag)
{
    this->open = flag;
}


int Trainer::getCapacity() const
{
    return this->capacity;
}

void Trainer::addCustomer(Customer* customer)
{
    if ((unsigned int)customersList.size() == (unsigned int)capacity) {
        return;
    }
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id)
{
    unsigned int sizecust = customersList.size();
    for (unsigned int i = 0; i < sizecust; i++) {
        if (customersList[i]->getId() == id) {
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    vector<OrderPair> vec;
    for (unsigned int i=0;i<orderList.size();i++){
        if(orderList[i].first!=id)
            vec.push_back((orderList[i]));
    }
    orderList.clear();
    for(OrderPair o :vec)
        orderList.push_back(o);
    if (orderList.size() == 0)
        open = false;
}

Customer* Trainer::getCustomer(int id)
{
    for (unsigned int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers()
{
    return this->customersList;
}

std::vector<OrderPair>& Trainer::getOrders()
{
    return this->orderList;
}



int Trainer::calculateSalary()
{
    int salary = 0;
    for (OrderPair pair : orderList) {
        salary = salary + pair.second.getPrice();
    }
    return salary;
}

int Trainer::getSalary()
{
    return this->salary;
}

bool Trainer::isOpen()
{
    return this->open;
}

int Trainer::getId()
{
    return this->id;
}

void Trainer::setId(int ID)
{
    this->id = ID;
}

void Trainer::addOrders(std::vector<OrderPair> orders)
{
    for (OrderPair o : orders) {
        orderList.push_back(o);
    }
}




void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    for (unsigned int i = 0; i < workout_ids.size(); i++) {
        for (unsigned int j = 0; j < workout_options.size(); j++) {
            if (workout_ids[i] == workout_options[j].getId())
                orderList.push_back(make_pair(customer_id, workout_options[j]));
        }
    }
}

void Trainer::openTrainer()
{
    open = true;
}

void Trainer::closeTrainer()
{
    open = false;
}
bool Trainer::getSpace()
{
    return this->getCapacity() - this->getCustomers().size() > 0;
}

void Trainer::setSalary(int salary)
{
    this->salary += salary;
}
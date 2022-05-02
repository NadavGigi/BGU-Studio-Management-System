#include "../include/Action.h"
using namespace std;
extern Studio* backup;
BaseAction::BaseAction() :errorMsg(""), status(ERROR), forlog("")
{

}

ActionStatus BaseAction::getStatus() const
{
    return this->status;
}


void BaseAction::complete()
{
    this->setStatus(COMPLETED);
}

void BaseAction::error(std::string errorMsg)
{
    this->errorMsg = errorMsg;
}

void BaseAction::setStatus(ActionStatus status)
{
    this->status = status;
}

std::string BaseAction::getStringForLog() const
{
    return this->forlog + " ";

}

std::string BaseAction::getErrorMsg() const
{
    return getStringStatus() + this->errorMsg;
}




void BaseAction::setString(std::string s)
{
    forlog = s;
}

string BaseAction::getStringStatus() const
{
    if (getStatus() == 1)
        return "Error";
    return "Complete";
}
OpenTrainer::~OpenTrainer() {
    for (Customer *c: customers) {
        if(c!=nullptr){
            delete c;
        }
        c = nullptr;

    }
    customers.clear();
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer*>& customersList) : trainerId(id), customers()
{
    for (Customer* customer_point : customersList) {
        customers.push_back(customer_point);
    }
}

void OpenTrainer::act(Studio& studio)
{
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen()) {
        this->setStatus(ERROR);
        this->error("Workout session does not exist or is already open");
        cout << "Error: Workout session does not exist or is already open" << endl;
        return;
    }
    trainer->setOpen(true);
    for (Customer* customer_p : this->customers) {
        trainer->addCustomer(customer_p->clone());
        delete customer_p;
        customer_p=nullptr;
    }
    this->customers.clear();
    complete();
}

std::string OpenTrainer::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}

Order::Order(int id) : trainerId(id)
{

}

void Order::act(Studio& studio)
{
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr||!trainer->isOpen()) {
        this->setStatus(ERROR);
        this->error("Trainer does not exist or is already open");
        cout << "Error: Trainer does not exist or is already open" << endl;

        return;
    }
    std::vector<Customer*> customers = trainer->getCustomers();
    for (Customer* customer : customers) {
        vector<int> v = customer->order(studio.getWorkoutOptions());
        trainer->order(customer->getId(), v, studio.getWorkoutOptions());
        for (unsigned int i = 0; i < (unsigned int)v.size(); i++) {
            cout << customer->getName() + " is doing " + studio.getWorkout(v[i]).getName() << endl;
        }
    }
    complete();

}

std::string Order::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}




MoveCustomer::MoveCustomer(int src, int dst, int customerId) :srcTrainer(src), dstTrainer(dst), id(customerId)
{

}

void MoveCustomer::act(Studio& studio)
{
    Trainer* src = studio.getTrainer(srcTrainer);
    Trainer* dst = studio.getTrainer(dstTrainer);
    if (src == nullptr || dst == nullptr || !src->isOpen() || !dst->isOpen() ||src->getCustomer(id) == nullptr||!dst->getSpace()) {
        cout << "Error: Cannot move customer" << endl;
        error("Cannot move customer");
        setStatus(ERROR);
        return;
    }
    Trainer* t1 = studio.getTrainer(srcTrainer);
    Trainer* t2 = studio.getTrainer(dstTrainer);
    std::vector<OrderPair> srcvec = t1->getOrders();
    std::vector<OrderPair> dstvec;
    for (unsigned int i = 0; i < (unsigned int)srcvec.size(); i++) {
        if (srcvec[i].first == id)
            dstvec.push_back(srcvec[i]);
    }
    t2->addOrders(dstvec);
    t2->addCustomer(t1->getCustomer(id));
    t1->removeCustomer(id);
    complete();

}

std::string MoveCustomer::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}


Close::Close(int id) :trainerId(id)
{

}
void Close::act(Studio& studio)
{
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        this->error("Trainer does not exist or is not open");
        this->setStatus(ERROR);
        cout << "Error: Trainer does not exist or is not open" << endl;
        return;
    }
    trainer->setSalary(trainer->calculateSalary());
    cout << "Trainer " + std::to_string(trainer->getId()) + " closed. Salary " + std::to_string(trainer->getSalary()) + "NIS" << endl;
    for (Customer* customer : trainer->getCustomers()) {
        if(customer!=nullptr){
            delete customer;
            customer=nullptr;
        }
    }
    trainer->setOpen(false);
    complete();
    trainer->getCustomers().clear();
    trainer->getOrders().clear();

}
std::string Close::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}

CloseAll::CloseAll()
{
}

void CloseAll::act(Studio& studio) {
    for (Trainer *trainer: studio.getTrainers()) {
        if (trainer->isOpen()) {
            cout << "Trainer " + std::to_string(trainer->getId()) + " closed. Salary " +
                    std::to_string(trainer->getSalary() + trainer->calculateSalary()) + " NIS" << endl;
            trainer->setOpen(false);
        }
    }
}

std::string CloseAll::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}

PrintWorkoutOptions::PrintWorkoutOptions()
{
}

void PrintWorkoutOptions::act(Studio& studio)
{
    for (Workout workout : studio.getWorkoutOptions()) {
        cout << workout.getName() + ", " + std::to_string(workout.getType()) + ", " + std::to_string(workout.getPrice()) + "\n";
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}

PrintTrainerStatus::PrintTrainerStatus(int id) :trainerId(id)
{
}

void PrintTrainerStatus::act(Studio& studio)
{
    Trainer* trainer = studio.getTrainer(trainerId);
    bool open = trainer->isOpen();
    if (!open) {
        cout << "Trainer " + std::to_string(trainer->getId()) + " status: closed" << endl;
        setStatus(ERROR);
        return;
    }
    cout << "Trainer " + std::to_string(trainer->getId()) + " status: open" << endl;
    cout << "Customers:" << endl;
    for (Customer* customer : trainer->getCustomers()) {
        cout << std::to_string(customer->getId()) + " " + customer->getName() + "\n";
    }
    cout << "orders:\n";
    for (OrderPair op : trainer->getOrders()) {
        cout << op.second.getName() + " " + std::to_string(op.second.getPrice()) + "NIS " + std::to_string(op.first) << endl;
    }

    cout << "Current Trainer’s Salary: " + std::to_string(trainer->getSalary()+ trainer->calculateSalary()) + " Nis";
    complete();
}

std::string PrintTrainerStatus::toString() const
{
    return this->getStringForLog() +": "+ this->getErrorMsg();

}

BackupStudio::BackupStudio()
{
    complete();
}


std::string BackupStudio::toString() const
{
    return "backup :COMPLETED";
}

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Studio& studio)
{
    for (BaseAction* b : studio.getActionsLog()) {
        cout << b->getStringForLog()  + b->getStringStatus()  << endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const
{
    return this->getStringForLog() + this->getErrorMsg();
}


void BackupStudio::act(Studio& studio)
{
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    backup = new Studio(studio);
    complete();
}



RestoreStudio::RestoreStudio()
{
}

void RestoreStudio::act(Studio& studio)
{
    if (backup == nullptr) {
        error("no backup available");
    }
    else {
        studio = *backup;
        complete();

    }
}

std::string RestoreStudio::toString() const
{
    return "restore " + getStringStatus();
}
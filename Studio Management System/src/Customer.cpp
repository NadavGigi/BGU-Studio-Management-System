#include "../include/Customer.h"
#include <algorithm>
#include <memory>
using namespace std;

Customer::Customer(std::string c_name, int c_id) :name(c_name), id(c_id)
{
}
std::vector<int> CheapCustomer::order(const std::vector<Workout>& workout_options)

{
    vector<int> v = std::vector<int>();
    vector<unique_ptr<Workout>> v_unique_ptr;
    for (Workout workout : workout_options) {
        v_unique_ptr.push_back(move(unique_ptr<Workout>(new Workout(workout.getId(), workout.getName(), workout.getPrice(), workout.getType()))));
    }
    std::sort(v_unique_ptr.begin(), v_unique_ptr.end(), [](unique_ptr<Workout>& w1, unique_ptr<Workout>& w2) {
        if (w1->getPrice() == w2->getPrice()) return w1->getId() < w2->getId();
        return (w1->getPrice() < w2->getPrice());
    });
    v.push_back(v_unique_ptr[0]->getId());
    return v;



}
std::string Customer::getName() const
{
    return this->name;
}

int Customer::getId() const
{
    return this->id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id)
{
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout>& workout_options)
{
    std::vector<int> v = std::vector<int>();
    for (unsigned int i = 0; i < (unsigned int)workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO) {
            v.push_back(workout_options[i].getId());
        }
    }
    return v;
}

std::string SweatyCustomer::toString() const
{
    std::string res = this->getName() + ",swt ";
    return res;
}

CheapCustomer::CheapCustomer(std::string name, int id) :Customer(name, id)
{
}


std::string CheapCustomer::toString() const
{
    std::string res = this->getName() + ",chp ";
    return res;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) :Customer(name, id)
{
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout>& workout_options)
{

    vector<int> v = std::vector<int>();
    vector<unique_ptr<Workout>> v_unique_ptr;
    for (Workout workout : workout_options) {
        if (workout.getType() == ANAEROBIC)
            v_unique_ptr.push_back(move(unique_ptr<Workout>(new Workout(workout.getId(), workout.getName(), workout.getPrice(), workout.getType()))));
    }
    std::sort(v_unique_ptr.begin(), v_unique_ptr.end(), [](unique_ptr<Workout>& w1, unique_ptr<Workout>& w2) {
        if (w1->getPrice() == w2->getPrice()) return w1->getId() < w2->getId();
        return (w1->getPrice() > w2->getPrice());
    });

    for (unsigned int i = 0; i < (unsigned int)v_unique_ptr.size(); i++) {
        v.push_back(v_unique_ptr[i]->getId());
    }
    return v;

}

std::string HeavyMuscleCustomer::toString() const
{
    std::string res = this->getName() + ",mcl ";
    return res;

}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id)
{
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout>& workout_options)
{
    vector<int> v = std::vector<int>();
    vector<unique_ptr<Workout>> v_unique_ptr;
    vector<unique_ptr<Workout>> v_cardio;
    vector<unique_ptr<Workout>> v_mixed;
    vector<unique_ptr<Workout>> v_anaerobic;
    for (Workout workout : workout_options) {
        v_unique_ptr.push_back(move(unique_ptr<Workout>(new Workout(workout.getId(), workout.getName(), workout.getPrice(), workout.getType()))));
    }
    std::sort(v_unique_ptr.begin(), v_unique_ptr.end(), [](unique_ptr<Workout>& w1, unique_ptr<Workout>& w2) {
        if (w1->getPrice() == w2->getPrice()) return w1->getId() < w2->getId();
        return (w1->getPrice() < w2->getPrice());
    });
    for (unsigned int i = 0; i < (unsigned int)v_unique_ptr.size(); i++) {
        switch (v_unique_ptr[i]->getType()) {
            case CARDIO:
                v_cardio.push_back(move(v_unique_ptr[i]));
                break;
            case ANAEROBIC:
                v_anaerobic.push_back(move(v_unique_ptr[i]));
                break;
            case MIXED:
                v_mixed.push_back(move(v_unique_ptr[i]));
                break;
        }

    }
    v.push_back(v_cardio[0]->getId());
    v.push_back(v_mixed[v_mixed.size() - 1]->getId());
    v.push_back(v_anaerobic[0]->getId());

    return v;
}
std::string FullBodyCustomer::toString() const
{
    std::string res = this->getName() + ",fbd ";
    return res;

}

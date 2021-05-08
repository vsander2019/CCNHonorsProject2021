/****************************************************
file that delcares the classes used in the network simulator
Vanessa Sanders
4/15/2021
*****************************************************/


#ifndef  NETWORKV2_H__
#define NETWORKV2_H__


#include<string>
#include<queue>
#include<vector>
#include<iostream>
#include<utility>
#include<ctime>


using std::string; using std::queue; using std::priority_queue; using std::vector; using std::cout; using std::pair; using std::greater;

class Packet{
public:
    Packet(int id, int source, int destination, int delay = 0, int length = 32): 
        id_(id), source_(source), destination_(destination), delay_(delay), length_(length){}

    void setLength(int length){length_ = length;}

    void addDelay(int amount) {delay_ += amount;} 

    int getID() const {return id_;}
    int getDestiniation() const {return destination_;}
    int getSource() const {return source_;}
    int getDelay() const {return delay_;}
    int getLength() const {return length_;}

private:
    int id_;
    int source_;
    int destination_;
    int delay_;
    int length_;
};

//////////////////////////////////////////////////////////////////////////

class Router{
public:
    Router(int address, vector<vector<int>> table): address_(address), routingTable_(table){}
    int getAddress() const {return address_;}
    int getNextNode(Packet * p) {
        int dest = p->getDestiniation();
        vector<int>::iterator Route = routingTable_[dest].begin();
        while(Route != routingTable_[dest].end()){
            if(*Route == this->address_){
                return *(++Route);
            }
            ++Route;
        }
        return dest;
    }

    void pushPacket(Packet * p){routerQueue_.push(p);}
    void popPacket(){routerQueue_.pop();}
    Packet * getCurrentPacket(){return routerQueue_.front();}

private:
    queue<Packet *> routerQueue_;
    vector<vector<int>> routingTable_;
//    vector<Port *> ports_;
    int address_;
};

///////////////////////////////////////////////////////////////////////////

enum eventType{arrival, departure};

class Event{
public:
    Event(eventType t, int p, double time, Packet * pack): type_(t), place_(p), time_(time), thispack_(pack) {}
    
    eventType getType() const {return type_;}
    int getPlace() const {return place_;}
    double getTime() const {return time_;}
    Packet * getPacket() const {return thispack_;}
    bool operator> (Event const &right) const {
        if(this->time_ > right.time_){return true;}
        else{return false;}
    }
private:
    eventType type_;
    int place_;
    double time_;
    Packet * thispack_;
};

///////////////////////////////////////////////////////////////////////////

class Network{
public:
    Network(){}
    void setRoutingSystem( vector<Router *> system){routers = system;}
    void setEvents(priority_queue< Event, vector<Event>, greater<Event> > list){events = list;}
    void addEvent(Event e) {events.push(e);}
    void removeEvent() {events.pop();}
    Event getCurrentEvent() {return events.top();}

    void packetArrival(Packet * p, int node){
        srand(time(0));
        this->routers[node]->pushPacket(p);
        switch(node){
            case 0: p->addDelay(rand() % 10);
            break;
            case 1: p->addDelay(rand() % 15);
            break;
            case 2: p->addDelay(rand() % 7);
            break;
            case 3: p->addDelay(rand() % 9);
            break;
            case 4: p->addDelay(rand() % 13);
            break; 
        }
        Event nextEvent(departure, node, p->getDelay(), p);
        this->addEvent(nextEvent);
    }

    void packetDeparture(int node){
        srand(time(0)); 
        Packet * tmp = routers[node]->getCurrentPacket();
        if(routers[node]->getCurrentPacket()->getDestiniation() == node) {
            cout << "Packet # " << routers[node]->getCurrentPacket()->getID() << " has reached its destination." << std::endl;
            routers[node]->popPacket();
            return;}
        routers[node]->popPacket();
        cout << "Next node : " << routers[node]->getNextNode(tmp) << std::endl;
        switch(node){
            case 0: 
            if(routers[node]->getNextNode(tmp) == 1)
                tmp->addDelay((rand() % 40 + 30) + rand() % 3);
            else
                tmp->addDelay(rand() % 17 + 10);
            break;
            case 1:
            if(routers[node]->getNextNode(tmp) == 0)
                tmp->addDelay(rand() % 40 + 30);
            else if (routers[node]->getNextNode(tmp) == 3)
                tmp->addDelay(rand() % 10 + 5);
            else
                tmp->addDelay(rand() % 25 + 10);
            break;
            case 2: tmp->addDelay(rand() % 17 + 10);
            break;
            case 3: tmp->addDelay(rand() % 10 + 5);
            break;
            case 4: tmp->addDelay(rand() % 25 + 10);
            break; 
        }
        Event nextEvent(arrival, routers[node]->getNextNode(tmp), tmp->getDelay(), tmp);
        this->addEvent(nextEvent);
    }

    priority_queue< Event, vector<Event>, greater<Event> > & getEvents() {return events;}

private:
    vector<Router *> routers;
    priority_queue< Event, vector<Event>, greater<Event> > events;
    //int time_ = 0;
};

////////////////////////////////////////////////////////////////////////////


#endif
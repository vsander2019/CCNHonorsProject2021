/******************************************
Code that implements the sending and recieving of packets between nodes
Vanessa Sanders
4/16/2021
*******************************************/

#include"networkv2.h"
#include<list>
#include<ctime>



using std::list; using std::endl;


int main(){

srand(time(0));

//Setting up routing tables for each router

//router 0
vector<int> dest0 = {};
vector<int> dest1 = {0, 1};
vector<int> dest2 = {0, 2};
vector<int> dest3 = {0 , 1, 3};
vector<int> dest4 = {0, 1, 4};

vector<vector<int>> router0table = {dest0, dest1, dest2, dest3, dest4};

//router 1
dest0 = {1, 0};
dest1 = {};
dest2 = {1, 0, 2};
dest3 = {1, 3};
dest4 = {1, 4};

vector<vector<int>> router1table = {dest0, dest1, dest2, dest3, dest4};

//router 2
dest0 = {2, 0};
dest1 = {2, 0, 1};
dest2 = {};
dest3 = {2, 0, 1, 3};
dest4 = {2, 0, 1, 4};

vector<vector<int>> router2table = {dest0, dest1, dest2, dest3, dest4};

//router 3
dest0 = {3, 1, 0};
dest1 = {3, 1};
dest2 = {3, 1, 0, 2};
dest3 = {};
dest4 = {3, 1, 4};

vector<vector<int>> router3table = {dest0, dest1, dest2, dest3, dest4};

//router 4
dest0 = {4, 1, 0};
dest1 = {4, 1};
dest2 = {4, 1, 0, 2};
dest3 = {4, 1, 3};
dest4 = {};

vector<vector<int>> router4table = {dest0, dest1, dest2, dest3, dest4};

Router * router0 = new Router(0, router0table);
Router * router1 = new Router(1, router1table);
Router * router2 = new Router(2, router2table);
Router * router3 = new Router(3, router3table);
Router * router4 = new Router(4, router4table);

vector<Router *> nodes = {router0, router1, router2, router3, router4};

Network theNetwork;

theNetwork.setRoutingSystem(nodes);

//what the network looks like
/***********************************************
 
    node0--------------------node1
      |                     |     \
     |                     |       \
    |                     |         \
  node2                 node3      node4

***********************************************/

Packet * testPacket = new Packet(1, 0, 1); 
Packet * testPacket2 = new Packet(2, 1, 0); //this packet tests the effect of the start 
                                                  //and destination node using the previous packet to compare

Packet * testPacket3 = new Packet(3, 1, 4); //this packet test the efffect of moving between multiple nodes
                                                  //using the previous packet to compare
Packet * testPacket4 = new Packet(4, 2, 4);
Packet * testPacket5 = new Packet(5, 3, 2);
Packet * testPacket6 = new Packet(6, 4, 3);
Packet * testPacket7 = new Packet(7, 0, 2);


theNetwork.packetArrival(testPacket, testPacket->getSource());
theNetwork.packetArrival(testPacket2, testPacket2->getSource());
theNetwork.packetArrival(testPacket3, testPacket3->getSource());
theNetwork.packetArrival(testPacket4, testPacket4->getSource());
theNetwork.packetArrival(testPacket5, testPacket5->getSource());
theNetwork.packetArrival(testPacket6, testPacket6->getSource());
theNetwork.packetArrival(testPacket7, testPacket7->getSource());

while(!theNetwork.getEvents().empty()){
    if(theNetwork.getCurrentEvent().getType() == arrival){
        cout << "Arrival @ " << theNetwork.getCurrentEvent().getPlace() << endl;
        cout << "Packet # is: " << theNetwork.getCurrentEvent().getPacket()->getID() << endl;
        theNetwork.packetArrival(theNetwork.getCurrentEvent().getPacket(), theNetwork.getCurrentEvent().getPlace());
    }
    else{
        cout << "Departure @ " << theNetwork.getCurrentEvent().getPlace() << endl;
        cout << "Packet # is: " << theNetwork.getCurrentEvent().getPacket()->getID() << endl;
        theNetwork.packetDeparture(theNetwork.getCurrentEvent().getPlace());}
    theNetwork.getEvents().pop();
}

cout << "\n\n\n";

cout << "Packet 1 Delay: " <<testPacket->getDelay() << "ms" << endl;
cout << "Packet 2 Delay: " << testPacket2->getDelay()<< "ms" << endl;
cout << "Packet 3 Delay: " << testPacket3->getDelay()<< "ms" << endl;
cout << "Packet 4 Delay: " << testPacket4->getDelay()<< "ms" << endl;
cout << "Packet 5 Delay: " << testPacket5->getDelay()<< "ms" << endl;
cout << "Packet 6 Delay: " << testPacket6->getDelay()<< "ms" << endl;
cout << "Packet 7 Delay: " << testPacket7->getDelay()<< "ms" << endl;

}
#include <iostream>
#include <string>
#include "graph_class.h"

using namespace std;

void processCommand(char command, PersonGraph& pGraph);

//simple program for building and searching a graph of people, connected as friends
//initial data in a file called "people.txt", format as one person per line,
//after each person, their friends are one per line beginning with a "-". Each friend
//must also be on line to themselves as a person.

//after loading the file, the program goes into a command line loop with options to 
//add or delete people, add or delete friends, list the graph, list the friends of a 
//person, print a path between people, and list all people within a certain a 
//certain degree of separation

//the methods for implementing the functionality for each command are in the 
//PersonGraph class

int main(int argc, char **argv)
{
    const string FILENAME = "people.txt";
    
    PersonGraph pGraph;
    
    cout << "loading from file: " << FILENAME << endl << endl;
    pGraph.loadGraphFromFile(FILENAME);
    pGraph.printGraph();
    
    int command;
    do {
        cout << "\nMENU" << endl;
        cout << "1: print graph" << endl;
        cout << "2: list friends" << endl;
        cout << "3: list path between people" << endl;
        cout << "4: list people within n degrees of separation" << endl;
        cout << "5: add person" << endl;
        cout << "6: delete person" << endl;
        cout << "7: add friend" << endl;
        cout << "8: delete friend" << endl;
        cout << "9: quit" << endl << endl;
        if (cin >> command) {
            if (command >= 1 && command <= 8) {
                processCommand(command, pGraph);
            }
        } else {
            cout << "invalid entry" << endl;
            cin.clear();
            cin.ignore();
        }
    } while (command != 9);
     
	return 0;
}

void processCommand(char command, PersonGraph& pGraph) {
    string pName, fName;
    int degreeSep;
    if (command > 1) {
        cout << "enter person name" << endl;
        cin >> pName;
    }
    if (command >= 7 && command <= 8 ) {
        cout << "enter friend name" << endl;
        cin >> fName;
    }   
    cout << endl;
    switch (command) {
        case 1:
            pGraph.printGraph();
            break;
        case 2:
            pGraph.listFriends(pName);
            break;
        case 3:
            cout << "enter 2nd person name" << endl;
            cin >> fName;
            cout << endl;
            pGraph.listPathBetween(pName, fName);
            break;
        case 4:
            cout << "enter degrees of separation" << endl;
            cin >> degreeSep;
            cout << endl;
            pGraph.listPeopleWithinSep(pName, degreeSep);
            break;
        case 5:
            if (pGraph.addPerson(pName)) {
                cout << "person added" << endl;
            } else {
                cout << "no more room or person already exists" << endl;
            }
            break;
        case 6:
            if (pGraph.deletePerson(pName)) {
                cout << "person deleted" << endl;
            } else {
                cout << "no such person" << endl;
            }
            break;
        case 7:
            if (pGraph.addFriend(pName, fName)) {
                cout << "friend added" << endl;
            } else {
                cout << "person or friend does not exist, or already a friend of person" << endl;
            }
            break;
        case 8:
             if (pGraph.deleteFriend(pName, fName)) {
                cout << "friend deleted" << endl;
            } else {
                cout << "person or friend does not exist, or not a friend of person" << endl;
            }
            break;
    }
}

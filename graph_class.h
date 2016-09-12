#ifndef GRAPH_CLASS_H
#define GRAPH_CLASS_H

#include <string>
#include <fstream>
#include "person_class.h"

//the graph is represented as an adjacency list, implemented as an
//array of pointer to Person. Entries are intially NULL and set to a
//new Person when a person is added. If a person is deleted the entry
//is set to NULL again and can be re-used.
//A Person has a name and a friendList. 
//A friendList contains pointers to the head and tail of a list of friendNodes, 
//and an iterator. Each friendNode has an index to the array of Persons.

class PersonGraph {
private:
    static const int MAX_PERSONS = 25;
    int numPersons;
    int nextIndex;
    Person *persons[MAX_PERSONS];
    
public:
    PersonGraph() : numPersons(0), nextIndex(0) {
        for (int i = 0; i < MAX_PERSONS; i++) {
            persons[i] = NULL;
        }
    }
    ~PersonGraph() {
        for (int i = 0; i < nextIndex; i++) {
            if (persons[i] != NULL) {
                delete persons[i];
                persons[i] = NULL;
            }
        }
    }
      
    //public methods
    //getter and functions called by main
    int getNumPersons() { return numPersons; }
    bool addPerson(std::string pName);
    bool addFriend(std::string pName, std::string fName);
    bool deletePerson(std::string pName);
    bool deleteFriend(std::string pName, std::string fName);
    void loadGraphFromFile(std::string fileName);
    void printGraph();
    void listFriends(std::string pName);
    void listPathBetween(std::string pName, std::string fName);
    void listPeopleWithinSep(std::string pName, int degreeSep);
      
private:
    //helpers for loading data from file, finding a persons index given their name, listing
    void loadPeople(std::ifstream& ins);
    void loadFriends(std::ifstream& ins);
    int indexPerson(std::string fName);
    void listFriends(int index);
    void printList(FriendList& fList);
};

#endif
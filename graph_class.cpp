#include "graph_class.h"
#include <iostream>

using namespace std;

//If the person does not already exist, and if enough room in the array,
//create new person, find first NULL entry in array and set it to point to 
//new Person, and return true.
//Otherwise return false.

bool PersonGraph::addPerson(string pName) {
 
    int index = indexPerson(pName);
    if (index != -1) {
        return false;
    }
    int i;
    for (i = 0; i < nextIndex; i++) {
        if (persons[i] == NULL) {
            //found a NULL entry before the last person (due to a person 
            //being deleted), so add the new person here
            persons[i] = new Person(pName);
            break;
        }
    }
    if (i >= nextIndex) {
        if (nextIndex == MAX_PERSONS) {
            return false;
        }
        //next NULL entry was after all existing persons
        persons[nextIndex++] = new Person(pName);
    }
    numPersons++;
    return true;
}

//If the pName and fName exist in the array of Person,
//and the fName is not already a friend of the person, add
//the fName to the person's friend list, and the pName to the fName's friend list,
//and return true. 
//Otherwise return false.

bool PersonGraph::addFriend(string pName, string fName) {
    int index = indexPerson(pName);
    int fIndex = indexPerson(fName);
    
    //addFriend(fIndex) returns true if the fName was not already a friend of pName
    if ((index > -1) && (fIndex > -1) && persons[index]->addFriend(fIndex)) {
        persons[fIndex]->addFriend(index);
        return true;
    } else {
        return false;
    }
}

//If person pName exists, delete the person and set
//entry to NULL. Then delete pName from all person's friendLists. Return true.
//Otherwise return false.

bool PersonGraph::deletePerson(string pName) {
    int i;
    int index = indexPerson(pName);
    if (index > -1) {
        delete persons[index];
        persons[index] = NULL;
        numPersons--;
        for (i = 0; i < nextIndex; i++) {
            if (persons[i] != NULL) {
                persons[i]->deleteFriend(index);
            }
        }
        return true;
    } else {
        return false;
    }
}

//If pName and fName exist as Persons, and fName is a friend of pName,
//then delete fName from the person's friend list, and pName from the fName's friend list,
//and return true. 
//Otherwise return false.

bool PersonGraph::deleteFriend(string pName, string fName) {
    int index = indexPerson(pName);
    int fIndex = indexPerson(fName);
    if ((index > -1) && (fIndex > -1) && persons[index]->deleteFriend(fIndex)) {
        persons[fIndex]->deleteFriend(index);
        return true;
    } else {
        return false;
    }
}

//open file and load in two passes: one for the Persons, another for thier friends

void PersonGraph::loadGraphFromFile(string fileName) {
    const char *cfilename = fileName.c_str();
    ifstream ins(cfilename);
    if (!ins) {
        cerr << "failed to open " << cfilename << endl;
        return;
    }
    loadPeople(ins);
    ins.clear();
    ins.seekg(0, ios_base::beg) ;
    loadFriends(ins);
}

//list all Persons and their friends 

void PersonGraph::printGraph() {
    cout << "people in graph:" << endl;
    for (int i = 0; i < nextIndex; i++) {
        if (persons[i] != NULL) {
            cout << persons[i]->getName() << endl;
            listFriends(i);
        }
    }
}

//list all friends of person with name pName

void PersonGraph::listFriends(string pName) {
    int index = indexPerson(pName);
    if (index == -1 || persons[index] == NULL) {
        cout << "no such person" << endl;
        return;
    }
    cout << persons[index]->getName() << endl;
    listFriends(index);
}

//given two people, list the names of the people that form a chain of
//friend connections between them.
//Breadth-first search of graph.

void PersonGraph::listPathBetween(string pName, string fName) {
    int fIndex = indexPerson(pName);
    int index = indexPerson(fName);
    if (index == -1 || fIndex == -1) {
        cout << "no such person or 2nd person" << endl;
        return;
    }
    
    //for breadth-first search, keep lists of persons to visit and persons visited
    FriendList toVisitList, visitedList;
    
    //also to reconstruct path between people, keep a pair of lists, one with person visited,
    //the other with corresponding parent 
    //(reconstruct by tracing back parents, rather than making a tree)
    FriendList currentList, parentList;
    int currentIndex, parentIndex;
    
    //start with first person on list of persons to visit
    toVisitList.pushBack(index);
    while (!toVisitList.isEmpty()) {
        
        //get current person from toVisitList, put on visitedList
        currentIndex = toVisitList.popFront();
        visitedList.pushBack(currentIndex);
        
        if (currentIndex == fIndex) {
            //2nd person found
            break;
        } else {
            
            //for each friend, push onto toVisitList if not already visited
            //also add to current and parent lists
            FriendList::FriendIterator iter = persons[currentIndex]->getFriendIterator();
            while (iter.hasNext()) {
                if (!visitedList.find(*iter)) {
                    toVisitList.pushBack(*iter);
                    currentList.pushFront(*iter);
                    parentList.pushFront(currentIndex);
                }
                ++iter;
            }
        }
    }
    if (currentIndex == fIndex) {
        //path found
        //trace back parents, traversing current and parent lists in parallel
        cout << "path from " << pName << " to " << fName <<":" << endl;
        FriendList::FriendIterator currIter = currentList.begin();
        FriendList::FriendIterator parentIter = parentList.begin();
        while (currIter.hasNext() && parentIter.hasNext() && *currIter != currentIndex) {
            ++currIter;
            ++parentIter;
        }
        //for each current, print the name, save the parent index, then skip in both
        //lists until find the parent entry in the currentList
        cout << "    " << persons[*currIter]->getName() << endl;
        parentIndex = *parentIter;
        while (currIter.hasNext() && parentIter.hasNext()) {
            while (currIter.hasNext() && parentIter.hasNext() && *currIter != parentIndex) {
                ++currIter;
                ++parentIter;
            }
            //found next entry in currentList to print out
            //print name, and update parent index
            if (*currIter == parentIndex) {
                cout << "    " << persons[*currIter]->getName() << endl;
                parentIndex = *parentIter;
            }
        }
        cout << "    " << persons[parentIndex]->getName() << endl;
    } else {
        cout << "no path between " << pName << " and " << fName << endl;
    }
}

//For a given person, print all persons within the given degree of separation.
//Breadth-first search up to degreeSep levels

void PersonGraph::listPeopleWithinSep(string pName, int degreeSep) {
    int index = indexPerson(pName);
    if (index == -1) {
        cout << "no such person" << endl;
        return;
    }
    
    //for breadth-first search
    FriendList toVisitList, visitedList;
    
    //keep list of people to print out
    FriendList resultList;
    
    //keep list of level per person visited, in parallel with toVisitList
    FriendList sepCountList;
    
    int currentIndex, sepCount = 0;
    
    //start with given person, add to toVisitList and add 0 for corresponding
    //separation count
    toVisitList.pushBack(index);
    sepCountList.pushBack(0);
    while (!toVisitList.isEmpty() && sepCount <= degreeSep) {
        currentIndex = toVisitList.popFront();
        sepCount = sepCountList.popFront();
        if (sepCount <= degreeSep && !resultList.find(currentIndex)) {
            resultList.pushBack(currentIndex);
        }
        visitedList.pushBack(currentIndex);
        FriendList::FriendIterator iter = persons[currentIndex]->getFriendIterator();
        
        //for each friend of person, add to toVistList and add the current separation +1
        //to corresponding sepCountList
        while (iter.hasNext()) {
            if (!visitedList.find(*iter)) {
                toVisitList.pushBack(*iter);
                sepCountList.pushBack(sepCount+1);
            }
            ++iter;
        }
    }
    cout << "people within " << degreeSep << " of " << pName << ":" << endl;
    FriendList::FriendIterator resultIter = resultList.begin();
    while (resultIter.hasNext()) {
        cout << "    " << persons[*resultIter]->getName() << endl;
        ++resultIter;
    }
}

//////helpers

//return the index in the person array of a person with the given name.
//if it doesn't exist return -1

int PersonGraph::indexPerson(string fName) {
    for (int i = 0; i < nextIndex; i++) {
        if (persons[i] != NULL && persons[i]->getName() == fName) {
            return i;
        }
    }
    return -1;
}

//list all friends of a person at given index

void PersonGraph::listFriends(int index) {
    int fIndex;
    FriendList::FriendIterator iter = persons[index]->getFriendIterator();
    while (iter.hasNext()) {
        fIndex = *iter;
        cout << "    " << persons[fIndex]->getName() << endl;
        ++iter;
    }
}

//print the names on one list of a given friendList (used for debugging)

void PersonGraph::printList(FriendList& fList) {
    FriendList::FriendIterator iter = fList.begin();
    while (iter.hasNext()) {
        cout << persons[*iter]->getName() << "  ";
        ++iter;
    }
    cout << endl;
}

//first pass of loading from file: load the person (no "-") at beginning of line

void PersonGraph::loadPeople(ifstream& ins) {
    string name;
    while (ins >> name) {
        if (name.at(0) != '-') {
            addPerson(name);
        } 
    }
}

//second pass of loading from file: load the friends of each person

void PersonGraph::loadFriends(ifstream& ins) {
    string name, fName;
    int index = -1;
    while (ins >> name) {
        if (name.at(0) != '-') {
            //this is a person, increment to index to this person
            index++;
        } else {
            //get the friend name, if not already added, add fName to persons
            //friend list, and person to fNames friend list
            fName = name.substr(1);
            int fIndex = indexPerson(fName);
            if (fIndex > -1) {
                persons[index]->addFriend(fIndex);
                persons[fIndex]->addFriend(index);
            } else {
                cerr << fName << " is not a person, not added as friend" << endl;
            }
        }
    }
}

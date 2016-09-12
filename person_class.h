#ifndef PERSON_CLASS_H
#define PERSON_CLASS_H

#include <string>
#include "friend_list.h"

class Person {
private: 
    std::string personName;
    FriendList friendList;

public: 
    Person(std::string pName) : personName(pName) {};
    ~Person() {}
      
    std::string getName() {return personName; }
    void setName(std::string pName) { personName = pName; }
    FriendList::FriendIterator getFriendIterator() {return friendList.begin();}
    
    bool addFriend(int fIndex) {
        if (!friendList.find(fIndex)) {
            friendList.pushBack(fIndex);
            return true;
        } else {
            return false;
        }
    }
    
    bool deleteFriend(int fIndex) {
        return friendList.remove(fIndex);
    }
};

#endif
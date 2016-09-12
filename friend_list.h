#ifndef FRIEND_LIST_H
#define FRIEND_LIST_H

class FriendList {
private:
    #include "friend_node.h"
    FriendNode *head;
    FriendNode *tail;
    int numItems;
    
public:
    #include "friend_iterator.h"
    friend class FriendIterator;
    FriendList() {
        head = NULL;
        tail = NULL;
        numItems = 0;
    }
    ~FriendList() {
        while (head != NULL) {
            FriendNode *current = head;
            head = head->next;
            delete current;
        }
    }
    
    bool isEmpty() {
        return numItems == 0;
    }
    
    bool find(int fIndex) {
        FriendNode *current = head;
        while (current != NULL) {
            if (current->friendIndex == fIndex) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void pushFront(int item) {
        head = new FriendNode(item, head);
        if (head->next == NULL) {
            tail = head;
        }
        numItems++;
    }
    
    void pushBack(int item) {
        if (tail != NULL) {
            tail->next = new FriendNode(item, NULL);
            tail = tail->next;
            numItems++;
        } else {
            pushFront(item);
        }
    }
    

    int popFront() {
        if (head == NULL) {
            return -1;
        }
        int returnVal = head->friendIndex;
        FriendNode *temp = head;
        head = head->next;
        if (head == NULL) {
            tail = NULL;
        }
        delete temp;
        numItems--;
        return returnVal;
    }
    
    bool remove(int fIndex) {
        if (isEmpty()) {
            return false;
        }
        if (head->friendIndex == fIndex) {
            popFront();
            return true;
        }
        FriendNode *prev = head;
        FriendNode *current = head->next;
        while (current != NULL && current->friendIndex != fIndex) {
             prev = current;
             current = current->next;             
        }
        if (current != NULL) {
            prev->next = current->next;
            if (current->next == NULL) {
                tail = prev;
            }
            delete current;
            numItems--;
            return true;
        } else {
            return false;
        }
    }
    
    FriendIterator begin() {
        return FriendIterator(this, head);
    }
};

#endif
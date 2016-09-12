#ifndef FRIEND_ITERATOR_H
#define FRIEND_ITERATOR_H

class FriendIterator {
friend class FriendList;
private: 
    FriendList *parent;
    FriendNode *current;
    FriendIterator(FriendList *myParent, FriendNode *position) : 
        parent(myParent), current(position) {}
public:
    int operator*() {
        if (current == NULL) {
            return -1;
        }
        return current->friendIndex;
    }
    
    FriendIterator& operator++() {
        if (current != NULL ) {
            current = current->next;
        }
        return *this;
    }
    
    bool hasNext() {
        return (current != NULL);
    }
};

#endif
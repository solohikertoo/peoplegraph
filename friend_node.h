#ifndef FRIEND_NODE_H
#define FRIEND_NODE_H

struct FriendNode {
    int friendIndex;
    FriendNode *next;
    
    FriendNode() : friendIndex(-1), next(NULL) {} 
    FriendNode(int fIndex, FriendNode *fNext = NULL) : friendIndex(fIndex), next(fNext) {};
};

#endif
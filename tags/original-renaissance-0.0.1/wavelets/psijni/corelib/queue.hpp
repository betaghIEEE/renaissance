#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

template <class Item>

class queue
{
private:
    // iternal structure
    struct node {
        Item item; 
        node* next;
        node (Item x)
        {item = x; next = 0; }
    };
    typedef node *link;
    link head, tail;
public:
        queue () { head = 0};
    int empty();
    int put (Item myItem);
    Item get ();
};

#endif
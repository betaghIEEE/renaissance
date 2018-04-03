#include "queue.hpp"

int queue::empty()
{
    return head == 0;
}


int queue::put (Item myItem)
{
    link t = tail;
    tail = new node(x);
    if ( head == 0)
        head = tail;
    else t->next = tail;
}

Item queue::get ()
{
    Item v = head->item;
    link t = head->next;
    delete head;
    head = t;
    return v;
}
//
// LocklessQueue.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_THREAD_LOCKLESSQUEUE_H_INCLUDED
#define COIN_THREAD_LOCKLESSQUEUE_H_INCLUDED

#include <functional>


namespace coin
{
	
namespace thread
{

class LocklessQueue
///@with-apologies-to http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448
{
private:
    struct Node
    {
        std::function<void()> callback;
        Node*                 next = nullptr;

        Node() : callback() {}
        Node( std::function<void()> callback ) : callback(callback) {}
    };

    Node*              first_;   // Used by the producer only.
    std::atomic<Node*> divider_; // Shared by both producer and consumer.
    std::atomic<Node*> last_;    // Shared by both producer and consumer.

public:
    LocklessQueue();
    ~LocklessQueue();

    bool is_empty() const;

    void call_later( std::function<void()> callback );
    void flush();
    void clear();
};


inline
LocklessQueue::LocklessQueue()
: first_( new Node() ),
  divider_( first_ ),
  last_( first_ )
{
}


inline
LocklessQueue::~LocklessQueue()
{
    while( first_ )
    {
        Node* node = first_;
        first_     = node->next;
        delete node;
    }
}


inline
bool 
LocklessQueue::is_empty() const
{
	return divider_ == last_;
}


inline
void
LocklessQueue::call_later( std::function<void()> callback )
{
    last_.load()->next = new Node( callback ); //@!- In order to be more effective when used for audio, this should use an emplace new to avoid the stall.
    last_              = last_.load()->next;

    while( first_ != divider_ )
    {
        Node* node = first_;
        first_     = first_->next;
        delete node;
    }
}


inline
void
LocklessQueue::flush()
{
    while( divider_ != last_ )
    {
        std::function<void()> callback = divider_.load()->next->callback;
        divider_ = divider_.load()->next;
        callback();
    }
}

inline
void
LocklessQueue::clear()
{
    while( divider_ != last_ )
    {
        divider_ = divider_.load()->next;
    }
}

}

}

#endif //COIN_THREAD_LOCKLESSQUEUE_H_INCLUDED

#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
struct Node
{
    T value;
    Node* next;
    std::mutex* node_mutex;
};

template <class T>
class FineGrainedQueue
{
    Node<T>* head;
    std::mutex* queue_mutex;

public:
    void insertIntoMiddle(T value, int pos)
    {
        Node* head_, * tail_;// указатели на предыдущий и текущий элемент
        queue_mutex->lock();// head нельзя будет изменить

        head_ = this->head;
        tail_ = this->head->next;

        head_->node_mutex->lock(); //закрытие head

        int count = 0;
        Node<T> newNode;
        newNode.value = value;
        newNode.next = nullptr;

        queue_mutex->unlock(); // теперь можно открыть всю очередь

        if (tail_) //проверка, не явл head последним элементом
        {
            tail_->node_mutex->lock();
        }
        else
        {

        }


        while (tail_) // пока указатель на следующий элемент не нулевой
        {
            if (count == pos)
            {

                newNode.next = tail_;
                tail_ = *newNode;

                head_->node_mutex->unlock();
                tail_->node_mutex->unlock();

                return;
            }

            // если не нашли элемент для вставки, то двигаемся дальше по очереди
            Node* oldHead_ = head_;
            head_ = tail_;
            tail_ = head_->next;

            oldHead_->node_mutex->unlock();// обратите внимание, после сдвига что анлочится "старый" предыдущий элемент
            tail_->node_mutex->lock(); // а потом лочится новый текущий элемент

            count++;
        }

        tail_ = *newNode;//если  номер места вставки не найден,то вставляем элемент последним  

        head_->node_mutex->unlock();

    }
};


int main()
{

    FineGrainedQueue<int> myQueue;

}

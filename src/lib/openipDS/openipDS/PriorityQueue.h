/* 
 * File:   PriorityQueue.h
 * Author: gykovacs
 *
 * Created on February 24, 2011, 12:45 PM
 */

#ifndef PRIORITYQUEUE_H
#define	PRIORITYQUEUE_H

#include <utility>
#include <algorithm>
#include <vector>

namespace openip
{
    template<typename PRIORITY, typename DATA>
    class PriorityQueueElement: public std::pair<PRIORITY,DATA>
    {
    public:
        PriorityQueueElement(PRIORITY p, DATA d);

        PriorityQueueElement(const PriorityQueueElement& p);

        ~PriorityQueueElement();

        virtual bool operator==(const PriorityQueueElement& p);

        virtual bool operator<(const PriorityQueueElement& p);

        virtual bool operator>(const PriorityQueueElement& p);

        virtual bool operator<=(const PriorityQueueElement& p);

        virtual bool operator>=(const PriorityQueueElement& p);
    };

    template<typename PRIORITY, typename DATA>
    PriorityQueueElement<PRIORITY, DATA>::PriorityQueueElement(PRIORITY p, DATA d)
    : pair<PRIORITY, DATA>(p,d)
    {
    }

    template<typename PRIORITY, typename DATA>
    PriorityQueueElement<PRIORITY, DATA>::PriorityQueueElement(const PriorityQueueElement& p)
    : pair<PRIORITY,DATA>(p)
    {
    }

    template<typename PRIORITY, typename DATA>
    PriorityQueueElement<PRIORITY, DATA>::~PriorityQueueElement()
    {
    }

    template<typename PRIORITY, typename DATA>
    bool PriorityQueueElement<PRIORITY, DATA>::operator==(const PriorityQueueElement& p)
    {
        return this->first == p.first;
    }

    template<typename PRIORITY, typename DATA>
    bool PriorityQueueElement<PRIORITY, DATA>::operator>(const PriorityQueueElement& p)
    {
        return this->first > p.first;
    }

    template<typename PRIORITY, typename DATA>
    bool PriorityQueueElement<PRIORITY, DATA>::operator<(const PriorityQueueElement& p)
    {
        return this->first < p.first;
    }

    template<typename PRIORITY, typename DATA>
    bool PriorityQueueElement<PRIORITY, DATA>::operator>=(const PriorityQueueElement& p)
    {
        return this->first >= p.first;
    }

    template<typename PRIORITY, typename DATA>
    bool PriorityQueueElement<PRIORITY, DATA>::operator<=(const PriorityQueueElement& p)
    {
        return this->first <= p.first;
    }

    template<typename PRIORITY, typename DATA>
    class PriorityQueue: public std::vector<PriorityQueueElement<PRIORITY, DATA> >
    {
    public:
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::begin;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::end;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::front;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::pop_back;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::push_back;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::size;
        using std::vector<PriorityQueueElement<PRIORITY, DATA> >::operator[];

        PriorityQueue();

        PriorityQueue(const PriorityQueue& p);

        ~PriorityQueue();

        void init();

        void push(PriorityQueueElement<PRIORITY, DATA> p);

        void push(PRIORITY p, DATA d);

        PriorityQueueElement<PRIORITY, DATA> topPQE();

        void pop();

        DATA top();

        PRIORITY topP();
    };

    template<typename PRIORITY, typename DATA>
    PriorityQueue<PRIORITY, DATA>::PriorityQueue()
    : std::vector<PriorityQueueElement<PRIORITY, DATA> >()
    {
    }

    template<typename PRIORITY, typename DATA>
    PriorityQueue<PRIORITY, DATA>::PriorityQueue(const PriorityQueue& p)
    : std::vector<PriorityQueueElement<PRIORITY, DATA> >(p)
    {
    }

    template<typename PRIORITY, typename DATA>
    PriorityQueue<PRIORITY, DATA>::~PriorityQueue()
    {
    }

    template<typename PRIORITY, typename DATA>
    void PriorityQueue<PRIORITY, DATA>::init()
    {
        std::stable_sort(this->begin(), this->end());
    }

    template<typename PRIORITY, typename DATA>
    void PriorityQueue<PRIORITY, DATA>::push(PriorityQueueElement<PRIORITY, DATA> p)
    {
        this->push_back(p);
        std::stable_sort(this->begin(), this->end());
    }

    template<typename PRIORITY, typename DATA>
    void PriorityQueue<PRIORITY, DATA>::push(PRIORITY p, DATA d)
    {
        this->push_back(PriorityQueueElement<PRIORITY, DATA>(p,d));
        std::stable_sort(this->begin(), this->end());
    }

    template<typename PRIORITY, typename DATA>
    PriorityQueueElement<PRIORITY, DATA> PriorityQueue<PRIORITY, DATA>::topPQE()
    {
        return this->back();
    }

    template<typename PRIORITY, typename DATA>
    void PriorityQueue<PRIORITY, DATA>::pop()
    {
        this->erase(this->end()-1);
        std::stable_sort(this->begin(), this->end());
    }

    template<typename PRIORITY, typename DATA>
    DATA PriorityQueue<PRIORITY, DATA>::top()
    {
        return this->back().second;
    }

    template<typename PRIORITY, typename DATA>
    PRIORITY PriorityQueue<PRIORITY, DATA>::topP()
    {
        return this->back().first;
    }

    template<typename DATA>
    class PriorityQueueObjectBased: public std::vector<DATA>
    {
    public:
        using std::vector<DATA>::begin;
        using std::vector<DATA>::end;
        using std::vector<DATA>::front;
        using std::vector<DATA>::pop_back;
        using std::vector<DATA>::push_back;
        using std::vector<DATA>::size;
        using std::vector<DATA>::clear;

        PriorityQueueObjectBased();

        PriorityQueueObjectBased(const PriorityQueueObjectBased& p);

        ~PriorityQueueObjectBased();

        void init();

        void push(const DATA& p);

        void pop();

        DATA top();
    };

    template<typename DATA>
    PriorityQueueObjectBased<DATA>::PriorityQueueObjectBased()
    : std::vector<DATA>()
    {
    }

    template<typename DATA>
    PriorityQueueObjectBased<DATA>::PriorityQueueObjectBased(const PriorityQueueObjectBased& p)
    : std::vector<DATA>(p)
    {
    }

    template<typename DATA>
    PriorityQueueObjectBased<DATA>::~PriorityQueueObjectBased()
    {
    }

    template<typename DATA>
    void PriorityQueueObjectBased<DATA>::init()
    {
        std::stable_sort(this->begin(), this->end());
    }

    template<typename DATA>
    void PriorityQueueObjectBased<DATA>::push(const DATA& p)
    {
        this->push_back(p);
        std::stable_sort(this->begin(), this->end());
    }

    template<typename DATA>
    void PriorityQueueObjectBased<DATA>::pop()
    {
        this->erase(this->end()-1);
        std::stable_sort(this->begin(), this->end());
    }

    template<typename DATA>
    DATA PriorityQueueObjectBased<DATA>::top()
    {
        return this->back();
    }
}


#endif	/* PRIORITYQUEUE_H */


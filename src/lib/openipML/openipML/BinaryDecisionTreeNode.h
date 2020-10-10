/* 
 * File:   BinaryDecisionTreeNode.h
 * Author: gykovacs
 *
 * Created on November 4, 2011, 1:56 PM
 */

#ifndef BINARYDECISIONTREENODE_H
#define	BINARYDECISIONTREENODE_H

#include <openipML/BinaryTreeNode.h>

namespace openip
{
    template<typename T>
    class BinaryDecisionTreeNode: TreeNode<T>
    {
        public:
            BinaryDecisionTreeNode(bool terminal= false, int attribute= 0, T threshold= 0);

            BinaryDecisionTreeNode(const BinaryDecisionTreeNode& t);

            ~BinaryDecisionTreeNode();

            BinaryDecisionTreeNode<T>* child(int i);

            BinaryDecisionTreeNode<T>* children();
            
            BinaryDecisionTreeNode<T>* left();
            
            BinaryDecisionTreeNode<T>* right();

            int numberOfChildren;
            
            int attribute;
            
            T threshold;
            
            bool terminal;
            
            int label;
	    
	    float weight;

            BinaryDecisionTreeNode<T>** childrenNodes;
    };

    template<typename T>
    BinaryDecisionTreeNode<T>::BinaryDecisionTreeNode(bool terminal, int attribute, T threshold)
    {
        numberOfChildren= 2;
        childrenNodes= new BinaryDecisionTreeNode<T>*[2];
        for ( int i= 0; i < 2; ++i )
            childrenNodes[i]= NULL;
        this->terminal= terminal;
        if ( terminal )
        {
            this->label= attribute;
            this->attribute= -1;
        }
        else
        {
            this->attribute= attribute;
            this->label= -1;
        }
        this->threshold= threshold;
    }

    template<typename T>
    BinaryDecisionTreeNode<T>::BinaryDecisionTreeNode(const BinaryDecisionTreeNode& t)
    {
        this->numberOfChildren= numberOfChildren;
        for ( int i= 0; i < numberOfChildren; ++i )
            children[i]= t.children[i];
        this->terminal= t.terminal;
        this->label= t.label;
        this->attribute= t.attribute;
        this->threshold= t.threshold;
	this->weight= t.weight;
    }

    template<typename T>
    BinaryDecisionTreeNode<T>::~BinaryDecisionTreeNode()
    {
        for ( int i= 0; i < numberOfChildren; ++i )
            if ( childrenNodes[i] )
                delete childrenNodes[i];
        delete childrenNodes;
    }

    template<typename T>
    BinaryDecisionTreeNode<T>* BinaryDecisionTreeNode<T>::child(int i)
    {
        return childrenNodes[i];
    }

    template<typename T>
    BinaryDecisionTreeNode<T>* BinaryDecisionTreeNode<T>::children()
    {
        return childrenNodes;
    }
    
    template<typename T>
    BinaryDecisionTreeNode<T>* BinaryDecisionTreeNode<T>::left()
    {
        return childrenNodes[0];
    }
    
    template<typename T>
    BinaryDecisionTreeNode<T>* BinaryDecisionTreeNode<T>::right()
    {
        return childrenNodes[1];
    }
}

#endif	/* BINARYDECISIONTREENODE_H */


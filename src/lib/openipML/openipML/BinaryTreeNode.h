/* 
 * File:   BinaryTreeNode.h
 * Author: gykovacs
 *
 * Created on November 4, 2011, 1:47 PM
 */

#ifndef BinaryTreeNode_H
#define	BinaryTreeNode_H

#include <openipML/TreeNode.h>

namespace openip
{
    template<typename T>
    class BinaryTreeNode: TreeNode<T>
    {
        public:
            BinaryTreeNode();

            BinaryTreeNode(const BinaryTreeNode& t);

            ~BinaryTreeNode();

            BinaryTreeNode<T>* child(int i);

            BinaryTreeNode<T>* children();
            
            BinaryTreeNode<T>* left();
            
            BinaryTreeNode<T>* right();

            int numberOfChildren;

            BinaryTreeNode<T>* childrenNodes;
    };

    template<typename T>
    BinaryTreeNode<T>::BinaryTreeNode()
    {
        numberOfChildren= 2;
        childrenNodes= new BinaryTreeNode<T>*[2];
        for ( int i= 0; i < 2; ++i )
            childrenNodes[i]= NULL;
    }

    template<typename T>
    BinaryTreeNode<T>::BinaryTreeNode(const BinaryTreeNode& t)
    {
        this->numberOfChildren= numberOfChildren;
        for ( int i= 0; i < numberOfChildren; ++i )
            children[i]= t.children[i];
    }

    template<typename T>
    BinaryTreeNode<T>::~BinaryTreeNode()
    {
        for ( int i= 0; i < numberOfChildren; ++i )
            if ( childrenNodes[i] )
                delete childrenNodes[i];
        delete childrenNodes;
    }

    template<typename T>
    BinaryTreeNode<T>* BinaryTreeNode<T>::child(int i)
    {
        return childrenNodes[i];
    }

    template<typename T>
    BinaryTreeNode<T>* BinaryTreeNode<T>::children()
    {
        return childrenNodes;
    }
    
    template<typename T>
    BinaryTreeNode<T>* BinaryTreeNode<T>::left()
    {
        return childrenNodes[0];
    }
    
    template<typename T>
    BinaryTreeNode<T>* BinaryTreeNode<T>::right()
    {
        return childrenNodes[1];
    }
}

#endif	/* BinaryTreeNode_H */


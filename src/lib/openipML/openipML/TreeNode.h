/* 
 * File:   TreeNode.h
 * Author: gykovacs
 *
 * Created on November 4, 2011, 1:35 PM
 */

#ifndef TREENODE_H
#define	TREENODE_H

namespace openip
{
    template<typename T>
    class TreeNode
    {
        public:
            TreeNode(int n= 2);

            TreeNode(const TreeNode& t);

            ~TreeNode();

            TreeNode<T>* child(int i);

            TreeNode<T>* children();

            int numberOfChildren;

            TreeNode<T>** childrenNodes;
    };

    template<typename T>
    TreeNode<T>::TreeNode(int n)
    {
        numberOfChildren= n;
        childrenNodes= new TreeNode<T>*[n];
        for ( int i= 0; i < n; ++i )
            childrenNodes[i]= NULL;
    }

    template<typename T>
    TreeNode<T>::TreeNode(const TreeNode& t)
    {
        this->numberOfChildren= numberOfChildren;
        for ( int i= 0; i < numberOfChildren; ++i )
            children[i]= t.children[i];
    }

    template<typename T>
    TreeNode<T>::~TreeNode()
    {
        for ( int i= 0; i < numberOfChildren; ++i )
            if ( childrenNodes[i] )
                delete childrenNodes[i];
        delete childrenNodes;
    }

    template<typename T>
    TreeNode<T>* TreeNode<T>::child(int i)
    {
        return childrenNodes[i];
    }

    template<typename T>
    TreeNode<T>* TreeNode<T>::children()
    {
        return childrenNodes;
    }
}


#endif	/* TREENODE_H */


#include "binSearchTree.h"

int main()
{
	BinarySearchTree<int> bst;

	// Fancy way of not having to write BinarySearchTree<int> twice:
	const auto Order = decltype(bst)::Traversal::PostOrder;
	// const auto Order2 = decltype(t)::Traversal::PreOrder;
	// const auto Order3 = decltype(t)::Traversal::PostOrder;


	for (int n : {2, 1, 6, 3, 7, 5, 4 })
	{
		bst.insert(n);
	}

	// std::cout << "InOrder " << *bst.begin(Order) << std::endl;
	                                                             /*	        2                 */
	for (auto i = bst.begin(Order); i != bst.end(); i++)         /*    1         6            */
	{                                                            /*           3        7      */
		std::cout << *i<<" ";                                    /*              5            */
	}                                                            /*           4               */

	std::cout<<"\n(1 4 5 3 7 6 2) <-Expected traversal order"<<std::endl;

	std::cout<<std::endl;
		// bst.dot(std::cout);


	/*	bool tmp;
		tmp = nullptr == nullptr ? true : false;
		std::cout<<"-------   "<< tmp<<std::endl;*/


	// Insert order:  2, 1, 6, 3, 7, 5, 4
	// Expected traversal order:   1 4 5 3 7 6 2
	// Actual traversal order:     1 3 7 6 2

	// Test description:
	// - construct an empty BinarySearchTree<double>
	// - call tree.begin(Traversal::InOrder)
	//        - check (not (begin != end))
	// 	       --------------------------------------------------------------------------------
	// Console output:
	// --------------------------------------------------------------------------------
	// 	--------------------------------------------------------------------------------
	// 		Error output:
	// --------------------------------------------------------------------------------
	// 	--------------------------------------------------------------------------------
	// 		Result: segmentation fault
	//


//	std::cout << "InOrder end() " << *bst.end() << std::endl;

	/*std::cout << "InOrder " << *bst.begin(Order) << std::endl;
	std::cout << "PreOrder " << *bst.begin(Order2) << std::endl;
	std::cout << "PostOrder " << *bst.begin(Order3) << std::endl;
	bool tmp = *bst.begin(Order) != *bst.begin(Order2);
	bool tmp2 = *bst.begin(Order) != *bst.begin(Order3);
	std::cout << "InOrder != PreOrder ? " << tmp << std::endl;
	std::cout << "InOrder != PostOrder ? " << tmp2 << std::endl;*/

	//	bst.dot(std::cout);

	// Create a tree of integers:
	/*	BinarySearchTree<int> t;

		// Fancy way of not having to write BinarySearchTree<int> twice:
		const auto Order = decltype(t)::Traversal::InOrder;

		// Insert the values 4, 2, 6, 1, 3, 5 and 7:
		for (int n : {4, 2, 6, 1, 3, 5, 7})
		{
			t.insert(n);
		}

		// The following should print: 1234567
		for (auto i = t.begin(Order); i != t.end(); i++)
		{
			std::cout << *i;
		}*/

	return 0;
}
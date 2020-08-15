#include <cassert>
#include <cstddef>      // for definition of size_t
#include <functional>   // std::function
#include <memory>       // std::unique_ptr
#include <iostream>
#include <utility>
#include <stack>

template<typename T, typename Comparator = std::less<T>>
class BinarySearchTree
{
public:
	enum class Traversal
	{
		PreOrder,
		InOrder,
		PostOrder,
	};
private:
	struct Node
	{
		Node(T value)
			: element_(std::move(value))
		{}

		void dot(std::ostream &o) const
		{
			// NOTE: this is slightly more complicated than strictly
			//       necessary (using addresses as names, etc.), but
			//       it will produce valid Dot output even when the
			//       node values are things like money (e.g., $17)

			o
				<< "  \"" << this << "\""
				<< " [ label = \"" << element_ << "\" ];\n";

			if (left_)
			{
				o
					<< "  \"" << this << "\""
					<< " -> "
					<< "\"" << left_.get() << "\""
					<< " [ label = \"L\" ]\n";

				left_->dot(o);
			}

			if (right_)
			{
				o
					<< "  \"" << this << "\""
					<< " -> "
					<< "\"" << right_.get() << "\""
					<< " [ label = \"R\" ]\n";

				right_->dot(o);
			}
		}

		T element_;
		std::unique_ptr<Node> left_;
		std::unique_ptr<Node> right_;
		Node *parent_ = nullptr;
	};

public:
	//! Insert a new value into the appropriate place in the tree.
	void insert(T value)
	{
		insert(std::move(value), root_);
	}

	//Operator o.l. to add nodes using bst<<12<<31<<4<<...
	BinarySearchTree &operator<<(T value)
	{
		insert(std::move(value), root_);
		return *this;
	}


	void dot(std::ostream &o) const
	{
		o << "digraph {\n";

		if (root_)
		{
			root_->dot(o);
		}

		o << "}\n";
	}

	/**
	 * An iterator that can traverse the BST in some order.
	 * The iterator contains a "current" node, a stack of parent nodes and
	 * a Traversal value to remind it which strategy it's following.*/
	class Iterator
	{
	public:
		Iterator(Node *root, Traversal &order)
			: iter_(root), order_(order)
		{}

		/*used to call end*/
		Iterator()
			: iter_(nullptr), order_(Traversal::InOrder)
		{}

		/* Move to the next node in the tree that should be accessed.
		 * This operator method just calls private methods to try and
		 * keep the logic of the various traversal mechanisms clear. */
		Iterator operator++(int i)
		{
			if (order_ == Traversal::InOrder)
			{
				nextInOrder();
			}
			if (order_ == Traversal::PostOrder)
			{
				nextPostOrder();
			}
			if (order_ == Traversal::PreOrder)
			{
				nextPreOrder();
			}
			return *this;
		}

		//! Dereference the iterator at its current position
		const T &operator*()
		{
			return (this->iter_->element_);
		}

		//! Is this iterator *not* the same as another?
		bool operator!=(const Iterator &other)
		{
			return iter_ != other.iter_;
		}

		/*
		 * getStart() is called by begin() */
		Iterator getStart()
		{
			if (order_ == Traversal::PreOrder)
			{
				return *this;
			}
			if (order_ == Traversal::PostOrder)
			{
				while (iter_->left_.get() || iter_->right_.get())
				{
					iter_ = iter_->left_.get() ? iter_->left_.get() : iter_->right_.get();
				}
				return *this;

			}
			if (order_ == Traversal::InOrder)
			{
				while (iter_->left_)
				{
					iter_ = iter_->left_.get();
				}
				return *this;
			}
			return *this;
		}


	private:
		// add whatever else you need here

		void nextInOrder()
		{
			if (iter_->right_)
			{
				iter_ = iter_->right_.get();
				while (iter_->left_)
				{
					iter_ = iter_->left_.get();
				}
			} else
			{
				while (iter_->parent_ && iter_->parent_->right_.get() == iter_)
				{
					iter_ = iter_->parent_;
				}
				iter_ = iter_->parent_;
			}

		}


		void nextPostOrder()
		{
			if (iter_->parent_ == nullptr)
			{
				iter_ = iter_->parent_;
			} else if (iter_->parent_->right_ == nullptr || iter_ == iter_->parent_->right_.get())
			{
				iter_ = iter_->parent_;
			} else  //(iter_->parent_->right_ != nullptr && iter_ == iter_->parent_->left_)
			{
				iter_ = iter_->parent_->right_.get();
				while (iter_->left_)
				{
					iter_ = iter_->left_.get();
				}
			}
		}

		void nextPreOrder()
		{
			if (iter_->left_)
			{
				iter_ = iter_->left_.get();
			} else if (iter_->right_)
			{
				iter_ = iter_->right_.get();
			} else
			{
				auto tmp = iter_;
				iter_ = iter_->parent_;
				Comparator compareTmp;
				while (iter_)
				{
					if (compareTmp(tmp->element_, iter_->element_) && iter_->right_)
						break;
					iter_ = iter_->parent_;
				}
				iter_ = iter_ ? iter_->right_.get() : nullptr;

			}
		}

		Node *iter_ = nullptr;
		Node *root_ = nullptr;

		const Traversal order_;
	};


	/**
 	* Returns an iterator that can be used to traverse the tree in the given order.
 	* This iterator should visit every node in the tree exactly once, after which
 	* it should test equal to the iterator returned from `end()`.
 	*/
	Iterator begin(Traversal order)
	{
		Iterator beginIter(root_ ? root_.get() : nullptr, order);
		return beginIter.getStart();
	}

	/**
	 * The end of a tree traversal.
	 * The iterator returned by this method should be usable as the end-of-iteration
	 * marker for any iterator on this tree, whether it was traversing the tree in
	 * pre-, in- or post-order.
	 */
	Iterator end()
	{
		Iterator endOf;
		return endOf;
	}

private:

	/**
	 * Internal implementation of recursive insert.
	 * @param   value      the value to insert
	 * @param   node       the root of the (sub-)tree being inserted into;
	 *                     may be null if the (sub-)tree is empty
	 */
	void insert(T &&value, std::unique_ptr<Node> &node)
	{
		if (!node)
		{
			node = std::make_unique<Node>(std::move(value));
		} else if (compare_(value, node->element_))          /* if ("insert value" is less then current "node value")*/
		{
			if (node->left_ != nullptr)
			{
				insert(std::move(value), node->left_);
			} else
			{
				auto childNode = std::make_unique<Node>(std::move(value));
				childNode->parent_ = node.get();
				node->left_ = std::move(childNode);
			}

		} else if (compare_(node->element_, value))       /* if ("insert value" is NOT less then current "node value")*/
		{
			if (node->right_ != nullptr)
			{
				insert(std::move(value), node->right_);
			} else
			{
				auto childNode = std::make_unique<Node>(std::move(value));
				childNode->parent_ = node.get();
				node->right_ = std::move(childNode);
			}
		} else
		{
			// assuming no duplicates
		}
	}

	Comparator compare_;
	std::unique_ptr<Node> root_;
};


/*
 *                                 (7)
 *                              /       \
 *                          (5)           (10)
 *                         /   \        /      \
 *                      (3)     (6)   (8)       (14)
 *                      /   \           \        /
 *                    (1)   (4)         (9)    (12)
 *
 *        InOrder:  1,3,4,5,6,7,8,9,10
 *
 *        PostOrder: 1,4,3,6,5,9,8,12,14,10,7
 *
 *        PreOrder: 7,5,3,1,4,6,10,8,9,14,12
 *
 * */
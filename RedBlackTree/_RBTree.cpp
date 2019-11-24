#ifndef RBTREE_H
#define RBTREE_H
#define NIL NULL

enum BOOL_VAL {FALSE,TRUE};
enum COLOR { RED, BLACK };
typedef short BOOL;

template <typename T>
struct TreeNode{
  T value;
  int color;
  TreeNode<T>* l;
  TreeNode<T>* r;
  TreeNode<T>* p;
};

template <typename ElementType>
class RBTree {
private:
  void LEFT_RORATE(TreeNode<ElementType>* node);
  void RIGHT_RORATE(TreeNode<ElementType>* node);

public:
  RBTree();
  ElementType getElementByValue();
}

#endif // !_RBTREE_H

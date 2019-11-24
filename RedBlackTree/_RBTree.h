//红黑树
#ifndef RBTREE_H
#define RBTREE_H
#define NIL NULL

enum BOOL_VAL { FALSE, TRUE };
enum COLOR { RED, BLACK };
enum CMP { BIGGER, EQUAL, SMALLER };
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
  void LEFT_RORATE(const TreeNode<ElementType>* node);//左旋
  void RIGHT_RORATE(const TreeNode<ElementType>* node);//右旋
  void CHANGE_COLOR(const TreeNode<ElementType>* node);//变色
  void freeTree(TreeNode<ElementType>* node);
  int (*cmp)(const ElementType& e1,const ElementType& e2);//比较大小
  int len;
  TreeNode<ElementType>* head;
  TreeNode<ElementType>* nil;
public:
  //若e1大于e2，返回BIGGER；若e1等于e2，返回EQUAL；若e1小于e2，返回SMALLER
  RBTree(int(*cmp)(const ElementType& e1, const ElementType& e2));
  ~RBTree();
  void destory();
  ElementType* get(const ElementType element);
  ElementType* put(ElementType element);
  void remove(ElementType element);
  int length();
};

template <typename ElementType>
RBTree<ElementType>::RBTree(int(*_cmp)(const ElementType& e1, const ElementType& e2)) {
  this->cmp = _cmp;
  this.len = 0;
  nil = NULL;
  head = NULL;
}

template <typename ElementType>
RBTree<ElementType>::~RBTree() {
  freeTree(head);
}

template <typename ElementType>
void RBTree<ElementType>::LEFT_RORATE(const TreeNode<ElementType>* node) {
  TreeNode<ElementType>* n = node;
  if (n->r == nil) {
    return;
  }
  if (n==head) {
    head = n->r;
    n->r = head->l;
    head->l = n;
    return;
  }
  TreeNode<ElementType>* temp = node->p;
  if (temp->l == n) {
    temp->l = n->r;
  }
  else {
    temp->r = n->r;
  }
  temp = n->r;
  n->r = temp->l;
  temp->l = n;
}

template <typename ElementType>
void RBTree<ElementType>::RIGHT_RORATE(const TreeNode<ElementType>* node) {
  TreeNode<ElementType>* n = node;
  if (n->l == nil) {
    return;
  }
  if (n == head) {
    head = n->l;
    n->l = head->r;
    head->r = n;
    return;
  }
  TreeNode<ElementType>* temp = node->p;
  if (temp->l == n) {
    temp->l = n->l;
  }
  else {
    temp->r = n->l;
  }
  temp = node->l;
  n->l = temp->r;
  temp->r = n;
}

template <typename ElementType>
void RBTree<ElementType>::CHANGE_COLOR(const TreeNode<ElementType>* node) {
  node->color = node->color ? RED : BLACK;
}

template <typename ElementType>
ElementType* RBTree<ElementType>::get(ElementType element) {
  TreeNode<ElementType>* h = head;
  while (1) {
    if (h == nil) {
      return nil;
    }
    if (cmp(element, h->value) == EQUAL) {
      return h->value;
    }
    else if (cmp(element, h->value) == BIGGER) {
      h = h->l;
    }
    else {
      h = h->r;
    }
  }
}

template <typename ElementType>
int RBTree<ElementType>::length() {
  return len;
}

template <typename ElementType>
ElementType* RBTree<ElementType>::put(ElementType element) {
  len++;
  TreeNode<ElementType>* node = new TreeNode<ElementType>;
  node->value = element;
  node->color = RED;
  TreeNode<ElementType>* h = head;
  
  //1.头节点位空
  if (h = nil) {
    h = node;
    CHANGE_COLOR(node);
    return node;
  }

  //2.头节点不为空
  BOOL LEFT = TRUE;//标注插入位置为父节点的左右孩子
  while (h != nil) {
    node->p = h;
    //2.1结点已存在，更新结点
    if (cmp(node->value, h->value) == EQUAL) {
      len--;
      h->value = node->value;
      delete node;
      node = NULL;
      return h;
    }
    else if (cmp(node->value, h->value)==BIGGER) {
      h = h->l;
      LEFT = TRUE;
    }
    else {
      h = h->r;
      LEFT = FALSE;
    }
  }
  //2.2结点不存在，进行插入
  TreeNode<ElementType>* p = node->p;//标注插入位置的父节点
  if (LEFT) {
    p->l = node;
  }
  else {
    p->r = node;
  }
  TreeNode<ElementType>* s = node;//当前结点
  TreeNode<ElementType>* pp;//祖父结点
  //2.2.1结点进行从下到上的调节
  while (1) {
    CHANGE_COLOR(s);
    p = s->p;
    if (p == nil) {   
      return node;
    }
    if (p->color == BLACK) {
      CHANGE_COLOR(s);
      return node;
    }
    pp = p->p;
    if (p->l == s) {
      LEFT = TRUE;
    }
    else {
      LEFT = FALSE;
    }

    //当前结点为父节点左孩子
    if (LEFT) {
      //父节点为祖父结点左孩子
      if (pp->l == p) {
        s = p;
      }
      //父节点为祖父节点右孩子
      else {
        LEFT_RORATE(p);
      }
      RIGHT_RORATE(pp);
    }
    //当前结点为父节点右孩子
    else {
      //父节点为祖父节点右孩子
      if (pp->r == p) {
        s = p;
      }
      //父节点为祖父节点左孩子
      else {
        RIGHT_RORATE(p);
      }
      LEFT_RORATE(pp);
    }
  }
}

template <typename ElementType>
void RBTree<ElementType>::remove(ElementType element) {
  len--;
  TreeNode<ElementType>* node = get(element);
  if (node == nil) {
    len++;
    return;
  }

  //1.有两个子节点
  if (node->l != nil && node->r != nil) {
    TreeNode<ElementType>* s = node->r;
    while (s->l != nil) {
      s = s->l;
    }
    node->value = s->value;
    node = s;
  }

  //2.有一个子节点
  if (node->l != nil) {
    node->value = node->l->value;
    node = node->l;
  }
  else if (node->r != nill) {
    node->value = node->r->value;
    node = node->r;
  }

  TreeNode<ElementType>* p = node->p;
  
  //3.为无孩子结点，直接删除并调整父节点
  if (node->l == nil && node->r == nil) {
    //3.1红色结点
    if (node->color == RED) {
      if (p->l == node) {
        p->l = nil;
      }
      else {
        p->r = nil;
      }
    }
    //3.2黑色结点
    else {
      //3.2.1不为头节点
      if (p != nil) {
        TreeNode<ElementType>* bro;
        //3.2.1.1父节点为红
        if (p->color == RED) {
          //3.2.1.1.1为父节点的左孩子
          if (p->l==node) {
            bro = p->r;
            //3.2.1.1.1.1兄弟节点左孩子为空
            if (bro->l == nil) {
              LEFT_RORATE(p);
            }
            //3.2.1.1.1.2兄弟节点左孩子不为空，定为红色
            else {
              RIGHT_RORATE(bro);
              LEFT_RORATE(p);
              CHANGE_COLOR(p);
            }
          }
          //3.2.1.1.2为父节点的右孩子
          else {
            bro = p->l;
            //3.2.1.1.2.1兄弟节点右孩子为空
            if (bro->r == nil) {
              RIGHT_RORATE(p);
            }
            //3.2.1.1.2.2兄弟节点右孩子不为空，定为红
            else {
              LEFT_RORATE(bro);
              RIGHT_RORATE(p);
              CHANGE_COLOR(p);
            }
          }
        }
        //3.2.1.2父节点为黑
        else {
          //3.2.1.2.1为父节点左孩子
          if(p->l==node){
            bro = p->r;
            //3.2.1.2.1.1兄弟结点为红色
            if (bro->color == RED) {
              LEFT_RORATE(p);
              CHANGE_COLOR(p);
              CHANGE_COLOR(bro);
            }
            //3.2.1.2.1.2兄弟结点为黑色,若兄弟结点有孩子，定为红色
            else {
              //3.2.1.2.1.2.1兄弟结点左孩子不为空
              if (bro->l != nil) {
                CHANGE_COLOR(bro->l);
                RIGHT_RORATE(bro);
                LEFT_RORATE(p);
              }
              //3.2.1.2.1.2.2兄弟结点右孩子不为空
              else if (bro->r != nil) {
                CHANGE_COLOR(bro->r);
                LEFT_RORATE(bro);
                LEFT_RORATE(p);
              }
              //3.2.1.2.1.2.3兄弟结点孩子均为空
              else {
                CHANGE_COLOR(bro);
              }
            }
          }
          //3.2.1.2.2为父节点右孩子
          else {
            bro = p->l;
            //3.2.1.2.2.1兄弟结点为红色
            if (bro->color == RED) {
              RIGHT_RORATE(p);
              CHANGE_COLOR(p);
              CHANGE_COLOR(bro);
            }
            //3.2.1.2.2.2兄弟结点为黑色，若孩子不为空，必为红色
            else {
              //3.2.1.2.2.2.1兄弟结点左孩子不为空
              if (bro->r != nil) {
                CHANGE_COLOR(bro->l);
                LEFT_RORATE(bro);
                RIGHT_RORATE(p);
              }
              //3.2.1.2.2.2.2兄弟结点右孩子不为空
              else if (bro->l != nil) {
                CHANGE_COLOR(bro->r);
                RIGHT_RORATE(bro);
                RIGHT_RORATE(p);
              }
              ////3.2.1.2.2.2.3兄弟结点孩子均为空
              else {
                CHANGE_COLOR(bro);
              }
            }
          }
        }
      }
    }
  }
  delete node;
  node = NULL;
}

template <typename ElementType>
void RBTree<ElementType>::freeTree(TreeNode<ElementType>* node) {
  TreeNode<ElementType>* left;
  TreeNode<ElementType>* right;
  if (node != nil) {
    left = node->l;
    right = node->r;
    delete node;
    node = NULL;
    freeTree(left);
    freeTree(right);
  }
}

template <typename ElementType>
void RBTree<ElementType>::destory() {
  freeTree(head);
}
#endif // !_RBTREE_H

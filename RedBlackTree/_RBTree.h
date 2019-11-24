//红黑树
#ifndef RBTREE_H
#define RBTREE_H
#ifndef NULL
#define NULL ((void*)0)
#endif // !1



enum BOOL_VAL { FALSE, TRUE };
enum COLOR { RED, BLACK };
enum CMP { BIGGER, EQUAL, SMALLER };
typedef short BOOL;

template <typename T>
struct TreeNode{
  T value;
  int color;
  TreeNode<T>* l=NULL;
  TreeNode<T>* r=NULL;
  TreeNode<T>* p=NULL;
};

template <typename ElementType>
class RBTree {
private:
  void LEFT_RORATE(const TreeNode<ElementType>* node);//左旋
  void RIGHT_RORATE(const TreeNode<ElementType>* node);//右旋
  void CHANGE_COLOR(TreeNode<ElementType>* node);//变色
  void freeTree(TreeNode<ElementType>* node);
  int (*cmp)(const ElementType& e1,const ElementType& e2);//比较大小
  int len;
  TreeNode<ElementType>* head;
  TreeNode<ElementType>* nil;
public:
  //若e1大于e2，返回BIGGER；若e1等于e2，返回EQUAL；若e1小于e2，返回SMALLER
  RBTree(int(*cmp)(const ElementType& e1, const ElementType& e2));
  ~RBTree();
  TreeNode<ElementType>* getHead();
  void destory();
  TreeNode<ElementType>* get(const ElementType element);
  TreeNode<ElementType>* put(ElementType element);
  void remove(ElementType element);
  int length();
};

template <typename ElementType>
RBTree<ElementType>::RBTree(int(*_cmp)(const ElementType& e1, const ElementType& e2)) {
  cmp = _cmp;
  len = 0;
  nil = NULL;
  head = NULL;
}

template <typename ElementType>
RBTree<ElementType>::~RBTree() {
  freeTree(head);
}

template <typename ElementType>
TreeNode<ElementType>* RBTree<ElementType>::getHead() {
  return head;
}

template <typename ElementType>
void RBTree<ElementType>::LEFT_RORATE(const TreeNode<ElementType>* node) {
  TreeNode<ElementType>* n = (TreeNode<ElementType>*)node;
  if (n->r == nil) {
    return;
  }
  if (n==head) {
    head = n->r;
    n->r = head->l;
    if (n->r != nil) {
      n->r->p = n;
    }
    head->l = n;
    n->p = head;
    head->p = NULL;
    return;
  }
  TreeNode<ElementType>* temp = n->p;
  if (temp->l == n) {
    temp->l = n->r;
  }
  else {
    temp->r = n->r;
  }
  temp = n->r;
  n->r = temp->l;
  if (n->r != nil) {
    n->r->p = n;
  }
  temp->l = n;
  temp->p = n->p;
  n->p = temp;
}

template <typename ElementType>
void RBTree<ElementType>::RIGHT_RORATE(const TreeNode<ElementType>* node) {
  TreeNode<ElementType>* n = (TreeNode<ElementType>*)node;
  if (n->l == nil) {
    return;
  }
  if (n == head) {
    head = n->l;
    n->l = head->r;
    if (n->l != nil) {
      n->l->p = n;
    }
    head->r = n;
    n->p = head;
    head->p = NULL;
    return;
  }
  TreeNode<ElementType>* temp = n->p;
  if (temp->l == n) {
    temp->l = n->l;
  }
  else {
    temp->r = n->l;
  }
  temp = n->l;
  n->l = temp->r;
  if (n->l != nil) {
    n->l->p = n;
  }
  temp->r = n;
  temp->p = n->p;
  n->p = temp;
}

template <typename ElementType>
void RBTree<ElementType>::CHANGE_COLOR(TreeNode<ElementType>* node) {
  node->color = node->color ? RED : BLACK;
}

template <typename ElementType>
TreeNode<ElementType>* RBTree<ElementType>::get(ElementType element) {
  TreeNode<ElementType>* h = head;
  while (1) {
    if (h == nil) {
      return nil;
    }
    if (cmp(element, h->value) == EQUAL) {
      return h;
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
TreeNode<ElementType>* RBTree<ElementType>::put(ElementType element) {
  len++;
  TreeNode<ElementType>* node = new TreeNode<ElementType>;
  node->value = element;
  node->color = RED;
  TreeNode<ElementType>* h = head;
  
  //1.头节点为空
  if (h == nil) {
    head = node;
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
    p = s->p;
    if (p == nil) { 
      CHANGE_COLOR(s);
      return node;
    }
    if (p->color == BLACK) {
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
        CHANGE_COLOR(s);
        RIGHT_RORATE(pp);   
        s = p;
      }
      //父节点为祖父节点右孩子
      else {
        RIGHT_RORATE(p);
        CHANGE_COLOR(p);
        LEFT_RORATE(pp);
      }
      
    }
    //当前结点为父节点右孩子
    else {
      //父节点为祖父节点右孩子
      if (pp->r == p) {
        CHANGE_COLOR(s);
        LEFT_RORATE(pp);
        s = p;
      }
      //父节点为祖父节点左孩子
      else {
        LEFT_RORATE(p);
        CHANGE_COLOR(p);
        RIGHT_RORATE(pp);
      }
      
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
  else if (node->r != nil) {
    node->value = node->r->value;
    node = node->r;
  }

  TreeNode<ElementType>* p;
  TreeNode<ElementType>* r = node;
  TreeNode<ElementType>* bro;
  BOOL LEFT;
  //3.为无孩子结点，直接删除并调整父节点
  while (1) {
    LEFT = TRUE;
    p = r->p;
    //当前结点头节点
    if (p == nil) {
      break;
    }
    if (p->r == r) {
      LEFT = FALSE;
    }
    //红色结点，跳出
    if (r->color == RED) {
      break;
    }

    //结点为黑色
    if (LEFT) {
      bro = p->r;
    }
    else {
      bro = p->l;
    }
    //兄弟结点为红，父节点为黑
    if (bro->color == RED) {
      CHANGE_COLOR(p);
      CHANGE_COLOR(bro);
      if (LEFT) {
        LEFT_RORATE(p);
      }
      else {
        RIGHT_RORATE(p);
      }
    }
    //兄弟结点为黑
    else {
      if (LEFT) {
        if (bro->r!=nil&&bro->r->color == RED) {
          bro->color = p->color;
          if (bro->r != nil) {
            bro->r->color = BLACK;
          }       
          p->color = BLACK;
          LEFT_RORATE(p);
          break;
        }
        else if(bro->l!=nil&&bro->l->color==RED){
          CHANGE_COLOR(bro);
          CHANGE_COLOR(bro->l);
          RIGHT_RORATE(bro);
        }
        else {
          bro->color = RED;
          if (p->color == RED) {
            CHANGE_COLOR(p);
            break;
          }
          r = p;
        }
      }
      else {
        if (bro->l != nil && bro->l->color == RED) {
          bro->color = p->color;
          if (bro->l != nil) {
            bro->l->color = BLACK;
          }
          p->color = BLACK;
          RIGHT_RORATE(p);
          break;
        }
        else if(bro->r != nil && bro->r->color == RED){
          CHANGE_COLOR(bro);
          CHANGE_COLOR(bro->r);
          LEFT_RORATE(bro);
        }
        else {
          bro->color = RED;
          if (p->color == RED) {
            CHANGE_COLOR(p);
            break;
          }
          r = p;
        }
      }
    }
  }
  if (node->p != nil) {
    if (node->p->l == node) {
      node->p->l = nil;
    }
    else {
      node->p->r = nil;
    }
  }
  else {
    head = nil;
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

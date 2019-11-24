//�����
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
  void LEFT_RORATE(const TreeNode<ElementType>* node);//����
  void RIGHT_RORATE(const TreeNode<ElementType>* node);//����
  void CHANGE_COLOR(const TreeNode<ElementType>* node);//��ɫ
  void freeTree(TreeNode<ElementType>* node);
  int (*cmp)(const ElementType& e1,const ElementType& e2);//�Ƚϴ�С
  int len;
  TreeNode<ElementType>* head;
  TreeNode<ElementType>* nil;
public:
  //��e1����e2������BIGGER����e1����e2������EQUAL����e1С��e2������SMALLER
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
  
  //1.ͷ�ڵ�λ��
  if (h = nil) {
    h = node;
    CHANGE_COLOR(node);
    return node;
  }

  //2.ͷ�ڵ㲻Ϊ��
  BOOL LEFT = TRUE;//��ע����λ��Ϊ���ڵ�����Һ���
  while (h != nil) {
    node->p = h;
    //2.1����Ѵ��ڣ����½��
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
  //2.2��㲻���ڣ����в���
  TreeNode<ElementType>* p = node->p;//��ע����λ�õĸ��ڵ�
  if (LEFT) {
    p->l = node;
  }
  else {
    p->r = node;
  }
  TreeNode<ElementType>* s = node;//��ǰ���
  TreeNode<ElementType>* pp;//�游���
  //2.2.1�����д��µ��ϵĵ���
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

    //��ǰ���Ϊ���ڵ�����
    if (LEFT) {
      //���ڵ�Ϊ�游�������
      if (pp->l == p) {
        s = p;
      }
      //���ڵ�Ϊ�游�ڵ��Һ���
      else {
        LEFT_RORATE(p);
      }
      RIGHT_RORATE(pp);
    }
    //��ǰ���Ϊ���ڵ��Һ���
    else {
      //���ڵ�Ϊ�游�ڵ��Һ���
      if (pp->r == p) {
        s = p;
      }
      //���ڵ�Ϊ�游�ڵ�����
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

  //1.�������ӽڵ�
  if (node->l != nil && node->r != nil) {
    TreeNode<ElementType>* s = node->r;
    while (s->l != nil) {
      s = s->l;
    }
    node->value = s->value;
    node = s;
  }

  //2.��һ���ӽڵ�
  if (node->l != nil) {
    node->value = node->l->value;
    node = node->l;
  }
  else if (node->r != nill) {
    node->value = node->r->value;
    node = node->r;
  }

  TreeNode<ElementType>* p = node->p;
  
  //3.Ϊ�޺��ӽ�㣬ֱ��ɾ�����������ڵ�
  if (node->l == nil && node->r == nil) {
    //3.1��ɫ���
    if (node->color == RED) {
      if (p->l == node) {
        p->l = nil;
      }
      else {
        p->r = nil;
      }
    }
    //3.2��ɫ���
    else {
      //3.2.1��Ϊͷ�ڵ�
      if (p != nil) {
        TreeNode<ElementType>* bro;
        //3.2.1.1���ڵ�Ϊ��
        if (p->color == RED) {
          //3.2.1.1.1Ϊ���ڵ������
          if (p->l==node) {
            bro = p->r;
            //3.2.1.1.1.1�ֵܽڵ�����Ϊ��
            if (bro->l == nil) {
              LEFT_RORATE(p);
            }
            //3.2.1.1.1.2�ֵܽڵ����Ӳ�Ϊ�գ���Ϊ��ɫ
            else {
              RIGHT_RORATE(bro);
              LEFT_RORATE(p);
              CHANGE_COLOR(p);
            }
          }
          //3.2.1.1.2Ϊ���ڵ���Һ���
          else {
            bro = p->l;
            //3.2.1.1.2.1�ֵܽڵ��Һ���Ϊ��
            if (bro->r == nil) {
              RIGHT_RORATE(p);
            }
            //3.2.1.1.2.2�ֵܽڵ��Һ��Ӳ�Ϊ�գ���Ϊ��
            else {
              LEFT_RORATE(bro);
              RIGHT_RORATE(p);
              CHANGE_COLOR(p);
            }
          }
        }
        //3.2.1.2���ڵ�Ϊ��
        else {
          //3.2.1.2.1Ϊ���ڵ�����
          if(p->l==node){
            bro = p->r;
            //3.2.1.2.1.1�ֵܽ��Ϊ��ɫ
            if (bro->color == RED) {
              LEFT_RORATE(p);
              CHANGE_COLOR(p);
              CHANGE_COLOR(bro);
            }
            //3.2.1.2.1.2�ֵܽ��Ϊ��ɫ,���ֵܽ���к��ӣ���Ϊ��ɫ
            else {
              //3.2.1.2.1.2.1�ֵܽ�����Ӳ�Ϊ��
              if (bro->l != nil) {
                CHANGE_COLOR(bro->l);
                RIGHT_RORATE(bro);
                LEFT_RORATE(p);
              }
              //3.2.1.2.1.2.2�ֵܽ���Һ��Ӳ�Ϊ��
              else if (bro->r != nil) {
                CHANGE_COLOR(bro->r);
                LEFT_RORATE(bro);
                LEFT_RORATE(p);
              }
              //3.2.1.2.1.2.3�ֵܽ�㺢�Ӿ�Ϊ��
              else {
                CHANGE_COLOR(bro);
              }
            }
          }
          //3.2.1.2.2Ϊ���ڵ��Һ���
          else {
            bro = p->l;
            //3.2.1.2.2.1�ֵܽ��Ϊ��ɫ
            if (bro->color == RED) {
              RIGHT_RORATE(p);
              CHANGE_COLOR(p);
              CHANGE_COLOR(bro);
            }
            //3.2.1.2.2.2�ֵܽ��Ϊ��ɫ�������Ӳ�Ϊ�գ���Ϊ��ɫ
            else {
              //3.2.1.2.2.2.1�ֵܽ�����Ӳ�Ϊ��
              if (bro->r != nil) {
                CHANGE_COLOR(bro->l);
                LEFT_RORATE(bro);
                RIGHT_RORATE(p);
              }
              //3.2.1.2.2.2.2�ֵܽ���Һ��Ӳ�Ϊ��
              else if (bro->l != nil) {
                CHANGE_COLOR(bro->r);
                RIGHT_RORATE(bro);
                RIGHT_RORATE(p);
              }
              ////3.2.1.2.2.2.3�ֵܽ�㺢�Ӿ�Ϊ��
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

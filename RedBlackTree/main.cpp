#include <iostream>
#include "_RBTree.h"
using namespace std;

int cmpp(const double& a, const double& b) {
  if (a < b) {
    return SMALLER;
  }
  else if (a > b) {
    return BIGGER;
  }
  else {
    return EQUAL;
  }
}

void preOrder(TreeNode<double>* h) {
  if (h == NULL) {
    return;
  }
  if (h->p == NULL) {
    cout << "头节点 ";
  }
  else {
    cout << "父节点"<< h->p->value<<" ";
    if (h->p->l == h) {
      cout << "左";
    }
    else if (h->p->r == h) {
      cout << "右";
    }
    else {
      cout << "error";
    }
  }

  if (h->color) {
    cout << "黑 ";
  }
  else {
    cout << "红 ";
  }
  cout <<h->value<< endl;
  preOrder(h->l);  
  preOrder(h->r);
}
int main() {
  RBTree<double> tree(&cmpp);
  double map[]{
  1,2,3,4,5,6,7,8,9,10,
  11,12,12,13,14,15,45,
  56,45,34,23,34,46,67,
  78,11,12,21,32,43
  };
  /*
  11,12,12,13,14,15,45,
  56,45,34,23,34,46,67,
  78,90,11,12,21,32,43
  */
  int len = sizeof(map) / sizeof(double);
  for (int i = 0; i < len; i++) {
    tree.put(map[i]);
  }
  auto h = tree.getHead();
  preOrder(h);
  cout << tree.length()<<"+++++++++++++"<<endl;
  for (int i = 0; i < len; i++) {
    tree.remove(map[i]);
  }
  h = tree.getHead();
  preOrder(h);
  cout << tree.length() << "+++++++++++++" << endl;
  return 0;
}
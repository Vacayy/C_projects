#include "rbtree.h"

#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rb_insert_fixup(rbtree *t, node_t *z);

// rbtree 구조체 초기화 함수
rbtree *new_rbtree(void) { // 반환 타입 = rbtree *
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree 구조체 선언

  // sentinel(경계) 노드 메모리할당 및 초기화
  p->nil = (node_t *)calloc(1, sizeof(node_t)); 
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = p->nil;
  p->nil->left = p->nil;
  p->nil->right = p->nil;

  // 초기상태에서, 루트 노드는 sentinel 노드로 설정
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

////////////////////// insert 연산 구현부 ////////////////////////
void left_rotate(rbtree *t, node_t *x){ // x = 부모 노드, y = 할아버지 노드 
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  };
  y->parent = x-> parent;
  if (x->parent == t->nil){
    t->root = y;    
  } else if (x == x->parent->left){
    x->parent->left = y;
  } else {
    x->parent->right = y;    
  };
  y->left = x;
  x->parent = y;
};

void right_rotate(rbtree *t, node_t *x){ 
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  };
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;    
  } else if (x == x->parent->right){
    x->parent->right = y;
  } else {
    x->parent->left = y;    
  };
  y->right = x;
  x->parent = y;
};

void rb_insert_fixup(rbtree *t, node_t *z){
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else if (z == z->parent->right){
        z = z->parent;
        left_rotate(t, z);
      };
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t, z);
    } else {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else if (z == z->parent->left){
        z = z->parent;
        right_rotate(t, z);
      };
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      left_rotate(t, z);
    }
  }

};

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // 노드 생성
  z->key = key;
  
  node_t *y = t->nil;
  node_t *current = t->root;

  while(current != t->nil){
    y = current;
    if (key < current->key){
      current = current->left;
    } else {
      current = current->right;
    };
  };

  z->parent = y;

  if (y == t->nil){
    t->root = z;
  } else if (z->key < y->key){
    y->left = z;
  } else {
    y->right = z;
  };

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t, z);
  
  return t->root;
}
////////////////////////////////////////////////////////////////

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

#include "rbtree.h"

#include <stdlib.h>

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
int right_rotate(rbtree *t, node_t *x){ // x = 부모 노드, y = 할아버지 노드 
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){

  };

};

int rb_insert_fixup(void){

};

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  
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

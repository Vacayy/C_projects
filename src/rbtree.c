#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rb_insert_fixup(rbtree *t, node_t *z);
void rb_transplant(rbtree *t, node_t *old, node_t *new);
node_t *find_successor(const rbtree *t, node_t *root);
void rb_delete_fixup(rbtree *t, node_t *have_extra_black);
void inorder_traversal(rbtree *t, node_t *x, key_t *arr, int *i);

// rbtree 구조체 초기화 함수
rbtree *new_rbtree(void) { // 반환 타입 = rbtree *
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree 구조체 선언

  // sentinel(경계) 노드 메모리 할당 및 초기화
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


////////////////////// 회전 구현부 ////////////////////////
void left_rotate(rbtree *t, node_t *x){
  // x = 부모 노드, y = 할아버지 노드 
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

////////////////////// insert 연산 구현부 ////////////////////////
void rb_insert_fixup(rbtree *t, node_t *z){
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;

      } else {
          if (z == z->parent->right){
            z = z->parent;
            left_rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          right_rotate(t, z->parent->parent);
      };

    } else {
      node_t *y = z->parent->parent->left;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;

      } else {
          if (z == z->parent->left){
            z = z->parent;
            right_rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
      };
    }
  }
  t->root->color = RBTREE_BLACK;

};

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 삽입 노드 생성
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); 
  z->key = key;
  
  node_t *y = t->nil; 
  node_t *current = t->root;

  // 삽입 연산
  while(current != t->nil){
    y = current; // 삽입 노드의 부모 노드를 지목해주기 위해 기억
    if (z->key < current->key){
      current = current->left;
    } else {
      current = current->right;
    };
  };

  z->parent = y; // 부모 노드 지정

  if (y == t->nil){
    t->root = z; // 부모 노드가 nil이라면 해당 노드가 루트 노드
  } else if (z->key < y->key){
    y->left = z; // 왼쪽 자식
  } else {
    y->right = z; // 오른쪽 자식
  };

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // 재조정 연산
  rb_insert_fixup(t, z);   
  return t->root;
}
////////////////////// 탐색 연산 구현부 //////////////////////////
node_t *rbtree_find(const rbtree *t, const key_t key) {  
  // 트리가 비었는지 확인
  if (t->root == t->nil){
    return NULL;
  }

  // 루트 노드부터 탐색 시작
  node_t *current = t->root;
  
  // 탐색
  while (current != t->nil ){
    if(key == current->key){
      return current;      
    }else if (key < current->key){
      current = current->left; 
    }else{
      current = current->right;
    };
  };  

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // 왼쪽 자식이 nil 노드일때까지 왼쪽으로 파고들기
  node_t *current = t->root;

  while(current->left != t->nil){
    current = current->left;
  };

  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // 오른쪽 자식이 nil 노드일때까지 오른쪽으로 파고들기
  node_t *current = t->root;

  while(current-> right != t->nil){
    current = current->right;
  };

  return current;
}
/////////////////////// 삭제 연산 구현부 ///////////////////////////////

// old 노드에 대한 부모 노드의 포인터를 new 노드로 이식하는 함수
void rb_transplant(rbtree *t, node_t *old, node_t *new){
  if (old->parent == t->nil){ // old 노드가 루트 노드인 경우
    t->root = new;
  }else if (old == old->parent->left){ // old 노드가 어떤 노드의 왼쪽 자식인 경우
    old->parent->left = new; // new 노드가 새로운 왼쪽 자식
  }else{ // 오른쪽 자식인 경우
    old->parent->right = new;
  }

  new->parent = old->parent; // new 노드 부모 연결     
};

node_t *find_successor(const rbtree *t, node_t *root){
  // 오른쪽 subtree에서 최솟값 찾기
  node_t *current = root->right;

  while(current->left != t->nil){
    current = current->left;
  }

  return current;
}

void rb_delete_fixup(rbtree *t, node_t *have_extra_black){
  // 1) have_extra_black이 red인 경우, 2) 속성 #2를 위반한 경우는 바로 해당 노드를 black으로 지정
  while(have_extra_black != t->root && have_extra_black->color == RBTREE_BLACK){
    if (have_extra_black == have_extra_black->parent->left){ // 왼쪽 자식인 경우
      node_t *brother = have_extra_black->parent->right; // 형제 노드 지정
      
      // case 1. 형제 노드가 red인 경우
      if (brother->color == RBTREE_RED){
        // 1. 부모와 형제를 color flip
        brother->color = RBTREE_BLACK;
        brother->parent->color = RBTREE_RED;

        // 2. 부모 기준으로 좌회전
        left_rotate(t, have_extra_black->parent);
        brother = have_extra_black->parent->right; // 구조상 형제 바뀜        
      } 
  // 이 시점에서, case 1 상태였다면 case 2, 3, 4 중 하나와 동일한 상태가 되어있다.
      
      // case 2. 형제 = black & 형제의 양쪽 자식 = black인 경우
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        // 형제의 black과 현재 노드의 extra black을 모아서 부모에게 올리는 조치이다.
        // 그렇게 해도 black height가 유지되기 때문에 가능한 기법
        brother->color = RBTREE_RED; // 형제의 자식 노드는 모두 black이므로 형제는 문제가 없는 상태가 된다.
        
        have_extra_black = have_extra_black->parent; // 부모가 extra black을 물려받음
      }  
      else {
        // case 3. 형제 = black & 형제의 오른쪽 자식 = black, 왼쪽 자식 = red인 경우
        if (brother->right->color == RBTREE_BLACK){
          // 형제와 왼쪽 자식을 color flip
          brother->color = RBTREE_RED;
          brother->left->color = RBTREE_BLACK;

          // 형제 기준으로 우회전
          right_rotate(t, brother);
          brother = have_extra_black->parent->right;
        }
    // 이 시점에서, case 2, 3 상태였다면 case 4와 동일한 상태가 되어있다.
    
        // case 4. 형제 = black & 형제의 오른쪽 자식 = red인 경우
        brother->color = have_extra_black->parent->color; // 형제는 부모의 색을 물려받는다
        have_extra_black->parent->color = RBTREE_BLACK; // 형제의 부모와 오른쪽 자식은 black
        brother->right->color = RBTREE_BLACK; 
        
        left_rotate(t, brother->parent);
        have_extra_black = t->root;
      }

    } else { // 오른쪽 자식인 경우 -> 왼쪽 경우와 대칭
        node_t *brother = have_extra_black->parent->left; // 형제 노드 지정
        
        // case 1. 형제 노드가 red인 경우
        if (brother->color == RBTREE_RED){
          // 1. 부모와 형제를 color flip
          brother->color = RBTREE_BLACK;
          brother->parent->color = RBTREE_RED;

          // 2. 부모 기준으로 좌회전
          right_rotate(t, have_extra_black->parent);
          brother = have_extra_black->parent->left; // 구조상 형제 바뀜        
        } 
  // 이 시점에서, case 1 상태였다면 case 2, 3, 4 중 하나와 동일한 상태가 되어있다.
      
      // case 2. 형제 = black & 형제의 양쪽 자식 = black인 경우
        if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK){
          // 형제의 black과 현재 노드의 extra black을 모아서 부모에게 올리는 조치이다.
          // 그렇게 해도 black height가 유지되기 때문에 가능한 기법
          brother->color = RBTREE_RED; // 형제의 자식 노드는 모두 black이므로 형제는 문제가 없는 상태가 된다.
          
          have_extra_black = have_extra_black->parent; // 부모가 extra black을 물려받음
        }  
        else {
          // case 3. 형제 = black & 형제의 오른쪽 자식 = black, 왼쪽 자식 = red인 경우
          if (brother->left->color == RBTREE_BLACK){
            // 형제와 왼쪽 자식을 color flip
            brother->color = RBTREE_RED;
            brother->right->color = RBTREE_BLACK;

            // 형제 기준으로 우회전
            left_rotate(t, brother);
            brother = have_extra_black->parent->left;
          }

          brother->color = have_extra_black->parent->color; 
          have_extra_black->parent->color = RBTREE_BLACK; 
          brother->left->color = RBTREE_BLACK; 
          
          right_rotate(t, brother->parent);
          have_extra_black = t->root;
        }
    }
  }
  have_extra_black->color = RBTREE_BLACK;  
};


int rbtree_erase(rbtree *t, node_t *target) {

  node_t *y = target; // 삭제되는 색을 기록하기 위함
  node_t *x; 
  color_t original_color = y->color; // 삭제되는 색 == 색을 잃는 노드의 색

  if (target->left == t->nil){ // 삭제 노드가 오른쪽 자식만 지닐 경우 or 자식이 없는 경우
    x = target->right; // 오른쪽 노드가 대체 노드
    rb_transplant(t, target, target->right); // 오른쪽 노드로 부모 관계 이식
    
  } else if (target->right == t->nil){ // 삭제 노드가 왼쪽 자식만 지닐 경우
    x = target->left;
    rb_transplant(t, target, target->left);

  } else { // 삭제 노드가 2개의 자식 노드를 지닐 경우
    // 후계자가 대체함
    y = find_successor(t, target);

    original_color = y->color; // 후계자의 원래 색상 저장
    x = y->right; // 후계자 오른쪽 자식이 기존 후계자 자리를 대체
    
    if (y->parent == target){ // 삭제 노드가 바닥 +1 레벨에 있었다면
      x->parent = y; // 대체한 노드의 부모는 여전히 후계자
    } else { // 후계자가 한 칸 이상 밑에서 올라온 경우
      rb_transplant(t, y, y->right); // 후계자 자리에 오른쪽 자식 올려주기
      // 삭제 노드의 서브트리를 후계자에게 넘겨주기
      y->right = target->right; 
      y->right->parent = y; 
    }

    rb_transplant(t, target, y); // target의 부모 관계를 후계자에게 물려주기
    // 후계자 -> 삭제노드로부터 왼쪽 서브트리 마저 물려받기
    y->left = target->left; 
    y->left->parent = y; 
    y->color = target->color;
  }

  if (original_color == RBTREE_BLACK){
    rb_delete_fixup(t, x);
  };
  
  free(target); // 해당 노드 메모리 해제
  return 0;
};



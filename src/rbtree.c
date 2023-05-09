#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
// 추가된 함수 전방선언
/* void free_node(rbtree *t, node_t *x); */
void rbtree_insert_fixup(rbtree *t, node_t *z);
void rbtree_delete_fixup(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void left_rotate(rbtree *t, node_t *x);
void transplant(rbtree *t, node_t *x, node_t *y);
/*a
RBTree 특성 (5개)
1. 모든노드 RED / BLACK
2. **루트노드는 BLACK**
3. 모든 NIL 노드는 BLACK
4. 노드가 RED 면, 자녀는 BLACK (**RED는 연속하면 안됨**)
5. 임의의 경로(자신을 제외)부터 **자손 NIL 까지 BLACK 개수는 동일**
*/
// solve (sentinel used)
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // 센티널 노드 초기화 후 색깔만 검은색으로 지정후 초기화
  node_t *nilnode = (node_t *)calloc(1,sizeof(node_t));
  nilnode->color = RBTREE_BLACK;

  p->nil = nilnode;
  // 초기 루트를 센티널로 초기화
  p->root = nilnode;

  return p;
}
// solve
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  node_t *cur = t->root;

  
  node_t *temp;
    // 루트 노드부터 가장 작은 노드까지 순회하며 노드를 해제한다.
    while (cur != t->nil) {
        // 가장 낮은 값을 가진 노드로 이동
        if (cur->left != t->nil) {
            cur = cur->left;
        }
        // 가장 낮은 값이 오른쪽 서브트리 있다면
        else if (cur->right != t->nil) {
            temp = cur->right;
            // 오른쪽 서브트리의 가장 작은 값을 찾는다
            while (temp->left != t->nil) {
                temp = temp->left;
            }
            cur = temp;
        }
        // 둘다 자식이 둘다 센티넬을 만난다면 메모리 해제
        else {
            temp = cur;
            cur = cur->parent;
            if (cur != t->nil && temp == cur->left) {
                cur->left = t->nil;
            } else if (cur != t->nil && temp == cur->right) {
                cur->right = t->nil;
            }
            free(temp);
        }
    }

  // nil 노드와 트리를 가리키는 포인터를 해제한다.
  free(t->nil);
  free(t);
}

// solve 
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  // 새로운 노드 할당
  // 삽입 할때 처음엔 색은 무조건 레드로 지정
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  // 루트 부터 newnode가 들어갈 위치를 찾는 과정
  node_t *y = t->nil;
  node_t *x = t->root;

  while(x != t->nil){
    // 지금 curosr 의 부모 노드
    y = x; 
    if(new_node->key < x->key){
      x = x->left;
    }
    else{
      //(key >= cursor->key) 
      // 키가 중복되더라도 mutilset 이므로 오른쪽에 넣어준다.
      x = x->right;
    }
  }
  
  new_node->parent = y;

  // 루트 노드가 비어있다면
  if(y == t->nil){
    t->root = new_node;
  }
  else if(new_node->key < y->key){
    y->left = new_node;
  }
  else{
    y->right = new_node;
  }
  
  rbtree_insert_fixup(t, new_node);

  return new_node;
}
// solve
void rbtree_insert_fixup(rbtree *t, node_t *z) 
{
  node_t *y;
  
  while (z->parent->color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      y = z->parent->parent->left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// solve
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *cur = t->root;

  // 루트에서 시작하여 트리를 순회하며 key 값을 가진 노드를 찾는다.
  // key 값과 같은 노드를 찾으면 while문을 나온다
  while (cur != t->nil && cur->key != key){
    if(key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  // 검색 결과에 따라 해당 노드의 포인터를 반환한다.
  if (cur != t->nil) {
    return cur;
  } else {
    return NULL;
  }
  //return t->root;
}
// solve
node_t *rbtree_min(const rbtree *t)
{
   if(t->nil == t->root){
    return NULL;
  }
  // TODO: implement min
  node_t *cur = t->root;

  // 가장 작은 key 값을 가진 노드는 트리의 왼쪽 끝에 위치한다.
  while (cur->left != t->nil) {
    cur = cur->left;
  }

  return cur;
  //return t->root;
}
// solve
node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement max
  if(t->nil == t->root){
    return NULL;
  }

  node_t *cur = t->root;

  // 가장 큰 key 값을 가진 노드는 트리의 오른쪽 끝에 위치한다.
  while (cur->right != t->nil) {
    cur = cur->right;
  }

  return cur;
  //return t->root;
}

// solve
int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase

  node_t *y = p;
  node_t *x = t->nil;
  // y 의 원래 가지고 있던 색깔 저장
  color_t y_origin_color = y->color;
  // 삭제 하는 노드의 왼쪽 자식이 없거나 둘다 없다면
  if(p->left == t->nil){
    x = p->right;
    transplant(t,p,p->right);
  }// 삭제 하는 노드의 오른쪽 자식이 없다면
  else if(p->right == t->nil){
    x = p->left;
    transplant(t,p,p->left);
  }// 자식이 둘다 있다면 
  else{
    y = p->right;
    while(y->left != t->nil){
      y = y->left;
    }
    y_origin_color = y->color;
    x = y->right;
    if(y->parent == p){
      x->parent = y;
    }
    else{
      transplant(t,y,y->right);
      y->right=p->right;
      y->right->parent = y;
    }
    transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if(y_origin_color == RBTREE_BLACK){
    // 삭제 한후 밸런싱 로정
    rbtree_delete_fixup(t,x);
  }

  free(p);

  return 0;
}
// solve
void rbtree_delete_fixup(rbtree *t, node_t *x) 
{
  while (x != t->root && x->color == RBTREE_BLACK) {
    // CASE 1 ~ 4 : LEFT CASE
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // CASE 1 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else{ 
        if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
        }

        // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    // CASE 5 ~ 8 : RIGHT CASE
    else {
      node_t *w = x->parent->left;

      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else 
      {
          if (w->left->color == RBTREE_BLACK) {
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            left_rotate(t, w);
            w = x->parent->left;
            }

        // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

// solve
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // 트리를 순서대로 순회해서 배열로 만들어야한다
  // TODO: implement to_array

  //alt+ shift + a 
  node_t *cur = rbtree_min(t);
  int i = 0;
  while (cur != t->nil && i < n) {
    arr[i++] = cur->key;

    // 현재 노드의 오른쪽 서브트리에서 가장 작은 값을 찾음
    if (cur->right != t->nil) {
      cur = cur->right;
      while (cur->left != t->nil) {
        cur = cur->left;
      }
    }
    // 현재 노드가 오른쪽 자식 노드가 아니면, 현재 노드의 부모 노드 중에서 가장 작은 값을 찾음
    else {
      while (cur->parent != t->nil && cur == cur->parent->right) {
        cur = cur->parent;
      }
      cur = cur->parent;
    }
  }
 

  return 0;
}

// add func 왼쪽 회전 하는 함수
void left_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } 
  else if (x == x->parent->left) {
    x->parent->left = y;
  } 
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
// add func 오른쪽 회전 하는 함수
void right_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->left;
  x->left = y->right;
  
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } 
  else if (x == x->parent->right) {
    x->parent->right = y;
  } 
  else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}
// add func 삭제시 자식을 자신이 있는자리로 바꾸는 함수 
// x = 삭제되는 노드 y = 자식노드
void transplant(rbtree *t, node_t *x, node_t *y)
{
  if(x->parent == t->nil){
    t->root = y;
  }else if(x == x->parent->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }
  y->parent = x->parent;
} 

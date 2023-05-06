#include "rbtree.h"
#include <stdlib.h>


static void right_rotate(rbtree *t, node_t *x);
static void left_rotate(rbtree *t, node_t *x);
/*a
RBTree 특성 (5개)
1. 모든노드 RED / BLACK
2. **루트노드는 BLACK**
3. 모든 NIL 노드는 BLACK
4. 노드가 RED 면, 자녀는 BLACK (**RED는 연속하면 안됨**)
5. 임의의 경로(자신을 제외)부터 **자손 NIL 까지 BLACK 개수는 동일**
*/
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // 센티널 노드 초기화 후 색깔만 검은색으로 지정후 초기화
  p->nil = (node_t *)calloc(1,sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = p->nil->left = p->nil->right = p->nil;
  // 초기 루트를 센티널로 초기화
  p->root = p->nil;
  /*  
  // 아무것도 없으니 널값으로 만듬
  p->root = NULL;
  // 보통 반복문이나 재귀를 종료하는 조건으로 존재하는 값
  p->nil = NULL;
  */
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  node_t *nil = t->nil;
  node_t *cur = t->root;

  // 루트 노드부터 가장 작은 노드까지 순회하며 노드를 해제한다.
  while (cur != nil) {
    // 현재 노드의 왼쪽 서브트리를 순회하여 가장 작은 노드를 찾는다.
    while (cur->left != nil) {
      cur = cur->left;
    }

    // 현재 노드의 오른쪽 자식 노드가 없으면서, 현재 노드의 부모 노드가 nil이 아닐 때까지
    // 현재 노드와 그 부모 노드를 번갈아가며 탐색하며 노드를 해제한다.
    while (cur->right == nil) {
      node_t *x = cur;
      cur = cur->parent;
      free(x);
    }

    // x 노드의 오른쪽 서브트리가 존재하면서, 그 서브트리에서 가장 작은 노드를 찾아
    // x 노드를 대체한 후, 그 노드와 그 부모 노드를 번갈아가며 탐색하며 노드를 해제한다.
    node_t *x = cur;
    cur = cur->right;
    free(x);
  }

  // nil 노드와 트리를 가리키는 포인터를 해제한다.
  free(nil);
  free(t);
}
/*
// 삽입전 RB트리 속성을 만족 상태

// 삽입 방식은 일반적인 BST와 동일(삽입노드의 색깔은 항상RED)
// 삽입 후 RB트리 속성 위반 여부 확인 // 따로 함수 만들면 안되나/
// RB트리 속성을 위반했다면 재조성(insert-fixup)

// RB트리 속성을 다시 만족
*/

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  // 새로운 노드 할당
  // 삽입 할때 처음엔 색은 무조건 레드로 지정
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = NULL;
  
  new_node->left = t->nil;
  new_node->right = t->nil;

  node_t *current = t->root;
  /*
  // 탐색을 진행할 포인터
  node_t *ptr;
  // 루트 노드가 비어있을 경우
  if (t->root == NULL)
  {
    t->root = new_node;
  }
  else
  { // 루트 노드가 존재할때
    ptr = t->root;
    while (ptr)
    {
      // 현재 탐색 포인터의 위치 값과 같은 값일때
      if(key == ptr->key)
      {

      }
      // 현재 탐색 포인터의 위치 값보다 작을 때
      if(key > ptr->key)
      {
        
      }
      // 현재 탐색 포인터의 위치 값보다 클때
      if(key < ptr->key)
      {

      }
    }
  }
  */
  while(current != t->nil){
    if(key < current->key)
    {
      current = current->left;
    }
    else if(key > current->key)
    {
      current = current->right;
    }
    else // 똑같은 값일때
    {
      return current;
    }
  }
  new_node->parent = current;
  if(current == t->nil)
  {
    t->root = new_node;
  }
  else if(key < current->key)
  {
    current->left = new_node;
  }
  else
  {
    current->right = new_node;
  }

  /*
  // insert-fixup 구조
  //   while (부모가 RED) {

  //     // CASE 1.
  //     if 부모/삼촌 == RED이면,
  //         부모/삼촌 모두 BLACK으로 바꾸고, 할아버지를 RED로 변경
  //         할아버지에서 다시 시작

  //     // CASE 2/3. 삼촌 == BLACK
  //     else {
  //         // CASE 2.
  //         if 할아버지/부모/자신 꺾인상테
  //             회전해서 부모/자신을 펴준다. CASE 3 상태가 됨

  //         // CASE 3. 할아버지/부모/자신 펴진상테
  //         부모/할아버지 서로 색바꾸기
  //         할아버지 기준 회전 // 회전 어떻게함?
  //     }
  // }
  // // 종료전
  // 루트를 BLACK으로 변경
  */

  // RB 트리 속성을 유지하기 위해 트리 수정
  while (new_node->parent->color == RBTREE_RED) {
    if (new_node->parent == new_node->parent->parent->left) {
      node_t *uncle = new_node->parent->parent->right;
      if (uncle->color == RBTREE_RED) {  // case 1
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      } else {
        if (new_node == new_node->parent->right) {  // case 2
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
        new_node->parent->color = RBTREE_BLACK;  // case 3
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    } else {  // symmetric cases
      node_t *uncle = new_node->parent->parent->left;
      if (uncle->color == RBTREE_RED) {  // case 1
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      } else {
        if (new_node == new_node->parent->left) {  // case 2
          new_node = new_node->parent;
          right_rotate(t, new_node);
        }
        new_node->parent->color = RBTREE_BLACK;  // case 3
        new_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, new_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;  // 루트가 검은색인지 확인
  return new_node;
  //return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *cur = t->root;
  node_t *nil = t->nil;

  // 루트에서 시작하여 트리를 순회하며 key 값을 가진 노드를 찾는다.
  while (cur != nil && cur->key != key) {
    if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  // 검색 결과에 따라 해당 노드의 포인터를 반환한다.
  if (cur != nil) {
    return cur;
  } else {
    return NULL;
  }
  //return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement min
  node_t *cur = t->root;
  node_t *nil = t->nil;

  // 가장 작은 key 값을 가진 노드는 트리의 왼쪽 끝에 위치한다.
  while (cur->left != nil) {
    cur = cur->left;
  }

  return cur;
  //return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement max
  node_t *cur = t->root;
  node_t *nil = t->nil;

  // 가장 큰 key 값을 가진 노드는 트리의 오른쪽 끝에 위치한다.
  while (cur->right != nil) {
    cur = cur->right;
  }

  return cur;
  //return t->root;
}
/*
삭제전 RB 트리 속성 만족

삭제 방식은 일반적인 BST와 동일
삭제후 RB트리 속성 위반 여부 확인
RB트리 속성을 위반했다면 재조정(delete-fixup)

RB트리 속성을 다시 만족
*/

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  // p가 NULL이면 에러
  if (p == t->nil) {
    return 0;
  }

  node_t *x, *y;
  // 삭제할 노드 p의 자식 노드를 y로 설정
  if (p->left == t->nil || p->right == t->nil) {
    y = p;
  } else {
    y = rbtree_min(t);
  }

  // y의 자식 노드를 x로 설정
  if (y->left != t->nil) {
    x = y->left;
  } else {
    x = y->right;
  }

  // x의 부모 노드를 y의 부모 노드로 설정
  x->parent = y->parent;

  // y가 루트 노드이면 x를 새로운 루트 노드로 설정
  if (y->parent == t->nil) {
    t->root = x;
  } else if (y == y->parent->left) {
    // y가 부모 노드의 왼쪽 자식 노드이면 x를 왼쪽 자식 노드로 설정
    y->parent->left = x;
  } else {
    // y가 부모 노드의 오른쪽 자식 노드이면 x를 오른쪽 자식 노드로 설정
    y->parent->right = x;
  }

  // y가 삭제할 노드 p가 아니면 y의 키 값을 p의 키 값으로 복사
  if (y != p) {
    p->key = y->key;
  }
  
  // y의 색이 BLACK이면 RB-Tree의 성질을 위배할 수 있습니다.
  if (y->color == RBTREE_BLACK) 
  {
    while (x != t->root && x->color == RBTREE_BLACK) {
      // x가 부모 노드의 왼쪽 자식 노드인 경우
      if (x == x->parent->left) {
        node_t *w = x->parent->right;
        // CASE 1: 형제 노드 w가 RED인 경우
        if (w->color == RBTREE_RED) {
          w->color = RBTREE_BLACK;
          x->parent->color = RBTREE_RED;
          left_rotate(t, x->parent);
          w = x->parent->right;
        }
        // CASE 2: w의 자식 노드들이 모두 BLACK인 경우
        if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
          w->color = RBTREE_RED;
          x = x->parent;
        }
        else {
          // CASE 3: w의 오른쪽 자식 노드가 BLACK인 경우
          if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
          }
          // CASE 4: w의 오른쪽 자식 노드가 RED인 경우
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          left_rotate(t, x->parent);
          x = t->root;
        }
      }
      else 
    {
      node_t *w = x->parent->left;
      // CASE 1: 형제 노드 w가 RED인 경우
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // CASE 2: w의 자식 노드들이 모두 BLACK인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        // CASE 3: w의 왼쪽 자식 노드가 BLACK인 경우
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // CASE 4: w의 왼쪽 자식 노드가 RED인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
        }
      }
    }
  }
  /*
    //delete-fixup 구조
    while (타겟이 루트아님 && 타겟 == BLACK) {

      // CASE 1.
      if 형제 == RED
          형제/부모 서로 색바꾸기, 부모기준 회전 (타겟이 내려가도록) // 이 회전을 어떻게함?

      // CASE 2.
      if 형제 == BLACK, 형제의 자식 둘다 블랙
          형제/자신의 블랙을 부모로 올리기 -> 형제를 RED로 변경, 부모에서 다시 fixup

      else {
          // CASE 3.
          if 형제 == BLACK, 부모/형제/(형제의 꺾인 자식) == RED
              자식 RED와 형제 서로 색바꾸기, 펴지게 회전 (RED가 바깥쪽에 오게)
              CASE 4가 됨

          // CASE 4. 형제 == BLACK, 부모/형제/(형제의 펴진 자식) == RED
          부모/형제 서로 색바꾸기
          형제의(펴진 자식) = BLACK
          부모기준 회전 (타겟이 내려가도록)
          타겟을 루트로 설정 -> while 종료
      }
  }

  // 종료전
  타겟을 BLACK으로 변경
  */
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array

  return 0;
}


static void left_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void right_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}
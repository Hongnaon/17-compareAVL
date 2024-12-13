#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 노드 구조체 정의. AVL 트리의 경우 높이(height) 정보도 포함
typedef struct Node {
    int key; // 노드의 키 값
    struct Node* left, * right; // 왼쪽, 오른쪽 자식 노드
    int height; // AVL 트리를 위한 높이 정보
} Node;

// 비교 횟수와 탐색 횟수를 기록하는 전역 변수
int compareCount = 0;
int searchCount = 0;

// 유틸리티 함수: 두 값 중 더 큰 값을 반환
int max1(int a, int b) {
    return (a > b) ? a : b;
}

// 노드의 높이를 반환. NULL 노드는 높이가 0으로 간주
int height(Node* n) {
    return (n == NULL) ? 0 : n->height;
}

// 새로운 노드를 생성
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1; // 새 노드의 초기 높이는 1
    return node;
}

// AVL 트리: 오른쪽 회전 함수
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    // 높이 갱신
    y->height = max1(height(y->left), height(y->right)) + 1;
    x->height = max1(height(x->left), height(x->right)) + 1;

    return x; // 새로운 루트를 반환
}

// AVL 트리: 왼쪽 회전 함수
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    // 높이 갱신
    x->height = max1(height(x->left), height(x->right)) + 1;
    y->height = max1(height(y->left), height(y->right)) + 1;

    return y; // 새로운 루트를 반환
}

// AVL 트리: 균형 인수 계산
int getBalance(Node* n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}

// AVL 트리: 노드 삽입 함수
Node* insertAVL(Node* node, int key) {
    if (node == NULL) // 삽입 위치에 도달하면 새로운 노드 생성
        return createNode(key);

    compareCount++; // 비교 횟수 증가
    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node; // 중복 키는 삽입하지 않음

    // 높이 갱신
    node->height = 1 + max1(height(node->left), height(node->right));

    // 균형 확인 및 회전 수행
    int balance = getBalance(node);

    // LL 회전
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR 회전
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR 회전
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL 회전
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // 변경된 루트를 반환
}

// 주어진 서브트리에서 최소값을 가진 노드를 반환
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// AVL 트리: 노드 삭제 함수
Node* deleteAVL(Node* root, int key) {
    if (root == NULL)
        return root;

    compareCount++; // 비교 횟수 증가
    if (key < root->key)
        root->left = deleteAVL(root->left, key);
    else if (key > root->key)
        root->right = deleteAVL(root->right, key);
    else {
        // 삭제 노드가 하나 또는 없는 자식을 가진 경우
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) { // 자식이 없는 경우
                temp = root;
                root = NULL;
            }
            else // 자식이 하나인 경우
                *root = *temp;

            free(temp);
        }
        else {
            // 두 자식을 가진 경우
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteAVL(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    // 높이 갱신
    root->height = 1 + max1(height(root->left), height(root->right));

    // 균형 확인 및 회전 수행
    int balance = getBalance(root);

    // LL 회전
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR 회전
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR 회전
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL 회전
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// 노드를 탐색하는 함수 (BST와 AVL 공통)
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    compareCount++; // 비교 횟수 증가
    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}

// 이진 탐색 트리: 노드 삽입
Node* insertBST(Node* root, int key) {
    if (root == NULL)
        return createNode(key);

    compareCount++; // 비교 횟수 증가
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);

    return root;
}

// 이진 탐색 트리: 노드 삭제
Node* deleteBST(Node* root, int key) {
    if (root == NULL)
        return root;

    compareCount++; // 비교 횟수 증가
    if (key < root->key)
        root->left = deleteBST(root->left, key);
    else if (key > root->key)
        root->right = deleteBST(root->right, key);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteBST(root->right, temp->key);
    }

    return root;
}

// 2,000번의 연산을 실행하는 AVL 트리 테스트 배치
Node* doAVLBatch(Node* root) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0: 삽입, 1: 삭제, 2: 탐색
        int B = rand() % 1000; // 0~999의 임의 숫자 생성

        if (A == 0)
            root = insertAVL(root, B);
        else if (A == 1)
            root = deleteAVL(root, B);
        else {
            search(root, B); // 탐색 시 비교 횟수 기록
            searchCount++;
        }
    }
    return root;
}

// 2,000번의 연산을 실행하는 이진 탐색 트리 테스트 배치
Node* doBinaryBatch(Node* root) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0: 삽입, 1: 삭제, 2: 탐색
        int B = rand() % 1000; // 0~999의 임의 숫자 생성

        if (A == 0)
            root = insertBST(root, B);
        else if (A == 1)
            root = deleteBST(root, B);
        else {
            search(root, B); // 탐색 시 비교 횟수 기록
            searchCount++;
        }
    }
    return root;
}

// 트리의 메모리를 해제
void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// 메인 함수: AVL 및 BST 실험 수행
int main(int argc, char* argv[]) {
    Node* root = NULL;
    srand(time(NULL)); // 랜덤 시드 초기화
    float allCount = 0.0;

    // AVL 트리 실험 실행
    root = doAVLBatch(root);
    allCount = (float)compareCount / searchCount; // 평균 비교 횟수 계산
    printf("average AVL compare count: %.2f\n", allCount);

    freeTree(root); // 메모리 해제
    root = NULL;
    compareCount = searchCount = 0; // 카운트 초기화

    // 이진 탐색 트리 실험 실행
    root = doBinaryBatch(root);
    allCount = (float)compareCount / searchCount; // 평균 비교 횟수 계산
    printf("average Bin compare count: %.2f\n", allCount);

    freeTree(root); // 메모리 해제
    return 0;
}

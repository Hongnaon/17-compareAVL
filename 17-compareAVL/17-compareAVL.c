#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ��� ����ü ����. AVL Ʈ���� ��� ����(height) ������ ����
typedef struct Node {
    int key; // ����� Ű ��
    struct Node* left, * right; // ����, ������ �ڽ� ���
    int height; // AVL Ʈ���� ���� ���� ����
} Node;

// �� Ƚ���� Ž�� Ƚ���� ����ϴ� ���� ����
int compareCount = 0;
int searchCount = 0;

// ��ƿ��Ƽ �Լ�: �� �� �� �� ū ���� ��ȯ
int max1(int a, int b) {
    return (a > b) ? a : b;
}

// ����� ���̸� ��ȯ. NULL ���� ���̰� 0���� ����
int height(Node* n) {
    return (n == NULL) ? 0 : n->height;
}

// ���ο� ��带 ����
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1; // �� ����� �ʱ� ���̴� 1
    return node;
}

// AVL Ʈ��: ������ ȸ�� �Լ�
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    // ���� ����
    y->height = max1(height(y->left), height(y->right)) + 1;
    x->height = max1(height(x->left), height(x->right)) + 1;

    return x; // ���ο� ��Ʈ�� ��ȯ
}

// AVL Ʈ��: ���� ȸ�� �Լ�
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    // ���� ����
    x->height = max1(height(x->left), height(x->right)) + 1;
    y->height = max1(height(y->left), height(y->right)) + 1;

    return y; // ���ο� ��Ʈ�� ��ȯ
}

// AVL Ʈ��: ���� �μ� ���
int getBalance(Node* n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}

// AVL Ʈ��: ��� ���� �Լ�
Node* insertAVL(Node* node, int key) {
    if (node == NULL) // ���� ��ġ�� �����ϸ� ���ο� ��� ����
        return createNode(key);

    compareCount++; // �� Ƚ�� ����
    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node; // �ߺ� Ű�� �������� ����

    // ���� ����
    node->height = 1 + max1(height(node->left), height(node->right));

    // ���� Ȯ�� �� ȸ�� ����
    int balance = getBalance(node);

    // LL ȸ��
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR ȸ��
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR ȸ��
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL ȸ��
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // ����� ��Ʈ�� ��ȯ
}

// �־��� ����Ʈ������ �ּҰ��� ���� ��带 ��ȯ
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// AVL Ʈ��: ��� ���� �Լ�
Node* deleteAVL(Node* root, int key) {
    if (root == NULL)
        return root;

    compareCount++; // �� Ƚ�� ����
    if (key < root->key)
        root->left = deleteAVL(root->left, key);
    else if (key > root->key)
        root->right = deleteAVL(root->right, key);
    else {
        // ���� ��尡 �ϳ� �Ǵ� ���� �ڽ��� ���� ���
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) { // �ڽ��� ���� ���
                temp = root;
                root = NULL;
            }
            else // �ڽ��� �ϳ��� ���
                *root = *temp;

            free(temp);
        }
        else {
            // �� �ڽ��� ���� ���
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteAVL(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    // ���� ����
    root->height = 1 + max1(height(root->left), height(root->right));

    // ���� Ȯ�� �� ȸ�� ����
    int balance = getBalance(root);

    // LL ȸ��
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR ȸ��
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR ȸ��
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL ȸ��
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// ��带 Ž���ϴ� �Լ� (BST�� AVL ����)
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    compareCount++; // �� Ƚ�� ����
    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}

// ���� Ž�� Ʈ��: ��� ����
Node* insertBST(Node* root, int key) {
    if (root == NULL)
        return createNode(key);

    compareCount++; // �� Ƚ�� ����
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);

    return root;
}

// ���� Ž�� Ʈ��: ��� ����
Node* deleteBST(Node* root, int key) {
    if (root == NULL)
        return root;

    compareCount++; // �� Ƚ�� ����
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

// 2,000���� ������ �����ϴ� AVL Ʈ�� �׽�Ʈ ��ġ
Node* doAVLBatch(Node* root) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0: ����, 1: ����, 2: Ž��
        int B = rand() % 1000; // 0~999�� ���� ���� ����

        if (A == 0)
            root = insertAVL(root, B);
        else if (A == 1)
            root = deleteAVL(root, B);
        else {
            search(root, B); // Ž�� �� �� Ƚ�� ���
            searchCount++;
        }
    }
    return root;
}

// 2,000���� ������ �����ϴ� ���� Ž�� Ʈ�� �׽�Ʈ ��ġ
Node* doBinaryBatch(Node* root) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0: ����, 1: ����, 2: Ž��
        int B = rand() % 1000; // 0~999�� ���� ���� ����

        if (A == 0)
            root = insertBST(root, B);
        else if (A == 1)
            root = deleteBST(root, B);
        else {
            search(root, B); // Ž�� �� �� Ƚ�� ���
            searchCount++;
        }
    }
    return root;
}

// Ʈ���� �޸𸮸� ����
void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// ���� �Լ�: AVL �� BST ���� ����
int main(int argc, char* argv[]) {
    Node* root = NULL;
    srand(time(NULL)); // ���� �õ� �ʱ�ȭ
    float allCount = 0.0;

    // AVL Ʈ�� ���� ����
    root = doAVLBatch(root);
    allCount = (float)compareCount / searchCount; // ��� �� Ƚ�� ���
    printf("average AVL compare count: %.2f\n", allCount);

    freeTree(root); // �޸� ����
    root = NULL;
    compareCount = searchCount = 0; // ī��Ʈ �ʱ�ȭ

    // ���� Ž�� Ʈ�� ���� ����
    root = doBinaryBatch(root);
    allCount = (float)compareCount / searchCount; // ��� �� Ƚ�� ���
    printf("average Bin compare count: %.2f\n", allCount);

    freeTree(root); // �޸� ����
    return 0;
}

//Task 1: Range Query and Successor/Predecessor Finder using BST
#include <iostream>
#include <limits>
using namespace std;

//keeps asking until user enters a proper number
int readInt(const char* msg) {
    int val;
    cout << msg;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, enter a number: ";
    }
    return val;
}

//node of the bst, holds value and left/right pointers
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

//my own stack using linked lis
struct StackNode {
    Node* treeNode;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;

public:
    Stack() {
        top = nullptr;
    }

    ~Stack() {
        while (!isEmpty())
            pop();
    }

    //add on top
    void push(Node* n) {
        StackNode* temp = new StackNode;
        temp->treeNode = n;
        temp->next = top;
        top = temp;
    }

    //remove from top and return the tree node
    Node* pop() {
        if (isEmpty())
            return nullptr;
        StackNode* temp = top;
        Node* n = temp->treeNode;
        top = top->next;
        delete temp;
        return n;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

class BST {
private:
    Node* root;

    //recursive insert, smaller goes left and bigger goes right
    Node* insert(Node* node, int val, bool& added) {
        if (node == nullptr) {
            added = true;
            return new Node(val);
        }
        if (val < node->data)
            node->left = insert(node->left, val, added);
        else if (val > node->data)
            node->right = insert(node->right, val, added);
        //duplicates are ignored
        return node;
    }

    //left, root, right gives sorted order
    void inorder(Node* node) {
        if (node == nullptr)
            return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    //recursive range query, only go to sides that can have keys in range
    void rangeRecursive(Node* node, int low, int high, int& comparisons) {
        if (node == nullptr)
            return;

        //if node is bigger than low then left side can still have values
        comparisons++;
        if (low < node->data)
            rangeRecursive(node->left, low, high, comparisons);

        //print node if it lies inside the range
        comparisons++;
        if (node->data >= low && node->data <= high)
            cout << node->data << " ";

        //if node is smaller than high then right side can still have values
        comparisons++;
        if (node->data < high)
            rangeRecursive(node->right, low, high, comparisons);
    }

    //delete all nodes using postorder so no memory leak
    void destroy(Node* node) {
        if (node == nullptr)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() {
        root = nullptr;
    }

    ~BST() {
        destroy(root);
    }

    //returns false if value was already in tree
    bool insert(int val) {
        bool added = false;
        root = insert(root, val, added);
        return added;
    }

    bool isEmpty() {
        return root == nullptr;
    }

    void inorder() {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }
        inorder(root);
        cout << endl;
    }

    //finds predecessor and successor by walking down from root
    void findPredSucc(int key) {
        Node* pred = nullptr;
        Node* succ = nullptr;
        Node* curr = root;

        while (curr != nullptr) {
            if (key < curr->data) {
                //current node is bigger so it can be successor, move left
                succ = curr;
                curr = curr->left;
            }
            else if (key > curr->data) {
                //current node is smaller so it can be predecessor, move right
                pred = curr;
                curr = curr->right;
            }
            else {
                //key found, pred is max of left subtree
                if (curr->left != nullptr) {
                    Node* temp = curr->left;
                    while (temp->right != nullptr)
                        temp = temp->right;
                    pred = temp;
                }
                //succ is min of right subtree
                if (curr->right != nullptr) {
                    Node* temp = curr->right;
                    while (temp->left != nullptr)
                        temp = temp->left;
                    succ = temp;
                }
                break;
            }
        }

        if (curr == nullptr)
            cout << "Key " << key << " not found in BST (showing nearest values)" << endl;

        cout << "Inorder Predecessor of " << key << ": ";
        if (pred != nullptr) cout << pred->data << endl;
        else cout << "None" << endl;

        cout << "Inorder Successor of " << key << ": ";
        if (succ != nullptr) cout << succ->data << endl;
        else cout << "None" << endl;
    }

    void rangeQueryRecursive(int low, int high) {
        int comparisons = 0;
        cout << "Recursive Range [" << low << ", " << high << "]: ";
        rangeRecursive(root, low, high, comparisons);
        cout << endl;
        cout << "Comparisons (Recursive): " << comparisons << endl;
    }

    //iterative range query using a stack, works like inorder but skips useless parts
    void rangeQueryIterative(int low, int high) {
        int comparisons = 0;
        Stack st;
        Node* curr = root;

        cout << "Iterative Range [" << low << ", " << high << "]: ";
        while (curr != nullptr || !st.isEmpty()) {
            //go left as long as values are not below low
            while (curr != nullptr) {
                comparisons++;
                if (curr->data >= low) {
                    st.push(curr);
                    curr = curr->left;
                }
                else {
                    //too small so whole left side is useless, jump right
                    curr = curr->right;
                }
            }

            //nothing left in stack means no more values in range
            if (st.isEmpty())
                break;

            curr = st.pop();

            //nodes come out in sorted order so we can stop once we pass high
            comparisons++;
            if (curr->data > high)
                break;

            cout << curr->data << " ";
            curr = curr->right;
        }
        cout << endl;
        cout << "Comparisons (Iterative): " << comparisons << endl;
    }
};

int main() {
    BST tree;

    cout << "===== Task 1: Range Query and Successor/Predecessor =====" << endl << endl;

    //taking the sequence from user to build the bst
    int n = readInt("Enter number of elements: ");
    while (n <= 0)
        n = readInt("Number of elements must be positive, enter again: ");

    cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; i++) {
        int val;
        while (!(cin >> val)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input, enter remaining integers: ";
        }
        if (!tree.insert(val))
            cout << "(" << val << " is a duplicate, ignored)" << endl;
    }

    //menu so we can test many keys and ranges in one run
    int choice;
    do {
        cout << endl << "----- Menu -----" << endl;
        cout << "1. Display Inorder Traversal" << endl;
        cout << "2. Find Predecessor and Successor" << endl;
        cout << "3. Range Query (Recursive and Iterative)" << endl;
        cout << "4. Insert New Key" << endl;
        cout << "5. Exit" << endl;
        choice = readInt("Enter choice: ");
        cout << endl;

        if (choice == 1) {
            cout << "Inorder Traversal: ";
            tree.inorder();
        }
        else if (choice == 2) {
            int key = readInt("Enter key: ");
            tree.findPredSucc(key);
        }
        else if (choice == 3) {
            int low = readInt("Enter lower bound: ");
            int high = readInt("Enter upper bound: ");
            //if user enters them backwards just swap
            if (low > high) {
                int temp = low;
                low = high;
                high = temp;
                cout << "Bounds swapped to [" << low << ", " << high << "]" << endl;
            }
            tree.rangeQueryRecursive(low, high);
            tree.rangeQueryIterative(low, high);
        }
        else if (choice == 4) {
            int val = readInt("Enter key to insert: ");
            if (tree.insert(val))
                cout << val << " inserted" << endl;
            else
                cout << val << " already exists" << endl;
        }
        else if (choice != 5) {
            cout << "Invalid choice" << endl;
        }
    } while (choice != 5);

    cout << "Exiting..." << endl;
    return 0;
}
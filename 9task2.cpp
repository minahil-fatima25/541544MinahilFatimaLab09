//Task 2: Word Frequency Indexing using BST
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//each node stores a unique word and how many times it came
struct WordNode {
    string word;
    int frequency;
    WordNode* left;
    WordNode* right;
    WordNode(string w, int f) {
        word = w;
        frequency = f;
        left = nullptr;
        right = nullptr;
    }
};

//second tree that is ordered by frequency instead of word, used for sorting
class FrequencyBST {
private:
    WordNode* root;

    //bigger frequency goes right, equal or smaller goes left
    WordNode* insert(WordNode* node, const string& w, int f) {
        if (node == nullptr)
            return new WordNode(w, f);
        if (f > node->frequency)
            node->right = insert(node->right, w, f);
        else
            node->left = insert(node->left, w, f);
        return node;
    }

    //reverse inorder (right, root, left) gives high to low
    void reverseInorder(WordNode* node) {
        if (node == nullptr)
            return;
        reverseInorder(node->right);
        cout << node->word << " : " << node->frequency << endl;
        reverseInorder(node->left);
    }

    void destroy(WordNode* node) {
        if (node == nullptr)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    FrequencyBST() {
        root = nullptr;
    }

    ~FrequencyBST() {
        destroy(root);
    }

    void insert(const string& w, int f) {
        root = insert(root, w, f);
    }

    void displayHighToLow() {
        reverseInorder(root);
    }
};

class WordBST {
private:
    WordNode* root;
    int totalWords;

    //insert word alphabetically, if already there just increase count
    WordNode* insert(WordNode* node, const string& w) {
        if (node == nullptr)
            return new WordNode(w, 1);
        if (w < node->word)
            node->left = insert(node->left, w);
        else if (w > node->word)
            node->right = insert(node->right, w);
        else
            node->frequency++;
        return node;
    }

    //inorder prints words in alphabetical order
    void inorderPrint(WordNode* node) {
        if (node == nullptr)
            return;
        inorderPrint(node->left);
        cout << node->word << " : " << node->frequency << endl;
        inorderPrint(node->right);
    }

    //copies every word into frequency tree, inorder so same counts stay alphabetical
    void copyToFrequencyTree(WordNode* node, FrequencyBST& ft) {
        if (node == nullptr)
            return;
        copyToFrequencyTree(node->left, ft);
        ft.insert(node->word, node->frequency);
        copyToFrequencyTree(node->right, ft);
    }

    //finds the highest frequency in the whole tree
    int maxFrequency(WordNode* node) {
        if (node == nullptr)
            return 0;
        int best = node->frequency;
        int leftMax = maxFrequency(node->left);
        int rightMax = maxFrequency(node->right);
        if (leftMax > best) best = leftMax;
        if (rightMax > best) best = rightMax;
        return best;
    }

    //prints every word that has the given frequency
    void printWithFrequency(WordNode* node, int freq) {
        if (node == nullptr)
            return;
        printWithFrequency(node->left, freq);
        if (node->frequency == freq)
            cout << node->word << " (" << node->frequency << " times)" << endl;
        printWithFrequency(node->right, freq);
    }

    //counts nodes, every node is one unique word
    int countUnique(WordNode* node) {
        if (node == nullptr)
            return 0;
        return 1 + countUnique(node->left) + countUnique(node->right);
    }

    void destroy(WordNode* node) {
        if (node == nullptr)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    WordBST() {
        root = nullptr;
        totalWords = 0;
    }

    ~WordBST() {
        destroy(root);
    }

    void insertWord(const string& w) {
        if (w.empty())
            return;
        root = insert(root, w);
        totalWords++;
    }

    //breaks paragraph into words, removes punctuation and makes lowercase
    void parseParagraph(const string& text) {
        string current = "";
        for (size_t i = 0; i < text.length(); i++) {
            char ch = text[i];
            if (isalpha(ch)) {
                current += (char)tolower(ch);
            }
            else {
                //any non letter means word ended
                insertWord(current);
                current = "";
            }
        }
        //last word if paragraph doesnt end with punctuation
        insertWord(current);
    }

    void displayAlphabetical() {
        cout << "--- Words in Alphabetical Order ---" << endl;
        if (root == nullptr) {
            cout << "No words found" << endl;
            return;
        }
        inorderPrint(root);
    }

    void displayByFrequency() {
        if (root == nullptr) {
            cout << "--- Words by Frequency (High to Low) ---" << endl;
            cout << "No words found" << endl;
            return;
        }
        FrequencyBST ft;
        copyToFrequencyTree(root, ft);
        cout << "--- Words by Frequency (High to Low) ---" << endl;
        ft.displayHighToLow();
    }

    void findMostFrequentWord() {
        cout << "--- Most Frequent Word(s) ---" << endl;
        if (root == nullptr) {
            cout << "No words found" << endl;
            return;
        }
        int highest = maxFrequency(root);
        printWithFrequency(root, highest);
    }

    void displaySummary() {
        cout << "--- Summary ---" << endl;
        cout << "Total Unique Words: " << countUnique(root) << endl;
        cout << "Total Word Count: " << totalWords << endl;
    }
};

int main() {
    cout << "===== Task 2: Word Frequency Indexing using BST =====" << endl << endl;

    //taking the whole paragraph in one line using getline
    string paragraph;
    cout << "Enter a paragraph: ";
    getline(cin, paragraph);
    cout << endl;

    //build the word tree from paragraph
    WordBST index;
    index.parseParagraph(paragraph);

    index.displayAlphabetical();
    cout << endl;
    index.displayByFrequency();
    cout << endl;
    index.findMostFrequentWord();
    cout << endl;
    index.displaySummary();

    return 0;
}
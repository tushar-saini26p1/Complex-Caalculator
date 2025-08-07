/*

Simple calculator program using expression tree
    → Node class
    → ExpTree class
    → getPrecedence() to get precedence of operators
    → removeSpace() for removing white spaces
    → overloaded insertNode() to insert operand and operator
    → construct() to construct expression tree from string input
    → printLevelOrder() to display expression tree

pros:
    → for combination of integer operands and +, -, *, /, ^, () operations
    → nested paranthesis are allowed
    → can evaluate expression tree

cons:
    → mulitple simultaneous operators (urnary operators) not allowed
    → not for double or floating operands
    → not precise (due to / operation and integer result)

*/
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
using namespace std;

class Node{
public:
    int data; // to store operand
    char symbol; // to store operator
    int nodePriority;
    Node* left, *right;

    // constructor
    explicit Node(int d) 
        : data{d}, symbol{'\0'}, left{nullptr}, right{nullptr} {}

    explicit Node(char c)
        : symbol{c}, left{nullptr}, right{nullptr}, nodePriority{} {}
};

class ExpTree{
public:
    Node* root;

    // constructor
    ExpTree()
        : root{nullptr} {}

    Node* insertNode(char c, int priority);
    void insertNode(int d, Node* ptr);
    int getPrecedence(char c);
    void construct(string s);
    int eval(Node* focusNode);

    void printInorder(Node* focusNode);
    void printLevelOrder();
};

// return pointer to node after inserting operator
Node* ExpTree::insertNode(char c, int priority) {
    if (root -> symbol == '\0' || root -> nodePriority > priority) {
        Node* temp = new Node(c);
        temp -> left = root;
        temp -> nodePriority = priority;
        root = temp;
        return root;
    }

    Node* focusNode = root;

    while (priority > (focusNode -> right) -> nodePriority && (focusNode -> right) -> symbol != '\0')
        focusNode = focusNode -> right;

    Node* temp = new Node(c);
    temp -> left = focusNode -> right;
    temp -> nodePriority = priority;
    focusNode -> right = temp;
    return focusNode -> right;
}

// function to insert operand
void ExpTree::insertNode(int d, Node* ptr) {
    if (root == nullptr)
        root = new Node(d);
    else
        ptr -> right = new Node(d);
}

// function to return precedence for binary operators
int ExpTree::getPrecedence(char c) {
    unordered_map<char, int> pMap;

    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    return pMap[c];
}

void ExpTree::construct(string s) {

    int priority{0};
    Node* temp{nullptr};

    if (s[0] == '-')
        s.insert(s.begin(), {'0'}); // for expression beginning with - sign

    s.insert(s.begin(), '\0');

    for (int i{1}, j{1}; j < s.size(); j++) {

        // update priority on basis of paranthesis
        if (s[j] == '(') {
            priority += 100;
            s.erase(s.begin() + j);
            j--;
            continue;
        }
        
        if (s[j] == ')') {
            priority -= 100;
            s.erase(s.begin() + j);
            j--;
            continue;
        }

        if (isdigit(s[j]) == false) // j is an operator
            temp = insertNode(s[j], getPrecedence(s[j]) + priority);

        else {
            if (isdigit(s[j]) == true && isdigit(s[j - 1]) == false) // i is the start of integer
                i = j;
            if (isdigit(s[j]) == true && isdigit(s[j + 1]) == false) // j is end of integer
                insertNode(stoi(s.substr(i, j - i + 1)), temp);            
        }
    }
}

int ExpTree::eval(Node* focusNode) {
    if (root == nullptr)
        return 0;
    
    else if (focusNode -> symbol == '\0')
        return focusNode -> data;

    else {
        switch (focusNode -> symbol) {
            case '+':
                return eval(focusNode -> left) + eval(focusNode -> right);

            case '-':
                return eval(focusNode -> left) - eval(focusNode -> right);

            case '*':
                return eval(focusNode -> left) * eval(focusNode -> right);

            case '/':
                return eval(focusNode -> left) / eval(focusNode -> right);

            case '^':
                return pow(eval(focusNode -> left), eval(focusNode -> right));
        }
    }
    return 0;
}

//*************************************************** print functions ***********************************************************
void ExpTree::printInorder(Node* focusNode) {
    if (focusNode == nullptr)
        return;

    printInorder(focusNode -> left);
    if (focusNode -> symbol == '\0')
        cout << focusNode -> data << "   ";
    else 
        cout << focusNode -> symbol << "   ";
    printInorder(focusNode -> right);
}

void ExpTree::printLevelOrder() {
    if (root == nullptr)
        return;

    queue<Node*> q;
    q.push(root);
    int n{1};

    while(q.empty() == false) {
        Node* temp = q.front();

        if (temp -> symbol == '\0')
            cout << temp -> data << "   ";
        else 
            cout << temp -> symbol << "   ";
            
        q.pop();
        --n;

        if (temp -> left != nullptr)
            q.push(temp -> left);
        if (temp -> right != nullptr)
            q.push(temp -> right);

        if (n == 0) {
            n = q.size();
            cout << endl;
        }
    }
}

// function to remove white spaces
void removeSpace(string &str) {
    for (int i{}; i < str.size(); i++) {
        if (str[i] == ' ') {
            str.erase(str.begin() + i);
            i--;
        }
    }
}

int main() {
    string s{"-1 + (2 + 9 )* 4^3/2"};
    removeSpace(s);
    ExpTree exptree;
    exptree.construct(s);
    exptree.printInorder(exptree.root);
    cout << endl;
    exptree.printLevelOrder();
    cout << exptree.eval(exptree.root) << endl;
}

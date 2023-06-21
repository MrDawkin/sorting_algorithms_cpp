#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <iomanip>
#include <queue>

using namespace std;
using namespace std::chrono;

struct config{
    string sort;
    string output;
    string input;
    string time_file;
    int iters;
    int dataAmnt;
};

struct node
{
    int key;
    node *left, *right;
    node(int val){
        key = val;
        left = nullptr;
        right = nullptr;
    }
};

void dataIn(string input_file, vector<vector<int>> &data, int dataAmount){
    ifstream infile(input_file);
    int i = 0;

    while (infile && i < dataAmount)
    { 
        string s;
        if (!getline( infile, s )) break;

        istringstream ss( s );
        vector <int> record;
        int j = 0;
        while (ss && j < dataAmount)
        {
            string s;
            if (!getline( ss, s, ',' )) break;
            record.push_back(stoi(s));
            j++;
        }
        data.push_back(record);
        i++;
    }
}

void dataOut(string output_file, vector<vector<int>> &data, int data_amount){
    fstream output;
    output.open(output_file, ios::out);
    for(int i = 0; i < data_amount; i++){
        for(int j = 0; j < data_amount; j++){
            output << data[i][j] << ", ";
        }
        output << endl;
    }
    output.close();
}

void parseFile(ifstream &config_file, config *&data)
{
    string line;
    while(getline(config_file, line)){ //wczytywanie konfiguracji do struktury data
            istringstream config_line(line);
            string key;
            if(getline(config_line, key, '=')){
                string val;
                if(getline(config_line, val)){
                    if(key == "input_file") data->input += val;
                    if(key == "output_file") data->output += val;
                    if(key == "complete_time") data->time_file += val;
                    if(key == "sort_type") data->sort += val;
                    if(key == "iterations") data->iters = stoi(val);
                    if(key == "data_amount") data->dataAmnt = stoi(val);
                }
            }    
        }
}

void timeOutput(long int times[], string output_file, int iteramount){
    fstream output;
    output.open(output_file, ios::out);

    for(int i = 0; i< iteramount; i++){
        output << times[i] << endl;
    }
    output.close();
}

struct node* newNode(int item)
{
    struct node* temp = (struct node*)malloc(
                  sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}
  
struct node* insert(struct node* node, int key)
{
      
    // If the tree is empty, return a new node
    if (node == NULL)
        return newNode(key);
  
    // Otherwise, recur down the tree
    if (key < node->key)
    {
        node->left = insert(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = insert(node->right, key);
    }
  
    // Return the node pointer
    return node;
}

struct node* search(struct node* root, int key) {
    if (root == NULL || root->key == key)
       return root;
    
    if (root->key < key)
       return search(root->right, key);

    return search(root->left, key);
}

struct node* minValueNode(struct node* node)
{
    struct node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;
 
    return current;
}

// Function to delete a node with the given key from BST
struct node* remove(struct node* root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = remove(root->left, key);

    else if (key > root->key)
        root->right = remove(root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        struct node* temp = minValueNode(root->right);

        root->key = temp->key;

        root->right = remove(root->right, temp->key);
    }
    return root;
}

struct node* rotateLeft(struct node* root, struct node* node)
{
    struct node* right = node->right;
    node->right = right->left;
    right->left = node;
    if (root == node) root = right;
    return right;
}


struct node *rotateRight(struct node *node)
{
    struct node *temp;
    temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

int countNodes(struct node *root)
{
    int count = 0;
    if (root != NULL) {
        count = 1;
        count += countNodes(root->left);
        count += countNodes(root->right);
    }
    return count;
}

void createBackbone(struct node *&root)
{
    struct node *temp, *rightNode;

    temp = root;
    rightNode = root->right;

    while (rightNode != NULL) {
        if (temp == root) {
            root = temp->right;
        }
        temp->right = rightNode->right;
        rightNode->right = temp->left;
        temp->left = rightNode;

        temp = temp->left;
        rightNode = temp->right;
    }
}

int closestPowerOfTwo(int n)
{
    int m = 1;
    while (m <= n) {
        m = m << 1;
    }
    return m >> 1;
}

void createBalancedTree(struct node *&root, int n)
{
    int m = closestPowerOfTwo(n + 1) - 1;
    int i;
    struct node *temp;

    while (m > 0) {
        temp = root;
        for (i = 0; i < n - m; ++i) {
            temp = temp->right;
        }
        temp->left = rotateRight(temp->left);
        for (i = 0; i < m - 1; ++i) {
            temp = temp->right;
            temp->left = rotateRight(temp->left);
        }
        n = m - 1;
        m = closestPowerOfTwo(n + 1) - 1;
    }
}

void balanceBST(struct node *&root)
{
    int n = countNodes(root);
    createBackbone(root);
    createBalancedTree(root, n);
}

void display(node* root) {
    if (root == nullptr) {
        cout << "Puste drzewo" << endl;
        return;
    }
    queue<node*> q;
    q.push(root);
    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            node* temp = q.front();
            q.pop();
            cout << temp->key << " ";
            if (temp->left != nullptr)
                q.push(temp->left);
            if (temp->right != nullptr)
                q.push(temp->right);
        }
        cout << endl;
    }
}

void menu(struct node* root)
{
  int choice = 0; 
  int key = 0;

  cout << "\n\nBST \n1: Wyswietl drzewo \n2: Wyszukaj element \n3: Dodaj element \n4: Usun element \n5: Zakoncz\n";
  cin >> choice;
        switch (choice) {
            case 1:
            display(root);
            break;

            case 2:
            cout <<"Wpisz element do wyszukania: ";
            cin  >> key;
             if (search(root, key)) {
                    cout << key << " znalezione w drzewie\n";
                } else {
                    cout << "Nie znaleziono elementu w drzewie\n";
                }
            break;

            case 3:
            cout << "Podaj element do dodania: ";
            cin >> key;
            insert(root, key);
            cout << key << " zostalo dodane\n";
            break;

            case 4:
            cout << "Podaj element do usuniecie: ";
            cin >> key;
                if (remove(root, key)) {
                    cout << key << " zostalo usuniete\n";
                } else {
                    cout << "Nie znaleziono elementu w drzewie\n";
                }
                break;
            case 5:
            exit(0);

            default:
            return;
        }
}


int main(){

    ifstream file("config.cfg"); //otwarcie pliku sterujacego
    config * cfg = new config;
    parseFile(file, cfg);
    vector<vector<int>> data;
    data.reserve(1000*1000*sizeof(int));
    dataIn(cfg->input, data, cfg->dataAmnt);
    struct node* root = NULL;

    auto start = steady_clock::now(); // pomiar czasu rozpoczęcia
    for(int i = 0; i < cfg->dataAmnt; i++){
        for(int j = 0; j < cfg->dataAmnt; j++){
            root = insert(root, data[i][j]);
        }
    }
    auto end = steady_clock::now(); // pomiar czasu zakończenia 
    auto diff = duration_cast<nanoseconds>(end - start).count(); // obliczenie czasu w nanosekundach

    cout << "Czas budowania drzewa: " << diff << " ns";

    while(true)
    menu(root);

    dataOut(cfg->output, data, cfg->dataAmnt);

    delete cfg;
    return 0;
}
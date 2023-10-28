#include "receiver.h"
#include <vector>

int max(int a, int b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

class Map {
private:
    Map* iterator(int first)
    {
        Map* temp = root;

        while (temp != nullptr &&
            temp->first != first) {
            if (first < temp->first) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return temp;
    }
    const Map* iterator(int first) const
    {
        Map* temp = root;
        while (temp != nullptr
            && temp->first != first) {
            if (first < temp->first) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return temp;
    }
    const int search(int first) const
    {
        const Map* temp = iterator(first);
        if (temp != nullptr) {
            return temp->second;
        }
        return 0;
    }
    Map* create(int first)
    {
        Map* newnode = (Map*)malloc(sizeof(Map));
        newnode->first = first;
        newnode->second = 0;
        newnode->left = nullptr;
        newnode->right = nullptr;
        newnode->par = nullptr;
        newnode->depth = 1;
        return newnode;
    }

    void right_rotation(Map* x)
    {
        Map* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->par = x;
        }
        if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        else if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        y->par = x->par;
        y->right = x;
        x->par = y;
    }

    void left_rotation(Map* x)
    {
        Map* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->par = x;
        }
        if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        else if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        y->par = x->par;
        y->left = x;
        x->par = y;
    }

    void helper(Map* node)
    {
        // If left skewed
        if (depthf(node->left)
            - depthf(node->right) > 1) {

            if (depthf(node->left->left)
                > depthf(node->left->right)) {
                node->depth
                    = max(depthf(node->right) + 1,
                        depthf(node->left->right) + 1);
                node->left->depth
                    = max(depthf(node->left->left) + 1,
                        depthf(node) + 1);
                right_rotation(node);
            }
            else {
                node->left->depth = max(
                    depthf(node->left->left) + 1,
                    depthf(node->left->right->left)
                    + 1);
                node->depth
                    = max(depthf(node->right) + 1,
                    depthf(node->left->right->right) + 1);
                node->left->right->depth
                    = max(depthf(node) + 1,
                        depthf(node->left) + 1);
                left_rotation(node->left);
                right_rotation(node);
            }
        }
 
        // If right skewed
        else if (depthf(node->left)
                - depthf(node->right) < -1) {
            if (depthf(node->right->right)
                > depthf(node->right->left)) {
                node->depth
                    = max(depthf(node->left) + 1,
                        depthf(node->right->left) + 1);
                node->right->depth
                    = max(depthf(node->right->right) + 1,
                        depthf(node) + 1);
                left_rotation(node);
            }
            else {
                node->right->depth = max(
                    depthf(node->right->right) + 1,
                    depthf(node->right->left->right) + 1);
                node->depth = max(
                    depthf(node->left) + 1,
                    depthf(node->right->left->left) + 1);
                node->right->left->depth
                    = max(depthf(node) + 1,
                        depthf(node->right) + 1);
                right_rotation(node->right);
                left_rotation(node);
            }
        }
    }
 
    // Balancing the tree about the "node"
    void balance(Map* node)
    {
        while (node != root) {
            int d = node->depth;
            node = node->par;
            if (node->depth < d + 1) {
                node->depth = d + 1;
            }
            if (node == root
                && depthf(node->left) 
                - depthf(node->right) > 1) {
                if (depthf(node->left->left)
                    > depthf(node->left->right)) {
                    root = node->left;
                }
                else {
                    root = node->left->right;
                }
                helper(node);
                break;
            }
            else if (node == root
                    && depthf(node->left) 
                    - depthf(node->right)
                            < -1) {
                if (depthf(node->right->right)
                    > depthf(node->right->left)) {
                    root = node->right;
                }
                else {
                    root = node->right->left;
                }
                helper(node);
                break;
            }
            helper(node);
        }
    }
 
    // Utility method to return the
    // "depth" of the subtree at the "node"
    int depthf(Map* node)
    {
        if (node == nullptr)
 
            // If it is null node
            return 0;
        return node->depth;
    }
 
    // Function to insert a value in map
    Map* insert(int first)
    {
        cnt++;
        Map* newnode = create(first);
        if (root == nullptr) {
            root = newnode;
            return root;
        }
        Map *temp = root, *prev = nullptr;
        while (temp != nullptr) {
            prev = temp;
            if (first < temp->first) {
                temp = temp->left;
            }
            else if (first > temp->first) {
                temp = temp->right;
            }
            else {
                free(newnode);
                cnt--;
                return temp;
            }
        }
        if (first < prev->first) {
            prev->left = newnode;
        }
        else {
            prev->right = newnode;
        }
        newnode->par = prev;
        balance(newnode);
        return newnode;
    }
 
    // Returns the previous node in
    // inorder traversal of the AVL Tree.
    Map* inorderPredecessor(Map* head)
    {
        if (head == nullptr)
            return head;
        while (head->right != nullptr) {
            head = head->right;
        }
        return head;
    }
 
    // Returns the next node in
    // inorder traversal of the AVL Tree.
    Map* inorderSuccessor(Map* head)
    {
        if (head == nullptr)
            return head;
        while (head->left != nullptr) {
            head = head->left;
        }
        return head;
    }
 
public:
    // Root" is kept static because it's a class
    // property and not an instance property
    static class Map* root;
    static int cnt;
 
    // "first" is key and "second" is value
    Map *left, *right, *par;
    int first, second, depth;

    int& operator[](int key) {
        return insert(key)->second;
    }

    const int operator[](int key) const
    {
        return search(key);
    }
 
    // Count returns whether an element
    // exists in the Map or not
    int count(int first)
    {
        Map* temp = iterator(first);
        if (temp != nullptr) {
            return 1;
        }
        return 0;
    }
 
    // Returns number of elements in the map
    int size(void) {
        return cnt;
    }
 
    // Removes an element given its key
    void erase(int first, Map* temp = root)
    {
        Map* prev = nullptr;
        cnt--;
        while (temp != nullptr &&
            temp->first != first) {
            prev = temp;
            if (first < temp->first) {
                temp = temp->left;
            }
            else if (first > temp->first) {
                temp = temp->right;
            }
        }
        if (temp == nullptr) {
            cnt++;
            return;
        }
        if (cnt == 0 && temp == root) {
            free(temp);
            root = nullptr;
            return;
        }
        Map* l
            = inorderPredecessor(temp->left);
        Map* r
            = inorderSuccessor(temp->right);
        if (l == nullptr && r == nullptr) {
            if (prev == nullptr) {
                root = nullptr;
            }
            else {
                if (prev->left == temp) {
                    prev->left = nullptr;
                }
                else {
                    prev->right = nullptr;
                }
                free(temp);
                balance(prev);
            }
            return;
        }
        Map* start;
        if (l != nullptr) {
            if (l == temp->left) {
                l->right = temp->right;
                if (l->right != nullptr) {
                    l->right->par = l;
                }
                start = l;
            }
            else {
                if (l->left != nullptr) {
                    l->left->par = l->par;
                }
                start = l->par;
                l->par->right = l->left;
                l->right = temp->right;
                l->par = nullptr;
                if (l->right != nullptr) {
                    l->right->par = l;
                }
                l->left = temp->left;
                temp->left->par = l;
            }
            if (prev == nullptr) {
                root = l;
            }
            else {
                if (prev->left == temp) {
                    prev->left = l;
                    l->par = prev;
                }
                else {
                    prev->right = l;
                    l->par = prev;
                }
                free(temp);
            }
            balance(start);
            return;
        }
        else {
            if (r == temp->right) {
                r->left = temp->left;
                if (r->left != nullptr) {
                    r->left->par = r;
                }
                start = r;
            }
            else {
                if (r->right != nullptr) {
                    r->right->par = r->par;
                }
                start = r->par;
                r->par->left = r->right;
                r->left = temp->left;
                r->par = nullptr;
                if (r->left != nullptr) {
                    r->left->par = r;
                }
                r->right = temp->right;
                temp->right->par = r;
            }
            if (prev == nullptr) {
                root = r;
            }
            else {
                if (prev->right == temp) {
                    prev->right = r;
                    r->par = prev;
                }
                else {
                    prev->left = r;
                    r->par = prev;
                }
                free(temp);
            }
            balance(start);
            return;
        }
    }
     
    // Returns if the map is empty or not
    bool empty(void)
    {
        if (root == nullptr)
            return true;
        return false;
    }
     
    // Given the key of an element it updates
    // the value of the key
    void update(int first, int second)
    {
        Map* temp = iterator(first);
        if (temp != nullptr) {
            temp->second = second;
        }
    }
 
    // Deleting the root of
    // the tree each time until the map
    // is not empty
    void clear(void)
    {
        while (root != nullptr) {
            erase(root->first);
        }
    }
 
    // Inorder traversal of the AVL tree
    void iterate(Map* head = root)
    {
        if (root == nullptr)
            return;
        if (head->left != nullptr) {
            iterate(head->left);
        }
        std::cout << head->first << ' ';
        if (head->right != nullptr) {
            iterate(head->right);
        }
    }
 
    // Returns a pointer/iterator to the element
    // whose key is first
    Map* find(int first) {
        return iterator(first);
    }
 
    // Overloaded insert method,
    // takes two parameters - key and value
    void insert(int first, int second)
    {
        Map* temp = iterator(first);
        if (temp == nullptr) {
            insert(first)->second = second;
        }
        else {
            temp->second = second;
        }
    }
};

Map* Map::root = nullptr;
int Map::cnt = 0;

int hash(std::string message){
    int answer = 0;
    int p = 31;
    for(auto s : message){
        answer += int(s)*p;
        p *= p;    
    }
    return answer;
}

void part1(){
    Receiver rcv;
    std::string message = rcv.readIML();
    message.resize(message.size()-1);
    // std::cout << message;
    rcv.terminate();
    std::istringstream iss(message);
    std::string token;
    std::vector<std::vector<std::string>> tokens;

    // Tokenize the input string using hashtags as delimiters
    while (std::getline(iss, token, '#')) {
        std::istringstream space_iss(token);
        std::string space_token;
        std::vector<std::string> temp_vector;

        // Tokenize using space as the second delimiter
        while (space_iss >> space_token) {
            temp_vector.push_back(space_token);
        }

        // Add the tokens to the 2D vector
        tokens.push_back(temp_vector);
    }

    // Output the tokens
    // for (const auto& t : tokens) {
    //     for (const auto& inner_token : t) {
    //         std::cout << inner_token << std::endl;
    //     }
    // }

    Map estimator;
    Map order;
    for(const auto& t : tokens){
        estimator[hash(t[0])] = -1;
    }

    for(const auto& t : tokens){
        if(estimator[hash(t[0])] == -1){
            if(t[2] == "s"){
                std::cout << t[0] << " " << t[1] << " b" << std::endl; 
            }
            else{
                std::cout << t[0] << " " << t[1] << " s" << std::endl;
            }
            int price = stoi(t[1]);
            estimator[hash(t[0])] = price;
        }
        else{
            if(t[2] == "s"){
                int sell_price = stoi(t[1]);
                if(sell_price < estimator[hash(t[0])]){
                    estimator[hash(t[0])] = sell_price;
                    std::cout << t[0] << " " << t[1] << " b" << std::endl;
                }
                else{
                    std::cout << "No Trade" << std::endl;
                }
            }
            else{
                int buy_price = stoi(t[1]);
                if(buy_price > estimator[hash(t[0])]){
                    estimator[hash(t[0])] = buy_price;
                    std::cout << t[0] << " " << t[1] << " s" << std::endl;
                }
                else{
                    std::cout << "No Trade" << std::endl;
                }
            }
        }
    }
}

int main() {
    part1();
    return 0;
}
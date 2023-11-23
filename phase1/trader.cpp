#include "receiver.h"
#include <vector>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class binaryHeap{

    public:

        // dynamic array so we'll never
        // run out of elements :)
        vector<int> _arr;

        int parent(int i) { return i/2; }

        void heapify_up(int i) {

            // iterate until reached index 0
            while(i > 0) {
                if(_arr[parent(i)] > _arr[i]) {

                    // if parent key is higher than current key
                    // then fix it by swapping both of it
                    int tmp = _arr[parent(i)];
                    _arr[parent(i)] = _arr[i];
                    _arr[i] = tmp;
                }
                i = parent(i);
            }
        }

        void heapify_down(int i) {

            // fix bin heap structure after deletion
            while(i < _arr.size()-1) {

                int i_min = _arr[i*2] > _arr[i*2+1] ? i*2+1 : i*2;

                // continuously fix the heap tree by
                // swapping if node's childrens is lower than current node
                if(_arr[i] > _arr[i_min]) {
                    int tmp = _arr[i];
                    _arr[i] = _arr[i_min];
                    _arr[i_min] = tmp;
                }

                i = i_min;
            }

        }

    public:

        binaryHeap() {
            _arr.reserve(20);
            _arr.push_back(0);
        }


        void insert(int key) {
            _arr.push_back(key);
            heapify_up(_arr.size()-1);
        }

        int extractMin() {

            // move last key in array to root
            int tmp = _arr[1]; // store value before remove
            _arr[1] = _arr[_arr.size()-1];

            // after moved last to front, then remove that value
            _arr.pop_back();

            // fix tree heap property back
            heapify_down(1);

            return tmp;
        }

        void debug_print() {
            for(int i = 0; i <= _arr.size()-1; i++) {
                cout << _arr[i] << ' ';
            }
            cout << endl;
        }

};



class runningMedian{
    private:
    binaryHeap negLesser;
    binaryHeap greater;

    public:
    double getMedian()
    {

        while(greater._arr.size() + 1 < negLesser._arr.size())
        {
            greater.insert(-1*negLesser.extractMin());
        }

        while(negLesser._arr.size() + 1 < greater._arr.size())
        {
            negLesser.insert(-1*greater.extractMin());
        }

        if(negLesser._arr.size()==greater._arr.size())
        {
            int a = greater.extractMin();
            greater.insert(a);
            int b = -1 * negLesser.extractMin();
            negLesser.insert(-1*b);
            return (a+b)/2.0;
        }
        else if(negLesser._arr.size()==1+greater._arr.size())
        {
            
            int b = -1 * negLesser.extractMin();
            negLesser.insert(-1*b);
            return (double)b;
        }
        else if(negLesser._arr.size()+1==greater._arr.size())
        {
            int a = greater.extractMin();
            greater.insert(a);
            return (double)a;
        }
        else
        {
            std::cout<<"median stream aint working"<<std::endl;
        }

    }
    void newElement(int i)
    {
        if(i >= getMedian())
        {
            greater.insert(i);
        }
        else
        {
            negLesser.insert(-1*i);
        }
    }
};





//////////////////////////////////////////////////////////////////////////////////////////////////


static inline void ReplaceChar(std::string &str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}


int parseInt(const std::string& text) {
    int result = 0;
    bool isNeg = false;
    size_t i = 0;

    if (text[0] == '-') {
        isNeg = true;
        i = 1;
    }

    for (; i < text.length(); i++) {
        char c = text[i];
        if (c >= '0' && c <= '9') {
            int digit = c - '0';
            result = result * 10 + digit;
        } else {
            throw std::invalid_argument("Invalid character in the input string");
        }
    }

    if (isNeg) {
        result = -result;
    }

    return result;
}

template <typename T>
int findinvec(std::vector<T> vec, T key)
{
    for(int i = 0; i<vec.size(); i++)
    {
        if(vec[i] == key)
        {
            return i;
        }
    }
    return -1;
}


bool isAlphabetic(const std::string& str) {
    for (char c : str) {
        if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z')) {
            return false;
        }
    }
    return true;
}

bool isSignedNumeric(const std::string& str) {
    for (char c : str) {
        if (!((c >= '0' && c <= '9') || (c == '-'))) {
            return false;
        }
    }
    return true;
}

long pow(long base, long exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (exponent % 2 == 0) {
        long half_pow = pow(base, exponent / 2);
        return half_pow * half_pow;
    } else {
        long half_pow = pow(base, (exponent - 1) / 2);
        return base * half_pow * half_pow;
    }
}

std::vector<int> maxUptoLinearComb(std::vector<std::vector<int>> &hist, std::vector<int> &prius, std::vector<int> &quantis)
{   
    // TODO: Complete
    
    /*
        Weird return format, returns vector of quantity for each trade taken, and an extra element at end containing profit
    */
    int profit = 0;
    std::vector<int> retvalue;
    int dictsize = hist[hist.size()-1].size();              // techincally hist should be rectangle
    long treesize = 1;
    for (size_t i = 0; i < quantis.size(); i++)
    {
        treesize *= (quantis[i]+1);
    }
    

    // std::cout<<treesize<<std::endl;

    for(long i = 1; i < treesize; i++)
    {
        std::vector<int> temptrads;
        int tempret = 0;
        bool valid_arbitrage = false;
        bool thisvalid;
        int hld = i;
        int pgg = 0;
        while (hld > 0) 
        {

            temptrads.push_back(hld % (quantis[pgg]+1));
            hld /= (quantis[pgg++]+1);
        
        }

        for(int pkl=temptrads.size(); pkl<hist.size(); pkl++)
        {
            temptrads.push_back(0);
        }
        // for(int pkl=0; pkl<temptrads.size(); pkl++)
        // {
        //     std::cout<<temptrads[pkl];
        // }
        // std::cout<<std::endl;

        for(int p=0; p<dictsize; p++)
        {   int chckr = 0;
            thisvalid = true;
            for(int j = 0; j<temptrads.size(); j++)
            {
                chckr += temptrads[j]*hist[j][p];
            }
            if(chckr != 0)
            {
                thisvalid = false;
                break;
            }
        }
        if(thisvalid)
        {
        
            for(int j=0; j<temptrads.size(); j++)
            {
                tempret += prius[j] * temptrads[j];
            }
            if(tempret>=profit)
            {
                profit = tempret;
                retvalue = temptrads;

                valid_arbitrage = true;
            }
        }


    }



    for(int i = retvalue.size(); i < prius.size(); i++)
    {
        retvalue.push_back(0);
    }
    retvalue.push_back(profit);

    return retvalue;

}

void part3() {

    // Receiver rcv;
    // //sleep(5);
    // std::string message = rcv.readIML();
    std::string message = "X 1 Y -1 10 2 b#\nZ -1 Y 1 -15 5 b#\nZ 1 X -1 10 4 b#\nG 1 X 1 10 5 s#\nZ 1 G 1 50 7 b#$";
    ReplaceChar(message, std::string("\n"), std::string(" "));

    if (!message.empty()) {
        message.erase(message.size() - 1);
    }
    std::istringstream isis(message);
    
    std::vector<std::string> tokens;
    std::string tok;
    while (std::getline(isis, tok, ' ')) {
        tokens.push_back(tok);
    }

    // for(int k = 0; k < tokens.size(); k++)
    // {
    //     std::cout<<tokens[k]<<std::endl;
    // }

    std::vector<std::string> dict;
    std::vector<std::vector<int>> history;
    std::vector<int> pricehist;
    std::vector<int> trade_quantity;
    std::vector<bool> isBuy;
    
    int i=0;

    // std::cout<<isAlphabetic("Z")<<std::endl;

    while(i<tokens.size()) 
    {
        
            // std::cout<<tokens[i];
            // for(char c: tokens[i])
            // {
            //     std::cout<<'^'<<int(c);

            // } 
            // std::cout<<std::endl;
            
            if(isAlphabetic(tokens[i]))
            {
                // std::cout<<tokens[i]<<std::endl;
                if(findinvec(dict, tokens[i]) == -1)
                {dict.push_back(tokens[i]);}
            }
            i++;

    }

    i = 0;
    // for(int k = 0; k < dict.size(); k++)
    // {
    //     std::cout<<dict[k]<<std::endl;
    // }
    std::vector<int> curr(dict.size(), 0);

    while(i<tokens.size()) 
    {       

        
    
    
        if(isAlphabetic(tokens[i]) && isSignedNumeric(tokens[i+1]))
        {
            int j = findinvec(dict, tokens[i]);

            // if(j == -1)                  // shouldnt be necessary
            // {
            //     j = dict.size();
            //     dict.push_back(tokens[i]);
            // }
            curr[j] = parseInt(tokens[i+1]);
            




            
        }

        else
        {   
            
            if(tokens[i+2][1] != '#')
            {
                std::cout << "We fucked up somewhere, hard. " << tokens[i] << " " << tokens[i+1] << std::endl;
            }
            
            // for(int ppp=0; ppp<curr.size(); ppp++)
            // {
            //     std::cout<<curr[ppp];
            // }
            // std::cout<<std::endl;
            history.push_back(curr);
            pricehist.push_back(parseInt(tokens[i]));
            trade_quantity.push_back(parseInt(tokens[i+1]));

            if(tokens[i+2][0] == 's')       // must reverse all signs
            {
                isBuy.push_back(false);
                pricehist[pricehist.size()-1] *= -1;

                for(int jkl = 0; jkl<history[pricehist.size()-1].size(); jkl++)
                {
                    history[pricehist.size()-1][jkl] *= -1;
                }
            }
            else
            {
                
                isBuy.push_back(true);
            }
            for(int ij =0; ij<curr.size(); ij++)
            {
                curr[ij] = 0;
            }
            i+=1;
        }

        i += 2;

    }

    for(int k = 0; k < history.size(); k++)
    {
        for(int kk = 0; kk < history[k].size(); kk++)
        {
         
            std::cout<<history[k][kk]*trade_quantity[k]<<" ";
        }
        std::cout<<"\t"<<pricehist[k]<<std::endl;
    }
    std::cout<<"aawwwwwwwwwwaaaa"<<std::endl;


    std::vector<int> clc = maxUptoLinearComb(history, pricehist, trade_quantity);
    for(int i=0; i<clc.size(); i++)
    {
        std::cout << clc[i] << '\n';
    }


    
}


std::vector<int> maxLinearComb(std::vector<std::vector<int>> &hist, std::vector<int> &prius)
{
    
    /*
        Weird return format, returns vector of 0s & 1s for each trade taken, and an extra element at end containing profit
    */
    int profit = 0;
    std::vector<int> retvalue;
    int dictsize = hist[hist.size()-1].size();              // techincally hist should be rectangle
    long treesize = pow(2, hist.size());

    // std::cout<<treesize<<std::endl;

    for(long i = 1; i < treesize; i++)
    {
        std::vector<int> temptrads;
        int tempret = 0;
        bool valid_arbitrage = false;
        bool thisvalid;
        int hld = i;
        while (hld > 0) 
        {

            temptrads.push_back(hld % 2);
            hld /= 2;
        }

        for(int pkl=temptrads.size(); pkl<hist.size(); pkl++)
        {
            temptrads.push_back(0);
        }
        // for(int pkl=0; pkl<temptrads.size(); pkl++)
        // {
        //     std::cout<<temptrads[pkl];
        // }
        // std::cout<<std::endl;

        for(int p=0; p<dictsize; p++)
        {   int chckr = 0;
            thisvalid = true;
            for(int j = 0; j<temptrads.size(); j++)
            {
                chckr += temptrads[j]*hist[j][p];
            }
            if(chckr != 0)
            {
                thisvalid = false;
                break;
            }
        }
        if(thisvalid)
        {
        
            for(int j=0; j<temptrads.size(); j++)
            {
                tempret += prius[j] * temptrads[j];
            }
            if(tempret>=profit)
            {
                profit = tempret;
                retvalue = temptrads;

                valid_arbitrage = true;
            }
        }


    }



    for(int i = retvalue.size(); i < prius.size(); i++)
    {
        retvalue.push_back(0);
    }
    retvalue.push_back(profit);

    return retvalue;

}



void part2() {

    // Receiver rcv;
    // //sleep(5);
    // std::string message = rcv.readIML();
    std::string message = "X 1 Y -1 10 b#\nZ -1 Y 1 -15 b#\nZ 1 X -1 10 b#\nG 1 X 1 10 s#\nZ 1 G 1 50 b#$";
    ReplaceChar(message, std::string("\n"), std::string(" "));

    if (!message.empty()) {
        message.erase(message.size() - 1);
    }
    std::istringstream isis(message);
    
    std::vector<std::string> tokens;
    std::string tok;
    while (std::getline(isis, tok, ' ')) {
        tokens.push_back(tok);
    }

    // for(int k = 0; k < tokens.size(); k++)
    // {
    //     std::cout<<tokens[k]<<std::endl;
    // }

    std::vector<std::string> dict;
    std::vector<std::vector<int>> history;
    std::vector<int> pricehist;
    std::vector<bool> isBuy;
    
    int i=0;

    // std::cout<<isAlphabetic("Z")<<std::endl;

    while(i<tokens.size()) 
    {
        
            // std::cout<<tokens[i];
            // for(char c: tokens[i])
            // {
            //     std::cout<<'^'<<int(c);

            // } 
            // std::cout<<std::endl;
            
            if(isAlphabetic(tokens[i]))
            {
                // std::cout<<tokens[i]<<std::endl;
                if(findinvec(dict, tokens[i]) == -1)
                {dict.push_back(tokens[i]);}
            }
            i++;

    }

    i = 0;
    // for(int k = 0; k < dict.size(); k++)
    // {
    //     std::cout<<dict[k]<<std::endl;
    // }
    std::vector<int> curr(dict.size(), 0);

    while(i<tokens.size()) 
    {       

        
    
    
        if(isAlphabetic(tokens[i]) && isSignedNumeric(tokens[i+1]))
        {
            int j = findinvec(dict, tokens[i]);

            // if(j == -1)                  // shouldnt be necessary
            // {
            //     j = dict.size();
            //     dict.push_back(tokens[i]);
            // }
            curr[j] = parseInt(tokens[i+1]);
            




            
        }

        else
        {   
            
            if(tokens[i+1][1] != '#')
            {
                std::cout << "We fucked up somewhere, hard. " << tokens[i] << " " << tokens[i+1] << std::endl;
            }
            
            // for(int ppp=0; ppp<curr.size(); ppp++)
            // {
            //     std::cout<<curr[ppp];
            // }
            std::cout<<std::endl;
            history.push_back(curr);
            pricehist.push_back(parseInt(tokens[i]));

            if(tokens[i+1][0] == 's')       // must reverse all signs
            {
                isBuy.push_back(false);
                pricehist[pricehist.size()-1] *= -1;

                for(int jkl = 0; jkl<history[pricehist.size()-1].size(); jkl++)
                {
                    history[pricehist.size()-1][jkl] *= -1;
                }
            }
            else
            {
                isBuy.push_back(true);
            }
            for(int ij =0; ij<curr.size(); ij++)
            {
                curr[ij] = 0;
            }
        }

        i += 2;

    }

    for(int k = 0; k < history.size(); k++)
    {
        for(int kk = 0; kk < history[k].size(); kk++)
        {
         
            std::cout<<history[k][kk]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"aaaaaaaaaaaaaaa"<<std::endl;


    std::vector<int> clc = maxLinearComb(history, pricehist);
    for(int i=0; i<clc.size(); i++)
    {
        std::cout << clc[i] << '\n';
    }


    
}
        



// int main() {

//     // Receiver rcv;
//     // sleep(5);
//     // std::string message = rcv.readIML();
//     // std::cout << message;

//     part2();

//     return 0;
// }

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
    part3();
    return 0;
}

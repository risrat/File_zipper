#include<iostream>
#include "headers/mapOfCharacters.cpp"
#include "headers/classes.cpp"
#include <fstream> 

string bin(long n){
    long i;
    string s = "";
    s+="0";
    for (i = 1 << 30; i > 0; i = i / 2){
        if((n & i) != 0){
            s+="1";
        }
        else{
            s+="0";
        }
    }
    return s;
}

string fixedCode(int e, int r, int index){
    string output = "";
    //if index lies in [1,2r]
    if(index >=1 && index<=2*r){
        //represent index-1 in (e+1) bits
        string s = bin(index-1);
        for(int i = s.size()-e-1; i<s.size(); i++){
            output+=s[i];
        }
    }
    //else if index lies in [2r+1, infinity]
    else{
        //represent (index-r-1) in e bits
        string s = bin(index-r-1);
        for(int i = s.size()-e; i<s.size(); i++){
            output+=s[i];
        }
    }
    return output;
}

AdaptiveHuffmanTreeNode* reqNode;

void update(AdaptiveHuffmanTreeNode* head, string element, string &ans, string temp){
    if(head == NULL){
        return;
    }
    if(head->character == element){
        ans = temp;
        reqNode = head;
        return;
    }
    
    update(head->left, element, ans, temp+"0");
    update(head->right, element, ans, temp+"1");
    
}

void updateTree(AdaptiveHuffmanTreeNode* head){
    head->weight++;
    head=head->parent;
    while(head!=NULL){
        if(head->left->weight > head->right->weight){
            AdaptiveHuffmanTreeNode* temp = head->left;
            head->left = head->right;
            head->right = temp;
        }
        head->weight = head->left->weight + head->right->weight;
        head=head->parent;
    }
}

void findNytCode(AdaptiveHuffmanTreeNode* head, AdaptiveHuffmanTreeNode* nyt, string &nytCode, string temp){
    if(head == NULL){
        return;
    }
    if(head->left == NULL && head->right == NULL && head==nyt){
        nytCode = temp;
        return;
    }
    findNytCode(head->left,nyt, nytCode, temp+"0");
    findNytCode(head->right,nyt, nytCode, temp+"1");
}


string encode(string input){
    //2 parameters e and r
    // m = 94 = 2^e + r
    //e = 6, r = 30
    int e = 6;
    int r = 30;
    AdaptiveHuffmanTree* tree = new AdaptiveHuffmanTree();
    string output = "";

    map<string, int> mapOfChars = mapOfCharacters();
    map<string, int> existsInTree;
    string s(1, input[0]);
    existsInTree[s] = 1;

    //setting up tree with input[0]
    output += fixedCode(e, r, mapOfChars[s]);
    AdaptiveHuffmanTreeNode* head = new AdaptiveHuffmanTreeNode(1);
    tree->head = head;
    tree->head->left = tree->nyt;
    tree->nyt->parent = tree->head;
    tree->head->right = new AdaptiveHuffmanTreeNode(1, s);
    tree->head->right->parent = tree->head;
    tree->nytCode = "0";

    for(int i = 1; i < input.size(); i++){
        string s(1, input[i]);

        //if s is in tree -> update tree
        if(existsInTree[s]){
            string ans = "";
            update(tree->head, s, ans, "");
            updateTree(reqNode);
            // cout << s << " " << ans << endl;
            output+=ans;
        }
        //else if s is not in tree -> add s to tree
        else{
            output = output + tree->nytCode + fixedCode(e, r, mapOfChars[s]);
            existsInTree[s] = 1;

            AdaptiveHuffmanTreeNode* newNode = new AdaptiveHuffmanTreeNode(1);
            newNode->parent = tree->nyt->parent;
            newNode->left = tree->nyt;
            newNode->right = new AdaptiveHuffmanTreeNode(1, s);
            newNode->right->parent = newNode;
            tree->nyt->parent->left = newNode;
            tree->nyt->parent = newNode;

            newNode = newNode->parent;
            while(newNode!=NULL){
                if(newNode->left->weight > newNode->right->weight){
                    AdaptiveHuffmanTreeNode* temp = newNode->left;
                    newNode->left = newNode->right;
                    newNode->right = temp;
                }
                newNode->weight = newNode->left->weight + newNode->right->weight;
                newNode=newNode->parent;
            }
            string nytCode = "";
            findNytCode(tree->head,tree->nyt, nytCode, "");
            tree->nytCode = nytCode;
            
        }
    }

    return output;
}

string mainCompressFunction(string s){
    return encode(s);
}

int main(){ 
    // string input = "My name is Aryaman Gupta. I am a student at JIIT{[]]]} Noida!";
    string input = "";
    string myText;

    // Read from the text file
    ifstream MyReadFile("./uploads/file.txt");

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
    // Output the text from the file
        input+=myText;
    }
    cout << encode(input) << endl;
    return 0;
}
#include "headers/classes.cpp"
#include "headers/mapOfNumbers.cpp"
#include<iostream>
#include <fstream> 

int binaryToDecimal(string str)
{
    int dec_num = 0;
    int power = 0 ;
    int n = str.length() ;
   
      for(int i = n-1 ; i>=0 ; i--){
      if(str[i] == '1'){
        dec_num += (1<<power) ;
      }
      power++ ;
    }
   
    return dec_num;
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

void updateTree(string current_char, AdaptiveHuffmanTree* tree){
    AdaptiveHuffmanTreeNode* newNode = new AdaptiveHuffmanTreeNode(1);
    newNode->parent = tree->nyt->parent;
    newNode->left = tree->nyt;
    newNode->right = new AdaptiveHuffmanTreeNode(1, current_char);
    newNode->right->parent = newNode;
    tree->nyt->parent->left = newNode;
    tree->nyt->parent = newNode;

    newNode = newNode->parent;
    while(newNode!=NULL){
        newNode->weight = newNode->left->weight + newNode->right->weight;
        if(newNode->left->weight > newNode->right->weight){
            AdaptiveHuffmanTreeNode* temp = newNode->left;
            newNode->left = newNode->right;
            newNode->right = temp;
        }
        newNode=newNode->parent;
    }
    string nytCode = "";
    findNytCode(tree->head,tree->nyt, nytCode, "");
    tree->nytCode = nytCode;
}

string decode(string input){
    //2 parameters e and r
    // m = 94 = 2^e + r
    //e = 6, r = 30
    int e = 6;
    int r = 30;
    AdaptiveHuffmanTree* tree = new AdaptiveHuffmanTree();
    string output = "";

    map<int, string> mp = mapOfNumbers();

    int i;
    string temp = "";
    string current_char = "";

    for(i = 0; i < e; i++){
        temp += input[i];
    }
    //is bit value < r?
    if(binaryToDecimal(temp)<r){
        // i = i+1;
        temp += input[i];
        output += mp[binaryToDecimal(temp)+1]; 
        current_char = mp[binaryToDecimal(temp)+1];
        i = i+1;
    }
    else{
        output += mp[binaryToDecimal(temp) + r + 1];
        current_char = mp[binaryToDecimal(temp) + r + 1];
    }
    AdaptiveHuffmanTreeNode* head = new AdaptiveHuffmanTreeNode(1);
    tree->head = head;
    tree->head->left = tree->nyt;
    tree->nyt->parent = tree->head;
    tree->head->right = new AdaptiveHuffmanTreeNode(1, current_char);
    tree->head->right->parent = tree->head;
    tree->nytCode = "0";

    while(i<input.size()){
        temp = "";
        string tempNytCode = "";
        if(i + tree->nytCode.size() < input.size()){
            for(int j = i; j < i + tree->nytCode.size(); j++){
                tempNytCode += input[j];
            }
        }
        //if nyt code, i.e. element does not exist in tree
        if(tempNytCode == tree->nytCode){
            
            i = i+tree->nytCode.size();
            int j;

            for(j= i; j < i + e; j++){
                temp += input[j];
            }
            
            if(binaryToDecimal(temp) < r){
                temp += input[j];
                output += mp[binaryToDecimal(temp) + 1]; 
                current_char = mp[binaryToDecimal(temp) + 1];
                i = j+1;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
            else{
                output += mp[binaryToDecimal(temp) + r + 1];
                current_char = mp[binaryToDecimal(temp) + r + 1];
                i = j;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
        }
        //if not nyt code, i.e. element already exists in tree
        else{
            AdaptiveHuffmanTreeNode* curr = tree->head;
            while(curr->character==""){
                if(input[i] == '0'){
                    curr = curr->left;
                }
                else{
                    curr = curr->right;
                }
                temp += input[i];
                i++;
            }

            output += curr->character;
            curr->weight+=1;
            curr = curr->parent;
            
            while(curr!=NULL){
                curr->weight = curr->left->weight + curr->right->weight;
                if(curr->left->weight > curr->right->weight){
                    AdaptiveHuffmanTreeNode* temp = curr->left;
                    curr->left = curr->right;
                    curr->right = temp;
                }
                curr=curr->parent;
            }
        }
    }

    return output;
}

int main(){
    string input = "";
    string myText;

    // Read from the text file
    ifstream MyReadFile("./uploads/file-compressed.bin");

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
    // Output the text from the file
        input+=myText;
    }

    cout << decode(input) << endl;
    return 0;
}
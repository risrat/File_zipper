#include<string>
using namespace std;

class AdaptiveHuffmanTreeNode{
    public:
        int weight;
        string character;
        AdaptiveHuffmanTreeNode* left;
        AdaptiveHuffmanTreeNode* right;
        AdaptiveHuffmanTreeNode* parent;

        AdaptiveHuffmanTreeNode(int weight = 0, string character = ""){
            this->weight = weight;
            this->character = character;
            this->left = NULL;
            this->right = NULL;
            this->parent = NULL;
        }
};

class AdaptiveHuffmanTree{
    public:
        AdaptiveHuffmanTreeNode* head;
        AdaptiveHuffmanTreeNode* nyt;
        string nytCode;

        AdaptiveHuffmanTree(){
            AdaptiveHuffmanTreeNode *nyt = new AdaptiveHuffmanTreeNode();
            this->head = nyt;
            this->nyt = nyt;
            this->nytCode = "";
        }
};
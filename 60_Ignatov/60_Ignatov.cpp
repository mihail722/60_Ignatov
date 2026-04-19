using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include "60_Ignatov.h"
#include <stdexcept>

int exprNode::globalCounter = 0;

// Конструктор операнда
exprNode::exprNode(int val, int pos)
{
    this->value = val;
    this->type = VALUE;
    this->left = nullptr;
    this->right = nullptr;
    this->pos = pos;
    this->nodeId = globalCounter++;
}

// Конструктор операции
exprNode::exprNode(exprNodeType type, exprNode* left, exprNode* right, int pos)
{
    this->value = 0;
    this->type = type;
    this->left = left;
    this->right = right;
    this->pos = pos;
    this->nodeId = globalCounter++;
}

// Деструктор 
exprNode::~exprNode()
{
    delete left;
    delete right;
}

// Геттеры
int exprNode::getValue() const
{ 
    return value; 
}

exprNode* exprNode::getLeft() const
{ 
    return left; 
}

exprNode* exprNode::getRight() const
{ 
    return right;
}

int exprNode::getId() const
{ 
    return nodeId; 
}

exprNodeType exprNode::getType() const
{ 
    return type; 
}
    
int exprNode::getPos() const
{ 
    return pos; 
}

// Сеттеры
void exprNode::setValue(int val)
{
    this->value = val;
}
void exprNode::setLeft(exprNode* node)
{
    this->left = node;
}

void exprNode::setRight(exprNode* node)
{
    this->right = node;
}

void exprNode::setType(exprNodeType type)
{
    this->type = type;
}


int exprNode::calculate(exprNode* node)
{
    return 0;
}

bool isOperand(const string& token)
{
    return false;
}

exprNodeType getOperationType(const string& token)
{
    return VALUE;
}

vector<pair<string, int>> tokenize(const string& line)
{
    vector<pair<string, int>> tokens = {};

    return tokens;
}

bool isOperation(const string& token)
{
    return false;
}

exprNode* buildTree(const vector<pair<string, int>>& tokens, vector<error>& errors)
{
    return nullptr;
}

exprNode* parseExpression(const string& line, vector<error>& errors)
{
    return nullptr;
}

void generateGraph(exprNode* root, ofstream& out)
{
    return;
}

void writeGraph(exprNode* root, ofstream& out)
{
    return;
}

int main(int argc, char* argv[])
{
    return 0;
}
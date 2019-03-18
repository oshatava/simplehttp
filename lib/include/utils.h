#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <map>
#include "logger.h"

namespace server
{
namespace utils
{

std::string urlDecode(const std::string &in);
std::vector<std::string> split(const std::string &s);
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

template <class T>
class Matcher
{
  private:
    typedef std::string (*GetterFn)(T &t);
    typedef bool (*Op)(std::string &, std::string &);
    typedef bool (*OpB)(bool, bool);


    class Node{
        public:
            virtual bool calculate(T &t) = 0;
            virtual ~Node(){}
    };

    class OpNode:public Node{
        public:
            Op op;
            GetterFn fn;
            std::string value;
        public:
        OpNode(Op op, GetterFn fn, std::string value):op(op), fn(fn), value(value){}
        OpNode(const OpNode &o):OpNode(o.op, o.fn, o.value){}

        bool calculate(T &t){
            std::string fv = fn(t);
            return op(fv, value);
        }
    };

    class OpbNode:public Node{
        public:
        OpB op;
        Node *left, *right;

        public:
        OpbNode():op(opOr),left(NULL), right(NULL){}
        OpbNode(OpB op, Node *left, Node *right):op(op), left(left), right(right){}
        OpbNode(const OpbNode &o):OpbNode(o.op, o.left, o.right){}

        ~OpbNode(){
            if(left!=NULL) delete left;
            if(right!=NULL) delete right;
        }
        
        bool calculate(T &t){
            if(left!=NULL && right!=NULL)
                return op(left->calculate(t), right->calculate(t));
            else if(left!=NULL)
                return left->calculate(t);
            else if(right!=NULL)
                return right->calculate(t);
            else 
                return true;
        }
    };


    std::map<std::string, GetterFn> fields;

    OpbNode root;

    bool isField(std::string name)
    {
        return fields.find(name) != fields.end();
    }

    bool calculate(T &t, std::string &opName, std::string &fieldName, std::string &value, bool &res)
    {
        Op op = getOp(opName);
        if (op != NULL)
        {
            std::string fieldValue = fields[fieldName](t);
            res = op(fieldValue, value);
            return true;
        }
        return false;
    }

    bool calculate(std::string &opName, bool a, bool b, bool &res)
    {
        OpB op = getOpB(opName);
        if (op != NULL)
        {
            res = op(a, b);
            return true;
        }
        return false;
    }

    static bool opEq(std::string &a, std::string &b)
    {
        return a.compare(b) == 0;
    }

    static bool opNeq(std::string &a, std::string &b)
    {
        return a.compare(b) != 0;
    }

    static bool opAnd(bool a, bool b)
    {
        return a && b;
    }

    static bool opOr(bool a, bool b)
    {
        return a || b;
    }

    Op getOp(std::string &op)
    {
        if (op.compare("eq") == 0)
            return opEq;
        else if (op.compare("neq") == 0)
            return opNeq;
        else
            return NULL;
    }

    OpB getOpB(std::string &op)
    {
        if (op.compare("or") == 0)
            return opOr;
        else if (op.compare("and") == 0)
            return opAnd;
        else
            return NULL;
    }

  protected:
    void registerField(std::string name, GetterFn fn)
    {
        fields[name] = fn;
    }
    void build(std::string &term){
        std::vector<std::string> words = server::utils::split(term);
        std::vector<std::string>::iterator i = words.begin();
        bool isFirst = true;
        OpbNode *lastOpB = &root;        
        while (i != words.end())
        {
            std::string w = *i;
            if (isField(w))
            {
                std::string fieldName = w;
                std::string opS = *(++i);
                std::string value = *(++i);
                Op op = getOp(opS);
                if(op!=NULL){
                    if (isFirst)
                    {
                        lastOpB->left = new OpNode(op, fields[fieldName], value);
                        isFirst = false;
                    }else{
                        OpbNode *newNode = new OpbNode();
                        newNode->left = new OpNode(op, fields[fieldName], value);
                        lastOpB->right = newNode;
                        lastOpB = newNode;
                    }
                }                
            }
            else
            {
                lastOpB->op = getOpB(w);
            }
            i++;
        }
    }
  public:
    Matcher(){}

    bool match(T &t)
    {
        /* operations
         - eq
         - neq
         - and 
         - or
        */
        return root.calculate(t);
    }
};

#define matcher(type, name) registerField(#name, [](type &t) {std::stringstream ss; ss<<t.name; return ss.str(); });

} // namespace utils
} // namespace server

#endif
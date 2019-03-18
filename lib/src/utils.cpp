#include "utils.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include "utils.h"
#include "entity.h"

static unsigned char hexTable[256] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 16, 16, 16, 16, 16, 16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

inline bool space(char c){
    return std::isspace(c);
}
 
inline bool notspace(char c){
    return !std::isspace(c);
}

void server::utils::ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void server::utils::rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void server::utils::trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

inline void assertDistance(int left)
{
    if (left < 2)
    {
    }
}

std::string server::utils::urlDecode(const std::string &source)
{
    int size = source.length() + 1;
    char *out = new char[size];
    bzero(out, size);
    int outpos = 0;
    int i = 0;
    while (i < size)
    {
        char p = source[i++];
        switch (p)
        {
        case '%':
        {
            if (size - i - 1 < 2)
                server::ServerError::throwFor("Incomplete percent encode sequence!");
            char a1 = hexTable[static_cast<unsigned char>(source[i++])];
            char a2 = hexTable[static_cast<unsigned char>(source[i++])];
            out[outpos++] = (a1 << 4) | a2;
        }
        break;
        case '+':
            out[outpos++] = ' ';
            break;
        default:
            out[outpos++] = p;
            break;
        }
    }
    std::string ret(out);
    delete [] out;
    return ret;
}

std::vector<std::string> server::utils::split(const std::string& s){
    typedef std::string::const_iterator iter;
    std::vector<std::string> ret;
    iter i = s.begin();
    while(i!=s.end()){
        i = std::find_if(i, s.end(), notspace); // find the beginning of a word
        iter j= std::find_if(i, s.end(), space); // find the end of the same word
        if(i!=s.end()){
            ret.push_back(std::string(i, j)); //insert the word into vector
            i = j; // repeat 1,2,3 on the rest of the line.
        }
    }
    return ret;
}
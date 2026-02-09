#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include<string>
#include<cctype>
#include <vector>

class utils{

public:
    static std::string trim(std::string str) {
        int s = 0, e = str.size() - 1;

        while (s <= e && std::isspace(str[s])) s++;
        while (e >= s && std::isspace(str[e])) e--;

        return str.substr(s, e - s + 1);
    }

    static std::vector<std::string> split(std::string str, char delimiter){
        std::stringstream ss(str);
        std::string item;
        std::vector<std::string> items;

        while(std::getline(ss, item, delimiter)) items.push_back(item);

        return items;
    }

    static std::string toLowerCase(std::string str){
        for(char c: str){
            c=tolower(c);
        }
        return str;
       }
};

#endif

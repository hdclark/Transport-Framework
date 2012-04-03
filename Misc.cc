
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <ctime> //For the gen_time_random().


void gen_random(std::string *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    (*s).clear();
    for(int i=0; i<len; ++i)   (*s) += alphanum[rand()%(sizeof(alphanum)-1)];
    return;
}
void seed_time(void){
    //Seed the rand() function with the current time.
    srand(time(NULL));
    return;
}

std::string inttostring(long int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}

std::string floattostring(float number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}


template <class T>
std::string Xtostring(T numb){
    std::stringstream ss;
    ss << numb;
    return ss.str();
}

template std::string Xtostring<int>(int);
template std::string Xtostring<long int>(long int);
template std::string Xtostring<float>(float);
template std::string Xtostring<double>(double);


template <class T>
T stringtoX(std::string text){
    std::stringstream ss(text);
    T temp;
    ss >> temp;
    return temp;
}

template float stringtoX<float>(std::string);
template double stringtoX<double>(std::string);
template int stringtoX<int>(std::string);
template unsigned int stringtoX<unsigned int>(std::string);
template long int stringtoX<long int>(std::string);


//Turns a string into a number suitable for seeding a PRNG.
unsigned long simple_hash(std::string &str){
    //See the excellent resource at http://www.cse.yorku.ca/~oz/hash.html . A bunch of nearly one-liners.
    //This is the djb2 algorithm.
    unsigned long hash = 5381;
    int c;

    if(str.empty()) return 0;

    const char *spot = str.data();

    while(c = *spot++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}




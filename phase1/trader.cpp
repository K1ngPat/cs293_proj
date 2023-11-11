#include "receiver.h"
#include <vector>

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

std::vector<int> maxLinearComb(std::vector<std::vector<int>> hist, std::vector<int> prius)
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
        



int main() {

    // Receiver rcv;
    // sleep(5);
    // std::string message = rcv.readIML();
    // std::cout << message;

    part2();

    return 0;
}


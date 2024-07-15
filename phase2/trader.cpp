// This is your trader. Place your orders from here


#include <string>
#include <vector>
#include<iostream>

int randNatural(int seed1, int seed2)   // no-patern-ness increases from MSB to LSB
{
    int a = seed1 ^ seed2;
    for(int i = 0; i < seed2 % 97; i++)
    {
        seed1 = (seed1 * seed1) % seed2;
        a = a ^ seed1;
    }
    if(a>=0)
    {
        return a;
    }
    return -a;
}

bool bernoulliRV(double chancesTrue, int seed)
{
    for(int i = 0; i < 79; i++)
    {
        seed = (seed * seed) % 10079;
    }
    if(seed/10079 < chancesTrue)
    {
        return true;
    }
    return false;
}


int vectorMeasure(std::vector<int> a)
{
    int sum = 0;
    for(int j=0; j<a.size(); j++)
    {
        sum += a[j] * a[j];
    }
    return sum;
}
int vectorDifferenceMeasure(std::vector<int> a, std::vector<int> b)
{
    int sum = 0;
    for(int j=0; j<a.size(); j++)
    {
        sum += (a[j] - b[j]) * (a[j] - b[j]);
    }
    return sum;
}

int vectorSumMeasure(std::vector<int> a, std::vector<int> b)
{
    int sum = 0;
    for(int j=0; j<a.size(); j++)
    {
        sum += (a[j] + b[j]) * (a[j] + b[j]);
    }
    return sum;
}


std::vector<int> returnHoldings(std::vector<std::vector<int>> &hist, std::vector<int> &quantis)
{
    std::vector<int> j(hist[0].size(), 0);
    for(int jp = 0; jp < quantis.size(); jp++)
    {
        for(int i = 0; i < j.size(); i++)
        {
            j[i] += hist[jp][i] * quantis[jp];
        }
    }

    return j;
}



bool pruneRow(std::vector<std::vector<int>> &hist, std::vector<int> &quantis, int i)
{
    i = i%hist.size();
    bool retval = false;
    for(int j = 0; j<hist[i].size(); j++)
    {
        if(hist[i][j]==0)
        {
            continue;
        }
        bool isPositive = (hist[i][j]>0);
        int volumeByRest = 0;

        for(int p = 0; p<hist.size(); p++)
        {
            if(p==i)
            {
                continue;
            }
            if(isPositive && hist[p][j] < 0)
            {
                volumeByRest -= hist[p][j] * quantis[p];
            }
            if(!isPositive && hist[p][j] > 0)
            {
                volumeByRest += hist[p][j] * quantis[p];
            }
        }
        volumeByRest = (isPositive) ? volumeByRest/hist[i][j] : volumeByRest/(-hist[i][j]);
        volumeByRest++;
        if(volumeByRest < quantis[i])
        {
            quantis[i] = volumeByRest;
            retval = true;
        }
    }
    return retval;
}


void pruneQuantities(std::vector<std::vector<int>> &hist, std::vector<int> &quantis)
{   
    while(true)
    {   
        bool hld = false;

        for(int i = 0; i<hist.size(); i++)
        {
            hld = hld || pruneRow(hist, quantis, i);
        }
        if(!hld)
        {
            return;
        }
    }

}


std::vector<int> maxUptoLinearComb(std::vector<std::vector<int>> &hist, std::vector<int> &prius, std::vector<int> &quantis)
{   
    
    int profit = 0;
    std::vector<int> retvalue;
    int dictsize = hist[hist.size()-1].size();              // techincally hist should be rectangle
    long treesize = 1;
    for (size_t i = 0; i < quantis.size(); i++)
    {
        treesize *= (quantis[i]+1);
    }
    

    // std::cout<<treesize<<std::endl;
    std::vector<int> temptrads(hist.size(),0);

    for(long i = 1; i < treesize; i++)
    {
        
        int tempret = 0;
        bool valid_arbitrage = false;
        bool thisvalid;
        int hld = i;
        int pgg = 0;

        while (hld > 0) 
        {

            temptrads[pgg] = (hld % (quantis[pgg]+1));
            hld /= (quantis[pgg++]+1);
        
        }

        for(int pkl=pgg; pkl<temptrads.size(); pkl++)
        {
            temptrads[pkl] = 0;
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
            if(tempret>profit)
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

std::vector<int> polyArbit(std::vector<std::vector<int>> &hist, std::vector<int> &prius, std::vector<int> &quantis)
{

    if(!(quantis.size() == prius.size() && prius.size() == hist.size()))
    {
        return std::vector<int>(1,-1);
    }
    int recentHistory = 0;
    int nonZeroFound = 0;

    //-----------------------Hyperparameters---------------------------------

    long randomValuesPerTrade = 3;
    int numRecentTrades = 7;
    
    for(int i = hist.size()-1; i >=0; i--)
    {
        recentHistory++;
        if(quantis[i] != 0)
        {
            nonZeroFound++;
        }
        if(nonZeroFound == numRecentTrades)
        {
            break;
        }
    }
    int profit = 0;

    std::vector<int> profitablePortfolio(quantis.size(),0);
    int available_trades = 0;
    for(int ppp=0; ppp<quantis.size(); ppp++)
    {
        if(quantis[ppp]!=0)
        {
            available_trades++;
        }
    }
    int number_of_random_shots = 200 * available_trades * available_trades;

    //------------------ Complete arbit on limited history ------------------- (Polynomial in quantities)

    if(((int)hist.size()-recentHistory)>0)
    {

        std::vector<std::vector<int>> recHist;
        for(int ppq=0; ppq<recentHistory; ppq++)
        {
            recHist.push_back(hist[hist.size() - recentHistory + ppq]);
        }
        std::vector<int> recQuantis(&quantis[quantis.size()-recentHistory], &quantis[quantis.size()-1]);
        std::vector<int> recPrius(&prius[hist.size()-recentHistory], &prius[hist.size()-1]);
        pruneQuantities(recHist, recQuantis);
        std::vector<int> recentHistoryArbitrage = maxUptoLinearComb(recHist, recPrius, recQuantis);
        profit = recentHistoryArbitrage[recentHistoryArbitrage.size()-1];
        recentHistoryArbitrage.pop_back();
        for(int ijk=recentHistoryArbitrage.size()-1; ijk>=0; ijk--)
        {
            profitablePortfolio[profitablePortfolio.size() - recentHistoryArbitrage.size() + ijk] = recentHistoryArbitrage[ijk];
        }
    }
    else
    {
        
        pruneQuantities(hist, quantis);
        std::vector<int> recentHistoryArbitrage = maxUptoLinearComb(hist, prius, quantis);
        profit = recentHistoryArbitrage[recentHistoryArbitrage.size()-1];
        recentHistoryArbitrage.pop_back();
        return recentHistoryArbitrage;
    }


    //-------------- My stochastic modulus reducing logic------------------ (Polynomial in number of orders I think?)



    for(int patrick = 0; patrick < number_of_random_shots; patrick++)   // everything inside is O(n^2 * |stocks|)
    {    
        std::vector<int> tradesTaken(quantis.size(),0);
        for(int i=0; i<tradesTaken.size(); i++)
        {
            tradesTaken[i] = randNatural(long(&tradesTaken[i])+patrick, long(&hist[i])*patrick) % (quantis[i] + 1);
        }

        int mod = vectorMeasure(returnHoldings(hist,tradesTaken));
        
        for(int i=0; i<tradesTaken.size(); i++)
        {
            if(quantis[i] == 0)
            {
                continue;
            }
            int x = tradesTaken[i];
            
            // bool changed = false;

            for(int j = 0; j < randomValuesPerTrade; j++)
            {
                tradesTaken[i] = randNatural(long(&hist[i][j%hist[0].size()]), (j*j*j)-j+hist[i][j%hist[0].size()]) % (quantis[i] + 1);
                int testmod = vectorMeasure(returnHoldings(hist,tradesTaken));              // O(ns)
                if(testmod < mod)
                {
                    mod = testmod;
                    x = tradesTaken[i];
                    // changed = true;
                }
            }
            tradesTaken[i] = x;
        }



        std::vector<int> holdings = returnHoldings(hist, tradesTaken);
        bool validArb = true;
        
        for(int i =0; i<holdings.size(); i++)
        {
            if(holdings[i] != 0)
            {
                validArb = false;
                break;
            }

        }

        if(validArb)
        {
            int interimProfit = 0;
            for(int ijk = 0; ijk < quantis.size(); ijk++)
            {
                interimProfit += tradesTaken[ijk] * prius[ijk];
            }
            if(interimProfit>0)
            {
                bool isMergeable = true;
                for(int ijk = 0; ijk < quantis.size(); ijk++)
                {
                    isMergeable = isMergeable && (tradesTaken[ijk] + profitablePortfolio[ijk] <= quantis[ijk]);
                }
                if(isMergeable)
                {
                    for(int ijk = 0; ijk < quantis.size(); ijk++)
                    {
                        profitablePortfolio[ijk] += tradesTaken[ijk];

                    }
                    profit += interimProfit;
                }
            }
            if(interimProfit>profit)
            {
                profit = interimProfit;
                profitablePortfolio = tradesTaken;
            }
            
        }     


    }
    return profitablePortfolio;

}

std::vector<int> polyRandomSampling(std::vector<std::vector<int>> &hist, std::vector<int> &prius, std::vector<int> &quantis)
{
    long randomValuesPerTrade = 3;

    if(!(quantis.size() == prius.size() && prius.size() == hist.size()))
    {
        return std::vector<int>(1,-1);
    }
    int profit = 0;
    std::vector<int> profitablePortfolio(quantis.size(),0);
    int number_of_random_shots = 20 * hist.size() * hist.size() * hist.size() * hist.size();

    for(int patrick = 0; patrick < number_of_random_shots; patrick++)   // everything inside is O(n^2 * |stocks|)
    {    
        std::vector<int> tradesTaken(quantis.size(),0);
        for(int i=0; i<tradesTaken.size(); i++)
        {
            tradesTaken[i] = randNatural(long(&tradesTaken[i])+patrick, long(&hist[i])*patrick) % (quantis[i] + 1);
        }

        


        std::vector<int> holdings = returnHoldings(hist, tradesTaken);
        bool validArb = true;
        
        for(int i =0; i<holdings.size(); i++)
        {
            if(holdings[i] != 0)
            {
                validArb = false;
                break;
            }

        }

        if(validArb)
        {
            int interimProfit = 0;
            for(int ijk = 0; ijk < quantis.size(); ijk++)
            {
                interimProfit += tradesTaken[ijk] * prius[ijk];
            }
            if(interimProfit>profit)
            {
                profit = interimProfit;
                profitablePortfolio = tradesTaken;
            }
            
        }     


    }
    return profitablePortfolio;

}



std::vector<int> medianTrading(std::vector<std::vector<int>> &hist, std::vector<int> &prius, std::vector<int> &quantis, int medianPrice)
{
    std::vector<int> retvalue(hist.size(), 0);

    
    for(int ijk = 0; ijk < hist.size(); ijk++)
    {
        
        bool seenSingle = false;
        bool noIssues = true;
        bool isBuy = false;
        for(int pqr=0; pqr<hist[0].size(); pqr++)
        {
            
            if(seenSingle && hist[ijk][pqr] != 0)
            {
                noIssues = false;
                break;
            }
            if(!seenSingle && (hist[ijk][pqr] == 1 || hist[ijk][pqr] == -1))
            {
                seenSingle = true;
                if(hist[ijk][pqr] == 1)
                {
                    isBuy = true;
                }
            }
            if(hist[ijk][pqr] != 0 && hist[ijk][pqr] != 1 && hist[ijk][pqr] != -1)
            {
                noIssues = false;
                break;
            }

        }

        
        if(noIssues && seenSingle && quantis[ijk] > 0 && isBuy && prius[ijk] > medianPrice)
        {
            retvalue[ijk] = quantis[ijk];
        }
        else if(noIssues && seenSingle && quantis[ijk] > 0 && !isBuy && prius[ijk] > -medianPrice)
        {
            retvalue[ijk] = quantis[ijk];
        }

    }


    return retvalue;
}




int main()
{

    std::vector<std::vector<int>> history = {
        {1, -1, 0, 0, 0, 0},
        {0, 1, -1, 0, 0, 0},
        {-1, 0, 1, 0, 0, 0},
        {0, 0, 0, 2, 3, 0},
        {0, 0, 0, 0, 5, -3},
        {0, 0, 0, -2, 0, 2},
        {0, 0, 0, 0, -8, 1},
        {2, -3, 0, 0, 0, 0},
        {0, 4, -2, 0, 0, 0},
        {-2, 1, 2, 0, 0, 0}
    };
    std::vector<int> quant = {5, 5, 2, 8, 0, 12, 1000, 2, 1, 4};

    // std::vector<std::vector<int>> history = {
    //     {1, 1, 0},
    //     {2, 1, -1},
    //     {-3, -2, 1}
    // };
    // std::vector<int> quant = {5, 5, 2000};

    std::vector<int> prices = {10, -15, 10, 10, 12, 10, 13, 15, 10, 100};
    pruneQuantities(history, quant);
    std::vector<int> returned = polyArbit(history, prices, quant);
    int profited = 0;
    for(int i = 0; i< returned.size(); i++)
    {
        std::cout<<returned[i]<<"\n";
        profited += returned[i] * prices[i];
    }

    std::cout<<profited<<"\n";


    return 0;
}


#include "market.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

// create a priority queue based on price, then based on time, then based on alphabetical order
// you do this for every single std::vector<std::string> stocks and make a buy and sell tree
// if it doesnt match with the best buy, then just insert it in the sell priority queue

int findBroker(std::vector<std::pair<std::string, std::vector<int>>> brokers, std::string broker){
    for(int i = 0; i < brokers.size(); i++){
        if(brokers[i].first == broker){
            return i;
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------------------------
struct Order{
    int start_time;
    std::string broker;
    bool buy_or_sell;
    std::vector<std::string> stocks;
    int price;
    int quantity;
    int end_time;
};

bool inHeaps(std::vector<std::pair<std::vector<std::string>, std::vector<Order>>> heaps, std::vector<std::string> stocks){
    for(auto& thing : heaps){
        if(stocks == thing.first){
            return true;
        }
    }
    return false;
}

int findIndex(std::vector<std::pair<std::vector<std::string>, std::vector<Order>>> heaps, std::vector<std::string> stocks, bool flag){
    int index = 0;
    for(auto& thing : heaps){
        if(stocks == thing.first){
            if(flag == 0){
                return index;
            }
            else{
                return (index + 1);
            }
        }
        index += 1;
    }
    return -1;
}

int compareString(std::string a, std::string b){
    int a_size = a.size();
    int b_size = b.size();

    int min = a_size > b_size ? b_size : a_size;

    for(int i = 0; i < min; i++){
        if(a[i] > b[i]){
            return 1;
        }
        else if(a[i] < b[i]){
            return -1;
        }
    }

    if(a_size > b_size){
        return 1;
    }
    else if(a_size < b_size){
        return -1;
    }
    else{
        return 0;
    }
}

//------------------------------------------------------------------------------------------------------------------------
// Buy Heap functions implemented

bool buyCompare(Order a, Order b){
    if(a.price > b.price){
        return 1;
    }
    else if(a.price < b.price){
        return 0;
    }
    else{
        if(a.start_time < b.start_time){
            return 1;
        }
        else if(a.start_time > b.start_time){
            return 0;
        }
        else{
            if(compareString(a.broker, b.broker) < 0){
                return 1;
            }
            else if(compareString(a.broker, b.broker) > 0){
                return 0;
            }
        }
    }
}

void swap(std::vector<Order>& heap, int i, int j){
    Order temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void heapifyBuy(std::vector<Order>& heap, int i){
    int c;
    if(2*i+1 > heap.size()-1 && 2*i+2 > heap.size()-1){
		return;
	}
	else if(2*i + 1 > heap.size()-1){
        c = buyCompare(heap[i], heap[2*i+2]) ? i : 2*i+2;
	}
	else if(2*i + 2 > heap.size()-1){
        c = buyCompare(heap[i], heap[2*i+1]) ? i : 2*i+1;
	}
	else{
        int temp = buyCompare(heap[i], heap[2*i+1]) ? i : 2*i+1;
        c = buyCompare(heap[temp], heap[2*i+2]) ? temp : 2*i+2;
	}
    if(c == i){
        return;
    }
    swap(heap, c, i);
    heapifyBuy(heap, c);
}

void deleteMaxBuy(std::vector<Order>& heap){
    if(heap.size() > 1){
        swap(heap, 0, heap.size()-1);
    }
    heap.pop_back();
    if(heap.size() > 0){
        heapifyBuy(heap, 0);
    }
}

void heapInsertBuy(std::vector<Order>& heap, Order a){
    int i = heap.size();
    heap.push_back(a);
    while(i > 0 && buyCompare(heap[i], heap[(i-1)/2])){
        swap(heap, i, (i-1)/2);
        i = (i-1)/2;
    }
}

//------------------------------------------------------------------------------------------------------------------------
// Sell Heap functions implemented

bool sellCompare(Order a, Order b){
    if(a.price < b.price){
        return 1;
    }
    else if(a.price > b.price){
        return 0;
    }
    else{
        if(a.start_time < b.start_time){
            return 1;
        }
        else if(a.start_time > b.start_time){
            return 0;
        }
        else{
            if(compareString(a.broker, b.broker) < 0){
                return 1;
            }
            else if(compareString(a.broker, b.broker) > 0){
                return 0;
            }
        }
    }
}

void heapInsertSell(std::vector<Order>& heap, Order a){
    int i = heap.size();
    heap.push_back(a);
    while(i > 0 && sellCompare(heap[i], heap[(i-1)/2])){
        swap(heap, i, (i-1)/2);
        i = (i-1)/2;
    }
}

void heapifySell(std::vector<Order>& heap, int i){
    int c;
    if(2*i+1 > heap.size()-1 && 2*i+2 > heap.size()-1){
        return;
	}
	else if(2*i + 1 > heap.size()-1){
        c = sellCompare(heap[i], heap[2*i+2]) ? i : 2*i+2;
	}
	else if(2*i + 2 > heap.size()-1){
        c = sellCompare(heap[i], heap[2*i+1]) ? i : 2*i+1;
	}
	else{
        int temp = sellCompare(heap[i], heap[2*i+1]) ? i : 2*i+1;
        c = sellCompare(heap[temp], heap[2*i+2]) ? temp : 2*i+2;
	}
    if(c == i){
        return;
    }
    swap(heap, c, i);
    heapifySell(heap, c);
}

void deleteMaxSell(std::vector<Order>& heap){
    // std::cout << heap.size() << std::endl;
    if(heap.size() > 1){
        swap(heap, 0, heap.size()-1);
    }
    // std::cout << heap.size() << std::endl;
    heap.pop_back();
    // std::cout << heap.size() << std::endl;
    if(heap.size() > 0){
        heapifySell(heap, 0);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
market::market(int argc, char** argv)
{}

void market::start()
{
//----------------------------------------------------------------------------------------------------------------------
// Preprocess stuff and make it tangible to work with by storing it in a vector

	std::ifstream input_file("output.txt");
    std::string line;
    std::vector<Order> orders;
    while(std::getline(input_file, line) && line != "!@"){
        if(line != "TL"){
            std::istringstream space_iss(line);
            std::string space_token;
            int count = 0;
            Order order;
            std::vector<std::string> stocks;
            while(space_iss >> space_token){
                if(count == 0){
                    order.start_time = stoi(space_token);
                    count += 1;
                    continue;
                }
                else if(count == 1){
                    order.broker = space_token;
                    count += 1;
                    continue;
                }
                else if(count == 2){
                    space_token == "SELL" ? order.buy_or_sell = 0 : order.buy_or_sell = 1;
                    count += 1;
                    continue;
                }
                else if(count == 3){
                    if(space_token[0] != '$'){
                        stocks.push_back(space_token);
                    }
                    else{
                        order.stocks = stocks;
                        space_token.erase(0,1);
                        order.price = stoi(space_token);
                        count += 2;
                        continue;
                    }
                }
                else if(count == 5){
                    space_token.erase(0,1);
                    order.quantity = stoi(space_token);
                    count += 1;
                    continue;
                }
                else if(count == 6){
                    order.end_time = stoi(space_token) + order.start_time;
                }
            }
            orders.push_back(order);
        }
    }

// -----------------------------------------------------------------------------------------------------------------
// Initialise heaps for all possible linear combinations
    
    std::vector<std::pair<std::vector<std::string>, std::vector<Order>>> heaps;

    for(auto& order : orders){
        if(!inHeaps(heaps, order.stocks)){
            std::vector<Order> thing = {};
            heaps.push_back(std::make_pair(order.stocks, thing));
           
        }
    }

    heaps.resize(2*heaps.size());

    for(int i = heaps.size()/2-1; i >= 0; i--){
        heaps[2*i] = heaps[i];
    }

    // Made two heaps for buy and sell
    for(int i = 0; i < heaps.size(); i++){
        if(i%2 == 1){
            heaps[i].first = heaps[i-1].first;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    
    // RedBlackTree brokers;
    // for(auto& order : orders){
    //     if(!brokers.exists(hash(order.broker))){
    //         brokers.insert(hash(order.broker));
    //     }
    // }

    std::vector<std::pair<std::string, std::vector<int>>> brokers;

    for(auto& order : orders){
        if(!(findBroker(brokers, order.broker) + 1)){
            std::vector<int> v = {0,0,0};
            brokers.push_back(make_pair(order.broker, v));
        }
    }

//---------------------------------------------------------------------------------------------------------------------
// Go over all the orders, if no match, then add to heap. if match, then output and then subtract quantity
    int money = 0;
    int num_trades = 0;
    int num_shares = 0;

    for(auto& order : orders){
        if(order.buy_or_sell == 1){
            int index = findIndex(heaps, order.stocks, 1);
            if(heaps[index-1].second.size() == 0){
                // Implement this
                heapInsertBuy(heaps[index].second, order);
            }
            else{
                bool flag = true;
                while(flag){
                    if(heaps[index-1].second.size() == 0){
                        flag = false;
                    }
                    else if((heaps[index-1].second[0].end_time >= heaps[index-1].second[0].start_time) && (heaps[index-1].second[0].end_time < order.start_time)){
                        deleteMaxSell(heaps[index-1].second);
                    }
                    else{
                        flag = false;
                    }
                }
                flag = true;
                while(flag){
                    if(heaps[index-1].second.size() > 0){
                        if(heaps[index-1].second[0].price <= order.price){
                            if(heaps[index-1].second[0].quantity > order.quantity){
                                heaps[index-1].second[0].quantity -= order.quantity;
                                std::cout << order.broker << " purchased " << order.quantity << " share of ";
                                for(auto& item : heaps[index-1].first){
                                    std::cout << item << " ";
                                }    
                                std::cout << "from " << heaps[index-1].second[0].broker << " for $" << heaps[index-1].second[0].price << "/share" << std::endl;
                                flag = false;
                                money += order.quantity*heaps[index-1].second[0].price;
                                num_trades += 1;
                                num_shares += order.quantity;
                                brokers[findBroker(brokers, order.broker)].second[0] += order.quantity;
                                brokers[findBroker(brokers, heaps[index-1].second[0].broker)].second[1] += order.quantity;
                                brokers[findBroker(brokers, order.broker)].second[2] -= order.quantity*heaps[index-1].second[0].price;
                                brokers[findBroker(brokers, heaps[index-1].second[0].broker)].second[2] += order.quantity*heaps[index-1].second[0].price;
                            }
                            else{
                                std::cout << order.broker << " purchased " << heaps[index-1].second[0].quantity << " share of ";
                                for(auto& item : heaps[index-1].first){
                                    std::cout << item << " ";
                                }    
                                std::cout << "from " << heaps[index-1].second[0].broker << " for $" << heaps[index-1].second[0].price << "/share" << std::endl;
                                order.quantity -= heaps[index-1].second[0].quantity;
                                money += heaps[index-1].second[0].quantity*heaps[index-1].second[0].price;
                                num_trades += 1;
                                num_shares += heaps[index-1].second[0].quantity;
                                brokers[findBroker(brokers, order.broker)].second[0] += heaps[index-1].second[0].quantity;
                                brokers[findBroker(brokers, heaps[index-1].second[0].broker)].second[1] += heaps[index-1].second[0].quantity;
                                brokers[findBroker(brokers, order.broker)].second[2] -= heaps[index-1].second[0].quantity*heaps[index-1].second[0].price;
                                brokers[findBroker(brokers, heaps[index-1].second[0].broker)].second[2] += heaps[index-1].second[0].quantity*heaps[index-1].second[0].price;
                                deleteMaxSell(heaps[index-1].second);
                            }
                        }
                        else{
                            heapInsertBuy(heaps[index].second, order);
                            flag = false;
                        }
                    }
                    else{
                        heapInsertBuy(heaps[index].second, order);
                        flag = false;
                    }
                }
            }
        }
        else{
            int index = findIndex(heaps, order.stocks, 0);
            if(heaps[index+1].second.size() == 0){
                // Implement this
                heapInsertSell(heaps[index].second, order);
            }
            else{
                bool flag = true;
                while(flag){
                    if(heaps[index+1].second.size() == 0){
                        flag = false;
                    }
                    else if((heaps[index+1].second[0].end_time >= heaps[index+1].second[0].start_time) && (heaps[index+1].second[0].end_time < order.start_time)){
                        deleteMaxBuy(heaps[index+1].second);
                    }
                    else{
                        flag = false;
                    }
                }
                flag = true;
                while(flag){
                    if(heaps[index+1].second.size() > 0){
                        if(heaps[index+1].second[0].price >= order.price){
                            if(heaps[index+1].second[0].quantity > order.quantity){
                                heaps[index+1].second[0].quantity -= order.quantity;
                                std::cout << heaps[index+1].second[0].broker << " purchased " << order.quantity << " share of ";
                                for(auto& item : heaps[index+1].first){
                                    std::cout << item << " ";
                                }    
                                std::cout << "from " << order.broker << " for $" << heaps[index+1].second[0].price << "/share" << std::endl;
                                flag = false;
                                money += order.quantity*heaps[index+1].second[0].price;
                                num_trades += 1;
                                num_shares += order.quantity;
                                brokers[findBroker(brokers, order.broker)].second[1] += order.quantity;
                                brokers[findBroker(brokers, heaps[index+1].second[0].broker)].second[0] += order.quantity;
                                brokers[findBroker(brokers, order.broker)].second[2] += order.quantity*heaps[index+1].second[0].price;
                                brokers[findBroker(brokers, heaps[index+1].second[0].broker)].second[2] -= order.quantity*heaps[index+1].second[0].price;
                            }
                            else{
                                std::cout << heaps[index+1].second[0].broker << " purchased " << heaps[index+1].second[0].quantity << " share of ";
                                for(auto& item : heaps[index+1].first){
                                    std::cout << item << " ";
                                }   
                                std::cout << "from " << order.broker << " for $" << heaps[index+1].second[0].price << "/share" << std::endl;
                                order.quantity -= heaps[index+1].second[0].quantity;
                                money += heaps[index+1].second[0].quantity*heaps[index+1].second[0].price;
                                num_trades += 1;
                                num_shares += heaps[index+1].second[0].quantity;
                                brokers[findBroker(brokers, order.broker)].second[1] += heaps[index+1].second[0].quantity;
                                brokers[findBroker(brokers, heaps[index+1].second[0].broker)].second[0] += heaps[index+1].second[0].quantity;
                                brokers[findBroker(brokers, order.broker)].second[2] += heaps[index+1].second[0].quantity*heaps[index+1].second[0].price;
                                brokers[findBroker(brokers, heaps[index+1].second[0].broker)].second[2] -= heaps[index+1].second[0].quantity*heaps[index+1].second[0].price;
                                deleteMaxBuy(heaps[index+1].second);
                            }
                        }
                        else{
                            heapInsertSell(heaps[index].second, order);
                            flag = false;
                        }
                    }
                    else{
                        heapInsertSell(heaps[index].second, order);
                        flag = false;
                    }
                }
            }
        }
    }

    std::cout << std::endl;
    std::cout << "---End of Day---" << std::endl;
    std::cout << "Total Amount of Money Transferred: $" << money << std::endl;
    std::cout << "Number of Completed Trades: " << num_trades << std::endl;
    std::cout << "Number of Shares Traded: " << num_shares << std::endl;

    for(int i = 0; i < brokers.size(); i++){
        std::cout << brokers[i].first << " bought " << brokers[i].second[0] << " and sold " << brokers[i].second[1] << " for a net transfer of $" << brokers[i].second[2] << std::endl;
    }
    
}



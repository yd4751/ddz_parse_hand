// ddz.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "data_define.h"
#include "CParseCardsInfo.h"
#include "CHandCardsInfo.h"

#include <ctime>
#include <chrono>
using namespace std;
using namespace DDZDataDefine;
using namespace std::chrono;


/*
/*评分逻辑思维：



0.由于新策略引入手牌轮次参数，所以不再考虑拆分价值。

1.牌力基础价值：我们认为10属于中等位置，即<10单牌价值为负，大于10的单牌价值为正

2.控手的价值定义：我们认为一次控手权可以抵消一次手中最小牌型，最小牌型（3）的价值为-7，即我们定义一次控手权的价值为7

3.单牌的价值定义：该牌的基础价值

4.对牌的价值定义：我们认为对牌与单牌价值相等（均可以被三牌带出）故其价值为该牌的基础价值

5.三牌的价值定义：

  三不带：     该牌的基础价值

  三带一：     我们认为通常带出去的牌价值一定无正价值故其价值为该牌的基础价值

  三带二：     我们认为通常带出去的牌价值一定无正价值故其价值为该牌的基础价值

6.四牌的价值定义：

  炸弹：       我们认为炸弹会享有一次控手权利且炸弹被管的概率极小，故其无负价值，非负基础价值+7

  四带二单：   我们认为四带二单管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)

  四带二对：   我们认为四带二对管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)

7.王炸的价值定义：已知炸2价值为15-3+7=19分，故王炸价值为20分。

8.顺子的价值定义：

  单顺：       我们认为单顺的价值等于其最大牌的单体价值，且2不参与顺子，故顺子的权值依次提升1

  双顺：       我们认为双顺的价值等于其最大牌的单体价值，且2不参与顺子，故顺子的权值依次提升1

  飞机不带：   由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)

  飞机带双翅： 由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)

  飞机带单翅： 由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)

9.根据数值分布，我们暂定：   <10不叫分，10-14叫一分，15-19叫两分，20以上叫三分

*/

std::string CatsCards(const std::vector<int>& cards)
{
	std::ostringstream str("[");
	str << hex;
	for(auto it : cards)
	{
		str << it << ",";
	}
	str << "]";
	return str.str();
}

int main()
{
	CHandCardsInfo test;
	//std::vector<int> cards = {0x3,0x3,0x5,0x8,0x6,0x7,0x8,0x6,0x7,0x8,0x6,0x7,0x2,0x2,};
	std::vector<int> cards = { 0x13,0x24,0x35,0x15,0x27,0x28,0x6,0x7,0x9,0x6,0x2,0x2, };

	auto start = system_clock::now();
	test.Init(cards);
	auto duration = duration_cast<milliseconds>(system_clock::now() - start);
	cout << duration.count() << endl;
	//ChildCardInfo out = test.GetNewOut();
	std::vector<int> out = test.GetOutCards();
	//cout << "[OUT]:" << out.nMaxValue << " : " << out.nCount << "  [Adds]:" << out.vAddCards.size() << endl;
	cout << CatsCards(out) << endl;
}





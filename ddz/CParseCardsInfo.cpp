#include "CParseCardsInfo.h"
#include "CCombination.h"
using namespace DDZDataDefine;

CParseCardsInfo::CParseCardsInfo()
{
}


CParseCardsInfo::~CParseCardsInfo()
{
}

std::string CParseCardsInfo::CatCardsToHexString(const std::vector<DDZDataDefine::CardValue>& cards)
{
	std::ostringstream str;
	str << std::hex;
	str << "[";
	for (auto it : cards)
	{
		str << "0x" << it << ",";
	}
	str << "]";
	return str.str();
}


CParseCardsInfo& CParseCardsInfo::GetInstance()
{
	static CParseCardsInfo m_Parse;
	return m_Parse;
}

void CParseCardsInfo::Init(const std::vector<DDZDataDefine::CardValue>& cards)
{
	Reset();
	std::vector<CardValue>	handCards(cards);
	Sort(handCards);

	std::cout << CatCardsToHexString(handCards) << std::endl;
	//找到独立牌
	HandCardsParse coutInfo;
	ChildCardsParse childCardInfo;
	GetCardCountInfo(handCards, coutInfo);
	ParseAllIndependentCards(coutInfo, childCardInfo);
	RemoveCards(coutInfo, childCardInfo);

	//
	CardValueInfo curCardValueInfo;
	ChildCardsParse curChildCardInfo;
	HandCardsParse  tmpHandCards;
	std::multimap<CardValueInfo, int,std::greater<CardValueInfo>> allCardsValueInfo;
	
	//统计时间
	//auto start = std::chrono::system_clock::now();
	//剩下牌可以组成一手牌，直接计算总价值
	tmpHandCards = coutInfo;
	std::vector<CardValue> tmpRemove;
	curChildCardInfo = childCardInfo;
	ParseLeftCards(tmpHandCards, curChildCardInfo, tmpRemove);
	if (tmpHandCards.empty())
	{
		curCardValueInfo.CalcCardsValueInfo(curChildCardInfo);
		allCardsValueInfo.emplace(curCardValueInfo, 0);
	}
	else
	{
		std::vector<std::vector<CardValue>> combCards;
		//只需要枚举个数大于1的牌
		HandCardsParse combContInfo;
		for (auto it : coutInfo) if (it.second > 1) combContInfo[it.first] = it.second;
		CParseCardsInfo::GetInstance().GetAllCardsCombination(combContInfo, combCards);
		for (auto& itRemove : combCards)
		{
			tmpHandCards = coutInfo;
			RemoveCards(tmpHandCards, itRemove);
			curChildCardInfo = childCardInfo;
			//
			ParseLeftCards(tmpHandCards, curChildCardInfo, itRemove);

			curCardValueInfo.Reset();
			curCardValueInfo.CalcCardsValueInfo(curChildCardInfo);
			allCardsValueInfo.emplace(curCardValueInfo, 0);
		}
	}
	//统计时间
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;

	allCardsValueInfo.begin()->first.DumpResult();
}

void CParseCardsInfo::DumpResult(const std::vector<ChildCardInfo>& childCardInfo)
{
	std::cout << "Parse result:" << std::endl;
	for (auto it : childCardInfo)
	{
		std::cout << "[TYPE]:" << it.nType << " [MaxCard]:" << it.nMaxValue << " [Count]:" << it.nCount << std::endl;
	}
}
void CParseCardsInfo::Reset()
{
}
void CParseCardsInfo::Sort(std::vector<CardValue>& cards)
{
	//升序
	sort(cards.begin(), cards.end(), [](CardValue left, CardValue right) {
		return left < right;
	});
}


ChildCardInfo CParseCardsInfo::FindStright(HandCardsParse& handCards, CardValue nStart, int nMaxLength)
{
	if(handCards.empty())
		return ChildCardInfo(CARD_3, 0, CARD_TYPE_ERROR);

	int nCurCount = 1;
	auto itPre = handCards.begin();
	auto itNext = itPre;
	++itNext;
	while (itNext != handCards.end())
	{
		if (itPre->first + 1 == itNext->first)
		{
			++nCurCount;
			if (nCurCount >= nMaxLength)
			{
				return ChildCardInfo(itNext->first, nCurCount, CARD_TYPE_STRIGHT);
			}
		}
		else
		{
			if (nCurCount >= 5)
			{
				return ChildCardInfo(itPre->first, nCurCount, CARD_TYPE_STRIGHT);
			}
			else
			{
				nCurCount = 1;
			}
		}

		++itPre;
		++itNext;
	}

	//不满足最大，但是可找到顺子
	if (nCurCount >= 5)
	{
		return ChildCardInfo((--itNext)->first, nCurCount, CARD_TYPE_STRIGHT);
	}

	return ChildCardInfo(CARD_3, 0, CARD_TYPE_ERROR);
}
ChildCardsParse  CParseCardsInfo::FindAllStright(HandCardsParse& handCards, CardValue nStart, int nMaxLength)
{
	ChildCardsParse allStright;
	while (nStart <= handCards.rbegin()->first)
	{
		ChildCardInfo result = FindStright(handCards,nStart, nMaxLength);
		if (result.nCount > 0)
		{
			allStright[CARD_TYPE_STRIGHT].emplace_back(result);
		}
		++nStart;
	}

	return allStright;
};
void CParseCardsInfo::CombineStright(ChildCardsParse& strights)
{
	auto& inputStrights = strights[CARD_TYPE_STRIGHT];
	for (auto itLeft = inputStrights.begin(); itLeft != inputStrights.end(); ++itLeft)
	{
		for (auto itRight = inputStrights.begin(); itRight != inputStrights.end(); ++itRight)
		{
			if (itLeft != itRight)
			{
				//前后组合
				if (itLeft->nMaxValue == (itRight->nMaxValue - itRight->nCount))
				{
					itLeft->nMaxValue = itRight->nMaxValue;
					itLeft->nCount += itRight->nCount;
					inputStrights.erase(itRight);
					return CombineStright(strights);
				}
				//合并
				//if (*itLeft == *itRight)  单顺->对顺->三连顺
				if(itLeft->nCount == itRight->nCount && itLeft->nMaxValue == itRight->nMaxValue)
				{
					if (itLeft->nType == CARD_TYPE_STRIGHT) itLeft->nType = CARD_TYPE_STRIGHT_PAIR;
					else if (itLeft->nType == CARD_TYPE_STRIGHT_PAIR) itLeft->nType = CARD_TYPE_STRIGHT_THREE;
					inputStrights.erase(itRight);
					return CombineStright(strights);
				}
			}
		}
	}
}

void CParseCardsInfo::GetAllStright(HandCardsParse& handCards, ChildCardsParse& strights)
{
	strights.clear();
	auto& inputStrights = strights[CARD_TYPE_STRIGHT];
	while (true)
	{
		ChildCardInfo info = FindStright(handCards,CARD_3, 5);
		if (info.nCount <= 0)
			break;

		inputStrights.emplace_back(info);
		EraseCards(handCards, info);
	}
	//检查剩余牌获取最长顺子
	for (auto it = handCards.begin(); it != handCards.end();)
	{
		bool bUsed = false;
		for (auto& itStright : inputStrights)
		{
			CardValue nTarget = (CardValue)(itStright.nMaxValue + 1);
			if (nTarget == it->first)
			{
				itStright.nMaxValue = nTarget;
				++itStright.nCount;

				it->second -= 1;
				if (it->second <= 0)
				{
					bUsed = true;
					handCards.erase(it++);
					break;
				}
			}
		}

		if (!bUsed) ++it;
	}
}
void CParseCardsInfo::ParseAllIndependentCards(HandCardsParse& handCards, ChildCardsParse& chaildInfo)
{
	
	ChildCardsParse maxStrightInfo = FindAllStright(handCards,CARD_3, handCards.size());
	if (!maxStrightInfo.empty())
	{
		std::map<CardValue,int> vUsedCards;
		for (auto it : maxStrightInfo[CARD_TYPE_STRIGHT])
		{
			int nCount = 0;
			while (nCount < it.nCount)
			{
				vUsedCards[(CardValue)(it.nMaxValue - nCount)] = 0;
				++nCount;
			}
		}

		std::map<CardValue, int> cardCountInfo;
		for (auto it : handCards)
		{
			//剩余无关联牌
			if(vUsedCards.find(it.first) == vUsedCards.end())
			{
				if (cardCountInfo.find(it.first) == cardCountInfo.end()) cardCountInfo[it.first] = it.second;
				else cardCountInfo[it.first] += it.second;
			}
		}


		for (auto it : cardCountInfo)
		{
			CardType type = CARD_TYPE_ERROR;
			if (it.second == 1) type = CARD_TYPE_SINGLE;
			else if (it.second == 2) type = CARD_TYPE_PAIR;
			else if (it.second == 3) type = CARD_TYPE_THREE;
			else if (it.second == 4) type = CARD_TYPE_BOMB;
			else abort();
			chaildInfo[type].emplace_back(it.first, 1, type);
		}
	}
	else
	{
		for (auto it : handCards)
		{
			CardType type = CARD_TYPE_ERROR;
			if (it.second == 1) type = CARD_TYPE_SINGLE;
			else if (it.second == 2) type = CARD_TYPE_PAIR;
			else if (it.second == 3) type = CARD_TYPE_THREE;
			else if (it.second == 4) type = CARD_TYPE_BOMB;
			else abort();
			chaildInfo[type].emplace_back(it.first, 1, type);
		}
	}
	
}

void CParseCardsInfo::GetCardCountInfo(std::vector<CardValue>& handCards, HandCardsParse& coutInfo)
{
	coutInfo.clear();
	for (auto it : handCards)
	{
		if (coutInfo.find(it) == coutInfo.end()) coutInfo[it] = 1;
		else ++coutInfo[it];
	}
};
void CParseCardsInfo::GetAllCardsCombination(HandCardsParse& coutInfo, std::vector<std::vector<CardValue>>& combCards)
{
	combCards.clear();
	std::vector<CardValue> inputCards;
	for (auto it : coutInfo)
	{
		if (it.second <= 1) continue;
		inputCards.emplace_back(it.first);
	}

	std::vector<std::vector<int>> combInfo;
	for (auto it : coutInfo)
	{
		if (it.second <= 0) continue;

		combInfo.emplace_back();
		for (int i= 0; i <= it.second; i++)
		{
			combInfo[combInfo.size() - 1].emplace_back(i);
		}
	}

	std::vector<std::vector<int>>  result = CombinationLevelTwo::GetInstance().CombinateCN1(combInfo);
	for (auto itFirst : result)
	{
		combCards.emplace_back();
		for(unsigned int i=0; i< itFirst.size(); i++)
		{
			if (itFirst[i] > 0)
			{
				CardValue targetCard = inputCards[i];
				auto itCur = combCards.rbegin();
				itCur->insert(itCur->end(), itFirst[i], targetCard);
			}
		}
	}
};
void CParseCardsInfo::ParseLeftCards(HandCardsParse& handCards, ChildCardsParse& curChildCardInfo, std::vector<CardValue>& removeCards)
{
	if (handCards.empty()) return;
	//找到所有最小五连顺
	ChildCardsParse minFiveStrightInfo;
	GetAllStright(handCards, minFiveStrightInfo);
	//合并顺子
	CombineStright(minFiveStrightInfo);
	//
	for (auto it : minFiveStrightInfo[CARD_TYPE_STRIGHT])
		curChildCardInfo[CARD_TYPE_STRIGHT].emplace_back(it);
	//
	AddCards(handCards, removeCards);

	for (auto it : handCards)
	{
		CardType type = CARD_TYPE_SINGLE;
		if (it.second == 2) type = CARD_TYPE_PAIR;
		else if (it.second == 3) type = CARD_TYPE_THREE;
		else if (it.second == 4) type = CARD_TYPE_BOMB;
		curChildCardInfo[type].emplace_back(it.first, 1, type);
	}
};
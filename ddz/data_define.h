#ifndef __H_DDA_DATA_DEINE_H__
#define __H_DDA_DATA_DEINE_H__
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <assert.h>
#include <ctime>
#include <chrono>

#include <iostream>

namespace DDZDataDefine
{
	class ChildCardInfo;
	class CardValueInfo;
	//牌值
	// 1~12-3~A  21-2  31-RedKing  41-BlackKing
	enum CardValue
	{
		CARD_NULL = 0,
		CARD_3 = 3,
		CARD_4 = 4,
		CARD_5 = 5,
		CARD_6 = 6,
		CARD_7 = 7,
		CARD_8 = 8,
		CARD_9 = 9,
		CARD_10 = 10,
		CARD_J = 11,
		CARD_Q = 12,
		CARD_K = 13,
		CARD_A = 14,
		CARD_2 = 21,
		CARD_BLACK = 31,
		CARD_RED = 41,
		CARD_MAX = 100,
	};

	//牌型定义
	enum CardType
	{
		CARD_TYPE_ERROR = 0,					//无效牌
		CARD_TYPE_SINGLE,						//单牌	
		CARD_TYPE_PAIR,							//对子
		CARD_TYPE_THREE,						//三张
		CARD_TYPE_BOMB,							//炸弹
		CARD_TYPE_STRIGHT,						//单连顺
		CARD_TYPE_STRIGHT_PAIR,					//对连顺
		CARD_TYPE_STRIGHT_THREE,				//三张连顺
		CARD_TYPE_THREE_WITH_SINGLE,			//三张带一个
		CARD_TYPE_THREE_WITH_PAIR,				//三张带一对
		CARD_TYPE_BOMB_WITH_DOUBLE,				//炸弹带二单
		CARD_TYPE_BOMB_WITH_PAIR,				//炸弹带一对
		CARD_TYPE_STRIGHT_THREE_WITH_SINGLE,	//三张连顺带一个
		CARD_TYPE_STRIGHT_THREE_WITH_PAIR,		//三张连顺带一对
		CARD_TYPE_ROCKET,						//火箭
	};

	//子牌（不包含带牌）
	class ChildCardInfo
	{
	public:
		//如：334455      分别为：3 5 6
		DDZDataDefine::CardValue					nMaxValue;		//最大值
		int							nCount;			//数量
		std::vector<DDZDataDefine::CardValue>		vAddCards;		//附加牌
		DDZDataDefine::CardType					nType;			//牌型
		//
		ChildCardInfo() {}
		ChildCardInfo(DDZDataDefine::CardValue card, int count, DDZDataDefine::CardType type)
		{
			vAddCards.clear();
			nCount = count;
			nMaxValue = card;
			nType = type;
		}
		bool operator==(const ChildCardInfo& right)
		{
			if (this->nMaxValue != right.nMaxValue) return false;
			if (this->nCount != right.nCount) return false;
			if (this->nType != right.nType) return false;
			if (this->vAddCards != right.vAddCards) return false;
			return true;
		}
	};

	CardValue& operator++ (CardValue& value);

	//基础价值
	int GetSingleCardWeight(CardValue card);
	int GetCardsWeight(CardType type, CardValue nMaxCard,int nCount);

	//
	typedef std::map<DDZDataDefine::CardValue, int, std::less<DDZDataDefine::CardValue>>  HandCardsParse;
	typedef std::map<DDZDataDefine::CardType, std::vector<DDZDataDefine::ChildCardInfo>, std::greater<DDZDataDefine::CardType>>  ChildCardsParse;

	void EraseCards(std::vector<CardValue>& handCards, CardValue card, int count);
	void EraseCards(HandCardsParse& handCards, CardValue card, int count);
	template<typename T>
	void EraseCards(T& handCards, const ChildCardInfo& info)
	{
		switch (info.nType)
		{
		case CARD_TYPE_SINGLE:						//单牌
			return EraseCards(handCards, info.nMaxValue, 1);
		case CARD_TYPE_PAIR:						//对子
			return EraseCards(handCards, info.nMaxValue, 2);
		case CARD_TYPE_THREE:						//三张
			return EraseCards(handCards, info.nMaxValue, 3);
		case CARD_TYPE_BOMB:						//炸弹
			return EraseCards(handCards, info.nMaxValue, 4);
		case CARD_TYPE_STRIGHT_THREE:				//三张连顺
		case CARD_TYPE_THREE_WITH_SINGLE:			//三张带一个
		case CARD_TYPE_THREE_WITH_PAIR:				//三张带一对
		case CARD_TYPE_STRIGHT_THREE_WITH_SINGLE:	//三张连顺带一个
		case CARD_TYPE_STRIGHT_THREE_WITH_PAIR:		//三张连顺带一对
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 3);
			}
			for (unsigned int i = 0; i < info.vAddCards.size(); i++)
			{
				EraseCards(handCards, info.vAddCards[i], 1);
			}
			return;
		case CARD_TYPE_STRIGHT:							//单连顺
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 1);
			}
			return;
		case CARD_TYPE_STRIGHT_PAIR:					//对连顺
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 2);
			}
			return;
		case CARD_TYPE_BOMB_WITH_DOUBLE:				//炸弹带二单
		case CARD_TYPE_BOMB_WITH_PAIR:					//炸弹带一对
			EraseCards(handCards, info.nMaxValue, 4);
			for (unsigned int i = 0; i < info.vAddCards.size(); i++)
			{
				EraseCards(handCards, info.vAddCards[i], 1);
			}
			return;
		case CARD_TYPE_ROCKET:						//火箭
			EraseCards(handCards, CardValue::CARD_RED, 1);
			EraseCards(handCards, CardValue::CARD_BLACK, 1);
			return;
		}
	};
	///////////////////////////////////////////////////////////

	//手牌价值
	class CardValueInfo
	{
	public:
		int nWeight;		//权重
		int nTurn;			//手数
		ChildCardsParse  splitInfo;
		CardValueInfo()
		{
			Reset();
		}
		CardValueInfo(int weight, int turn)
		{
			nWeight = weight;
			nTurn = turn;
		}
		CardValueInfo(const CardValueInfo& right)
		{
			nWeight = right.nWeight;
			nTurn = right.nTurn;
			splitInfo = right.splitInfo;
		}
		bool operator<(const CardValueInfo& right) const 
		{
			//手数大的权重更小
			if (nTurn != right.nTurn) return nTurn > right.nTurn;
			if (nWeight != right.nWeight) return nWeight < right.nWeight;
			return false;
		}
		bool operator>(const CardValueInfo& right) const
		{
			//手数大的权重更小
			if (nTurn != right.nTurn) return nTurn < right.nTurn;
			if (nWeight != right.nWeight) return nWeight > right.nWeight;
			return false;
		}
		bool operator()(const CardValueInfo& left,const CardValueInfo& right)
		{
			return right < left;
		}
		void Reset()
		{
			nWeight = 0;
			nTurn = 0;
			splitInfo.clear();
		}
		void CalcCardsValueInfo(const ChildCardsParse& info)
		{
			ArrangeSplitCardsInfo(info);
			for (auto& it : splitInfo)
			{
				nTurn += it.second.size();
				for (auto& itSecond : it.second)
				{
					nWeight += GetCardsWeight(it.first, itSecond.nMaxValue, itSecond.nCount);
				}
			}
		}
		void CalcCardsValueInfo(const std::vector<ChildCardInfo>& info)
		{
			Reset();
			//ArrangeSplitCardsInfo(info);
			for (auto& it : splitInfo)
			{
				nTurn += it.second.size();
				for (auto& itSecond : it.second)
				{
					nWeight += GetCardsWeight(it.first, itSecond.nMaxValue, itSecond.nCount);
				}
			}
		};
		//整理拆牌信息
		/*
			三张 ->
				三连顺
				三带1
			对子 -> 对顺
			四张 -> 
				四带1
				四带2
		*/
		void SplitCardsInfo(ChildCardsParse& info, CardType nTargetType,CardType nType, size_t nMinCount)
		{
			if (info.find(nType) == info.end()) return;
			if (info[nType].size() < nMinCount) return;

			std::sort(splitInfo[nType].begin(), splitInfo[nType].end(), [](const ChildCardInfo& left, const ChildCardInfo& right) {
				if (left.nCount != right.nCount) return left.nCount < right.nCount;
				if (left.nMaxValue != right.nMaxValue) return left.nMaxValue < right.nMaxValue;
				return false;
			});

			size_t nCurCount = 1;
			CardValue nLastCard = CARD_NULL;
			auto itCur = info[nType].begin();
			auto itStart = itCur;
			while (itCur != info[nType].end())
			{
				if (nLastCard != CARD_NULL && (nLastCard + 1 == itCur->nMaxValue))
				{
					++nCurCount;
				}
				else
				{
					if (nCurCount >= nMinCount)
					{
						info[nType].erase(itStart, itCur);
						info[nTargetType].emplace_back(nLastCard, nCurCount, nTargetType);
						if (info[nType].empty()) info.erase(nType);
						return SplitCardsInfo(info, nTargetType, nType, nMinCount);
					}
					itStart = itCur;
					nCurCount = 1;
				}
				nLastCard = itCur->nMaxValue;
				++itCur;
			}

			if (nCurCount >= nMinCount)
			{
				info[nType].erase(itStart, itCur);
				info[nTargetType].emplace_back(nLastCard, nCurCount, nTargetType);
				if (info[nType].empty()) info.erase(nType);
				return SplitCardsInfo(info, nTargetType, nType, nMinCount);
			}
		}
		void ArrangeSplitCardsInfo(const ChildCardsParse& info)
		{
			splitInfo.clear();
			splitInfo = info;

			//对子 -> 对顺
			SplitCardsInfo(splitInfo,CARD_TYPE_STRIGHT_PAIR,CARD_TYPE_PAIR,3);

			//三张 -> 三连顺
			SplitCardsInfo(splitInfo, CARD_TYPE_STRIGHT_THREE, CARD_TYPE_THREE, 2);
		}
		void DumpResult() const 
		{
			for (auto it : splitInfo)
			{
				std::cout << "[TYPE]:" << it.first<<"  [CardsInfo, CARD : COUNT ]:";
				for (auto itSecond : it.second)
				{
					std::cout << "[" << itSecond.nMaxValue << ":" << itSecond.nCount << "]";
				}
				std::cout << std::endl;
			}
		}
	};
}
#endif

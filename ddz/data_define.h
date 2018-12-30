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
	//��ֵ
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

	//���Ͷ���
	enum CardType
	{
		CARD_TYPE_ERROR = 0,					//��Ч��
		CARD_TYPE_SINGLE,						//����	
		CARD_TYPE_PAIR,							//����
		CARD_TYPE_THREE,						//����
		CARD_TYPE_BOMB,							//ը��
		CARD_TYPE_STRIGHT,						//����˳
		CARD_TYPE_STRIGHT_PAIR,					//����˳
		CARD_TYPE_STRIGHT_THREE,				//������˳
		CARD_TYPE_THREE_WITH_SINGLE,			//���Ŵ�һ��
		CARD_TYPE_THREE_WITH_PAIR,				//���Ŵ�һ��
		CARD_TYPE_BOMB_WITH_DOUBLE,				//ը��������
		CARD_TYPE_BOMB_WITH_PAIR,				//ը����һ��
		CARD_TYPE_STRIGHT_THREE_WITH_SINGLE,	//������˳��һ��
		CARD_TYPE_STRIGHT_THREE_WITH_PAIR,		//������˳��һ��
		CARD_TYPE_ROCKET,						//���
	};

	//���ƣ����������ƣ�
	class ChildCardInfo
	{
	public:
		//�磺334455      �ֱ�Ϊ��3 5 6
		DDZDataDefine::CardValue					nMaxValue;		//���ֵ
		int							nCount;			//����
		std::vector<DDZDataDefine::CardValue>		vAddCards;		//������
		DDZDataDefine::CardType					nType;			//����
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

	//������ֵ
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
		case CARD_TYPE_SINGLE:						//����
			return EraseCards(handCards, info.nMaxValue, 1);
		case CARD_TYPE_PAIR:						//����
			return EraseCards(handCards, info.nMaxValue, 2);
		case CARD_TYPE_THREE:						//����
			return EraseCards(handCards, info.nMaxValue, 3);
		case CARD_TYPE_BOMB:						//ը��
			return EraseCards(handCards, info.nMaxValue, 4);
		case CARD_TYPE_STRIGHT_THREE:				//������˳
		case CARD_TYPE_THREE_WITH_SINGLE:			//���Ŵ�һ��
		case CARD_TYPE_THREE_WITH_PAIR:				//���Ŵ�һ��
		case CARD_TYPE_STRIGHT_THREE_WITH_SINGLE:	//������˳��һ��
		case CARD_TYPE_STRIGHT_THREE_WITH_PAIR:		//������˳��һ��
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 3);
			}
			for (unsigned int i = 0; i < info.vAddCards.size(); i++)
			{
				EraseCards(handCards, info.vAddCards[i], 1);
			}
			return;
		case CARD_TYPE_STRIGHT:							//����˳
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 1);
			}
			return;
		case CARD_TYPE_STRIGHT_PAIR:					//����˳
			for (int i = 0; i < info.nCount; i++)
			{
				EraseCards(handCards, (CardValue)(info.nMaxValue - i), 2);
			}
			return;
		case CARD_TYPE_BOMB_WITH_DOUBLE:				//ը��������
		case CARD_TYPE_BOMB_WITH_PAIR:					//ը����һ��
			EraseCards(handCards, info.nMaxValue, 4);
			for (unsigned int i = 0; i < info.vAddCards.size(); i++)
			{
				EraseCards(handCards, info.vAddCards[i], 1);
			}
			return;
		case CARD_TYPE_ROCKET:						//���
			EraseCards(handCards, CardValue::CARD_RED, 1);
			EraseCards(handCards, CardValue::CARD_BLACK, 1);
			return;
		}
	};
	///////////////////////////////////////////////////////////

	//���Ƽ�ֵ
	class CardValueInfo
	{
	public:
		int nWeight;		//Ȩ��
		int nTurn;			//����
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
			//�������Ȩ�ظ�С
			if (nTurn != right.nTurn) return nTurn > right.nTurn;
			if (nWeight != right.nWeight) return nWeight < right.nWeight;
			return false;
		}
		bool operator>(const CardValueInfo& right) const
		{
			//�������Ȩ�ظ�С
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
		//���������Ϣ
		/*
			���� ->
				����˳
				����1
			���� -> ��˳
			���� -> 
				�Ĵ�1
				�Ĵ�2
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

			//���� -> ��˳
			SplitCardsInfo(splitInfo,CARD_TYPE_STRIGHT_PAIR,CARD_TYPE_PAIR,3);

			//���� -> ����˳
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

#ifndef __H_PARSECARDINFO_H__
#define __H_PARSECARDINFO_H__
#include "data_define.h"


class CParseCardsInfo
{
public:
	CParseCardsInfo();
	virtual ~CParseCardsInfo();

public:
	static CParseCardsInfo& GetInstance();
	void Init(const std::vector<DDZDataDefine::CardValue>& cards, DDZDataDefine::CardValueInfo&);

protected:
	void DumpResult(const std::vector<DDZDataDefine::ChildCardInfo>& childCardInfo);
	std::string CatCardsToHexString(const std::vector<DDZDataDefine::CardValue>& cards);

protected:
	void Reset();
	void Sort(std::vector<DDZDataDefine::CardValue>& cards);
	template<typename T>
	void RemoveCards(T& handCards, std::vector<DDZDataDefine::ChildCardInfo>& childCardInfo)
	{
		for (auto it : childCardInfo)
		{
			DDZDataDefine::EraseCards(handCards, it);
		}
	};
	template<typename T>
	void RemoveCards(T& handCards, DDZDataDefine::ChildCardsParse& childCardInfo)
	{
		for (auto it : childCardInfo)
		{
			//DDZDataDefine::EraseCards(handCards, it.second);
			RemoveCards(handCards,it.second);
		}
	};
	template<typename T>
	void RemoveCards(T& handCards, DDZDataDefine::ChildCardInfo& childCardInfo)
	{
		DDZDataDefine::EraseCards(handCards, childCardInfo);
	};
	template<typename T>
	void RemoveCards(T& handCards, std::vector<DDZDataDefine::CardValue>& cards)
	{
		for(auto it : cards)
			DDZDataDefine::EraseCards(handCards,it,1);
	};
	void AddCards(DDZDataDefine::HandCardsParse& handCards, std::vector<DDZDataDefine::CardValue>& cards)
	{
		for (auto it : cards)
		{
			if (handCards.find(it) == handCards.end()) handCards[it] = 1;
			else ++handCards[it];
		}
	};

protected:
	void GetCardCountInfo(std::vector<DDZDataDefine::CardValue>& handCards, DDZDataDefine::HandCardsParse& coutInfo);
	void GetAllCardsCombination(DDZDataDefine::HandCardsParse& coutInfo, std::vector<std::vector<DDZDataDefine::CardValue>>& combCards);

protected:
	DDZDataDefine::ChildCardInfo FindStright(DDZDataDefine::HandCardsParse& handCards, DDZDataDefine::CardValue nStart, int nMaxLength);
	DDZDataDefine::ChildCardsParse  FindAllStright(DDZDataDefine::HandCardsParse& handCards, DDZDataDefine::CardValue nStart, int nMaxLength);
	void GetAllStright(DDZDataDefine::HandCardsParse& handCards, DDZDataDefine::ChildCardsParse& strights);
	void CombineStright(DDZDataDefine::ChildCardsParse& strights);

protected:
	void ParseAllIndependentCards(DDZDataDefine::HandCardsParse& handCards, DDZDataDefine::ChildCardsParse& chaildInfo);
	void ParseLeftCards(DDZDataDefine::HandCardsParse& handCards, DDZDataDefine::ChildCardsParse& curChildCardInfo, std::vector<DDZDataDefine::CardValue>& removeCards);
};

#endif
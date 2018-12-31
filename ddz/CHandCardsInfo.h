#ifndef __H_HANDCARDSINFO_H__
#define __H_HANDCARDSINFO_H__
#include "data_define.h"

class CHandCardsInfo
{
	std::vector<int>																	m_SrcCards;
	std::vector<DDZDataDefine::CardValue>												m_HandCards;
	DDZDataDefine::CardValueInfo														m_ValueInfo;

public:
	CHandCardsInfo();
	~CHandCardsInfo();

protected:
	void Sort(std::vector<DDZDataDefine::ChildCardInfo>& info)
	{
		std::sort(info.begin(), info.end(), [](DDZDataDefine::ChildCardInfo& left, DDZDataDefine::ChildCardInfo& right) {
			if (left.nMaxValue != right.nMaxValue) return left.nMaxValue < right.nMaxValue;
			if (left.nCount != right.nCount) return left.nCount < right.nCount;
			return false;
		});
	}

protected:
	DDZDataDefine::ChildCardInfo GetNewOut();
	int ChangeCardValue(DDZDataDefine::CardValue card);

public:
	DDZDataDefine::CardValue ChangeValue(int card);
	void Reset();
	void Init(std::vector<int> cards);


	std::vector<int> GetOutCards();
};

#endif
#ifndef __H_HANDCARDSINFO_H__
#define __H_HANDCARDSINFO_H__
#include "data_define.h"

class CHandCardsInfo
{
	std::vector<DDZDataDefine::CardValue>												m_HandCards;
	DDZDataDefine::CardValueInfo														m_ValueInfo;
	std::map<DDZDataDefine::CardType, std::vector<DDZDataDefine::ChildCardInfo>>		m_ChildCardInfo;

public:
	CHandCardsInfo();
	~CHandCardsInfo();

public:
	DDZDataDefine::CardValue ChangeValue(int card);
	void Reset();
	void Init(std::vector<int> cards);
};

#endif
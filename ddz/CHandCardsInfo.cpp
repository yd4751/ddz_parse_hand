#include "CHandCardsInfo.h"
#include "CParseCardsInfo.h"
using namespace DDZDataDefine;

CHandCardsInfo::CHandCardsInfo()
{
}


CHandCardsInfo::~CHandCardsInfo()
{
}



CardValue CHandCardsInfo::ChangeValue(int card)
{
	switch (card & 0x0F)
	{
	case 1: return CARD_A;
	case 2: return CARD_2;
	case 3: return CARD_3;
	case 4: return CARD_4;
	case 5: return CARD_5;
	case 6: return CARD_6;
	case 7: return CARD_7;
	case 8: return CARD_8;
	case 9: return CARD_9;
	case 0xA: return CARD_10;
	case 0xB: return CARD_J;
	case 0xC: return CARD_Q;
	case 0xD: return CARD_K;
	case 0xE: return CARD_BLACK;
	case 0xF: return CARD_RED;
	}
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
	return CARD_NULL;
};

void CHandCardsInfo::Reset()
{
	m_HandCards.clear();
	m_ChildCardInfo.clear();
	m_ValueInfo.Reset();
};
void CHandCardsInfo::Init(std::vector<int> cards)
{
	Reset();
	for (auto it : cards)
	{
		m_HandCards.emplace_back(ChangeValue(it));
	}

	CParseCardsInfo::GetInstance().Init(m_HandCards);
};
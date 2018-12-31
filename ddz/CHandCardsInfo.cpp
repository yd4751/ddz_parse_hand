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
	case 1: return DDZ_CARD_A;
	case 2: return DDZ_CARD_2;
	case 3: return DDZ_CARD_3;
	case 4: return DDZ_CARD_4;
	case 5: return DDZ_CARD_5;
	case 6: return DDZ_CARD_6;
	case 7: return DDZ_CARD_7;
	case 8: return DDZ_CARD_8;
	case 9: return DDZ_CARD_9;
	case 0xA: return DDZ_CARD_10;
	case 0xB: return DDZ_CARD_J;
	case 0xC: return DDZ_CARD_Q;
	case 0xD: return DDZ_CARD_K;
	case 0xE: return DDZ_CARD_BLACK;
	case 0xF: return DDZ_CARD_RED;
	}
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
	return DDZ_CARD_NULL;
};

void CHandCardsInfo::Reset()
{
	m_SrcCards.clear();
	m_HandCards.clear();
	m_ValueInfo.Reset();
};
#define CARD_VALUE(x)   	(x&0x0F)
#define CARD_COLOR(x)		(x&0xF0)
void CHandCardsInfo::Init(std::vector<int> cards)
{
	Reset();
	m_SrcCards = cards;
	std::sort(m_SrcCards.begin(), m_SrcCards.end(), [](int left, int right) {
		if (CARD_VALUE(left) != CARD_VALUE(right))	return CARD_VALUE(left) < CARD_VALUE(right);
		if (CARD_COLOR(left) != CARD_COLOR(right))	return CARD_COLOR(left) < CARD_COLOR(right);
		return false;
	});
	for (auto it : cards)
	{
		m_HandCards.emplace_back(ChangeValue(it));
	}

	CParseCardsInfo::GetInstance().Init(m_HandCards, m_ValueInfo);
};
int CHandCardsInfo::ChangeCardValue(CardValue card)
{
	switch (card)
	{
	case DDZ_CARD_3:
		return 0x03;
	case DDZ_CARD_4:
		return 0x04;
	case DDZ_CARD_5:
		return 0x05;
	case DDZ_CARD_6:
		return 0x06;
	case DDZ_CARD_7:
		return 0x07;
	case DDZ_CARD_8:
		return 0x08;
	case DDZ_CARD_9:
		return 0x09;
	case DDZ_CARD_10:
		return 0x0A;
	case DDZ_CARD_J:
		return 0x0B;
	case DDZ_CARD_Q:
		return 0x0C;
	case DDZ_CARD_K:
		return 0x0D;
	case DDZ_CARD_A:
		return 0x01;
	case DDZ_CARD_2:
		return 0x02;
	case DDZ_CARD_BLACK:
		return 0x0E;
	case DDZ_CARD_RED:
		return 0x0F;
	}
};
std::vector<int> CHandCardsInfo::GetOutCards()
{
	std::vector<int> outCards;
	ChildCardInfo cardsInfo = GetNewOut();
	for (int i = 0; i < cardsInfo.nCount; i++)
	{
		switch (cardsInfo.nType)
		{
		case CARD_TYPE_SINGLE:						//����	
		case CARD_TYPE_STRIGHT:						//����˳
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			break;
		case CARD_TYPE_PAIR:							//����
		case CARD_TYPE_STRIGHT_PAIR:					//����˳
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			break;
		case CARD_TYPE_THREE:						//����
		case CARD_TYPE_STRIGHT_THREE:				//������˳
		case CARD_TYPE_THREE_WITH_SINGLE:			//���Ŵ�һ��
		case CARD_TYPE_THREE_WITH_PAIR:				//���Ŵ�һ��
		case CARD_TYPE_STRIGHT_THREE_WITH_SINGLE:	//������˳��һ��
		case CARD_TYPE_STRIGHT_THREE_WITH_PAIR:		//������˳��һ��
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			break;
		case CARD_TYPE_BOMB:							//ը��
		case CARD_TYPE_BOMB_WITH_DOUBLE:				//ը��������
		case CARD_TYPE_BOMB_WITH_PAIR:					//ը����һ��
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			outCards.emplace_back(ChangeCardValue((CardValue)(cardsInfo.nMaxValue - i)));
			break;
		case CARD_TYPE_ROCKET:						//���
			outCards.emplace_back(ChangeCardValue(DDZ_CARD_RED));
			outCards.emplace_back(ChangeCardValue(DDZ_CARD_BLACK));
			break;
		}
	}

	for (auto it : cardsInfo.vAddCards)
	{
		outCards.emplace_back(ChangeCardValue(it));
	}

	std::sort(outCards.begin(), outCards.end(), [](int left, int right) {
		return left < right;
	});

	auto itLast = outCards.begin();
	for (auto itHand = m_SrcCards.begin(); itHand != m_SrcCards.end(); )
	{
		for (auto it = itLast; it != outCards.end();)
		{
			if (CARD_VALUE(*itHand) == *it)
			{
				*it = *itHand;

				++it;
			}
			++itHand;
			if (itHand == m_SrcCards.end()) break;
		}
	}
	return outCards;
};
ChildCardInfo CHandCardsInfo::GetNewOut()
{
	ChildCardInfo outCardsInfo;
	if (m_ValueInfo.splitInfo.empty())
		return outCardsInfo;

	ChildCardsParse::iterator itResult;
	//����˳
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_STRIGHT_THREE);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		auto itSingles = m_ValueInfo.splitInfo.find(CARD_TYPE_SINGLE);
		if (itSingles->second.size() > itCardsInfo.size())
		{
			outCardsInfo = *itCardsInfo.begin();
			for (int i = 0; i < outCardsInfo.nCount; i++) outCardsInfo.vAddCards.emplace_back(itSingles->second[i].nMaxValue);
			return outCardsInfo;
		}
		auto itPairs = m_ValueInfo.splitInfo.find(CARD_TYPE_PAIR);
		if (itPairs->second.size() > itCardsInfo.size())
		{
			outCardsInfo = *itCardsInfo.begin();
			for (int i = 0; i < outCardsInfo.nCount; i++)
			{
				outCardsInfo.vAddCards.emplace_back(itPairs->second[i].nMaxValue);
				outCardsInfo.vAddCards.emplace_back(itPairs->second[i].nMaxValue);
			}
			return outCardsInfo;
		}
		//return *itCardsInfo.begin();
	}
	//��˳
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_STRIGHT_PAIR);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		return *itCardsInfo.begin();
	}
	//��˳
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_STRIGHT);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		return *itCardsInfo.begin();
	}
	//����
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_THREE);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		auto itSingles = m_ValueInfo.splitInfo.find(CARD_TYPE_SINGLE);
		if (itSingles->second.size() > itCardsInfo.size())
		{
			outCardsInfo = *itCardsInfo.begin();
			for (int i = 0; i < outCardsInfo.nCount; i++) outCardsInfo.vAddCards.emplace_back(itSingles->second[i].nMaxValue);
			return outCardsInfo;
		}
		auto itPairs = m_ValueInfo.splitInfo.find(CARD_TYPE_PAIR);
		if (itPairs->second.size() > itCardsInfo.size())
		{
			outCardsInfo = *itCardsInfo.begin();
			for (int i = 0; i < outCardsInfo.nCount; i++)
			{
				outCardsInfo.vAddCards.emplace_back(itPairs->second[i].nMaxValue);
				outCardsInfo.vAddCards.emplace_back(itPairs->second[i].nMaxValue);
			}
			return outCardsInfo;
		}
		return *itCardsInfo.begin();
	}
	//����
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_PAIR);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		return *itCardsInfo.begin();
	}
	//����
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_SINGLE);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
		return *itCardsInfo.begin();
	}
	/*//ը��
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_BOMB);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto& itCardsInfo = itResult->second;
		Sort(itCardsInfo);
	}
	//��ը
	itResult = m_ValueInfo.splitInfo.find(CARD_TYPE_ROCKET);
	if (itResult != m_ValueInfo.splitInfo.end())
	{
		auto itCardsInfo = itResult->second;
	}*/
	return outCardsInfo;
};
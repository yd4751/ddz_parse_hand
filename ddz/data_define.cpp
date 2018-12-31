#include "data_define.h"
#include <iostream>
using namespace std;

namespace DDZDataDefine
{
	CardValue& operator++ (CardValue& value)
	{
		switch (value)
		{
		case DDZ_CARD_NULL: return value = DDZ_CARD_3;
		case DDZ_CARD_3: return value = DDZ_CARD_4;
		case DDZ_CARD_4: return value = DDZ_CARD_5;
		case DDZ_CARD_5: return value = DDZ_CARD_6;
		case DDZ_CARD_6: return value = DDZ_CARD_7;
		case DDZ_CARD_7: return value = DDZ_CARD_8;
		case DDZ_CARD_8: return value = DDZ_CARD_9;
		case DDZ_CARD_9: return value = DDZ_CARD_10;
		case DDZ_CARD_10: return value = DDZ_CARD_J;
		case DDZ_CARD_J: return value = DDZ_CARD_Q;
		case DDZ_CARD_Q: return value = DDZ_CARD_K;
		case DDZ_CARD_K: return value = DDZ_CARD_A;
		case DDZ_CARD_A: return value = DDZ_CARD_2;
		case DDZ_CARD_2: return value = DDZ_CARD_BLACK;
		case DDZ_CARD_BLACK: return value = DDZ_CARD_RED;
		case DDZ_CARD_RED: return value = DDZ_CARD_MAX;
		case DDZ_CARD_MAX: return value = DDZ_CARD_MAX;
		}
		cout << __FUNCTION__ << ":" << __LINE__ << endl;
		return value = DDZ_CARD_NULL;
	}

	//���ƻ�����ֵ
	int GetSingleCardWeight(CardValue card)
	{
		switch (card)
		{
		case DDZ_CARD_3:		return -7;
		case DDZ_CARD_4:		return -6;
		case DDZ_CARD_5:		return -5;
		case DDZ_CARD_6:		return -4;
		case DDZ_CARD_7:		return -3;
		case DDZ_CARD_8:		return -2;
		case DDZ_CARD_9:		return -1;
		case DDZ_CARD_10:		return 0;
		case DDZ_CARD_J:		return 1;
		case DDZ_CARD_Q:		return 2;
		case DDZ_CARD_K:		return 3;
		case DDZ_CARD_A:		return 4;
		case DDZ_CARD_2:		return 5;
		case DDZ_CARD_BLACK:	return 6;
		case DDZ_CARD_RED:		return 7;
		}
		cout << __FUNCTION__ << ":" << __LINE__ << endl;
		return 0;
	};

	int GetCardsWeight(CardType type, CardValue nMaxCard, int nCount)
	{
		switch (type)
		{
		case CARD_TYPE_SINGLE:						//����
		case CARD_TYPE_PAIR:							//����
		case CARD_TYPE_THREE:						//����
		case CARD_TYPE_STRIGHT_THREE:				//������˳
		case CARD_TYPE_THREE_WITH_SINGLE:			//���Ŵ�һ��
		case CARD_TYPE_THREE_WITH_PAIR:				//���Ŵ�һ��
			return GetSingleCardWeight(nMaxCard);
		case CARD_TYPE_BOMB:							//ը��
			return GetSingleCardWeight(nMaxCard) > 0 ? GetSingleCardWeight(nMaxCard) + 7 : 7;
		case CARD_TYPE_BOMB_WITH_DOUBLE:				//ը��������
		case CARD_TYPE_BOMB_WITH_PAIR:				//ը����һ��
			return GetSingleCardWeight(nMaxCard) > 0 ? (GetSingleCardWeight(nMaxCard) + 7) / 2 : 7 / 2;
		case CARD_TYPE_STRIGHT:						//����˳
		case CARD_TYPE_STRIGHT_PAIR:					//����˳
			return GetSingleCardWeight(nMaxCard) + 1;
		case CARD_TYPE_STRIGHT_THREE_WITH_SINGLE:	//������˳��һ��
		case CARD_TYPE_STRIGHT_THREE_WITH_PAIR:		//������˳��һ��
			return GetSingleCardWeight(nMaxCard) > 0 ? (GetSingleCardWeight(nMaxCard) + 7) / 2 : 7 / 2;
		case CARD_TYPE_ROCKET:						//���
			return 20;
		default:
			return -7;
		}
	};

	void EraseCards(std::vector<CardValue>& handCards, CardValue card, int count)
	{
		for (auto it = handCards.begin(); it != handCards.end(); )
		{
			if (count <= 0) break;

			if (*it == card)
			{
				it = handCards.erase(it);
				--count;
			}
			else
				++it;
		}
	}

	void EraseCards(HandCardsParse& handCards, CardValue card, int count)
	{
		auto it = handCards.find(card);
		if (it != handCards.end())
		{
			if (it->second < count) abort();
			it->second -= count;
			if (it->second == 0) handCards.erase(it);
		}
	};
}
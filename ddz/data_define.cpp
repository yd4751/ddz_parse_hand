#include "data_define.h"
#include <iostream>
using namespace std;

namespace DDZDataDefine
{
	CardValue& operator++ (CardValue& value)
	{
		switch (value)
		{
		case CARD_NULL: return value = CARD_3;
		case CARD_3: return value = CARD_4;
		case CARD_4: return value = CARD_5;
		case CARD_5: return value = CARD_6;
		case CARD_6: return value = CARD_7;
		case CARD_7: return value = CARD_8;
		case CARD_8: return value = CARD_9;
		case CARD_9: return value = CARD_10;
		case CARD_10: return value = CARD_J;
		case CARD_J: return value = CARD_Q;
		case CARD_Q: return value = CARD_K;
		case CARD_K: return value = CARD_A;
		case CARD_A: return value = CARD_2;
		case CARD_2: return value = CARD_BLACK;
		case CARD_BLACK: return value = CARD_RED;
		case CARD_RED: return value = CARD_MAX;
		case CARD_MAX: return value = CARD_MAX;
		}
		cout << __FUNCTION__ << ":" << __LINE__ << endl;
		return value = CARD_NULL;
	}

	//单牌基础价值
	int GetSingleCardWeight(CardValue card)
	{
		switch (card)
		{
		case CARD_3:		return -7;
		case CARD_4:		return -6;
		case CARD_5:		return -5;
		case CARD_6:		return -4;
		case CARD_7:		return -3;
		case CARD_8:		return -2;
		case CARD_9:		return -1;
		case CARD_10:		return 0;
		case CARD_J:		return 1;
		case CARD_Q:		return 2;
		case CARD_K:		return 3;
		case CARD_A:		return 4;
		case CARD_2:		return 5;
		case CARD_BLACK:	return 6;
		case CARD_RED:		return 7;
		}
		cout << __FUNCTION__ << ":" << __LINE__ << endl;
		return 0;
	};

	int GetCardsWeight(CardType type, CardValue nMaxCard, int nCount)
	{
		switch (type)
		{
		case CARD_TYPE_SINGLE:						//单牌
		case CARD_TYPE_PAIR:							//对子
		case CARD_TYPE_THREE:						//三张
		case CARD_TYPE_STRIGHT_THREE:				//三张连顺
		case CARD_TYPE_THREE_WITH_SINGLE:			//三张带一个
		case CARD_TYPE_THREE_WITH_PAIR:				//三张带一对
			return GetSingleCardWeight(nMaxCard);
		case CARD_TYPE_BOMB:							//炸弹
			return GetSingleCardWeight(nMaxCard) > 0 ? GetSingleCardWeight(nMaxCard) + 7 : 7;
		case CARD_TYPE_BOMB_WITH_DOUBLE:				//炸弹带二单
		case CARD_TYPE_BOMB_WITH_PAIR:				//炸弹带一对
			return GetSingleCardWeight(nMaxCard) > 0 ? (GetSingleCardWeight(nMaxCard) + 7) / 2 : 7 / 2;
		case CARD_TYPE_STRIGHT:						//单连顺
		case CARD_TYPE_STRIGHT_PAIR:					//对连顺
			return GetSingleCardWeight(nMaxCard) + 1;
		case CARD_TYPE_STRIGHT_THREE_WITH_SINGLE:	//三张连顺带一个
		case CARD_TYPE_STRIGHT_THREE_WITH_PAIR:		//三张连顺带一对
			return GetSingleCardWeight(nMaxCard) > 0 ? (GetSingleCardWeight(nMaxCard) + 7) / 2 : 7 / 2;
		case CARD_TYPE_ROCKET:						//火箭
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
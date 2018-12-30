#ifndef __H_COMBINSTION_H__
#define __H_COMBINSTION_H__
#include <vector>
#include <map>

//一级组合
class CCombination
{
public:
	CCombination() {};
	virtual ~CCombination() {};

public:
	static CCombination& GetInstance()
	{
		static CCombination m_Combination;
		return m_Combination;
	}

protected:
	template<typename T>
	void CombinateCNN(const std::vector<T>& input,int nMaxDeep, int nCurDeep ,int nStart,std::vector<T>& vSubResult , std::vector < std::vector<T>>& vResult)
	{
		if (nCurDeep == nMaxDeep)
		{
			vResult.emplace_back(vSubResult);
			return;
		}

		//0 - (3)
		int nEnd = input.size() - nMaxDeep + nCurDeep + 1;

		for (unsigned int i = nStart; i < nEnd; i++)
		{
			vSubResult.emplace_back(input[i]);
			CombinateCNN(input,nMaxDeep,nCurDeep+1, i+1,vSubResult, vResult);
			if (!vSubResult.empty()) vSubResult.pop_back();
		}
	}

public:
	//C(N)(1)
	template<typename T>
	std::vector<T> CombinateCN1(const std::vector<T>& input)
	{
		std::vector<T> result;
		for (auto it : input)
		{
			result.emplace_back(it);
		}
		return result;
	};
	//C(N)(N)
	template<typename T>
	std::vector < std::vector<T>> CombinateCNN(const std::vector<T>& input, int nDeep)
	{
		std::vector < std::vector<T>> result;
		std::vector<T> subResult;
		CombinateCNN(input, nDeep, 0 ,0 ,subResult, result);
		return result;
	};
};

//二级组合
class CombinationLevelTwo
{
public:
	static CombinationLevelTwo& GetInstance()
	{
		static CombinationLevelTwo m_Combination;
		return m_Combination;
	}

protected:
	template<typename T>
	void CombinateCN1(const std::vector < std::vector<T>>& input,int nCurIndex, std::vector<T>& subResult, std::vector<std::vector<T>>& result)
	{
		if (subResult.size() == input.size())
		{
			result.emplace_back(subResult);
			return;
		}

		unsigned int nEnd = input[nCurIndex].size();
		for (unsigned int i = 0; i < nEnd; i++)
		{
			subResult.emplace_back(input[nCurIndex][i]);
			CombinateCN1(input,nCurIndex+1,subResult,result);
			if (!subResult.empty()) subResult.pop_back();
		}
	}

public:
	//C(N)(1)
	template<typename T>
	std::vector<std::vector<T>> CombinateCN1(const std::vector < std::vector<T>>& input)
	{
		std::vector<std::vector<T>> result;
		std::vector<T> subResult;
		CombinateCN1(input,0, subResult, result);
		return result;
	};
	/*//C(N)(N)
	template<typename T>
	std::vector < std::vector<T>> CombinateCNN(const std::vector<T>& input, int nDeep)
	{
		std::vector < std::vector<T>> result;
		std::vector<T> subResult;
		CombinateCNN(input, nDeep, 0, 0, subResult, result);
		return result;
	};*/
};
#endif
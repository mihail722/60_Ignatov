#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>
#include "..\60_Ignatov\60_Ignatov.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<>
            inline std::wstring ToString<errorType>(const errorType& t)
            {
                switch (t)
                {
                case MISSING_OPERAND:
                    return L"MISSING_OPERAND";
                case MISSING_OPERATION:
                    return L"MISSING_OPERATION";
                case INVALID_SYMBOL:
                    return L"INVALID_SYMBOL";
                case TOO_MANY_OPERATION:
                    return L"TOO_MANY_OPERATION";
                default:
                    return L"NO_ERROR";
                }
            }
        }
    }
}

namespace My60Ignatovtest
{
    bool compareTrees(exprNode* a, exprNode* b)
    {
        if (!a && !b)
        {
            return true;
        }
        if (!a || !b)
        {
            return false;
        }
        if (a->getType() != b->getType())
        {
            return false;
        }
        if (a->getPos() != b->getPos())
        {
            return false;
        }
        if (a->getType() == VALUE && a->getValue() != b->getValue())
        {
            return false;
        }

        return compareTrees(a->getLeft(), b->getLeft()) && compareTrees(a->getRight(), b->getRight());
    }

    exprNode* makeValue(int v, int pos)
    {
        return new exprNode(v, pos);
    }

    exprNode* makeOp(exprNodeType t, exprNode* l, exprNode* r, int pos)
    {
        return new exprNode(t, l, r, pos);
    }

    void assertErrorsEqual(const vector<error>& actual, const vector<error>& expected)
    {
        Assert::AreEqual(expected.size(), actual.size());

        for (size_t i = 0; i < expected.size(); i++)
        {
            Assert::AreEqual(expected[i].type, actual[i].type);
            Assert::AreEqual(expected[i].position, actual[i].position);
            Assert::AreEqual(expected[i].position2, actual[i].position2);
            Assert::AreEqual(expected[i].token, actual[i].token);
        }
    }
}
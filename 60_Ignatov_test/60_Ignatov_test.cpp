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

    TEST_CLASS(TokenizeTests)
    {
    public:
        // 1. "Строка из нескольких слов"
        TEST_METHOD(TestTokenize_MultipleWords)
        {
            string input = "one two three";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"one",0},{"two",4},{"three",8} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 2. "Строка из одного слова"
        TEST_METHOD(TestTokenize_SingleWord)
        {
            string input = "one";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"one",0} };
            Assert::AreEqual(expected.size(), result.size());
            Assert::AreEqual(expected[0].first, result[0].first);
            Assert::AreEqual(expected[0].second, result[0].second);
        }

        // 3. "Строка из большого количества слов" (25 слов)
        TEST_METHOD(TestTokenize_LargeAmountOfWords)
        {
            string input = "word1 word2 word3 word4 word5 word6 word7 word8 word9 word10 "
                "word11 word12 word13 word14 word15 word16 word17 word18 word19 word20 "
                "word21 word22 word23 word24 word25";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"word1", 0}, {"word2", 6}, {"word3", 12}, {"word4", 18}, {"word5", 24}, {"word6", 30},
                {"word7", 36}, {"word8", 42}, {"word9", 48}, {"word10", 54}, {"word11", 61}, {"word12", 68}, {"word13", 75}, {"word14", 82},
                {"word15", 89}, {"word16", 96}, {"word17", 103}, {"word18", 110}, {"word19", 117}, {"word20", 124}, {"word21", 131},
                {"word22", 138}, {"word23", 145}, {"word24", 152}, {"word25", 159} };

            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 4. "Строка с лидирующими пробелами"
        TEST_METHOD(TestTokenize_LeadingSpaces)
        {
            string input = "   Hello world";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",3},{"world",9} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 5. "Строка с замыкающими пробелами"
        TEST_METHOD(TestTokenize_TrailingSpaces)
        {
            string input = "Hello world ";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",0},{"world",6} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 6. "Много пробелов между словами"
        TEST_METHOD(TestTokenize_MultipleSpacesBetween)
        {
            string input = "Hello    world";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",0},{"world",9} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 7. "Строка с табуляцией"
        TEST_METHOD(TestTokenize_Tabulation)
        {
            string input = "Hello\tworld";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",0},{"world",6} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 8. "Строка с пробелами и табуляцией"
        TEST_METHOD(TestTokenize_SpacesAndTabs)
        {
            string input = " \tHello\tworld ";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",2},{"world",8} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 9. "В строке только пробельные символы"
        TEST_METHOD(TestTokenize_OnlyWhitespace)
        {
            string input = " \t ";
            auto result = tokenize(input);
            Assert::AreEqual(0, (int)result.size());
        }

        // 10. "Пустая строка"
        TEST_METHOD(TestTokenize_EmptyString)
        {
            string input = "";
            auto result = tokenize(input);
            Assert::AreEqual(0, (int)result.size());
        }

        // 11. "Строка из одного символа"
        TEST_METHOD(TestTokenize_SingleCharacter)
        {
            string input = "a";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"a",0} };
            Assert::AreEqual(expected.size(), result.size());
            Assert::AreEqual(expected[0].first, result[0].first);
            Assert::AreEqual(expected[0].second, result[0].second);
        }

        // 12. "В строке используются специальные символы"
        TEST_METHOD(TestTokenize_SpecialCharacters)
        {
            string input = "(2 + 2) = 4 !";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"(2",0},{"+",3},{"2)",5},{"=",8},{"4",10},{"!",12} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 13. "Строка состоит из одинаковых слов"
        TEST_METHOD(TestTokenize_IdenticalWords) {
            string input = "world world world world world";
            auto result = tokenize(input);
            Assert::AreEqual(5, (int)result.size());
            for (size_t i = 0; i < 5; ++i)
            {
                Assert::AreEqual(string("world"), result[i].first);
                Assert::AreEqual(static_cast<int>(i * 6), result[i].second);
            }
        }

        // 14. "Строка из одного огромного слова"
        TEST_METHOD(TestTokenize_HugeWord)
        {
            string input = "sssssqqqqqqQqqqqdddddddVvvvvvvvvvvvFfffffffffdrRrrgl,bl,lffGmbkmgkbkmkf]fl]rflrklfk``~jjdo+++=fKkvmf4;%»%??№";
            auto result = tokenize(input);
            Assert::AreEqual(1, (int)result.size());
            Assert::AreEqual(input, result[0].first);
            Assert::AreEqual(0, result[0].second);
        }

        // 15. "Строка, содержащая перевод строки"
        TEST_METHOD(TestTokenize_Newline)
        {
            string input = "Hello\nworld";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",0},{"world",6} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 16. "Строка с переводом строки и пробелами"
        TEST_METHOD(TestTokenize_NewlineAndSpaces)
        {
            string input = "  Hello\n world ";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",2},{"world",9} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }

        // 17. "Строка с переводом строки, пробелами и табуляцией"
        TEST_METHOD(TestTokenize_NewlineSpacesTabs)
        {
            string input = "Hello\n\tworld ";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"Hello",0},{"world",7} };
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, result[i].first);
                Assert::AreEqual(expected[i].second, result[i].second);
            }
        }
    };
}
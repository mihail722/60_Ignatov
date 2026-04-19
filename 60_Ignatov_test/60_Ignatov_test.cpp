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

    TEST_CLASS(TestBuildTree)
    {
    public:

        // 1. "Выражение из одного операнда"
        TEST_METHOD(BuildTree_SingleOperand)
        {
            vector<pair<string, int>> tokens = { {"1",0} };
            vector<error> errors;

            auto result = buildTree(tokens, errors);
            auto expected = makeValue(1, 0);

            Assert::IsTrue(compareTrees(result, expected));

            delete expected;
            delete result;
        }

        // 2. "Одиночная унарная операция"
        TEST_METHOD(BuildTree_UnaryOperation)
        {
            vector<pair<string, int>> tokens = { {"1",0},{"!",2} };
            vector<error> errors;

            auto result = buildTree(tokens, errors);
            auto expected = makeOp(NOT, nullptr, makeValue(1, 0), 2);

            Assert::IsTrue(compareTrees(result, expected));

            delete expected;
            delete result;
        }

        // 3. "Одиночная бинарная операция"
        TEST_METHOD(BuildTree_BinaryOperation)
        {
            vector<pair<string, int>> tokens = { {"1",0},{"0",2},{"^",4} };
            vector<error> errors;

            auto result = buildTree(tokens, errors);
            auto expected = makeOp(AND, makeValue(1, 0), makeValue(0, 2), 4);

            Assert::IsTrue(compareTrees(result, expected));

            delete expected;
            delete result;
        }

        // 4. "Несколько операций"
        TEST_METHOD(BuildTree_MultipleOperations)
        {
            vector<pair<string, int>> tokens =
            {
                {"1",0},{"0",2},{"^",4},
                {"1",6},{"!",8},
                {"-1",10},{"->",13},
                {"<->",16}
            };
            vector<error> errors;

            auto result = buildTree(tokens, errors);

            auto andNode = makeOp(AND, makeValue(1, 0), makeValue(0, 2), 4);

            auto notNode = makeOp(NOT, nullptr, makeValue(1, 6), 8);

            auto implNode = makeOp(IMPLICATION, notNode, makeValue(-1, 10), 13);

            auto expected = makeOp(EQUIVALENCE, andNode, implNode, 16);

            Assert::IsTrue(compareTrees(result, expected));

            delete expected;
            delete result;
        }

        // 5. "Выражение из 101 операции"
        TEST_METHOD(BuildTree_TooManyOperations)
        {
            vector<pair<string, int>> tokens;

            tokens.push_back({ "1", 0 });
            tokens.push_back({ "1", 2 });
            tokens.push_back({ "^", 4 });

            int pos = 6;

            for (int i = 0; i < 100; i++)
            {
                tokens.push_back({ "1", pos });
                pos += 2;
                tokens.push_back({ "^", pos });
                pos += 2;
            }

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {TOO_MANY_OPERATION, 0, 0, ""} };
            assertErrorsEqual(errors, expected);

        }

        // 6. "Отсутствует операция"
        TEST_METHOD(BuildTree_MissingOperation)
        {
            vector<pair<string, int>> tokens =
            {
                {"1",0},
                {"0",2},
                {"^",4},
                {"1",6},
                {"!",8},
                {"-1",10},
                {"->",13}
            };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {MISSING_OPERATION, 13, 4, ""} };

            assertErrorsEqual(errors, expected);
        }

        // 7. "Не хватает одного операнда"
        TEST_METHOD(BuildTree_MissingOperandBinary)
        {
            vector<pair<string, int>> tokens =
            {
                {"1",0},
                {"0",2},
                {"^",4},
                {"1",6},
                {"!",8},
                {"-1",10},
                {"->",13},
                {"<->",16},
                {"v",20}
            };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {MISSING_OPERAND, 20, 0, "v"} };

            assertErrorsEqual(errors, expected);
        }

        // 8. "Не хватает обоих операндов"
        TEST_METHOD(BuildTree_NoOperands)
        {
            vector<pair<string, int>> tokens = { {"v",0} };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {MISSING_OPERAND, 0, 0, "v"} };

            assertErrorsEqual(errors, expected);
        }

        // 9. "Унарная операция без операнда"
        TEST_METHOD(BuildTree_NotWithoutOperand)
        {
            vector<pair<string, int>> tokens =
            {
                {"!",0}
            };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {MISSING_OPERAND, 0, 0, "!"} };

            assertErrorsEqual(errors, expected);
        }

        // 10. "Некорректный символ"
        TEST_METHOD(BuildTree_InvalidSymbol)
        {
            vector<pair<string, int>> tokens =
            {
                {"1234",0},
                {"0",2},
                {"^",4}
            };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected = { {INVALID_SYMBOL, 0, 0, "1234"} };

            assertErrorsEqual(errors, expected);
        }

        // 11. "Множество ошибок"
        TEST_METHOD(BuildTree_MultipleErrors)
        {
            vector<pair<string, int>> tokens =
            {
                {"1234",0},
                {"0",5},
                {"^",7},
                {"167dfddf",9},
                {"!",18},
                {"-1",20},
                {"->qel",23},
                {"<->",29},
                {"v",33}
            };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected =
            {
                {INVALID_SYMBOL, 0, 0, "1234"},
                {INVALID_SYMBOL, 9, 0, "167dfddf"},
                {INVALID_SYMBOL, 23, 0, "->qel"},
                {MISSING_OPERATION, 33, 7, ""}
            };

            assertErrorsEqual(errors, expected);
        }

        // 12. "Пустой ввод"
        TEST_METHOD(BuildTree_EmptyInput)
        {
            vector<pair<string, int>> tokens;
            vector<error> errors;

            auto result = buildTree(tokens, errors);

            Assert::IsNull(result);
        }

        // 13. "Только операции"
        TEST_METHOD(BuildTree_OnlyOperators)
        {
            vector<pair<string, int>> tokens = { {"!", 0},{"!", 2},{"^", 4}, {"v", 6}, {"!", 8}, {"->", 10} };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected =
            {
                {MISSING_OPERAND, 0, 0, "!"},
                {MISSING_OPERAND, 4, 0, "^"},
                {MISSING_OPERAND, 6, 0, "v"},
                {MISSING_OPERAND, 10, 0, "->"}
            };

            assertErrorsEqual(errors, expected);
        }

        // 14. "Только операнды"
        TEST_METHOD(BuildTree_OnlyOperands)
        {
            vector<pair<string, int>> tokens = { {"1",0}, {"0",2}, {"-1",4}, {"1",7},{"0",9} };

            vector<error> errors;

            buildTree(tokens, errors);

            vector<error> expected =
            {
                {MISSING_OPERATION, 9, 7, ""},
                {MISSING_OPERATION, 7, 4, ""},
                {MISSING_OPERATION, 4, 2, ""},
                {MISSING_OPERATION, 2, 0, ""}
            };

            assertErrorsEqual(errors, expected);
        }

        // 15. "Ровно 100 операций"
        TEST_METHOD(BuildTree_Exactly100Operations)
        {
            vector<pair<string, int>> tokens;

            tokens.push_back({ "1",0 });
            tokens.push_back({ "1",2 });
            tokens.push_back({ "^",4 });

            int pos = 6;

            for (int i = 0; i < 99; i++)
            {
                tokens.push_back({ "1", pos });
                pos += 2;
                tokens.push_back({ "^", pos });
                pos += 2;
            }

            vector<error> errors;

            auto result = buildTree(tokens, errors);

            stack<exprNode*> st;

            st.push(makeValue(1, 0));
            st.push(makeValue(1, 2));

            auto right = st.top(); st.pop();
            auto left = st.top(); st.pop();

            st.push(makeOp(AND, left, right, 4));

            int currentPos = 6;

            for (int i = 0; i < 99; i++)
            {
                st.push(makeValue(1, currentPos));
                currentPos += 2;

                auto r = st.top(); st.pop();
                auto l = st.top(); st.pop();

                st.push(makeOp(AND, l, r, currentPos));
                currentPos += 2;
            }

            auto expected = st.top();

            Assert::IsTrue(compareTrees(result, expected));

            delete expected;
            delete result;
        }

    };
}
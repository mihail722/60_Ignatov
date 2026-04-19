#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

/*
 * @enum exprNodeType
 * @brief Перечисление типов узлов выражения
 */
enum exprNodeType
{
    VALUE,          ///< Операнд
    NOT,            ///< Унарный оператор отрицания
    AND,            ///< Бинарный оператор И
    OR,             ///< Бинарный оператор ИЛИ
    IMPLICATION,    ///< Бинарный оператор импликации
    EQUIVALENCE     ///< Бинарный оператор эквиваленции
};

/*
 * @enum errorType
 * @brief Перечисление типов ошибок 
 */
enum errorType
{
    NO_ERROR,           ///< Отсутствие ошибки
    INVALID_SYMBOL,     ///< Недопустимый символ в выражении
    TOO_MANY_OPERATION, ///< Обнаружено избыточное количество операций
    MISSING_OPERATION,  ///< Отсутствует операция для потенциальных операндов
    MISSING_OPERAND     ///< Недостаточно операндов для выполнения операции
};

/*
 * @class exprNode
 * @brief Класс, представляющий узел в синтаксическом дереве логического выражения
 */
class exprNode
{
private:
    int value;              ///< Значение узла 
    exprNodeType type;      ///< Тип узла 
    exprNode* left;         ///< Указатель на левого потомка
    exprNode* right;        ///< Указатель на правого потомка
    int nodeId;             ///< Уникальный идентификатор узла
    int pos;                ///< Позиция узла в исходном выражении

    static int globalCounter;    ///< Глобальный счётчик для генерации уникальных идентификаторов
    
public:

    /*
     * @brief Вычисляет значение узла и всего поддерева
     * @param node[in] - указатель на корень дерева выражения.
     * @return int - результат вычисления.
     */
    static int calculate(exprNode* node);

    /*
     * @brief Конструктор узла-операнда
     * @param val Значение операнда
     * @param pos Позиция в исходном выражении
     */
    exprNode(int val, int pos);

    /*
     * @brief Конструктор узла-операции
     * @param type Тип логической операции
     * @param left Указатель на левый операнд
     * @param right Указатель на правый операнд
     * @param pos Позиция операции в исходном выражении
     */
    exprNode(exprNodeType type, exprNode* left, exprNode* right, int pos);

    /*
     * @brief Деструктор
     */
    ~exprNode();

    /*
     * @brief Возвращает значение узла
     * @return int Значение (для операнда) или результат вычисления поддерева
     */
    int getValue() const;

    /*
     * @brief Возвращает указатель на левого потомка
     * @return exprNode* Указатель на левого потомка
     */
    exprNode* getLeft() const;

    /*
     * @brief Возвращает указатель на правого потомка
     * @return exprNode* Указатель на правого потомка
     */
    exprNode* getRight() const;

    /*
     * @brief Возвращает уникальный идентификатор узла
     * @return int Идентификатор узла
     */
    int getId() const;

    /*
     * @brief Возвращает тип узла
     * @return exprNodeType Тип узла 
     */
    exprNodeType getType() const;

    /*
     * @brief Возвращает позицию узла в исходном выражении
     * @return int Позиция (индекс символа в строке)
     */
    int getPos() const;
    
    /*
     * @brief Устанавливает левого потомка
     * @param node Указатель на левого потомка
     */
    void setLeft(exprNode* node);

    /*
     * @brief Устанавливает правого потомка
     * @param node Указатель на правого потомка
     */
    void setRight(exprNode* node);

    /*
     * @brief Устанавливает значение узла
     * @param val Значение узла
     */
    void setValue(int val);

    /*
     * @brief Устанавливает тип узла
     * @param type Тип узла
     */
    void setType(exprNodeType type);
};

/*
 * @struct error
 * @brief Структура для хранения информации об ошибке 
 */
struct error
{
    errorType type;     ///< Тип ошибки 
    int position;       ///< Позиция символа на котором произошла ошибка 
    int position2;      ///< Дополнительная позиция
    string token;       ///< Токен, вызвавший ошибку
};




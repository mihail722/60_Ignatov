#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

/**
 * @enum exprNodeType
 * @brief Перечисление типов узлов выражения
 */
enum exprNodeType
{
    VALUE,          ///< Операнд
    NOT,            ///< Унарный оператор отрицания
    AND,            ///< Бинарный оператор конъюнкции 
    OR,             ///< Бинарный оператор дизъюнкции 
    IMPLICATION,    ///< Бинарный оператор импликации
    EQUIVALENCE     ///< Бинарный оператор эквивалентности
};

/**
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

/**
 * @class exprNode
 * @brief Класс, представляющий узел в синтаксическом дереве логического выражения
 */
class exprNode
{
private:
    int value;              ///< Значение операнда или результат вычисления поддерева 
    exprNodeType type;      ///< Тип узла 
    exprNode* left;         ///< Указатель на левого потомка
    exprNode* right;        ///< Указатель на правого потомка
    int nodeId;             ///< Уникальный идентификатор узла
    int pos;                ///< Позиция узла в исходном выражении

    static int globalCounter;    ///< Глобальный счётчик для генерации уникальных идентификаторов
    
public:

    /**
     * @brief Вычисляет значение узла и всего поддерева
     * @param[in] node Указатель на корень дерева выражения
     * @return int Результат вычисления
     */
    static int calculate(exprNode* node);

    /**
     * @brief Конструктор узла-операнда
     * @param[in] val Значение операнда
     * @param[in] pos Позиция в исходном выражении
     */
    exprNode(int val, int pos);

    /**
     * @brief Конструктор узла-операции
     * @param[in] type Тип логической операции
     * @param[in] left Указатель на левый операнд
     * @param[in] right Указатель на правый операнд
     * @param[in] pos Позиция операции в исходном выражении
     */
    exprNode(exprNodeType type, exprNode* left, exprNode* right, int pos);

    /**
     * @brief Освобождает память поддерева узла
     */
    ~exprNode();

    /**
     * @brief Возвращает значение узла
     * @return int Значение (для операнда) или результат вычисления поддерева
     */
    int getValue() const;

    /**
     * @brief Возвращает указатель на левого потомка
     * @return exprNode* Указатель на левого потомка
     */
    exprNode* getLeft() const;

    /**
     * @brief Возвращает указатель на правого потомка
     * @return exprNode* Указатель на правого потомка
     */
    exprNode* getRight() const;

    /**
     * @brief Возвращает уникальный идентификатор узла
     * @return int Идентификатор узла
     */
    int getId() const;

    /**
     * @brief Возвращает тип узла
     * @return exprNodeType Тип узла 
     */
    exprNodeType getType() const;

    /**
     * @brief Возвращает позицию узла в исходном выражении
     * @return int Позиция (индекс символа в строке)
     */
    int getPos() const;
    
    /**
     * @brief Устанавливает левого потомка
     * @param[in] node Указатель на левого потомка
     */
    void setLeft(exprNode* node);

    /**
     * @brief Устанавливает правого потомка
     * @param[in] node Указатель на правого потомка
     */
    void setRight(exprNode* node);

    /**
     * @brief Устанавливает значение узла
     * @param[in] val Значение узла
     */
    void setValue(int val);

    /**
     * @brief Устанавливает тип узла
     * @param[in] type Тип узла
     */
    void setType(exprNodeType type);
};

/**
 * @struct error
 * @brief Структура для хранения информации об ошибке 
 */
struct error
{
    errorType type;     ///< Тип ошибки 
    int position;       ///< Позиция символа на котором произошла ошибка 
    int position2;      ///< Дополнительная позиция для второго символа на котором произошла ошибка 
    string token;       ///< Токен, вызвавший ошибку
};

/**
 * @brief Проверка, является ли токен операндом
 * @param[in] token Входной токен
 * @return true если токен является операндом, иначе false
 */
bool isOperand(const string& token);

/**
 * @brief Проверка, является ли токен операцией
 * @param[in] token Входной токен
 * @return true если токен является операцией, иначе false
 */
bool isOperation(const string& token);

/**
 * @brief Определение типа операции
 * @param[in] token Входной токен
 * @return exprNodeType Тип операции
 */
exprNodeType getOperationType(const string& token);

/**
 * @brief Разбиение строки на токены с сохранением позиций
 * @param[in] line Входная строка выражения
 * @return vector<pair<string, int>> Список токенов и их позиций
 */
vector<pair<string, int>> tokenize(const string& line);

/**
 * @brief Построение дерева троично-логического выражения
 * @param[in] tokens Вектор токенов
 * @param[out] errors Вектор для хранения ошибок
 * @return exprNode* Корень дерева выражения или nullptr при ошибке
 */
exprNode* buildTree(const vector<pair<string, int>>& tokens, vector<error>& errors);

/**
 * @brief Парсит троично-логическое выражение
 * @param[in] line Входная строка выражения
 * @param[out] errors Вектор для хранения ошибок
 * @return exprNode* Корень дерева выражения или nullptr при ошибке
 */
exprNode* parseExpression(const string& line, vector<error>& errors);

/**
 * @brief Генерация представления дерева в формате Graphviz
 * @param[in] root Указатель на корень дерева выражения
 * @param[out] out Поток вывода, в который записывается результат
 */
void generateGraph(exprNode* root, ofstream& out);

/**
 * @brief Записывает дерево выражения в файл в формате Graphviz (DOT)
 * @param[in] root Указатель на корень дерева выражения
 * @param[out] out Поток вывода, в который записывается результат
 */
void writeGraph(exprNode* root, ofstream& out);

/**
 * @brief Главная функция программы
 * @param[in] argc Количество аргументов командной строки
 * @param[in] argv Аргументы командной строки: argv[1] — входной файл, argv[2] — выходной файл
 * @return int Код завершения программы: 0 при успешном завершении, иначе 1
 */
int main(int argc, char* argv[]);


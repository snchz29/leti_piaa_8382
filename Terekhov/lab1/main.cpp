#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;

const bool temp_res = true;
struct Dot {
    int x, y;

    Dot(int x, int y) : x{x}, y{y} {}
};

struct Square {
    Dot corner;
    int a;

    Square(int x, int y, int a) : corner{Dot(x, y)}, a{(a > 1) ? a : 1} {}

    Square(Square const &s) : corner{Dot(s.corner.x, s.corner.y)}, a{s.a} {}

    bool isCross(const Square &square) {
        return !(corner.x + a <= square.corner.x ||
                 corner.x >= square.corner.x + square.a ||
                 corner.y + a <= square.corner.y ||
                 corner.y >= square.corner.y + square.a);
    }

    friend bool operator==(const Square &s1, const Square &s2) {
        return s1.corner.x == s2.corner.x && s1.corner.y == s2.corner.y && s1.a == s2.a;
    }

    friend std::ostream &operator<<(std::ostream &out, const Square &s) {
        out << s.corner.x + 1 << " " << s.corner.y + 1 << " " << s.a;
        return out;
    }

    friend Square operator*(const Square &s, int mul) {
        return Square(s.corner.x * mul, s.corner.y * mul, s.a * mul);
    }
};

class Squares {
    int a;
    std::vector<Square> arr;
public:
    Squares() : a{1} {}

    Squares(Squares &sqs) : a{sqs.a} {
        for (const auto &s : sqs.arr) {
            arr.push_back(s);
        }
    }

    void setA(int size) {
        a = size;
    }

    explicit Squares(int a) : a{(a > 1) ? a : 1} {}

    void print(int k) {
        cout << arr.size() << endl;
        for (const auto &s : arr) {
            cout << s * k << endl;
        }
    }

    int getA() const {
        return a;
    }

    int length() const {
        return arr.size();
    }

    bool canAdd(Square square) {
        if (square.corner.x + square.a > a ||
            square.corner.x < 0 ||
            square.corner.y + square.a > a ||
            square.corner.y < 0)
            return false;
        for (const auto &s : arr) {
            if (square.isCross(s)) {
                return false;
            }
        }
        return true;
    }

    int getSquareSize(int x, int y) {
        for (auto s: arr) {
            if (s.isCross({x, y, 1}))
                return s.a;
        }
        return 0;
    }

    Squares &addSquare(const Square &square) {
        if (canAdd(square))
            arr.push_back(square);
        return *this;
    }

    std::vector<Square>::iterator begin() {
        return arr.begin();
    }

    std::vector<Square>::iterator end() {
        return arr.end();
    }

    Square delSquare() {
        Square s = *(arr.end() - 1);
        arr.pop_back();
        return s;
    }

    int getArea() {
        return a * a;
    }

    void assign(Squares sqs) {
        arr.assign(sqs.begin(), sqs.end());
    }
};

Squares minSqs;
int min;
int countAdd = 0;
int countDel = 0;
int countCng = 0;
int countCmp = 0;
int areaMultiplier = 1;

void backtracking(Squares sqs, int size, int area) {
    //выход если невозможно одним квадратом со стороной size замостить оставшуюся площадь
    if (sqs.length() == min - 1 && sqs.getArea() - area > size * size) return;
    // переменная для контроля одноразовой вставки
    bool added = false;
    int i = 0;
    while (i < sqs.getA() && !added) {
        int j = 0;
        while (j < sqs.getA() && !added) {
            ++countCmp;
            if (sqs.canAdd({i, j, 1})) {
                ++countCmp;
                if (sqs.canAdd({i, j, size})) {
                    added = true;
                    sqs.addSquare({i, j, size});
                    if (temp_res)
                        cout << "Add square " << i << "\t" << j << "\t" << size << endl;
                    ++countAdd;
                } else return;
            } else j += sqs.getSquareSize(i, j); // перескакиваем текущий квадрат
        }
        ++i;
    }
// выход если исчерпали лимит квадратов и удаление последнего квадрата
    if (sqs.length() + 1 == min) {
        Square s = sqs.delSquare();
        if (temp_res)
            cout << "Del square " << s.corner.x << "\t" << s.corner.y << "\t" << s.a << endl;
        ++countDel;
        return;
    }
// если большой квадрат заполнен и можно улучшить минимальность то обновляем минимум
    if (sqs.getArea() - area == size * size && sqs.length() + 1 < min) {
        min = sqs.length() + 1;
        minSqs.assign(sqs);
        countCng++;
        if (temp_res){
        cout << endl << "New minimal" << endl ;
        minSqs.print(areaMultiplier);
        cout << endl;
        }
        Square s = sqs.delSquare();
        if (temp_res)
            cout << "Del square " << s.corner.x << "\t" << s.corner.y << "\t" << s.a << endl;
        ++countDel;
        return;
    }
// рекурсивный спуск с уменьшением стороны вставляемого квадрата
    for (int sz = sqs.getA() / 2; sz >= 1; sz--) {
        if (sz * sz <= sqs.getArea() - area)
            backtracking(sqs, sz, area + size * size);
    }
}

int main() {
    std::vector<int> primes = {29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
    int n = 0;
    while (n < 2 || n > 30) {
        cout << "N (2<=N<=30): ";
        cin >> n;
    }
    int area = 0;

// для оптимизации уменьшаем квадрат до квадрата со стороной равной простому числу, сохраняя коэффициент сжатия
    for (auto i : primes) {
        while (n % i == 0) {
            n /= i;
            areaMultiplier *= i;
            if (primes.end() != std::find(primes.begin(), primes.end(), n)) {
                break;
            }
        }
        if (primes.end() != std::find(primes.begin(), primes.end(), n)) {
            break;
        }
    }
    Squares squares(n);
    minSqs.setA(n);
    min = 2 * n;
// простые случаи рассмотрены без использования рекурсии
    if (n == 2) {
        minSqs.addSquare({0, 0, 1});
        minSqs.addSquare({1, 0, 1});
        minSqs.addSquare({0, 1, 1});
        minSqs.addSquare({1, 1, 1});
        countCng = 1;
        countAdd = 4;
    } else if (n == 3) {
        minSqs.addSquare({0, 0, 2});
        minSqs.addSquare({2, 0, 1});
        minSqs.addSquare({2, 1, 1});
        minSqs.addSquare({0, 2, 1});
        minSqs.addSquare({2, 2, 1});
        minSqs.addSquare({1, 2, 1});
        countCng = 1;
        countAdd = 6;
    }
// для больших нечетных N в углу всегда лежит квадрат со стороной (N + 1) / 2, а правее и ниже него квадраты со сторонами N / 2 (получено эмпирическим методом)
    else {
        squares.addSquare({0, 0, (n + 1) / 2});
        minSqs.addSquare({0, 0, (n + 1) / 2});
        area += ((n + 1) / 2) * ((n + 1) / 2);
        squares.addSquare({(n + 1) / 2, 0, n / 2});
        minSqs.addSquare({(n + 1) / 2, 0, n / 2});
        area += (n / 2) * (n / 2);
        squares.addSquare({0, (n + 1) / 2, n / 2});
        minSqs.addSquare({0, (n + 1) / 2, n / 2});
        area += (n / 2) * (n / 2);
        countAdd = 3 + 3;
// запуск рекурсивной серии попыток в порядке уменьшения стороны вставляемого квадрата
        for (int i = n / 2; i >= 1; i--) {
            backtracking(squares, i, area);
        }
    }
    cout << endl << "Answer!" << endl;
    minSqs.print(areaMultiplier);
    cout << endl;
    cout << "Count Adds: " << countAdd << endl;
    cout << "Count Dels: " << countDel << endl;
    cout << "Count Change: " << countCng << endl;
    cout << "Count Comp: " << countCmp << endl;
    cout << "Total: " << countAdd + countDel + countCng + countCmp << endl;
    return 0;
}

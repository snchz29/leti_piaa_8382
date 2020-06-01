#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
// пи-функция генерирует массив переходов
vector<int> piFunction(string img){
    vector<int> pi;
    pi.insert(pi.end(), 0); // первый символ образа имеет значение пи-функции равное нулю
    int i = 1; // счетчики
    int j = 0;
    while (i < img.size()){ // пока не дошли до конца строки первым счетчиком
        if (img[i] == img[j]){          // если символы на iом и jом месте равны,
            pi.insert(pi.end(), j + 1); // то добавляем в массив значений пи-функции j+1 (значение функции для символа на iом месте)
            i++;                        // увеличиваем счетчики
            j++;                        // и переходим к следующей итерации
        }
        else if (j == 0){               // если обнулился второй счетчик из-за следующего else-блока, но символы на iом и jом месте не равны
            pi.insert(pi.end(), 0);     // добавляем ноль в массив значений 
            i++;                        // продолжаем движение по строке 
        }
        else                            // если символы на iом и jом месте не равны и j не равно нулю,
            j = pi[j - 1];              // то значение вычисляется на основе уже существующего
    }
    return pi;
}

// поиск образа в строке
vector<int> kmp(string img, string haystack, vector<int> pi_vec){
    vector<int> find;   // массив ответов
    int img_ind = 0;    // счетчик для продвижения по образу
    int text_ind = 0;   // счетчик для продвижения по тексту
    while (text_ind < haystack.size()){
        if (haystack[text_ind] == img[img_ind]){    // если два одинаковых символа двигаемся вперед и по образу и по строке
            img_ind++;
            text_ind++;
            if (img_ind == img.size()){         // если дошли до конца образа, значит нашли вхождение
                find.insert(find.end(), text_ind - img_ind);
                img_ind = pi_vec[img_ind - 1];  // переходим не в начало образа, а на позицию равную предпоследнему значению пи-функции
            }
        }
        else if (img_ind == 0)  // если нет совпадения, и сравнение происходит с самым первым символом образа
            text_ind++;         // продвигаемся по тексту
        else                                // если нет совпадения, но по образу уже продвинулись от начала
            img_ind = pi_vec[img_ind - 1];  // то в образе перескакиваем на символ с индексом равным предыдущему значению пи-функции
    }
    return find;
}

int main(){
    string image;
    string haystack;
    cin >> image;   // чтение образа
    cin >> haystack;    // чтение текста
    vector<int> pi = piFunction(image); // вычисление пи-функции для образа
    vector<int> answer = kmp(image, haystack, pi);  // вызов функции поиска
    if (answer.empty()) // если ничего не нашли
        cout << -1 << endl;
    else{   // иначе выводим содержимое ответа
        for (int i = 0; i < answer.size() - 1; ++i)
            cout << answer[i] << ",";
        cout << answer[answer.size() - 1] << endl;
    }
    return 0;
}
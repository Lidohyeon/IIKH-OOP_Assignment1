#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include <string> // std::string: 문자열을 다루는 클래스를 사용하기 위해 포함
#include <vector>
#include <iomanip> 

/*
<vector> 헤더를 추가합니다.

#include <vector> 헤더는 C++ 표준 라이브러리에서 제공하는 벡터 컨테이너를 사용하기 위해 필요합니다.

벡터는 동적 배열로, 크기가 자동으로 조정되며 다양한 유용한 기능을 제공합니다.

이 헤더를 포함함으로써 std::vector 클래스를 사용할 수 있게 됩니다.

스스로 크기를 조절하는 '동적 배열(Dynamic Array)'로 생각하면 됩니다.
*/


/*
<iomanip> 헤더를 추가합니다.

setprecision(n)과 fixed: 소수점 정밀도 제어
실수(float, double)를 출력할 때 소수점 이하 자릿수를 제어하는 매우 중요한 기능입니다. fixed와 함께 사용되는 경우가 많습니다.

setprecision(n): 실수의 전체 유효 숫자를 n자리로 제한

fixed: setprecision의 의미를 소수점 이하 자릿수로 변경

특징: 두 조작자 모두 한 번 설정하면 계속 유지됩니다. (Sticky)
*/

using namespace std;

// 난이도를 나타내는 enum class를 클래스 외부에 정의 (A, B, C 세 가지 난이도)
// A: 어려움 (Advanced), B: 보통 (Medium), C: 쉬움 (Easy)

// 재료 구조체 추가-> 이걸로 인분 계산 가능하게 설정 

struct Ingredient {
    string name;
    double quantity;
    string unit;
};

enum class Difficulty
{
    A, // Hard - Advanced recipe
    B, // Medium - Intermediate recipe
    C  // Easy - Beginner recipe
};

class Recipe
{
private:
    string title;              // 요리 이름
    string procedure;          // 조리 방법
    int time;                  // 조리 시간 (분)
    vector<Ingredient> ingredient; // 재료 목록
    Difficulty difficulty;     // 난이도

public:
    // --- 생성자 ---
    Recipe(const string &title, const string &procedure, int time, const vector<Ingredient> &ingredient, Difficulty difficulty)
        : title(title), procedure(procedure), time(time), ingredient(ingredient), difficulty(difficulty) {}

    // --- Getter 함수들 ---
    const string &getTitle() const
    {
        return title;
    }
    const string &getProcedure() const
    {
        return procedure;
    }
    int getTime() const
    {
        return time;
    }
    const vector<Ingredient> &getIngredient() const
    {
        return ingredient;
    }
    Difficulty getDifficulty() const
    {
        return difficulty;
    }
    // --------------------

    // --- Setter 함수들 ---
    void setTitle(const string &newTitle)
    {
        this->title = newTitle;
    }
    void setProcedure(const string &newProcedure)
    {
        this->procedure = newProcedure;
    }
    void setTime(int newTime)
    {
        this->time = newTime;
    }
    void setIngredient(const vector<Ingredient> &newIngredient)
    {
        this->ingredient = newIngredient;
    }
    void setDifficulty(Difficulty newDifficulty)
    {
        this->difficulty = newDifficulty;
    }
    //--------------------

    // --- display 함수 ---
    void display() const
    {
        cout << "--- " << title << " ---" << endl;
        cout << "Time: " << time << " minutes" << endl;

        // 난이도를 문자열로 변환하여 출력
        cout << "Difficulty: ";
        switch (difficulty)
        {
        case Difficulty::A:
            cout << "A (Advanced/Hard)";
            break;
        case Difficulty::B:
            cout << "B (Intermediate/Medium)";
            break;
        case Difficulty::C:
            cout << "C (Beginner/Easy)";
            break;
        }
        cout << endl;

        cout << "\nIngredients:" << endl;
        for (const auto& ing : ingredient) {
            if (ing.quantity == 0) {
                cout << "- " << ing.unit << " of " << ing.name << endl;
            } else {
                // 2. 소수점 형식을 적용하여 출력 (소수점 2자리) iomanip 사용
                cout << "- " << fixed << setprecision(2) << ing.quantity << " " << ing.unit << " of " << ing.name << endl;
                /*
                setprecision(n): 실수의 전체 유효 숫자를 n자리로 제한
                fixed: setprecision의 의미를 소수점 이하 자릿수로 변경
                */
            }
        }

        cout << "\nProcedure:\n"
             << procedure << endl
             << endl;
    }
};

#endif // RECIPE_H
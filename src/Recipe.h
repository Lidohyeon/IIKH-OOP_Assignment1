#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // <<-- 1. <iomanip> 헤더를 추가합니다.

using namespace std;

// 난이도를 나타내는 enum class를 클래스 외부에 정의 (A, B, C 세 가지 난이도)
// A: 어려움 (Advanced), B: 보통 (Medium), C: 쉬움 (Easy)

// 재료 구조체 추가했습니다 이걸로 인분 계산 가능하게 설정하겠습니다. 10-05 pm10:45
struct Ingredient {
    string name;
    double quantity;
    string unit;
};

enum class Difficulty
{
    A, // Hard - Advanced recipes
    B, // Medium - Intermediate recipes
    C  // Easy - Beginner recipes
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
                // 2. 소수점 형식을 적용하여 출력
                cout << "- " << fixed << setprecision(2) << ing.quantity << " " << ing.unit << " of " << ing.name << endl;
            }
        }

        cout << "\nProcedure:\n"
             << procedure << endl
             << endl;
    }
};

#endif // RECIPE_H
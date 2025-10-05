#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 난이도를 나타내는 enum class를 클래스 외부에 정의 (A, B, C 세 가지 난이도)
enum class Difficulty {
    A,
    B,
    C
};

class Recipe {
private:
    string title;       // 요리 이름
    string procedure;   // 조리 방법
    int time;           // 조리 시간 (분)
    vector<string> ingredient; // 재료 목록
    Difficulty difficulty;     // 난이도

public:
    // --- 생성자 ---
    Recipe(const string& title, const string& procedure, int time, const vector<string>& ingredient, Difficulty difficulty)
        : title(title), procedure(procedure), time(time), ingredient(ingredient), difficulty(difficulty) {}

    // --- Getter 함수들 ---
    const string& getTitle() const { 
        return title; 
    }
    const string& getProcedure() const { 
        return procedure; 
    }
    int getTime() const { 
        return time; 
    }
    const vector<string>& getIngredient() const { 
        return ingredient; 
    }
    Difficulty getDifficulty() const { 
        return difficulty; 
    }

    // --- Setter 함수들 ---
    void setTitle(const string& newTitle) { 
        this->title = newTitle; 
    }
    void setProcedure(const string& newProcedure) { 
        this->procedure = newProcedure; 
    }
    void setTime(int newTime) { 
        this->time = newTime; 
    }
    void setIngredient(const vector<string>& newIngredient) { 
        this->ingredient = newIngredient; 
    }
    void setDifficulty(Difficulty newDifficulty) { 
        this->difficulty = newDifficulty; 
    }

    // --- display 함수 ---
    void display() const {
        cout << "--- " << title << " ---" << endl;
        cout << "Time: " << time << " minutes" << endl;
        
        // 난이도를 문자열로 변환하여 출력
        cout << "Difficulty: ";
        switch (difficulty) {
            case Difficulty::A:
                cout << "A (Easy)";
                break;
            case Difficulty::B:
                cout << "B (Medium)";
                break;
            case Difficulty::C:
                cout << "C (Hard)";
                break;
        }
        cout << endl;

        cout << "\nIngredients:" << endl;
        for (const auto& ing : ingredient) {
            cout << "- " << ing << endl;
        }
        cout << "\nProcedure:\n" << procedure << endl << endl;
    }
};

#endif // RECIPE_H
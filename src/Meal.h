#ifndef MEAL_H
#define MEAL_H

#include <iostream>
#include <vector>
#include <string>

//#include "Recipe.h"
using namespace std;

class Meal {
    private: 
    string mealType; // 아침, 점심, 저녁 구분   
    vector<string> recipes; // 그 끼니에 포함된 요리 이름 목록. 레시피 객체 리스트
    //vector<Recipe> recipes;
    int servings; // 몇 인분인지

    public:
    //생성자
    Meal(string type = "Meal", int s = 1) : mealType(type), servings(s) {} // 기본 1인분

    //레시피 추가
    void addRecipe(const string& recipeName)
    {
        recipes.push_back(recipeName);
        cout << recipeName << " added to " << mealType << " (" << servings << "servings)" <<  endl;
        //cout << recipe.getName() << " added to " << mealType << " (" << servings << " servings)" << endl;
    }

    //레시피 삭제(해당 끼니의 메뉴 수정을 위해)
    void removeRecipe(const string& recipeName)
    {
        for(auto it = recipes.begin(); it != recipes.end(); ++it)
        {
            if(*it == recipeName)
            {
                recipes.erase(it);
                cout << recipeName << " remove from " << mealType << endl;
                return;
            }
        }
        cout << "Recipe not found: " << recipeName << endl;
    }

    //Recipe.h 만들어지면 맞게 수정
    //   void removeRecipe(const string& recipeName) {
    //     for (auto it = recipes.begin(); it != recipes.end(); ++it) {
    //         if (it->getName() == recipeName) {
    //             cout << it->getName() << " removed from " << mealType << endl;
    //             recipes.erase(it);
    //             return;
    //         }
    //     }
    //     cout << "Recipe not found: " << recipeName << endl;
    // }

    //몇 인분인지 설정
    void setServings(int s)
    {
        if(s > 0) 
        {
            servings = s;
            cout << mealType << " servings set to " << servings << endl;
        }
        else
        {
            cout << "Invalid serving size." << endl;
        }
    }


    //현재 끼니의 정보(레시피, 인분) 출력
    void display() const
    {
        cout << "==== " << mealType << "Menu (" << servings << " servings) =====" << endl;
        if(recipes.empty())
        {
            cout << "(no recipes)" << endl;
        }
        else
        {
            for(const auto& r : recipes)
            {
                //r.print();
                cout << "--------------------"<< endl;
            }
        }
    }
    //getter
    string getMealType() const { return mealType; }
    int getServings() const { return servings; }
};


#endif
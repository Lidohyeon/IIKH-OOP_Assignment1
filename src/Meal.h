#ifndef MEAL_H
#define MEAL_H

#include <iostream>
#include <vector>
#include <string>

#include "Recipe.h"
#include "RecipeDatabase.h"

using namespace std;

/**
 * Meal 클래스
 * --------------------------
 * 한 끼니(아침/점심/저녁)에 포함된 Recipe들을 관리하는 클래스.
 * - mealType: Breakfast / Lunch / Dinner 등 구분
 * - recipes: 해당 끼니에 포함된 Recipe 객체 리스트
 * - servings: 인분 수
 *
 * 주요 기능:
 *  - addRecipe(): RecipeDatabase에서 이름으로 레시피를 찾아 추가
 *  - removeRecipe(): 레시피 삭제
 *  - setServings(): 인분 조정
 *  - display(): 레시피 목록 및 세부정보 출력
 */

class Meal {
    private: 
    string mealType; // 아침, 점심, 저녁 구분   
    vector<Recipe> recipes; // 그 끼니에 포함된 요리 이름 목록. 레시피 객체 리스트
    int servings; // 몇 인분인지

    public:
    //생성자
    Meal(string type = "Meal", int s = 1) : mealType(type), servings(s) {} // 기본 1인분

    //레시피 추가
    void addRecipe(const RecipeDatabase &db, const string &recipeName)
    {
        bool found = false;

        for (const auto &r : db.getRecipes()) // getRecipes()로 모든 레시피 접근
        {
            if (r.getTitle() == recipeName)
            {
                recipes.push_back(r);
                cout << r.getTitle() << " added to " << mealType << " (" << servings << " servings)" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << " Recipe not found in database: " << recipeName << endl;
        }
    }

    // 레시피 삭제(해당 끼니의 메뉴 수정을 위해)
    void removeRecipe(const string &recipeName)
    {
        for (auto it = recipes.begin(); it != recipes.end(); ++it)
        {
            if (it->getTitle() == recipeName)
            {
                cout << recipeName << "' removed from " << mealType << endl;
                recipes.erase(it);
                return;
            }
        }
        cout << "Recipe not found: " << recipeName << endl;
    }

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
        cout << "==== " << mealType << " Menu (" << servings << " servings) =====" << endl;
        if(recipes.empty())
        {
            cout << "(no recipes added)" << endl;
            return;
        }
        else
        {
            for(const auto& r : recipes)
            {
                cout << "--------------------\n";
                r.display(); // Recipe.h의 display() 호출 (세부정보 표시)
            }
        }
    }

    //getter
    string getMealType() const { return mealType; }
    int getServings() const { return servings; }

    // PlanManager 연동용 — Meal 내 레시피 제목만 반환
    vector<string> getRecipeTitles() const
    {
        vector<string> names;
        for (const auto &r : recipes)
        {
            names.push_back(r.getTitle());
        }
        return names;
    }

    // PlanManager 연동용 — Meal 내 전체 Recipe 반환
    const vector<Recipe> &getRecipes() const
    {
        return recipes;
    }
};


#endif
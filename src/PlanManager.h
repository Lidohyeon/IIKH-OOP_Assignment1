#ifndef PLANMANAGER_H
#define PLANMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Date.h"
#include "Meal.h"
// #include "RecipeDatabase.h"  // RecipeDatabase 완성 후 주석 해제
// #include "Recipe.h"          // Recipe 완성 후 주석 해제

using namespace std;

/**
 * PlanManager 클래스
 * 
 * 역할: 장기간의 식사 계획을 관리하고 조율하는 총괄 매니저
 * 
 * 핵심 협력 관계:
 * - Greeter: 사용자 입력(난이도)을 받아 PlanManager에 전달
 * - RecipeDatabase: 난이도별 레시피 검색 요청
 * - Recipe: 개별 레시피의 난이도 정보 및 재료 목록 제공
 * - Meal: 각 끼니의 레시피 및 인분 정보 관리
 * - Date: 날짜별 식사 계획 표현
 * 
 * 난이도 정의:
 * - A: 어려움 (복잡한 레시피, 시간 많이 소요)
 * - B: 보통 (중간 난이도)
 * - C: 쉬움 (간단한 레시피, 빠른 조리)
 */
class PlanManager
{
private:
    // 날짜를 키로, 그 날의 모든 Meal 정보를 값으로 저장
    // 예: "2024-10-05" -> [아침Meal, 점심Meal, 저녁Meal]
    map<string, vector<Meal>> mealPlan;
    
    // 계획 기간 (시작일과 종료일)
    Date startDate;
    Date endDate;
    
    // 난이도 제약 설정
    // 사용자가 설정한 '최대 난이도'
    // 예: 'C'로 설정 시 -> C(쉬움)만 추천 가능
    //     'B'로 설정 시 -> B(보통), C(쉬움) 추천 가능
    //     'A'로 설정 시 -> A(어려움), B(보통), C(쉬움) 모두 추천 가능
    char maxDifficultyLevel;
    
    // ✅ RecipeDatabase 객체 포인터 (필수 협력자!)
    // PlanManager는 레시피를 직접 관리하지 않고,
    // RecipeDatabase에게 난이도별 검색을 요청하는 방식으로 협력
    // RecipeDatabase* recipeDB;

public:
    // ==================== 생성자 및 소멸자 ====================
    
    /**
     * 기본 생성자
     * 기본값: 난이도 'A' (모든 난이도 허용)
     */
    PlanManager() : maxDifficultyLevel('A') //, recipeDB(nullptr)
    {
        cout << "PlanManager initialized (Default max difficulty: A - All levels allowed)" << endl;
        cout << "⚠️ RecipeDatabase not connected. Call setRecipeDatabase()." << endl;
    }
    
    /**
     * 매개변수 생성자 (RecipeDatabase 연결 포함)
     * @param start 계획 시작 날짜
     * @param end 계획 종료 날짜
     * @param db RecipeDatabase 객체 포인터 (필수 협력자)
     * @param diffLevel 최대 난이도 제약 (기본값 'A' - 모든 난이도 허용)
     */
    // PlanManager(const Date& start, const Date& end, 
    //             RecipeDatabase* db, char diffLevel = 'A')
    //     : startDate(start), endDate(end), 
    //       recipeDB(db), maxDifficultyLevel(diffLevel)
    // {
    //     cout << "PlanManager created from " << start.toString() 
    //          << " to " << end.toString() << endl;
    //     cout << "Max difficulty: " << diffLevel 
    //          << " (" << getDifficultyDescription(diffLevel) << ")" << endl;
    //     
    //     if (recipeDB == nullptr) {
    //         cout << "⚠️ Warning: RecipeDatabase is null!" << endl;
    //     }
    // }
    
    /**
     * 소멸자
     * 주의: RecipeDatabase는 외부에서 관리하므로 delete 하지 않음
     */
    ~PlanManager()
    {
        cout << "PlanManager destroyed. Total planned days: " 
             << mealPlan.size() << endl;
    }

    // ==================== 유틸리티 함수 ====================
    
    /**
     * 난이도 문자를 설명 문자열로 변환
     * @param level 난이도 ('A', 'B', 'C')
     * @return 난이도 설명
     */
    string getDifficultyDescription(char level) const
    {
        switch(level)
        {
            case 'A': return "Hard - Complex recipes";
            case 'B': return "Medium - Moderate recipes";
            case 'C': return "Easy - Simple recipes";
            default: return "Unknown";
        }
    }

    // ==================== RecipeDatabase 연결 ====================
    
    /**
     * RecipeDatabase 설정 (필수!)
     * PlanManager가 난이도 기반 추천을 하려면 반드시 호출되어야 함
     * @param db RecipeDatabase 객체 포인터
     */
    // void setRecipeDatabase(RecipeDatabase* db)
    // {
    //     recipeDB = db;
    //     if (db != nullptr) {
    //         cout << "✅ RecipeDatabase connected to PlanManager" << endl;
    //     } else {
    //         cout << "⚠️ RecipeDatabase set to null" << endl;
    //     }
    // }

    // ==================== 계획 설정 및 관리 ====================
    
    /**
     * 계획 기간 설정
     */
    void setPlanPeriod(const Date& start, const Date& end)
    {
        startDate = start;
        endDate = end;
        cout << "Plan period set: " << start.toString() 
             << " to " << end.toString() << endl;
    }
    
    /**
     * 난이도 제약 설정
     * @param level 최대 허용 난이도
     *              'C': 쉬운 것만 (C만 추천)
     *              'B': 중간까지 (B, C 추천 가능)
     *              'A': 모든 난이도 (A, B, C 모두 추천 가능)
     */
    void setDifficultyConstraint(char level)
    {
        if (level == 'A' || level == 'B' || level == 'C')
        {
            maxDifficultyLevel = level;
            cout << "Maximum difficulty level set to: " << level 
                 << " (" << getDifficultyDescription(level) << ")" << endl;
            
            // 제약 설명
            cout << "Allowed recipes: ";
            if (level == 'C') cout << "C (Easy only)" << endl;
            else if (level == 'B') cout << "B (Medium), C (Easy)" << endl;
            else cout << "A (Hard), B (Medium), C (Easy) - All levels" << endl;
        }
        else
        {
            cout << "Invalid difficulty level. Use A, B, or C." << endl;
        }
    }
    
    // ==================== 식사 배정 기능 ====================
    
    /**
     * 특정 날짜에 특정 끼니(Meal) 추가
     */
    void addMealToDate(const string& dateStr, const Meal& meal)
    {
        mealPlan[dateStr].push_back(meal);
        cout << "Meal (" << meal.getMealType() << ") added to " 
             << dateStr << endl;
    }
    
    /**
     * 특정 날짜의 모든 식사 삭제
     */
    void removeMealsFromDate(const string& dateStr)
    {
        if (mealPlan.find(dateStr) != mealPlan.end())
        {
            mealPlan.erase(dateStr);
            cout << "All meals removed from " << dateStr << endl;
        }
        else
        {
            cout << "No meals found for " << dateStr << endl;
        }
    }
    
    // ==================== 난이도 기반 추천 기능 ====================
    
    /**
     * ✅ Greeter로부터 호출되는 핵심 추천 메소드
     * 
     * 호출 흐름:
     * 1. Greeter: 사용자가 "4. Recommendation" 선택
     * 2. Greeter: 사용자에게 난이도(A/B/C) 입력 요청
     * 3. Greeter: planManager->requestRecipesByDifficulty(입력값) 호출 ⬅️ 여기!
     * 4. PlanManager: RecipeDatabase에 검색 요청
     * 5. PlanManager: 결과를 Greeter에 반환
     * 6. Greeter: 결과를 사용자에게 표시
     * 
     * @param desiredLevel 사용자가 원하는 난이도 ('A', 'B', 'C')
     * @return 추천된 레시피 이름 목록
     */
    vector<string> requestRecipesByDifficulty(char desiredLevel)
    {
        vector<string> recommendations;
        
        cout << "\n===== Recipe Recommendation (Difficulty: " << desiredLevel << ") =====" << endl;
        
        // ❌ Step 1: RecipeDatabase 연결 확인 (필수!)
        // if (recipeDB == nullptr) {
        //     cout << "❌ Error: RecipeDatabase not connected!" << endl;
        //     cout << "   Cannot provide recommendations." << endl;
        //     return recommendations;
        // }
        
        // Step 2: 난이도 유효성 검사
        if (desiredLevel < 'A' || desiredLevel > 'C')
        {
            cout << "❌ Invalid difficulty level: " << desiredLevel << endl;
            cout << "   Please use A (Hard), B (Medium), or C (Easy)." << endl;
            return recommendations;
        }
        
        // Step 3: 제약 조건 확인
        // maxDifficultyLevel이 'C'인데 사용자가 'A'를 요청하면 거부
        if (desiredLevel < maxDifficultyLevel)
        {
            cout << "⚠️ Requested difficulty " << desiredLevel 
                 << " (" << getDifficultyDescription(desiredLevel) << ")" << endl;
            cout << "   exceeds your constraint " << maxDifficultyLevel 
                 << " (" << getDifficultyDescription(maxDifficultyLevel) << ")" << endl;
            cout << "   Only recipes up to " << maxDifficultyLevel 
                 << " difficulty are allowed." << endl;
            return recommendations;
        }
        
        cout << "🔍 Searching recipes with difficulty: " << desiredLevel 
             << " (" << getDifficultyDescription(desiredLevel) << ")..." << endl;
        
        // ✅ Step 4: RecipeDatabase에게 검색 요청 (핵심 협력!)
        // vector<Recipe*> recipes = recipeDB->findByDifficulty(desiredLevel);
        // 
        // if (recipes.empty()) {
        //     cout << "No recipes found with difficulty " << desiredLevel << endl;
        //     return recommendations;
        // }
        // 
        // // Recipe 객체들을 문자열로 변환
        // for (Recipe* recipe : recipes) {
        //     if (recipe != nullptr) {
        //         recommendations.push_back(recipe->getName());
        //     }
        // }
        
        // ⚠️ 임시 더미 데이터 (RecipeDatabase 완성 전)
        if (desiredLevel == 'A') {
            recommendations.push_back("Coq au Vin (French Chicken Stew)");
            recommendations.push_back("Beef Wellington");
            recommendations.push_back("Homemade Pasta with Seafood");
        } else if (desiredLevel == 'B') {
            recommendations.push_back("Chicken Stir-fry");
            recommendations.push_back("Spaghetti Carbonara");
            recommendations.push_back("Grilled Salmon");
        } else { // C
            recommendations.push_back("Scrambled Eggs");
            recommendations.push_back("Simple Salad");
            recommendations.push_back("Toast with Jam");
        }
        
        cout << "✅ Found " << recommendations.size() << " recipe(s):" << endl;
        for (size_t i = 0; i < recommendations.size(); i++) {
            cout << "   " << (i+1) << ". " << recommendations[i] << endl;
        }
        
        return recommendations;
    }
    
    /**
     * 전체 계획 기간에 대해 자동으로 식사 배정
     * RecipeDatabase로부터 난이도별로 레시피를 받아와서 균형있게 배치
     * 
     * 배정 전략:
     * - 아침: C 난이도 (쉬운 것) - 시간이 부족한 아침
     * - 점심: C 또는 B 혼합 - 적당한 에너지 필요
     * - 저녁: B 또는 A (시간 여유 있을 때) - 여유있게 요리 가능
     */
    void generateBalancedPlan()
    {
        cout << "\n===== Generating Balanced Meal Plan =====" << endl;
        cout << "Period: " << startDate.toString() << " to " 
             << endDate.toString() << endl;
        cout << "Max difficulty constraint: " << maxDifficultyLevel 
             << " (" << getDifficultyDescription(maxDifficultyLevel) << ")" << endl;
        
        // ❌ RecipeDatabase 확인
        // if (recipeDB == nullptr) {
        //     cout << "❌ Cannot generate plan: RecipeDatabase not connected" << endl;
        //     return;
        // }
        
        cout << "\n📝 Balanced Plan Strategy:" << endl;
        cout << "   - Breakfast: C difficulty (Easy - quick morning meals)" << endl;
        cout << "   - Lunch: B or C difficulty (Medium-Easy - energy boost)" << endl;
        cout << "   - Dinner: A or B difficulty (Hard-Medium - time available)" << endl;
        cout << "   (All within your constraint: " << maxDifficultyLevel << ")\n" << endl;
        
        // 실제 구현 시:
        // 1. 시작일부터 종료일까지 Date 객체 생성하며 순회
        // 2. 각 날짜마다:
        //    - 아침: requestRecipesByDifficulty('C') 호출
        //    - 점심: 'B' 또는 'C' (랜덤 또는 번갈아)
        //    - 저녁: 'A' 또는 'B' (단, maxDifficultyLevel 체크)
        // 3. 받아온 레시피로 Meal 객체 생성
        // 4. addMealToDate()로 계획에 추가
        //
        // 예시 의사코드:
        // Date currentDate = startDate;
        // while (currentDate <= endDate) {
        //     string dateStr = currentDate.toString();
        //     
        //     // 아침 - 쉬운 것
        //     vector<string> breakfastRecipes = requestRecipesByDifficulty('C');
        //     Meal breakfast("Breakfast");
        //     breakfast.addRecipe(breakfastRecipes[0]);  // 첫 번째 추천
        //     addMealToDate(dateStr, breakfast);
        //     
        //     // 점심 - 중간
        //     vector<string> lunchRecipes = requestRecipesByDifficulty('B');
        //     Meal lunch("Lunch");
        //     lunch.addRecipe(lunchRecipes[0]);
        //     addMealToDate(dateStr, lunch);
        //     
        //     // 저녁 - 어려운 것 (제약 내에서)
        //     char dinnerDiff = (maxDifficultyLevel == 'A') ? 'A' : 'B';
        //     vector<string> dinnerRecipes = requestRecipesByDifficulty(dinnerDiff);
        //     Meal dinner("Dinner");
        //     dinner.addRecipe(dinnerRecipes[0]);
        //     addMealToDate(dateStr, dinner);
        //     
        //     currentDate = nextDay(currentDate);  // 다음 날로
        // }
        
        cout << "[Auto plan generation will be implemented here]" << endl;
    }
    
    // ==================== 계획 조회 기능 ====================
    
    /**
     * 특정 날짜의 식사 계획 조회
     */
    void viewPlanForDate(const string& dateStr) const
    {
        auto it = mealPlan.find(dateStr);
        
        if (it == mealPlan.end() || it->second.empty())
        {
            cout << "No meal plan found for " << dateStr << endl;
            return;
        }
        
        cout << "\n===== Meal Plan for " << dateStr << " =====" << endl;
        for (size_t i = 0; i < it->second.size(); i++)
        {
            cout << "\n[Meal #" << (i + 1) << "]" << endl;
            it->second[i].display();
        }
    }
    
    /**
     * 전체 기간의 식사 계획 요약 출력
     */
    void viewFullPlan() const
    {
        if (mealPlan.empty())
        {
            cout << "No meal plans created yet." << endl;
            return;
        }
        
        cout << "\n===== Full Meal Plan Summary =====" << endl;
        cout << "Total planned days: " << mealPlan.size() << endl;
        cout << "Difficulty constraint: " << maxDifficultyLevel 
             << " (" << getDifficultyDescription(maxDifficultyLevel) << ")" << endl;
        
        for (const auto& dayPlan : mealPlan)
        {
            cout << "\n📅 " << dayPlan.first << " - " 
                 << dayPlan.second.size() << " meal(s)" << endl;
        }
    }
    
    // ==================== 통합 장보기 목록 생성 ====================
    
    /**
     * ✅ 전체 계획의 모든 재료를 통합한 장보기 목록 생성
     * 
     * 실제 구현 로직:
     * 1. 모든 날짜(mealPlan의 모든 키) 순회
     * 2. 각 날짜의 모든 Meal 객체 순회
     * 3. 각 Meal의 모든 Recipe 이름 순회
     * 4. RecipeDatabase에서 Recipe 객체 가져오기
     * 5. Recipe의 재료 목록(ingredients) 추출
     * 6. map<string, double>로 재료명과 수량 관리
     *    - 동일 재료: 수량 합산
     *    - 새 재료: 추가
     * 7. 최종 map을 vector<string>으로 변환하여 반환
     * 
     * @return 통합된 재료 목록 ("재료명 - 총수량" 형식)
     */
    vector<string> generateShoppingList() const
    {
        vector<string> shoppingList;
        
        cout << "\n===== Generating Shopping List =====" << endl;
        
        // 계획이 비어있는지 확인
        if (mealPlan.empty())
        {
            cout << "No meals planned. Shopping list is empty." << endl;
            return shoppingList;
        }
        
        // ✅ 재료와 수량을 저장할 맵
        // key: 재료 이름, value: 총 수량
        map<string, double> ingredientMap;
        
        // ✅ 실제 구현 로직
        // Step 1: 모든 날짜 순회
        for (const auto& dayPlan : mealPlan)
        {
            const string& dateStr = dayPlan.first;
            const vector<Meal>& mealsOfDay = dayPlan.second;
            
            cout << "📅 Processing " << dateStr << " (" 
                 << mealsOfDay.size() << " meal(s))..." << endl;
            
            // Step 2: 해당 날짜의 모든 Meal 순회
            for (const Meal& meal : mealsOfDay)
            {
                cout << "   🍽️  " << meal.getMealType() << " (" 
                     << meal.getServings() << " serving(s))" << endl;
                
                // ⚠️ Meal 클래스에 Recipe 접근 메소드 필요!
                // 
                // 현재 Meal.h를 보면:
                // - recipes는 vector<string>으로 레시피 "이름"만 저장
                // - Recipe 객체가 아님
                //
                // 따라서 실제 구현 시 필요한 것:
                // 1. Meal에 getRecipes() 메소드 추가 (vector<string> 반환)
                // 2. 각 레시피 이름으로 RecipeDatabase 쿼리
                // 3. Recipe 객체에서 getIngredients() 호출
                //    (Recipe 클래스에 map<string, double> 형태로 재료 저장 필요)
                // 4. 재료들을 ingredientMap에 합산
                
                // const vector<string>& recipeNames = meal.getRecipes();
                // 
                // for (const string& recipeName : recipeNames)
                // {
                //     cout << "      🔍 Looking up recipe: " << recipeName << endl;
                //     
                //     // RecipeDatabase에서 Recipe 객체 검색
                //     Recipe* recipe = recipeDB->findByName(recipeName);
                //     
                //     if (recipe != nullptr)
                //     {
                //         // Recipe에서 재료 목록 가져오기
                //         // Recipe 클래스에 이런 메소드가 있어야 함:
                //         // map<string, double> getIngredients() const;
                //         // 
                //         // map<string, double> ingredients = recipe->getIngredients();
                //         
                //         // 인분 수 고려하여 수량 조정
                //         int servings = meal.getServings();
                //         
                //         // for (const auto& ingredient : ingredients)
                //         // {
                //         //     string ingredientName = ingredient.first;
                //         //     double baseAmount = ingredient.second;  // 1인분 기준
                //         //     double totalAmount = baseAmount * servings;
                //         //     
                //         //     // 동일 재료가 있으면 수량 합산, 없으면 추가
                //         //     ingredientMap[ingredientName] += totalAmount;
                //         //     
                //         //     cout << "         ✅ " << ingredientName 
                //         //          << ": +" << totalAmount << endl;
                //         // }
                //     }
                //     else
                //     {
                //         cout << "      ⚠️ Recipe not found: " << recipeName << endl;
                //     }
                // }
            }
        }
        
        // Step 3: map을 vector<string>으로 변환
        // cout << "\n📋 Consolidating ingredients..." << endl;
        // for (const auto& ingredient : ingredientMap)
        // {
        //     stringstream ss;
        //     ss << ingredient.first << " - " << ingredient.second;
        //     // 단위 추가하면 더 좋음 (예: "g", "ml", "개")
        //     // Recipe에 단위 정보도 저장되어 있다면:
        //     // ss << ingredient.first << " - " << ingredient.second << "g";
        //     shoppingList.push_back(ss.str());
        // }
        
        // ⚠️ 현재는 더미 데이터 (위 로직 구현 후 삭제)
        shoppingList.push_back("Chicken breast - 2000g");
        shoppingList.push_back("Mushrooms - 500g");
        shoppingList.push_back("Cheese - 300g");
        shoppingList.push_back("Eggs - 12개");
        shoppingList.push_back("[NOTE: Actual consolidation logic needed]");
        shoppingList.push_back("[Requires: Recipe.getIngredients() method]");
        shoppingList.push_back("[Requires: Meal.getRecipes() method]");
        
        cout << "\n✅ Shopping list generated with " << shoppingList.size() 
             << " item(s)." << endl;
        
        return shoppingList;
    }
    
    /**
     * 장보기 목록을 화면에 출력
     */
    void displayShoppingList() const
    {
        vector<string> list = generateShoppingList();
        
        if (list.empty())
        {
            cout << "\n🛒 Shopping list is empty." << endl;
            return;
        }
        
        cout << "\n╔════════════════════════════════╗" << endl;
        cout << "║    🛒 Shopping List 🛒         ║" << endl;
        cout << "╚════════════════════════════════╝" << endl;
        for (size_t i = 0; i < list.size(); i++)
        {
            cout << " [" << (i + 1) << "] " << list[i] << endl;
        }
        cout << "═════════════════════════════════" << endl;
    }
    
    // ==================== Getter 함수들 ====================
    
    /**
     * 현재 설정된 최대 난이도 반환
     */
    char getMaxDifficulty() const { return maxDifficultyLevel; }
    
    /**
     * 계획된 날짜 수 반환
     */
    int getPlannedDaysCount() const { return mealPlan.size(); }
    
    /**
     * 시작 날짜 반환
     */
    Date getStartDate() const { return startDate; }
    
    /**
     * 종료 날짜 반환
     */
    Date getEndDate() const { return endDate; }
};

#endif // PLANMANAGER_H
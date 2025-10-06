#ifndef PLANMANAGER_H
#define PLANMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Date.h"
#include "Meal.h"
#include "RecipeDatabase.h"
#include "Recipe.h"
#include <iomanip> // 1. <iomanip> 헤더를 추가 10-05 pm10:45
#include <sstream> // 1. stringstream 사용 위해 추가 10-05 pm10:45
#include <cstdlib> // ⭐️ rand, srand 함수 사용을 위해 추가
#include <ctime>   // ⭐️ time 함수 사용을 위해 추가

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
 * 난이도 정의 (Difficulty enum):
 * - A: 어려움 (Advanced/Hard - 복잡한 레시피, 시간 많이 소요)
 * - B: 보통 (Medium - 중간 난이도)
 * - C: 쉬움 (Easy - 간단한 레시피, 빠른 조리)
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
    // 예: Difficulty::C로 설정 시 -> C(쉬움)만 추천 가능
    //     Difficulty::B로 설정 시 -> B(보통), C(쉬움) 추천 가능
    //     Difficulty::A로 설정 시 -> A(어려움), B(보통), C(쉬움) 모두 추천 가능
    Difficulty maxDifficultyLevel;
    
    // RecipeDatabase 객체 포인터 (필수 협력자!)
    RecipeDatabase* recipeDB;

public:
    // ==================== 생성자 및 소멸자 ====================
    
    /**
     * 기본 생성자
     * 기본값: 난이도 Difficulty::A (모든 난이도 허용)
     */
    PlanManager() : maxDifficultyLevel(Difficulty::A), recipeDB(nullptr)
    {
        cout << "PlanManager initialized (Default max difficulty: A - All levels allowed)" << endl;
        cout << "⚠️ RecipeDatabase not connected. Call setRecipeDatabase()." << endl;
    }
    
    /**
     * 매개변수 생성자 (RecipeDatabase 연결 포함)
     */
    PlanManager(const Date& start, const Date& end, 
                RecipeDatabase* db, Difficulty diffLevel = Difficulty::A)
        : startDate(start), endDate(end), 
          recipeDB(db), maxDifficultyLevel(diffLevel)
    {
        cout << "PlanManager created from " << start.toString() 
             << " to " << end.toString() << endl;
        cout << "Max difficulty: " << getDifficultyDescription(diffLevel) << endl;
        
        if (recipeDB == nullptr) {
            cout << "⚠️ Warning: RecipeDatabase is null!" << endl;
        }
    }
    
    /**
     * 소멸자
     */
    ~PlanManager()
    {
        cout << "PlanManager destroyed. Total planned days: " 
             << mealPlan.size() << endl;
    }

    // ==================== 유틸리티 함수 ====================
    
    /**
     * Difficulty enum을 설명 문자열로 변환
     */
    string getDifficultyDescription(Difficulty level) const
    {
        switch(level)
        {
            case Difficulty::A: return "A (Advanced/Hard)";
            case Difficulty::B: return "B (Intermediate/Medium)";
            case Difficulty::C: return "C (Beginner/Easy)";
            default: return "Unknown";
        }
    }
    
    /**
     * 문자를 Difficulty enum으로 변환
     */
    Difficulty charToDifficulty(char c) const
    {
        switch(c)
        {
            case 'A': case 'a': return Difficulty::A;
            case 'B': case 'b': return Difficulty::B;
            case 'C': case 'c': return Difficulty::C;
            default: return Difficulty::A;
        }
    }

    // ==================== RecipeDatabase 연결 ====================
    
    /**
     * RecipeDatabase 설정 (필수!)
     */
    void setRecipeDatabase(RecipeDatabase* db)
    {
        recipeDB = db;
        if (db != nullptr) {
            cout << "✅ RecipeDatabase connected to PlanManager" << endl;
        } else {
            cout << "⚠️ RecipeDatabase set to null" << endl;
        }
    }

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
     */
    void setDifficultyConstraint(char level)
    {
        Difficulty newLevel = charToDifficulty(level);
        maxDifficultyLevel = newLevel;
        
        cout << "Maximum difficulty level set to: " 
             << getDifficultyDescription(newLevel) << endl;
        
        cout << "Allowed recipes: ";
        if (newLevel == Difficulty::C) 
            cout << "C (Easy only)" << endl;
        else if (newLevel == Difficulty::B) 
            cout << "B (Medium), C (Easy)" << endl;
        else 
            cout << "A (Hard), B (Medium), C (Easy) - All levels" << endl;
    }
    
    // ==================== 식사 배정 기능 ====================
    
    /**
     * 기존 메소드: Meal 객체를 직접 추가
     */
    void addMealToDate(const string& dateStr, const Meal& meal)
    {
        mealPlan[dateStr].push_back(meal);
        
        cout << "Meal added to " << dateStr << ":" << endl;
        cout << "  - Type: " << meal.getMealType() << endl;
        cout << "  - Servings: " << meal.getServings() << endl;
        
        // 추가된 레시피 목록 출력
        const vector<string>& recipeNames = meal.getRecipeTitles();
        if (!recipeNames.empty()) {
            cout << "  - Recipes: ";
            for (size_t i = 0; i < recipeNames.size(); i++) {
                cout << recipeNames[i];
                if (i < recipeNames.size() - 1) cout << ", ";
            }
            cout << endl;
        } else {
            cout << "  - No recipes added yet" << endl;
        }
    }
    
    /**
     * 🆕 편의 메소드 1: 날짜 + 끼니 타입 + 레시피 이름
     * 
     * 사용자가 "2024-10-05 저녁에 김치찌개 추가" 같은 형태로 간편하게 추가
     */
    void addRecipeToMeal(const string& dateStr, 
                     const string& mealType, 
                     const string& recipeName, 
                     int servings = 1)
{
    if (recipeDB == nullptr) {
        cout << "❌ Error: RecipeDatabase not connected!" << endl;
        return;
    }
    
    bool found = false;
    
    // 1. 이미 존재하는 Meal에 레시피 추가 시도
    if (mealPlan.find(dateStr) != mealPlan.end()) {
        for (Meal& meal : mealPlan[dateStr]) {
            if (meal.getMealType() == mealType) {
                meal.addRecipe(*recipeDB, recipeName);
                found = true;
                break;
            }
        }
    }
    
    // 2. Meal이 존재하지 않아 새로 생성해야 하는 경우
    if (!found) {
        // 새로운 Meal 객체 생성 및 레시피 추가 후 저장
        Meal newMeal(mealType, servings);
        newMeal.addRecipe(*recipeDB, recipeName);
        mealPlan[dateStr].push_back(newMeal);

        // ✅ 상세 피드백: 새로 생성됨
        cout << "✅ New " << mealType << " created and recipe '" << recipeName 
             << "' added on " << dateStr << endl;

    } else {
        // ✅ 상세 피드백: 기존 Meal에 추가됨
        cout << "✅ Recipe '" << recipeName << "' added to existing " 
             << mealType << " on " << dateStr << endl;
    }
}
    
    /**
     * 🆕 편의 메소드 3: 여러 레시피 한번에 추가
     */
    void addMultipleRecipesToMeal(const string& dateStr,
                                   const string& mealType,
                                   const vector<string>& recipeNames,
                                   int servings = 1)
    {
        if (recipeDB == nullptr) {
            cout << "❌ Error: RecipeDatabase not connected!" << endl;
            return;
        }
        
        Meal newMeal(mealType, servings);
        
        for (const string& recipeName : recipeNames) {
            newMeal.addRecipe(*recipeDB, recipeName);
        }
        
        mealPlan[dateStr].push_back(newMeal);
        
        cout << mealType << " added to " << dateStr << " with " 
             << recipeNames.size() << " recipe(s)" << endl;
    }
    
    /**
     * 특정 날짜의 모든 식사 삭제
     */
    void removeMealsFromDate(const string& dateStr)
    {
        if (mealPlan.find(dateStr) != mealPlan.end()) {
            mealPlan.erase(dateStr);
            cout << "All meals removed from " << dateStr << endl;
        } else {
            cout << "No meals found for " << dateStr << endl;
        }
    }
    
    // ==================== 난이도 기반 추천 기능 ====================
    
    /**
     * ✅ Greeter로부터 호출되는 핵심 추천 메소드
     * 
     * @param desiredLevel 사용자가 원하는 난이도 문자 ('A', 'B', 'C')
     * @return 추천된 레시피 이름 목록
     */
    vector<string> requestRecipesByDifficulty(char desiredLevel)
    {
        vector<string> recommendations;
        
        cout << "\n===== Recipe Recommendation (Difficulty: " << desiredLevel << ") =====" << endl;
        
        if (recipeDB == nullptr) {
            cout << "❌ Error: RecipeDatabase not connected!" << endl;
            return recommendations;
        }
        
        Difficulty targetDiff = charToDifficulty(desiredLevel);
        
        // 제약 조건 확인
        if (targetDiff < maxDifficultyLevel) {
            cout << "⚠️ Requested difficulty " << getDifficultyDescription(targetDiff) << endl;
            cout << "   exceeds your constraint " << getDifficultyDescription(maxDifficultyLevel) << endl;
            cout << "   Only recipes up to " << getDifficultyDescription(maxDifficultyLevel) 
                 << " are allowed." << endl;
            return recommendations;
        }
        
        cout << "🔍 Searching recipes with difficulty: " 
             << getDifficultyDescription(targetDiff) << "..." << endl;
        
        // RecipeDatabase에서 모든 레시피 가져와서 필터링
        const vector<Recipe>& allRecipes = recipeDB->getRecipes();
        
        for (const Recipe& recipe : allRecipes) {
            if (recipe.getDifficulty() == targetDiff) {
                recommendations.push_back(recipe.getTitle());
            }
        }
        
        if (recommendations.empty()) {
            cout << "No recipes found with difficulty " << desiredLevel << endl;
        } else {
            cout << "✅ Found " << recommendations.size() << " recipe(s):" << endl;
            for (size_t i = 0; i < recommendations.size(); i++) {
                cout << "   " << (i+1) << ". " << recommendations[i] << endl;
            }
        }
        
        return recommendations;
    }
    
    /**
     * 전체 계획 기간에 대해 자동으로 식사 배정
     *
     * 배정 전략:
     * - 아침: C 난이도 (쉬운 것)
     * - 점심: C 또는 B 혼합
     * - 저녁: B 또는 A (시간 여유)
     * - 모든 배정은 사용자가 설정한 최대 난이도(maxDifficultyLevel) 제약을 따름
     */
    void generateBalancedPlan()
    {
        cout << "\n===== Generating Balanced Meal Plan =====" << endl;
        cout << "Period: " << startDate.toString() << " to "
             << endDate.toString() << endl;
        cout << "Max difficulty constraint: " << getDifficultyDescription(maxDifficultyLevel) << endl;

        if (recipeDB == nullptr) {
            cout << "❌ Cannot generate plan: RecipeDatabase not connected" << endl;
            return;
        }

        // 1. 난이도별로 레시피 목록을 미리 분류합니다. (사용자 제약 조건 반영)
        vector<Recipe> easyRecipes, mediumRecipes, hardRecipes;
        for (const auto& recipe : recipeDB->getRecipes()) {
            switch (recipe.getDifficulty()) {
                case Difficulty::C:
                    easyRecipes.push_back(recipe);
                    break;
                case Difficulty::B:
                    if (maxDifficultyLevel != Difficulty::C)
                        mediumRecipes.push_back(recipe);
                    break;
                case Difficulty::A:
                    if (maxDifficultyLevel == Difficulty::A)
                        hardRecipes.push_back(recipe);
                    break;
            }
        }
        
        // 각 끼니에 사용할 수 있는 레시피 목록 준비
        vector<Recipe> breakfastPool = easyRecipes;
        vector<Recipe> lunchPool;
        lunchPool.insert(lunchPool.end(), easyRecipes.begin(), easyRecipes.end());
        lunchPool.insert(lunchPool.end(), mediumRecipes.begin(), mediumRecipes.end());
        vector<Recipe> dinnerPool;
        dinnerPool.insert(dinnerPool.end(), mediumRecipes.begin(), mediumRecipes.end());
        dinnerPool.insert(dinnerPool.end(), hardRecipes.begin(), hardRecipes.end());

        // 2. 레시피가 충분한지 확인합니다.
        if (breakfastPool.empty() || lunchPool.empty() || dinnerPool.empty()) {
            cout << "❌ Cannot generate plan: Not enough recipes for each meal type within the difficulty constraint." << endl;
            if(breakfastPool.empty()) cout << "   - No easy recipes for breakfast." << endl;
            if(lunchPool.empty()) cout << "   - No easy/medium recipes for lunch." << endl;
            if(dinnerPool.empty()) cout << "   - No medium/hard recipes for dinner." << endl;
            return;
        }
        
        cout << "\n📝 Balanced Plan Strategy:" << endl;
        cout << "   - Breakfast: C difficulty (Easy)" << endl;
        cout << "   - Lunch: B or C difficulty (Medium-Easy)" << endl;
        cout << "   - Dinner: A or B difficulty (Hard-Medium)" << endl;
        cout << "   (All within your constraint: " << getDifficultyDescription(maxDifficultyLevel) << ")\n" << endl;

        // 3. 시작일부터 종료일까지 하루씩 반복하며 계획 생성
        srand(time(0)); // 매번 다른 결과를 위해 난수 시드 초기화
        Date currentDate = startDate;
        while (currentDate <= endDate) {
            string dateStr = currentDate.toString().substr(0, 10); // "YYYY-MM-DD" 부분만 추출
            
            cout << "🗓️ Generating plan for " << dateStr << "..." << endl;
            
            // 해당 날짜의 기존 계획을 초기화
            mealPlan[dateStr].clear();

            // 아침: 쉬움(C) 레시피 중 무작위 선택
            string breakfastRecipe = breakfastPool[rand() % breakfastPool.size()].getTitle();
            addRecipeToMeal(dateStr, "Breakfast", breakfastRecipe, 1);

            // 점심: 쉬움(C) 또는 보통(B) 레시피 중 무작위 선택
            string lunchRecipe = lunchPool[rand() % lunchPool.size()].getTitle();
            addRecipeToMeal(dateStr, "Lunch", lunchRecipe, 1);
            
            // 저녁: 보통(B) 또는 어려움(A) 레시피 중 무작위 선택
            string dinnerRecipe = dinnerPool[rand() % dinnerPool.size()].getTitle();
            addRecipeToMeal(dateStr, "Dinner", dinnerRecipe, 1);
            
            // 다음 날짜로 이동
            currentDate.incrementDay();
        }
        
        cout << "\n✅ Balanced meal plan generated successfully!" << endl;
    }
    
    // ==================== 계획 조회 기능 ====================
    
    /**
     * 특정 날짜의 식사 계획 조회
     */
    void viewPlanForDate(const string& dateStr) const
    {
        auto it = mealPlan.find(dateStr);
        
        if (it == mealPlan.end() || it->second.empty()) {
            cout << "No meal plan found for " << dateStr << endl;
            return;
        }
        
        cout << "\n===== Meal Plan for " << dateStr << " =====" << endl;
        for (const Meal& meal : it->second)
        {
            // ⭐️ 핵심 변경점: meal.display() 대신 meal.displaySummary() 호출
            meal.displaySummary(); 
        }
    }
    
    /**
     * 전체 기간의 식사 계획 요약 출력
     */
    void viewFullPlan() const
    {
        if (mealPlan.empty()) {
            cout << "No meal plans created yet." << endl;
            return;
        }
        
        cout << "\n===== Full Meal Plan Summary =====" << endl;
        cout << "Total planned days: " << mealPlan.size() << endl;
        cout << "Difficulty constraint: " << getDifficultyDescription(maxDifficultyLevel) << endl;
        
        for (const auto& dayPlan : mealPlan) {
            cout << "\n📅 " << dayPlan.first << " - " 
                 << dayPlan.second.size() << " meal(s)" << endl;
        }
    }
    
    // ==================== 통합 장보기 목록 생성 ====================
    
    /**
     * ✅ 전체 계획의 모든 재료를 통합한 장보기 목록 생성
     * 
     * Meal이 Recipe 객체를 직접 가지고 있으므로 구현 간단해짐!
     */
    vector<string> generateShoppingList() const
    {
        vector<string> shoppingList;
        
        cout << "\n===== Generating Shopping List =====" << endl;
        
        if (mealPlan.empty()) {
            cout << "No meals planned. Shopping list is empty." << endl;
            return shoppingList;
        }
        
        // 재료와 수량을 저장할 맵
        map<string, double> ingredientCount;  // 재료명 -> 개수
        
        // Step 1: 모든 날짜 순회
        for (const auto& dayPlan : mealPlan) {
            const string& dateStr = dayPlan.first;
            const vector<Meal>& mealsOfDay = dayPlan.second;
            
            cout << "📅 Processing " << dateStr << " (" 
                 << mealsOfDay.size() << " meal(s))..." << endl;
            
            // Step 2: 각 Meal 순회
            for (const Meal& meal : mealsOfDay) {
                cout << "   🍽️  " << meal.getMealType() << " (" 
                     << meal.getServings() << " serving(s))" << endl;
                
                // Step 3: Meal에서 Recipe 객체들 가져오기
                const vector<Recipe>& recipes = meal.getRecipes();
                int servings = meal.getServings();
                
                // Step 4: 각 Recipe에서 재료 추출
                for (const Recipe& recipe : recipes) {
                    cout << "      🔍 Recipe: " << recipe.getTitle() << endl;
                    
                    // string vector가 아닌 Ingredient vector로 변경됨 10-05 pm10:45
                    const vector<Ingredient>& ingredients = recipe.getIngredient();
                    
                    // Step 5: 재료 카운트 (인분 수만큼 곱하기) vector<Ingredient>로 변경됨 10-05 pm10:45
                    // [수정] vector<Ingredient>를 올바르게 순회합니다.
                for (const Ingredient& ingredient : recipe.getIngredient()) {
                    
                    // 양(quantity)이 0보다 큰 재료만 장보기 목록에 추가
                    if (ingredient.quantity > 0) {
                        // [수정] '이름 (단위)'를 고유한 Key로 사용합니다.
                        // 예: "flour (cup)", "egg (ea)"
                        string key = ingredient.name + " (" + ingredient.unit + ")";
                        
                        // [수정] '재료의 양 * 인분 수'를 총량에 더해줍니다.
                        ingredientCount[key] += ingredient.quantity * servings;
                    }
                }
                }
            }
        }
        // ingredient 재료 수를 double 로 바꿔서 소수점 2자리까지만 출력하도록 변경
        // Step 6: map을 vector로 변환
        cout << "\n📋 Consolidating ingredients..." << endl;
        for (const auto& item : ingredientCount) {
            stringstream ss;
    
            // 2. cout에 하던 것과 똑같이, 소수점 2자리로 포맷 지정
            ss << fixed << setprecision(2) << item.second;
    
            // 3. 포맷팅이 완료된 문자열을 추출
            string formatted_quantity = ss.str();

            // 4. to_string 대신 포맷팅된 문자열을 사용하여 최종 entry 생성
            string entry = item.first + " (needed for " + formatted_quantity + " serving(s))";
    
            shoppingList.push_back(entry);
        }
        
        cout << "\n✅ Shopping list generated with " << shoppingList.size() 
             << " unique item(s)." << endl;
        
        return shoppingList;
    }
    
    /**
     * 장보기 목록을 화면에 출력
     */
    void displayShoppingList() const
    {
        vector<string> list = generateShoppingList();
        
        if (list.empty()) {
            cout << "\n Shopping list is empty." << endl;
            return;
        }
        
        cout << "\n╔════════════════════════════════╗" << endl;
        cout << "║    🛒 Shopping List 🛒         ║" << endl;
        cout << "╚════════════════════════════════╝" << endl;
        for (size_t i = 0; i < list.size(); i++) {
            cout << " [" << (i + 1) << "] " << list[i] << endl;
        }
        cout << "═════════════════════════════════" << endl;
    }
    
    // ==================== Getter 함수들 ====================
    
    Difficulty getMaxDifficulty() const { return maxDifficultyLevel; }
    int getPlannedDaysCount() const { return mealPlan.size(); }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
};

#endif // PLANMANAGER_H
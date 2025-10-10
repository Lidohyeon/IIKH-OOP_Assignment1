#ifndef GREETER_H
#define GREETER_H

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <ctime>
#include "RecipeDatabase.h" // 데이터베이스 클래스를 사용하기 위해 포함
#include "PlanManager.h"
#include "Date.h"
#include "Meal.h"

using namespace std;

class Greeter
{
private:
    RecipeDatabase &db;          // 데이터베이스 객체에 대한 '참조'
    PlanManager *planManager;    // PlanManager 객체에 대한 포인터
    std::vector<Date> schedules; // 일정을 저장할 벡터

    //===== Private Helper Functions (UI 처리) =====

    void showWelcome()
    {
        std::cout << "\n";
        std::cout << "██╗██╗██╗██╗  ██╗██╗  ██╗" << std::endl;
        std::cout << "██║██║██║██║ ██╔╝██║  ██║" << std::endl;
        std::cout << "██║██║██║█████╔╝ ███████║" << std::endl;
        std::cout << "██║██║██║██╔═██╗ ██╔══██║" << std::endl;
        std::cout << "██║██║██║██║  ██╗██║  ██║" << std::endl;
        std::cout << "╚═╝╚═╝╚═╝╚═╝  ╚═╝╚═╝  ╚═╝" << std::endl;
        std::cout << "\nWelcome to IIKH Data Management System!" << std::endl;
        std::cout << "=========================================" << std::endl;
    }

    void showMenu()
    {
        std::cout << "\n===== Main Menu =====\n";
        std::cout << "1. Insert New Recipe\n";
        std::cout << "2. Search for a Recipe\n";
        std::cout << "3. Delete a Recipe\n";
        std::cout << "4. Edit a Recipe\n";
        std::cout << "5. Sort Recipes by Title\n";
        std::cout << "6. List All Recipes\n";
        std::cout << "7. Recommendation\n";
        std::cout << "8. Schedule Management\n";
        std::cout << "9. Meal Management\n";
        std::cout << "10. Plan Manager\n";
        std::cout << "11. Save and Exit\n";
        std::cout << "==========================\n";
        std::cout << "> ";
    }

    void handleInsert()
    {
        db.insertRecipe(); // 데이터베이스 객체에게 삽입을 요청
    }

    void handleSearch()
    {
        db.searchRecipe(); // 데이터베이스 객체에게 검색을 요청
    }

    void handleDelete()
    {
        db.deleteRecipe(); // 데이터베이스 객체에게 삭제를 요청
    }

    void handleEdit()
    {
        db.editRecipe(); // 데이터베이스 객체에게 수정을 요청
    }

    void handleSort()
    {
        db.sortRecipe(); // 데이터베이스 객체에게 정렬을 요청
        std::cout << "Recipes have been sorted." << std::endl;
        // db.displayAll();
        //  정렬된 결과를 보여줌을 없앴습니다.sort 할 때 안나올겁니다! 10-05 pm10:45
    }

public:
    // 생성자: 사용할 데이터베이스를 외부에서 받음
    Greeter(RecipeDatabase &database) : db(database)
    {
        // 현재 시간 가져오기
        time_t now = time(0);
        tm *ltm = localtime(&now);

        // 현재 날짜 Date 객체 생성
        Date startDate(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, 0, 0, "Plan Start");

        // 1주일 후 날짜 계산
        time_t oneWeekLater = now + (7 * 24 * 60 * 60); // 7일 * 24시간 * 60분 * 60초
        tm *endTm = localtime(&oneWeekLater);
        Date endDate(1900 + endTm->tm_year, 1 + endTm->tm_mon, endTm->tm_mday, 23, 59, "Plan End");

        // PlanManager 생성 (시작날짜, 종료날짜 전달)
        planManager = new PlanManager(startDate, endDate);
        planManager->setRecipeDatabase(&db);
        planManager->setSharedSchedules(&schedules); // 스케줄 공유 설정
    }

    // 메인 메뉴 실행 함수
    void run()
    {
        showWelcome();

        int choice;
        while (true)
        {
            showMenu();
            std::cin >> choice;
            std::cin.ignore(); // 버퍼 클리어

            switch (choice)
            {
            case 1:
                handleInsert();
                break;
            case 2:
                handleSearch();
                break;
            case 3:
                handleDelete();
                break;
            case 4:
                handleEdit();
                break;
            case 5:
                handleSort();
                break;
            case 6:
                db.displayAll();
                break;
            case 7:
                handleRecommendation();
                break;
            case 8:
                scheduleMenu();
                break;
            case 9:
                mealMenu();
                break;
            case 10:
                planManagerMenu(); // Plan Manager 추가
                break;
            case 11:
                std::cout << "Saving and exiting..." << std::endl;
                db.saveToFile();
                std::cout << "Data saved successfully. Goodbye!" << std::endl;
                return;
            default:
                std::cout << "Invalid selection. Please try again." << std::endl;
                break;
            }
        }
    }

    // 추천 함수 (RecipeDatabase 기반)
    void handleRecommendation()
    {
        std::cout << "\n===== Recipe Recommendation =====" << std::endl;
        std::cout << "Select your cooking difficulty level:" << std::endl;
        std::cout << "A. Advanced (All difficulty levels)" << std::endl;
        std::cout << "B. Intermediate (Medium and Easy only)" << std::endl;
        std::cout << "C. Beginner (Easy only)" << std::endl;
        std::cout << "Please select (A/B/C): ";

        char difficultyChoice;
        std::cin >> difficultyChoice;

        // 대소문자 통일
        if (difficultyChoice >= 'a' && difficultyChoice <= 'c')
        {
            difficultyChoice = difficultyChoice - 'a' + 'A';
        }

        if (difficultyChoice != 'A' && difficultyChoice != 'B' && difficultyChoice != 'C')
        {
            std::cout << "Invalid selection. Please try again." << std::endl;
            return;
        }

        // RecipeDatabase에서 해당 난이도의 레시피들 가져오기
        std::vector<std::string> availableRecipes = getRecipesByDifficultyLevel(difficultyChoice);

        std::string difficultyLevel;
        switch (difficultyChoice)
        {
        case 'A':
            difficultyLevel = "Advanced (All levels)";
            break;
        case 'B':
            difficultyLevel = "Intermediate (Medium & Easy)";
            break;
        case 'C':
            difficultyLevel = "Beginner (Easy only)";
            break;
        }

        std::cout << "\n===== " << difficultyLevel << " Recommendations =====" << std::endl;

        if (availableRecipes.empty())
        {
            std::cout << "No recipes found for the selected difficulty level." << std::endl;
            std::cout << "Please add some recipes to the database first!" << std::endl;
            return;
        }

        // 랜덤하게 최대 5개 추천
        std::vector<std::string> recommendedRecipes;

        if (availableRecipes.size() <= 5)
        {
            // 5개 이하면 모든 레시피 추천
            recommendedRecipes = availableRecipes;
        }
        else
        {
            // 5개 초과면 랜덤하게 5개 선택
            std::vector<std::string> temp = availableRecipes;
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(temp.begin(), temp.end(), g);

            for (int i = 0; i < 5; i++)
            {
                recommendedRecipes.push_back(temp[i]);
            }
        }

        // 추천 레시피 출력
        for (size_t i = 0; i < recommendedRecipes.size(); i++)
        {
            std::cout << "⭐ " << (i + 1) << ". " << recommendedRecipes[i] << std::endl;
        }

        std::cout << "\nTotal " << recommendedRecipes.size() << " recipes recommended!" << std::endl;
        std::cout << "(" << availableRecipes.size() << " recipes available for " << difficultyLevel << ")" << std::endl;
    }

private:
    // RecipeDatabase에서 난이도별 레시피 검색 (private helper 함수)
    std::vector<std::string> getRecipesByDifficultyLevel(char maxLevel)
    {
        // RecipeDatabase에서 실제 레시피를 검색
        return db.getRecipesByDifficultyLevel(maxLevel);
    }

    // 일정 확인 함수
    void viewSchedule()
    {
        if (schedules.empty())
        {
            std::cout << "No schedules available." << std::endl;
            return;
        }

        std::cout << "\n===== Your Schedules =====" << std::endl;
        for (size_t i = 0; i < schedules.size(); i++)
        {
            std::cout << "[" << i + 1 << "] " << schedules[i].toString() << std::endl;
        }
        std::cout << "\nTotal schedules: " << schedules.size() << std::endl;
    }

    // 일정 추가 함수
    void addSchedule()
    {
        std::string scheduleInput;
        std::cout << "Enter new schedule (e.g., 2024-10-02 14:00 - Meeting with team - Recipe_name): ";
        std::getline(std::cin, scheduleInput);

        // Date 객체 생성
        Date newSchedule(scheduleInput);
        schedules.push_back(newSchedule);

        std::cout << "Schedule added successfully!" << std::endl;
        std::cout << "Total schedules: " << schedules.size() << std::endl;

        // 방금 추가된 일정 확인
        std::cout << "Added: " << newSchedule.toString() << std::endl;
    }

public:
    // 일정 관리 메뉴 함수
    void scheduleMenu()
    {
        int choice;

        while (true)
        {
            std::cout << "\n===== Schedule Management =====" << std::endl;
            std::cout << "1. View Schedules" << std::endl;
            std::cout << "2. Add Schedule" << std::endl;
            std::cout << "3. Back to Main Menu" << std::endl;
            std::cout << "Please select: ";

            std::cin >> choice;
            std::cin.ignore(); // 버퍼 클리어

            switch (choice)
            {
            case 1:
                viewSchedule();
                break;
            case 2:
                addSchedule();
                break;
            case 3:
                std::cout << "Returning to main menu..." << std::endl;
                return;
            default:
                std::cout << "Invalid selection. Please try again." << std::endl;
                break;
            }
        }
    }

    // Meal 관리 메뉴 함수
    void mealMenu()
    {
        std::cout << "\n===== Meal Management =====" << std::endl;
        std::cout << "Select meal type (1: Breakfast, 2: Lunch, 3: Dinner): ";
        int t;
        std::cin >> t;
        string type = (t == 1 ? "Breakfast" : (t == 2 ? "Lunch" : "Dinner"));
        Meal meal(type);

        while (true)
        {
            std::cout << "\n===== " << type << " Menu =====" << std::endl;
            std::cout << "1. Add Recipe" << std::endl;
            std::cout << "2. Remove Recipe" << std::endl;
            std::cout << "3. Set Servings" << std::endl;
            std::cout << "4. Display Meal" << std::endl;
            std::cout << "5. Back" << std::endl;
            std::cout << "Select: ";
            int c;
            std::cin >> c;

            if (c == 1)
            {
                string r;
                std::cout << "Enter recipe name: ";
                std::cin.ignore();
                getline(std::cin, r);
                meal.addRecipe(db, r);
            }
            else if (c == 2)
            {
                string r;
                std::cout << "Enter recipe name to remove: ";
                std::cin.ignore();
                getline(std::cin, r);
                meal.removeRecipe(r);
            }
            else if (c == 3)
            {
                int s;
                std::cout << "Enter servings: ";
                std::cin >> s;
                meal.setServings(s);
            }
            else if (c == 4)
            {
                meal.display();
            }
            else if (c == 5)
            {
                std::cout << "Returning to main menu..." << std::endl;
                break;
            }
            else
            {
                std::cout << "Invalid choice." << std::endl;
            }
        }
    }

    // Plan Manager 메뉴 함수
    void planManagerMenu()
    {
        int choice;

        while (true)
        {
            std::cout << "\n===== Plan Manager =====" << std::endl;
            std::cout << "1. Add Recipe to Meal (Date + Meal Type + Recipe)" << std::endl;
            std::cout << "2. View Plan for Date (Meals only)" << std::endl;
            std::cout << "3. View Integrated Plan for Date (Meals + Schedules)" << std::endl;
            std::cout << "4. View Full Plan (All data)" << std::endl;
            std::cout << "5. Display Shopping List" << std::endl;
            std::cout << "6. Set Difficulty Constraint" << std::endl;
            std::cout << "7. Generate Balanced Plan" << std::endl;
            std::cout << "8. Back to Main Menu" << std::endl;
            std::cout << "Select: ";

            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1)
            {
                string date, mealType, recipe;
                int servings;

                std::cout << "Enter date (YYYY-MM-DD): ";
                getline(std::cin, date);

                std::cout << "Enter meal type (Breakfast/Lunch/Dinner): ";
                getline(std::cin, mealType);

                std::cout << "Enter recipe name: ";
                getline(std::cin, recipe);

                std::cout << "Enter servings: ";
                std::cin >> servings;
                std::cin.ignore();

                planManager->addRecipeToMeal(date, mealType, recipe, servings);
            }
            else if (choice == 2)
            {
                string date;
                std::cout << "Enter date (YYYY-MM-DD): ";
                getline(std::cin, date);
                planManager->viewPlanForDate(date);
            }
            else if (choice == 3)
            {
                string date;
                std::cout << "Enter date (YYYY-MM-DD): ";
                getline(std::cin, date);
                planManager->viewIntegratedPlanForDate(date);
            }
            else if (choice == 4)
            {
                planManager->viewFullPlan();
            }
            else if (choice == 5)
            {
                planManager->displayShoppingList();
            }
            else if (choice == 6)
            {
                char level;
                std::cout << "Enter maximum difficulty level (A, B, C): ";
                std::cin >> level;
                std::cin.ignore();
                planManager->setDifficultyConstraint(level);
            }
            else if (choice == 7)
            {
                planManager->generateBalancedPlan();
            }
            else if (choice == 8)
            {
                std::cout << "Returning to main menu..." << std::endl;
                break;
            }
            else
            {
                std::cout << "Invalid choice." << std::endl;
            }
        }
    }

    // 소멸자
    ~Greeter()
    {
        delete planManager; // 동적 할당된 PlanManager 객체 해제
        std::cout << "Greeter object is being destroyed." << std::endl;
    }
};

#endif // GREETER_H

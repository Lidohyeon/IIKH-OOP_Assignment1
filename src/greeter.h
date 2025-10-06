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
        cout << "\n";
        cout << "██╗██╗██╗██╗  ██╗██╗  ██╗" << endl;
        cout << "██║██║██║██║ ██╔╝██║  ██║" << endl;
        cout << "██║██║██║█████╔╝ ███████║" << endl;
        cout << "██║██║██║██╔═██╗ ██╔══██║" << endl;
        cout << "██║██║██║██║  ██╗██║  ██║" << endl;
        cout << "╚═╝╚═╝╚═╝╚═╝  ╚═╝╚═╝  ╚═╝" << endl;
        cout << "\nWelcome to IIKH Data Management System!" << endl;
        cout << "=========================================" << endl;
    }

    void showMenu()
    {
        cout << "\n===== Main Menu =====\n";
        cout << "1. Insert New Recipe\n";
        cout << "2. Search for a Recipe\n";
        cout << "3. Delete a Recipe\n";
        cout << "4. Edit a Recipe\n";
        cout << "5. Sort Recipes by Title\n";
        cout << "6. List All Recipes\n";
        cout << "7. Recommendation\n";
        cout << "8. Schedule Management\n";
        cout << "9. Meal Management\n";
        cout << "10. Plan Manager\n";
        cout << "11. Save and Exit\n";
        cout << "==========================\n";
        cout << "> ";
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
        cout << "Recipes have been sorted." << endl;
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
                cout << "Saving and exiting..." << endl;
                db.saveToFile();
                cout << "Data saved successfully. Goodbye!" << endl;
                return;
            default:
                cout << "Invalid selection. Please try again." << endl;
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

public:
private:
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
        cout << "\n===== Meal Management =====" << endl;
        cout << "Select meal type (1: Breakfast, 2: Lunch, 3: Dinner): ";
        int t;
        cin >> t;
        string type = (t == 1 ? "Breakfast" : (t == 2 ? "Lunch" : "Dinner"));
        Meal meal(type);

        while (true)
        {
            cout << "\n===== " << type << " Menu =====" << endl;
            cout << "1. Add Recipe" << endl;
            cout << "2. Remove Recipe" << endl;
            cout << "3. Set Servings" << endl;
            cout << "4. Display Meal" << endl;
            cout << "5. Back" << endl;
            cout << "Select: ";
            int c;
            cin >> c;

            if (c == 1)
            {
                string r;
                cout << "Enter recipe name: ";
                cin.ignore();
                getline(cin, r);
                meal.addRecipe(db, r);
            }
            else if (c == 2)
            {
                string r;
                cout << "Enter recipe name to remove: ";
                cin.ignore();
                getline(cin, r);
                meal.removeRecipe(r);
            }
            else if (c == 3)
            {
                int s;
                cout << "Enter servings: ";
                cin >> s;
                meal.setServings(s);
            }
            else if (c == 4)
            {
                meal.display();
            }
            else if (c == 5)
            {
                cout << "Returning to main menu..." << endl;
                break;
            }
            else
            {
                cout << "Invalid choice." << endl;
            }
        }
    }

    // Plan Manager 메뉴 함수
    void planManagerMenu()
    {
        int choice;

        while (true)
        {
            cout << "\n===== Plan Manager =====" << endl;
            cout << "1. Add Recipe to Meal (Date + Meal Type + Recipe)" << endl;
            cout << "2. Add Recipe to Date (Date + Recipe only)" << endl;
            cout << "3. View Plan for Date" << endl;
            cout << "4. View Full Plan" << endl;
            cout << "5. Display Shopping List" << endl;
            cout << "6. Back to Main Menu" << endl;
            cout << "Select: ";

            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                string date, mealType, recipe;
                int servings;

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);

                cout << "Enter meal type (Breakfast/Lunch/Dinner): ";
                getline(cin, mealType);

                cout << "Enter recipe name: ";
                getline(cin, recipe);

                cout << "Enter servings: ";
                cin >> servings;
                cin.ignore();

                planManager->addRecipeToMeal(date, mealType, recipe, servings);
            }
            else if (choice == 2)
            {
                string date, recipe;
                int servings;

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);

                cout << "Enter recipe name: ";
                getline(cin, recipe);

                cout << "Enter servings: ";
                cin >> servings;
                cin.ignore();

                planManager->addRecipeToDate(date, recipe, servings);
            }
            else if (choice == 3)
            {
                string date;
                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                planManager->viewPlanForDate(date);
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
                cout << "Returning to main menu..." << endl;
                break;
            }
            else
            {
                cout << "Invalid choice." << endl;
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

#endif

// (Removed duplicate global functions and misplaced destructor. All schedule and meal management functions are implemented inside the Greeter class above.)

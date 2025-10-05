#ifndef GREETER_H
#define GREETER_H

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <stdlib.h>
#include "RecipeDatabase.h" // 데이터베이스 클래스를 사용하기 위해 포함
#include "Date.h"
#include "Meal.h"

using namespace std;

class Greeter
{
private:
    RecipeDatabase &db;          // 데이터베이스 객체에 대한 '참조'
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
        cout << "10. Save and Exit\n";
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
        db.displayAll(); // 정렬된 결과를 보여줌
    }

public:
    // 생성자: 사용할 데이터베이스를 외부에서 받음
    Greeter(RecipeDatabase &database) : db(database) {}

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
        std::cout << "A. Advanced (Hard recipes)" << std::endl;
        std::cout << "B. Intermediate (Medium recipes)" << std::endl;
        std::cout << "C. Beginner (Easy recipes)" << std::endl;
        std::cout << "Please select (A/B/C): ";

        char difficultyChoice;
        std::cin >> difficultyChoice;

        // 대소문자 통일
        if (difficultyChoice >= 'a' && difficultyChoice <= 'c')
        {
            difficultyChoice = difficultyChoice - 'a' + 'A';
        }

        std::string difficultyLevel;
        std::vector<std::string> recommendedRecipes;

        switch (difficultyChoice)
        {
        case 'A':
            difficultyLevel = "Advanced";
            recommendedRecipes = {
                "⭐ Beef Wellington - Classic British dish",
                "⭐ Homemade Ramen - Complex broth and toppings",
                "⭐ Duck Confit - French culinary technique",
                "⭐ Soufflé - Delicate French dessert",
                "⭐ Sushi Rolls - Japanese precision cooking"};
            break;
        case 'B':
            difficultyLevel = "Intermediate";
            recommendedRecipes = {
                "⭐ Pasta Carbonara - Creamy Italian classic",
                "⭐ Chicken Stir Fry - Balanced and nutritious",
                "⭐ Beef Bulgogi - Korean marinated beef",
                "⭐ Fish Tacos - Fresh and flavorful",
                "⭐ Vegetable Curry - Aromatic spices"};
            break;
        case 'C':
            difficultyLevel = "Beginner";
            recommendedRecipes = {
                "⭐ Scrambled Eggs - Easy breakfast dish",
                "⭐ Toast with Butter - Simple and quick",
                "⭐ Instant Noodles - 5-minute meal",
                "⭐ Grilled Cheese Sandwich - Classic comfort food",
                "⭐ Fruit Salad - Healthy and refreshing"};
            break;
        default:
            std::cout << "Invalid selection. Please try again." << std::endl;
            return;
        }

        std::cout << "\n===== " << difficultyLevel << " Level Recommendations =====" << std::endl;
        for (const auto &recipe : recommendedRecipes)
        {
            std::cout << recipe << std::endl;
        }
        std::cout << "\nTotal " << recommendedRecipes.size() << " recipes recommended for " << difficultyLevel << " level." << std::endl;
    }

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
                meal.addRecipe(r);
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

    // 소멸자
    ~Greeter()
    {
        std::cout << "Greeter object is being destroyed." << std::endl;
    }
};

#endif

// (Removed duplicate global functions and misplaced destructor. All schedule and meal management functions are implemented inside the Greeter class above.)

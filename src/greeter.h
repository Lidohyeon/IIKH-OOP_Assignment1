#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "Date.h"
#include "Meal.h"

class Greeter
{
private:
    std::vector<std::string> data; // 데이터를 저장할 벡터
    std::vector<Date> schedules;   // 일정을 저장할 벡터

public:
    // 생성자
    Greeter()
    {
        showWelcome();
        showMenu();
    }

    // 환영 메시지 표시 함수 (IIKH 큰 글씨)
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

    // 메뉴 표시 함수
    void showMenu()
    {
        int choice;

        while (true)
        {
            std::cout << "\n===== Menu Selection =====" << std::endl;
            std::cout << "1. Search" << std::endl;
            std::cout << "2. Insert" << std::endl;
            std::cout << "3. Delete" << std::endl;
            std::cout << "4. Recommendation" << std::endl;
            std::cout << "5. Schedule Management" << std::endl;
            std::cout << "6. Meal Management " << std ::endl;
            std::cout << "7. Exit" << std::endl;
            std::cout << "Please select: ";

            std::cin >> choice;

            switch (choice)
            {
            case 1:
                searchData();
                break;
            case 2:
                insertData();
                break;
            case 3:
                deleteData();
                break;
            case 4:
                recommendData();
                break;
            case 5:
                scheduleMenu();
                break;
            case 6:
                mealMenu();
                break;
            case 7:
                std::cout << "Exiting the program. Thank you!" << std::endl;
                return;
            default:
                std::cout << "Invalid selection. Please try again." << std::endl;
                break;
            }
        }
    }

    // 데이터 검색 함수
    void searchData()
    {
        if (data.empty())
        {
            std::cout << "No data stored." << std::endl;
            return;
        }

        std::string searchTerm;
        std::cout << "Enter data to search: ";
        std::cin.ignore(); // 이전 입력의 개행문자 제거
        std::getline(std::cin, searchTerm);

        bool found = false;
        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i].find(searchTerm) != std::string::npos)
            {
                std::cout << "Found data [" << i << "]: " << data[i] << std::endl;
                found = true;
            }
        }

        if (!found)
        {
            std::cout << "No data containing '" << searchTerm << "' was found." << std::endl;
        }
    }

    // 데이터 삽입 함수
    void insertData()
    {
        std::string newData;
        std::cout << "Enter data to add: ";
        std::cin.ignore(); // 이전 입력의 개행문자 제거
        std::getline(std::cin, newData);

        data.push_back(newData);
        std::cout << "Data added successfully." << std::endl;
        std::cout << "Current number of stored data: " << data.size() << std::endl;
    }

    // 데이터 삭제 함수
    void deleteData()
    {
        if (data.empty())
        {
            std::cout << "No data to delete." << std::endl;
            return;
        }

        // 현재 저장된 데이터 목록 표시
        std::cout << "Currently stored data:" << std::endl;
        for (size_t i = 0; i < data.size(); i++)
        {
            std::cout << "[" << i << "] " << data[i] << std::endl;
        }

        int index;
        std::cout << "Enter index of data to delete: ";
        std::cin >> index;

        if (index >= 0 && index < static_cast<int>(data.size()))
        {
            std::cout << "'" << data[index] << "' has been deleted." << std::endl;
            data.erase(data.begin() + index);
            std::cout << "Current number of stored data: " << data.size() << std::endl;
        }
        else
        {
            std::cout << "Invalid index." << std::endl;
        }
    }

    // 추천 함수
    void recommendData()
    {
        if (data.empty())
        {
            std::cout << "No data available for recommendation." << std::endl;
            return;
        }

        std::cout << "\n===== Data Recommendations =====" << std::endl;

        if (data.size() == 1)
        {
            std::cout << "Only one data available: " << data[0] << std::endl;
        }
        else if (data.size() <= 3)
        {
            std::cout << "All available data:" << std::endl;
            for (size_t i = 0; i < data.size(); i++)
            {
                std::cout << "⭐ " << data[i] << std::endl;
            }
        }
        else
        {
            std::cout << "Top 3 recommended data:" << std::endl;
            // 랜덤하게 3개 선택하거나 처음 3개 표시
            for (size_t i = 0; i < 3 && i < data.size(); i++)
            {
                std::cout << "⭐ " << data[i] << std::endl;
            }
            std::cout << "\nTotal " << data.size() << " data items available." << std::endl;
        }
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
        std::cout << "Enter new schedule (e.g., 2024-10-02 14:00 - Meeting with team): ";
        std::cin.ignore(); // 이전 입력의 개행문자 제거
        std::getline(std::cin, scheduleInput);

        // Date 객체 생성
        Date newSchedule(scheduleInput);
        schedules.push_back(newSchedule);

        std::cout << "Schedule added successfully!" << std::endl;
        std::cout << "Total schedules: " << schedules.size() << std::endl;

        // 방금 추가된 일정 확인
        std::cout << "Added: " << newSchedule.toString() << std::endl;
    }

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
        cout << "/n===== Meal Management =====" << endl;
        cout << "Select meal type (1: Breakfast, 2: Lunch, 3: Dinner): ";
        int t;
        cin >> t;
        string type = (t == 1 ? "Breakfast" : (t == 2 ? "Lunch" : "Dinner"));
        Meal meal(type);

        while (true)
        {
            cout << "/n===== " << type << " Menu =====" << endl;
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

// 난이도 선택 항목 추가
//  레시피 디비에게 받은
//  사용자의 요리 능력 (제약) 사용자의 능력을 고려해서 max difficult level을 사용자가 설정한다.
//